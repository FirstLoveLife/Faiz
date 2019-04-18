#ifndef UNIQUE_PTR
#define UNIQUE_PTR
#include "rider/faiz/common_type.hpp"
#include "rider/faiz/compressed_pair.hpp"
#include "rider/faiz/functional.hpp"
#include "rider/faiz/macros.hpp"
#include "rider/faiz/math/RelationAlgebra.hpp" // for totally_ordered
#include "rider/faiz/ptr_traits.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"
#include "rider/faiz/utility/swap.hpp"
#include <memory>
/*
Good FAQ: unique_ptr with incomplete classes:
https://stackoverflow.com/questions/5606750/deletion-of-pointer-to-incomplete-type-and-smart-pointers
https://stackoverflow.com/a/1951967/6949852
https://stackoverflow.com/a/17950450/6949852
https://stackoverflow.com/a/32269374/6949852

https://stackoverflow.com/a/6088400/6949852:
> shared_ptr determines the correct destructor to call at run-time - it isn't
part of the type signature. When you initialize or reset a shared_ptr to point
to a new object, it creates a "deleter" that can be used to destroy the object.
However, unique_ptr's destructor is part of its type, and it must be known at
compile-time.
 */

namespace Rider::Faiz
{
	using Math::AbstractAlgebra::RelationAlgebra::totally_ordered;

	tpl<typ T> using pointer_type_aux = _p<remove_reference_t<T>>;


	tpl<typ T> struct default_delete
	{
		static_assert(not_function_v<T>,
			"Oops, default_delete cannot be instantiated for function "
			"types, please give me a new type");

		// Constructs a default_delete object.
		constexpr default_delete() noexcept = default;

		// Constructs a default_delete object from another
		// default_delete object. This constructor will only
		// participate in overload resolution if U* is implicitly
		// convertible to T*.
		tpl<typ U, enable_if_t<is_convertible_v<U*, T*>>* = nullptr>
		default_delete(const default_delete<U>&) noexcept
		{}

		// At the point in the code the operator() is called, the type must
		// be complete. In some implementations a static_assert is used to
		// make sure this is the case. The reason for this requirement is
		// that calling delete on an incomplete type is undefined behavior
		// in C++ if the complete class type has a nontrivial destructor or
		// a deallocation function, as the compiler has no way of knowing
		// whether such functions exist and must be invoked.
		void
		operator()(T* ptr) const noexcept
		{
			static_assert(sizeof(T) > 0,
				"Oops, default_delete can not delete incomplete type");
			static_assert(not_void_v<T>,
				"Oops, default_delete can not delete incomplete type");
			delete ptr;
		}
	};

	// default_delete is the default destruction policy used by
	// unique_ptr when no deleter is specified.
	// 1) The non-specialized default_delete uses delete to deallocate
	// memory for a single object.
	// 2) A partial specialization for array
	// types that uses delete[] is also provided.
	tpl<typ T> struct default_delete<T[]>
	{
	private:
		// helper struct for default_delete<T[]>
		tpl<typ U> struct EnableIfConvertible
			: enable_if<is_convertible_v<U (*)[], T (*)[]>>
		{};


	public:
		// Constructs a default_delete object.
		constexpr default_delete() noexcept = default;

		// Constructs a default_delete<U[]> object from another
		// default_delete<U[]> object. This constructor will only
		// participate in overload resolution if U(*)[] is implicitly
		// convertible to T(*)[].
		tpl<typ U>
		default_delete(const default_delete<U[]>&,
			_t<EnableIfConvertible<U>>* = nullptr) noexcept
		{}

		//  At the point in the code the operator() is called, the type must
		// be complete. In some implementations a static_assert is used to
		// make sure this is the case. The reason for this requirement is
		// that calling delete on an incomplete type is undefined behavior
		// in C++ if the complete class type has a nontrivial destructor or
		// a deallocation function, as the compiler has no way of knowing
		// whether such functions exist and must be invoked.
		tpl<typ U> fn
		operator()(U* ptr) const noexcept->_t<EnableIfConvertible<U>>
		{
			static_assert(
				sizeof(T) > 0, "default_delete can not delete incomplete type");
			static_assert(
				not_void_v<T>, "default_delete can not delete void type");
			delete[] ptr;
		}
	};


	tpl<typ tDeleter> struct unique_ptr_deleter_sfinae
	{
		static_assert(not_reference_v<tDeleter>, "incorrect specialization");
		using lval_ref_type = const tDeleter&;
		using good_rval_ref_type = tDeleter&&;
		using enable_rval_overload = true_;
	};

	tpl<typ tDeleter> struct unique_ptr_deleter_sfinae<tDeleter const&>
	{
		using lval_ref_type = const tDeleter&;
		using bad_rval_ref_type = const tDeleter&&;
		using enable_rval_overload = false_;
	};

	tpl<typ tDeleter> struct unique_ptr_deleter_sfinae<tDeleter&>
	{
		using lval_ref_type = tDeleter&;
		using bad_rval_ref_type = tDeleter&&;
		using enable_rval_overload = false_;
	};


	tpl<typ T, typ tDeleter = default_delete<T>> class unique_ptr
		: totally_ordered<unique_ptr<T, tDeleter>>,
		  totally_ordered<nullptr_t, unique_ptr<T, tDeleter>>
	{
	public:
		// tDeleter, the function object or lvalue reference to function or
		// to function object, to be called from the destructor
		using deleter_type = tDeleter;
		// A synonym for the tpl parameter T.
		using element_type = T;

		//  A synonym for `Delete::pointer` if defined, otherwise `T *`.
		// IOW, `remove_reference<tDeleter>::type::pointer` if that
		// type exists, otherwise `T*`. Must satisfy *NullablePointer*

		using pointer
			= detected_or_t<element_type*, pointer_type_aux, deleter_type>;

		static_assert(not_rvalue_reference_v<deleter_type>,
			"Come on, rvalue_reference is not what I want");

		unique_ptr(unique_ptr& up) = delete;

		unique_ptr&
		operator=(unique_ptr&)
			= delete;

		tpl<typ uPointer, typ uDeleter> unique_ptr&
		operator=(unique_ptr<uPointer, uDeleter>&)
			= delete;

	private:
		using tDeleterSFINAE = unique_ptr_deleter_sfinae<tDeleter>;

		tpl<bool vDummy> using tLValRef
			= typ dependent_type<tDeleterSFINAE, vDummy>::lval_ref_type;

		tpl<bool vDummy> using tGoodRValRef
			= typ dependent_type<tDeleterSFINAE, vDummy>::good_rval_ref_type;

		tpl<bool vDummy> using BadRValRefType
			= typ dependent_type<tDeleterSFINAE, vDummy>::bad_rval_ref_type;


		tpl<bool vDummy,
			typ uDeleter = dependent_type_t<type_identity<deleter_type>,
				vDummy>> using EnableIfDeleterDefaultConstructible
			= enable_if_t < is_default_constructible_v<
								uDeleter> and not_pointer_v<uDeleter>> ;

		tpl<typ tArg> using EnableIfDeleterConstructible
			= enable_if_t<is_constructible_v<deleter_type, tArg>>;

		tpl<typ yaUnique, typ uPointer> using EnableIfMoveConvertible
			= enable_if_t < is_convertible_v<_p<yaUnique>,
								pointer> and not_array_v<uPointer>> ;

		tpl<typ tUDel> using EnableIfDeleterConvertible = enable_if_t
				< (is_reference_v<tDeleter> and is_same_v<tDeleter, tUDel>)
			or (not_reference_v<tDeleter> and is_convertible_v<tUDel, tDeleter>)
				> ;

		tpl<typ tUDel> using EnableIfDeleterAssignable
			= enable_if_t<is_assignable_v<tDeleter&, tUDel&&>>;

	protected:
		tight_pair<pointer, deleter_type> pair;

	public:
		tpl<bool vDummy = true,
			class = EnableIfDeleterDefaultConstructible<
				vDummy>> constexpr unique_ptr() noexcept
			: pair{pointer{}}
		{
			static_assert(not_pointer_v<deleter_type>,
				"WTF, If this constructor is instantiated with a pointer "
				"type or reference type, for the tpl argument D, the "
				"program is ill-formed.");
			static_assert(not_reference_v<deleter_type>,
				"WTF, If this constructor is "
				"instantiated with a pointer "
				"type or reference type, for "
				"the tpl argument D, the "
				"program is ill-formed.");
		}
		tpl<bool vDummy = true,
			typ = EnableIfDeleterDefaultConstructible<
				vDummy>> constexpr unique_ptr(nullptr_t) noexcept
			: pair{}
		{
			static_assert(not_pointer_v<deleter_type>,
				"WTF, If this constructor is instantiated with a pointer "
				"type or reference type, for the tpl argument D, the "
				"program is ill-formed.");
			static_assert(not_reference_v<deleter_type>,
				"WTF, If this constructor is "
				"instantiated with a pointer "
				"type or reference type, for "
				"the tpl argument D, the "
				"program is ill-formed.");
		}

		tpl<bool vDummy = true,
			typ = EnableIfDeleterDefaultConstructible<
				vDummy>> explicit unique_ptr(pointer ptr) noexcept
			: pair{ptr, {}}
		{
			static_assert(not_pointer_v<deleter_type>,
				"WTF, If this constructor is instantiated with a pointer "
				"type or reference type, for the tpl argument D, the "
				"program is ill-formed.");
			static_assert(not_reference_v<deleter_type>,
				"WTF, If this constructor is "
				"instantiated with a pointer "
				"type or reference type, for "
				"the tpl argument D, the "
				"program is ill-formed.");
		}

		tpl<bool vDummy = true,
			class = EnableIfDeleterConstructible<tLValRef<vDummy>>>
		unique_ptr(pointer ptr, tLValRef<vDummy> d) noexcept : pair(ptr, d)
		{}

		tpl<bool vDummy = true,
			class = EnableIfDeleterConstructible<tGoodRValRef<vDummy>>>
		unique_ptr(pointer ptr, tGoodRValRef<vDummy> d) noexcept
			: pair(ptr, move(d))
		{
			static_assert(not_reference_v<deleter_type>,
				"rvalue deleter bound to reference");
		}

		tpl<bool vDummy = true,
			class = EnableIfDeleterConstructible<BadRValRefType<vDummy>>>
		unique_ptr(pointer ptr, BadRValRefType<vDummy> d) = delete;


		unique_ptr(unique_ptr&& u) noexcept
			: pair(u.release(), forward<deleter_type>(u.get_deleter()))
		{}

		tpl<typ uPointer,
			typ uDeleter,
			class = EnableIfMoveConvertible<unique_ptr<uPointer, uDeleter>,
				uPointer>,
			class = EnableIfDeleterConvertible<uDeleter>>
		unique_ptr(unique_ptr<uPointer, uDeleter>&& u) noexcept
			: pair(u.release(), forward<uDeleter>(u.get_deleter()))
		{}


		// Returns the deleter object which would be used for destruction of
		// the managed object.
		fn
		get_deleter() noexcept->deleter_type&
		{
			return Faiz::get<1>(pair);
		}
		// Returns the deleter object which would be used for destruction of
		// the managed object.
		fn
		get_deleter() const noexcept->const deleter_type&
		{
			return Faiz::get<1>(pair);
		}

		fn
		get() const noexcept->pointer
		{
			return Faiz::get<0>(pair);
		}


		fn
		reset(pointer p = pointer()) noexcept->void
		{
			pointer tmp = Faiz::get<0>(pair);
			Faiz::get<0>(pair) = p;
			if(tmp)
			{
				Faiz::get<1>(pair)(tmp);
			}
		}

		fn
		swap(unique_ptr& u) noexcept->void
		{
			pair.swap(u.pair);
		}

		// Checks whether *this owns an object, i.e. whether get() !=
		// nullptr.
		// XXX: cost me much time here because lacking explicit......
		explicit operator bool() const noexcept
		{
			return Faiz::get<0>(pair) != nullptr;
		}


		fn
		release() noexcept->pointer
		{
			pointer t = Faiz::get<0>(pair);
			Faiz::get<0>(pair) = pointer();
			return t;
		}


		// may throw, e.g. if pointer defines a throwing operator*
		fn operator*() const->add_lvalue_reference_t<T>
		{
			return *Faiz::get<0>(pair);
		}

		fn operator->() const noexcept->pointer
		{
			return Faiz::get<0>(pair);
		}

		fn
		operator=(unique_ptr&& u) noexcept->unique_ptr&
		{
			reset(u.release());
			Faiz::get<1>(pair) = std::forward<deleter_type>(u.get_deleter());
			return *this;
		}

		tpl<typ uPointer,
			typ uDeleter,
			class = EnableIfMoveConvertible<unique_ptr<uPointer, uDeleter>,
				uPointer>,
			class = EnableIfDeleterAssignable<uDeleter>>
			fn
			operator=(unique_ptr<uPointer, uDeleter>&& u) noexcept->unique_ptr&
		{
			reset(u.release());
			Faiz::get<1>(pair) = forward<uDeleter>(u.get_deleter());
			return *this;
		}

		unique_ptr& operator=(nullptr_t) noexcept
		{
			reset();
			return *this;
		}


		~unique_ptr()
		{
			reset();
		}


	}; // class unique_ptr

	// Array:
	//   - Pointers to types derived from **T** are rejected by the
	//   constructors,
	// and by reset.
	//   - The observers *operator** and *operator->* are not
	//   provided.
	//   - The indexing observer *operator[]* is provided.
	tpl<typ T, typ tDeleter> class unique_ptr<T[], tDeleter>
	// : Math::AbstractAlgebra::RelationAlgebra::totally_ordered<
	// unique_ptr<T[], tDeleter>>
	{

	public:
		using deleter_type = tDeleter;
		using element_type = T;

		using pointer
			= detected_or_t<element_type*, pointer_type_aux, deleter_type>;

		unique_ptr&
		operator=(unique_ptr&)
			= delete;
		tpl<typ _Up> unique_ptr&
		operator=(unique_ptr<_Up>&)
			= delete;

	private:
		tpl<typ tFrom> struct CheckArrayPointerConversion
			: is_same<tFrom, pointer>
		{};

		// clang-format off
		tpl<typ tFromElem>
		struct CheckArrayPointerConversion<tFromElem*>
			: bool_<is_same_v<tFromElem*, pointer>
                    or (is_same_v<pointer, element_type*>
						and is_convertible_v<tFromElem (*)[], element_type (*)[]>)>
		{};

		// clang-format on

		tpl<typ From> static constexpr bool CheckArrayPointerConversion_v
			= CheckArrayPointerConversion<From>::value;


		using tDeleterSFINAE = unique_ptr_deleter_sfinae<deleter_type>;

		tpl<bool vDummy> using tLValRef
			= typ dependent_type<tDeleterSFINAE, vDummy>::lval_ref_type;

		tpl<bool vDummy> using tGoodRValRef
			= typ dependent_type<tDeleterSFINAE, vDummy>::good_rval_ref_type;

		tpl<bool vDummy> using BadRValRefType
			= typ dependent_type<tDeleterSFINAE, vDummy>::bad_rval_ref_type;

		tpl<bool vDummy,
			typ uDeleter = dependent_type_t<type_identity<deleter_type>,
				vDummy>> using EnableIfDeleterDefaultConstructible
			= enable_if_t < is_default_constructible_v<
								uDeleter> and not_pointer_v<uDeleter>> ;

		tpl<typ tArg> using EnableIfDeleterConstructible
			= enable_if_t<is_constructible_v<deleter_type, tArg>>;

		tpl<typ tArg> using EnableIfPointerConvertible
			= enable_if_t<CheckArrayPointerConversion_v<tArg>>;

		// clang-format off
		tpl<typ U,
			typ uPointer,
				 typ tElem = _e<U>>
		using EnableIfMoveConvertible = enable_if_t<is_array_v<uPointer>
			and is_same_v<pointer, element_type*>
			and is_same_v<_p<U>, tElem*>
			and is_convertible_v<tElem (*)[], element_type (*)[]>>;

		tpl<typ uDel>
		using EnableIfDeleterConvertible = enable_if_t<
			(is_reference_v<deleter_type> and is_same_v<deleter_type, uDel>)
			or (not_reference_v<deleter_type> and is_convertible_v<uDel, deleter_type>)>;
		// clang-format on

		tpl<typ uDel> using EnableIfDeleterAssignable
			= enable_if_t<is_assignable_v<deleter_type&, uDel&&>>;


	protected:
		tight_pair<pointer, deleter_type> pair;

	public:
		tpl<bool vDummy = true,
			class = EnableIfDeleterDefaultConstructible<
				vDummy>> constexpr unique_ptr() noexcept
			: pair(pointer(), {})
		{}

		tpl<bool vDummy = true,
			class = EnableIfDeleterDefaultConstructible<
				vDummy>> constexpr unique_ptr(nullptr_t) noexcept
			: pair(pointer(), {})
		{}

		tpl<typ tPointer,
			bool vDummy = true,
			class = EnableIfDeleterDefaultConstructible<vDummy>,
			class = EnableIfPointerConvertible<
				tPointer>> explicit unique_ptr(tPointer p) noexcept
			: pair(p, {})
		{}

		tpl<typ uPointer,
			bool vDummy = true,
			class = EnableIfDeleterConstructible<tLValRef<vDummy>>,
			class = EnableIfPointerConvertible<uPointer>>
		unique_ptr(uPointer p, tLValRef<vDummy> d) noexcept : pair(p, d)
		{}

		tpl<bool vDummy = true,
			class = EnableIfDeleterConstructible<tLValRef<vDummy>>>
		unique_ptr(nullptr_t, tLValRef<vDummy> d) noexcept : pair(nullptr, d)
		{}

		tpl<typ tPointer,
			bool vDummy = true,
			class = EnableIfDeleterConstructible<tGoodRValRef<vDummy>>,
			class = EnableIfPointerConvertible<tPointer>>
		unique_ptr(tPointer p, tGoodRValRef<vDummy> d) noexcept
			: pair(p, move(d))
		{
			static_assert(not_reference_v<deleter_type>,
				"rvalue deleter bound to reference");
		}

		tpl<bool vDummy = true,
			class = EnableIfDeleterConstructible<tGoodRValRef<vDummy>>>
		unique_ptr(nullptr_t, tGoodRValRef<vDummy> d) noexcept
			: pair(nullptr, move(d))
		{
			static_assert(not_reference_v<deleter_type>,
				"rvalue deleter bound to reference");
		}

		tpl<typ uPointer,
			bool vDummy = true,
			class = EnableIfDeleterConstructible<BadRValRefType<vDummy>>,
			class = EnableIfPointerConvertible<uPointer>>
		unique_ptr(uPointer p, BadRValRefType<vDummy> d) = delete;

		unique_ptr(unique_ptr&& u) noexcept
			: pair(u.release(), forward<deleter_type>(u.get_deleter()))
		{}

		fn
		operator=(unique_ptr&& u) noexcept->unique_ptr&
		{
			reset(u.release());
			Faiz::get<1>(pair) = forward<deleter_type>(u.get_deleter());
			return *this;
		}

		tpl<typ uPointer,
			typ uDeleter,
			class = EnableIfMoveConvertible<unique_ptr<uPointer, uDeleter>,
				uPointer>,
			class = EnableIfDeleterConvertible<uDeleter>>
		unique_ptr(unique_ptr<uPointer, uDeleter>&& u) noexcept
			: pair(u.release(), forward<uDeleter>(u.get_deleter()))
		{}

		tpl<typ uPointer,
			typ uDeleter,
			class = EnableIfMoveConvertible<unique_ptr<uPointer, uDeleter>,
				uPointer>,
			class = EnableIfDeleterAssignable<uDeleter>>
			unique_ptr&
			operator=(unique_ptr<uPointer, uDeleter>&& u) noexcept
		{
			reset(u.release());
			Faiz::get<1>(pair) = forward<uDeleter>(u.get_deleter());
			return *this;
		}

		fn operator=(nullptr_t) noexcept->unique_ptr&
		{
			reset();
			return *this;
		}
		~unique_ptr() noexcept
		{
			reset();
		}

		tpl<typ tPointer> fn
		reset(tPointer p) noexcept
			->enable_if_t<CheckArrayPointerConversion_v<tPointer>>
		{
			pointer tmp = Faiz::get<0>(pair);
			Faiz::get<0>(pair) = p;
			if(tmp)
			{
				Faiz::get<1>(pair)(tmp);
			}
		}


		fn reset(nullptr_t = nullptr) noexcept->void
		{
			pointer tmp = Faiz::get<0>(pair);
			Faiz::get<0>(pair) = nullptr;
			if(tmp)
			{
				Faiz::get<1>(pair)(tmp);
			}
		}


		fn
		release() noexcept->pointer
		{
			pointer tmp = Faiz::get<0>(pair);
			Faiz::get<0>(pair) = pointer();
			return tmp;
		}


		fn
		swap(unique_ptr& x) noexcept->void
		{
			pair.swap(x.pair);
		}

		// operator[] provides access to elements of an array managed by a
		// unique_ptr.
		// The parameter i shall be less than the number of elements in the
		// array; otherwise, the behavior is undefined.
		add_lvalue_reference_t<T> operator[](ptrdiff_t i) const
		{
			static_assert(Faiz::extent_v<T> == 0 || i < Faiz::extent_v<T>);
			// static_assert(Faiz::get<0>(pair));
			return Faiz::get<0>(pair)[i];
		}

		fn
		get() const noexcept->pointer
		{
			return Faiz::get<0>(pair);
		}

		fn
		get_deleter() noexcept->deleter_type&
		{
			return Faiz::get<1>(pair);
		}

		fn
		get_deleter() const noexcept->const deleter_type&
		{
			return Faiz::get<1>(pair);
		}

		explicit operator bool() const noexcept
		{
			return Faiz::get<0>(pair) != nullptr;
		}
	};


	tpl<typ T, typ D> fn
	swap(unique_ptr<T, D>& a, unique_ptr<T, D>& b) noexcept
	{
		a.swap(b);
	}


	tpl<typ T1, typ D1, typ T2, typ D2> fn
	operator==(const unique_ptr<T1, D1>& a, const unique_ptr<T2, D2>& b)
	{
		return (a.get() == b.get());
	}

	tpl<typ T, typ D> fn
	operator==(const unique_ptr<T, D>& a, nullptr_t) noexcept
	{
		return not a;
	}

	tpl<typ T, typ D> fn
	operator==(nullptr_t, const unique_ptr<T, D>& a) noexcept
	{
		return not a;
	}


	tpl<typ T1, typ D1, typ T2, typ D2> fn
	operator<(const unique_ptr<T1, D1>& a, const unique_ptr<T2, D2>& b)
	{
		using P1 = _p<Faiz::unique_ptr<T1, D1>>;
		using P2 = _p<Faiz::unique_ptr<T2, D2>>;
		using tCommon = common_type_t<P1, P2>;
		tCommon pT1 = a.get();
		tCommon pT2 = b.get();
		return std::less<tCommon>()(pT1, pT2);
	}


	tpl<typ T, typ D> fn
	operator<(const unique_ptr<T, D>& a, nullptr_t)
	{
		using pointer = _p<unique_ptr<T, D>>;
		return std::less<pointer>()(a.get(), nullptr);
	}

	tpl<typ T, typ D> fn
	operator<(nullptr_t, const unique_ptr<T, D>& b)
	{
		using pointer = _p<unique_ptr<T, D>>;
		pointer pT = b.get();
		return std::less<pointer>()(nullptr, pT);
	}

	// http://isocpp.org/files/papers/N3656.txt
	tpl<typ T> struct Unique_if
	{
		using Single_object = unique_ptr<T>;
	};

	tpl<typ T> struct Unique_if<T[]>
	{
		using Unknown_bound = unique_ptr<T[]>;
	};

	tpl<typ T, Faiz::size_t N> struct Unique_if<T[N]>
	{
		using Known_bound = void;
	};

	// lwg 2098
	tpl<typ T, typ... Args> typ Unique_if<T>::Single_object
	make_unique(Args&&... args)
	{
		if constexpr(is_constructible<T, Args...>())
		{
			return unique_ptr<T>(new T(Faiz::forward<Args>(args)...));
		}
		else
		{
			return unique_ptr<T>(new T{Faiz::forward<Args>(args)...});
		}
	}

	tpl<typ T> typ Unique_if<T>::Unknown_bound
	make_unique(size_t n)
	{
		using U = remove_extent_t<T>;
		return unique_ptr<T>(new U[n]());
	}

	tpl<typ T, typ... Args> typ Unique_if<T>::Known_bound
	make_unique(Args&&...)
		= delete;

	tpl<typ T, typ D> enable_if_t<std::is_swappable_v<D>, void>
	swap(unique_ptr<T, D>& x, unique_ptr<T, D>& y) noexcept
	{
		x.swap(y);
	}


} // namespace Rider::Faiz

#endif
