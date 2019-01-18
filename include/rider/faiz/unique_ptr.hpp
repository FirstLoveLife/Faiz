#ifndef UNIQUE_PTR
#define UNIQUE_PTR
#include "rider/faiz/common_type.hpp"
#include "rider/faiz/compressed_pair.hpp"
#include "rider/faiz/functional.hpp"
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

	template<typename T>
	using pointer_type_aux = _p<remove_reference_t<T>>;

	template<typename T>
	struct default_delete
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
		template<typename U, enable_if_t<is_convertible_v<U*, T*>>* = nullptr>
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
	template<typename T>
	struct default_delete<T[]>
	{
	private:
		// helper struct for default_delete<T[]>
		template<typename U>
		struct EnableIfConvertible
			: enable_if<is_convertible_v<U (*)[], T (*)[]>>
		{};


	public:
		// Constructs a default_delete object.
		constexpr default_delete() noexcept = default;

		// Constructs a default_delete<U[]> object from another
		// default_delete<U[]> object. This constructor will only
		// participate in overload resolution if U(*)[] is implicitly
		// convertible to T(*)[].
		template<typename U>
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
		template<typename U>
		auto
		operator()(U* ptr) const noexcept -> _t<EnableIfConvertible<U>>
		{
			static_assert(
				sizeof(T) > 0, "default_delete can not delete incomplete type");
			static_assert(
				not_void_v<T>, "default_delete can not delete void type");
			delete[] ptr;
		}
	};


	template<typename tDeleter>
	struct unique_ptr_deleter_sfinae
	{
		static_assert(not_reference_v<tDeleter>, "incorrect specialization");
		using lval_ref_type = const tDeleter&;
		using good_rval_ref_type = tDeleter&&;
		using enable_rval_overload = true_;
	};

	template<typename tDeleter>
	struct unique_ptr_deleter_sfinae<tDeleter const&>
	{
		using lval_ref_type = const tDeleter&;
		using bad_rval_ref_type = const tDeleter&&;
		using enable_rval_overload = false_;
	};

	template<typename tDeleter>
	struct unique_ptr_deleter_sfinae<tDeleter&>
	{
		using lval_ref_type = tDeleter&;
		using bad_rval_ref_type = tDeleter&&;
		using enable_rval_overload = false_;
	};


	template<typename T, typename tDeleter = default_delete<T>>
	class unique_ptr : totally_ordered<unique_ptr<T, tDeleter>>,
					   totally_ordered<nullptr_t, unique_ptr<T, tDeleter>>
	{
	public:
		// tDeleter, the function object or lvalue reference to function or
		// to function object, to be called from the destructor
		using deleter_type = tDeleter;
		// A synonym for the template parameter T.
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

		template<typename uPointer, typename uDeleter>
		unique_ptr&
		operator=(unique_ptr<uPointer, uDeleter>&)
			= delete;

	private:
		using tDeleterSFINAE = unique_ptr_deleter_sfinae<tDeleter>;

		template<bool vDummy>
		using tLValRef =
			typename dependent_type<tDeleterSFINAE, vDummy>::lval_ref_type;

		template<bool vDummy>
		using tGoodRValRef =
			typename dependent_type<tDeleterSFINAE, vDummy>::good_rval_ref_type;

		template<bool vDummy>
		using BadRValRefType =
			typename dependent_type<tDeleterSFINAE, vDummy>::bad_rval_ref_type;


		template<bool vDummy,
			typename uDeleter
			= dependent_type_t<type_identity<deleter_type>, vDummy>>
		using EnableIfDeleterDefaultConstructible = enable_if_t<
			is_default_constructible_v<uDeleter> and not_pointer_v<uDeleter>>;

		template<typename tArg>
		using EnableIfDeleterConstructible
			= enable_if_t<is_constructible_v<deleter_type, tArg>>;

		template<typename yaUnique, typename uPointer>
		using EnableIfMoveConvertible = enable_if_t<
			is_convertible_v<_p<yaUnique>, pointer> and not_array_v<uPointer>>;

		template<typename tUDel>
		using EnableIfDeleterConvertible = enable_if_t<
			(is_reference_v<tDeleter> and is_same_v<tDeleter, tUDel>)
			or (not_reference_v<
					tDeleter> and is_convertible_v<tUDel, tDeleter>)>;

		template<typename tUDel>
		using EnableIfDeleterAssignable
			= enable_if_t<is_assignable_v<tDeleter&, tUDel&&>>;

	protected:
		tight_pair<pointer, deleter_type> pair;

	public:
		template<bool vDummy = true,
			class = EnableIfDeleterDefaultConstructible<vDummy>>
		constexpr unique_ptr() noexcept : pair{pointer{}}
		{
			static_assert(not_pointer_v<deleter_type>,
				"WTF, If this constructor is instantiated with a pointer "
				"type or reference type, for the template argument D, the "
				"program is ill-formed.");
			static_assert(not_reference_v<deleter_type>,
				"WTF, If this constructor is "
				"instantiated with a pointer "
				"type or reference type, for "
				"the template argument D, the "
				"program is ill-formed.");
		}
		template<bool vDummy = true,
			typename = EnableIfDeleterDefaultConstructible<vDummy>>
		constexpr unique_ptr(nullptr_t) noexcept : pair{}
		{
			static_assert(not_pointer_v<deleter_type>,
				"WTF, If this constructor is instantiated with a pointer "
				"type or reference type, for the template argument D, the "
				"program is ill-formed.");
			static_assert(not_reference_v<deleter_type>,
				"WTF, If this constructor is "
				"instantiated with a pointer "
				"type or reference type, for "
				"the template argument D, the "
				"program is ill-formed.");
		}

		template<bool vDummy = true,
			typename = EnableIfDeleterDefaultConstructible<vDummy>>
		explicit unique_ptr(pointer ptr) noexcept : pair{ptr, {}}
		{
			static_assert(not_pointer_v<deleter_type>,
				"WTF, If this constructor is instantiated with a pointer "
				"type or reference type, for the template argument D, the "
				"program is ill-formed.");
			static_assert(not_reference_v<deleter_type>,
				"WTF, If this constructor is "
				"instantiated with a pointer "
				"type or reference type, for "
				"the template argument D, the "
				"program is ill-formed.");
		}

		template<bool vDummy = true,
			class = EnableIfDeleterConstructible<tLValRef<vDummy>>>
		unique_ptr(pointer ptr, tLValRef<vDummy> d) noexcept : pair(ptr, d)
		{}

		template<bool vDummy = true,
			class = EnableIfDeleterConstructible<tGoodRValRef<vDummy>>>
		unique_ptr(pointer ptr, tGoodRValRef<vDummy> d) noexcept
			: pair(ptr, move(d))
		{
			static_assert(not_reference_v<deleter_type>,
				"rvalue deleter bound to reference");
		}

		template<bool vDummy = true,
			class = EnableIfDeleterConstructible<BadRValRefType<vDummy>>>
		unique_ptr(pointer ptr, BadRValRefType<vDummy> d) = delete;


		unique_ptr(unique_ptr&& u) noexcept
			: pair(u.release(), forward<deleter_type>(u.get_deleter()))
		{}

		template<typename uPointer,
			typename uDeleter,
			class = EnableIfMoveConvertible<unique_ptr<uPointer, uDeleter>,
				uPointer>,
			class = EnableIfDeleterConvertible<uDeleter>>
		unique_ptr(unique_ptr<uPointer, uDeleter>&& u) noexcept
			: pair(u.release(), forward<uDeleter>(u.get_deleter()))
		{}


		// Returns the deleter object which would be used for destruction of
		// the managed object.
		auto
		get_deleter() noexcept -> deleter_type&
		{
			return Faiz::get<1>(pair);
		}
		// Returns the deleter object which would be used for destruction of
		// the managed object.
		auto
		get_deleter() const noexcept -> const deleter_type&
		{
			return Faiz::get<1>(pair);
		}

		pointer
		get() const noexcept
		{
			return Faiz::get<0>(pair);
		}


		auto
		reset(pointer p = pointer()) noexcept -> void
		{
			pointer tmp = Faiz::get<0>(pair);
			Faiz::get<0>(pair) = p;
			if(tmp)
			{
				Faiz::get<1>(pair)(tmp);
			}
		}

		auto
		swap(unique_ptr& u) noexcept -> void
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


		auto
		release() noexcept -> pointer
		{
			pointer t = Faiz::get<0>(pair);
			Faiz::get<0>(pair) = pointer();
			return t;
		}


		// may throw, e.g. if pointer defines a throwing operator*
		auto operator*() const -> add_lvalue_reference_t<T>
		{
			return *Faiz::get<0>(pair);
		}

		auto operator-> () const noexcept -> pointer
		{
			return Faiz::get<0>(pair);
		}

		auto
		operator=(unique_ptr&& u) noexcept -> unique_ptr&
		{
			reset(u.release());
			Faiz::get<1>(pair) = std::forward<deleter_type>(u.get_deleter());
			return *this;
		}

		template<typename uPointer,
			typename uDeleter,
			class = EnableIfMoveConvertible<unique_ptr<uPointer, uDeleter>,
				uPointer>,
			class = EnableIfDeleterAssignable<uDeleter>>
		auto
		operator=(unique_ptr<uPointer, uDeleter>&& u) noexcept -> unique_ptr&
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
	template<typename T, typename tDeleter>
	class unique_ptr<T[], tDeleter>
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
		template<typename _Up>
		unique_ptr&
		operator=(unique_ptr<_Up>&)
			= delete;

	private:
		template<typename tFrom>
		struct CheckArrayPointerConversion : is_same<tFrom, pointer>
		{};

		// clang-format off
		template<typename tFromElem>
		struct CheckArrayPointerConversion<tFromElem*>
			: bool_<is_same_v<tFromElem*, pointer>
                    or (is_same_v<pointer, element_type*>
						and is_convertible_v<tFromElem (*)[], element_type (*)[]>)>
		{};

		// clang-format on

		template<typename From>
		static constexpr bool CheckArrayPointerConversion_v
			= CheckArrayPointerConversion<From>::value;


		using tDeleterSFINAE = unique_ptr_deleter_sfinae<deleter_type>;

		template<bool vDummy>
		using tLValRef =
			typename dependent_type<tDeleterSFINAE, vDummy>::lval_ref_type;

		template<bool vDummy>
		using tGoodRValRef =
			typename dependent_type<tDeleterSFINAE, vDummy>::good_rval_ref_type;

		template<bool vDummy>
		using BadRValRefType =
			typename dependent_type<tDeleterSFINAE, vDummy>::bad_rval_ref_type;

		template<bool vDummy,
			typename uDeleter
			= dependent_type_t<type_identity<deleter_type>, vDummy>>
		using EnableIfDeleterDefaultConstructible = enable_if_t<
			is_default_constructible_v<uDeleter> and not_pointer_v<uDeleter>>;

		template<typename tArg>
		using EnableIfDeleterConstructible
			= enable_if_t<is_constructible_v<deleter_type, tArg>>;

		template<typename tArg>
		using EnableIfPointerConvertible
			= enable_if_t<CheckArrayPointerConversion_v<tArg>>;

		// clang-format off
		template<typename U,
			typename uPointer,
				 typename tElem = _e<U>>
		using EnableIfMoveConvertible = enable_if_t<is_array_v<uPointer>
			and is_same_v<pointer, element_type*>
			and is_same_v<_p<U>, tElem*>
			and is_convertible_v<tElem (*)[], element_type (*)[]>>;

		template<typename uDel>
		using EnableIfDeleterConvertible = enable_if_t<
			(is_reference_v<deleter_type> and is_same_v<deleter_type, uDel>)
			or (not_reference_v<deleter_type> and is_convertible_v<uDel, deleter_type>)>;
		// clang-format on

		template<typename uDel>
		using EnableIfDeleterAssignable
			= enable_if_t<is_assignable_v<deleter_type&, uDel&&>>;


	protected:
		tight_pair<pointer, deleter_type> pair;

	public:
		template<bool vDummy = true,
			class = EnableIfDeleterDefaultConstructible<vDummy>>
		constexpr unique_ptr() noexcept : pair(pointer(), {})
		{}

		template<bool vDummy = true,
			class = EnableIfDeleterDefaultConstructible<vDummy>>
		constexpr unique_ptr(nullptr_t) noexcept : pair(pointer(), {})
		{}

		template<typename tPointer,
			bool vDummy = true,
			class = EnableIfDeleterDefaultConstructible<vDummy>,
			class = EnableIfPointerConvertible<tPointer>>
		explicit unique_ptr(tPointer p) noexcept : pair(p, {})
		{}

		template<typename uPointer,
			bool vDummy = true,
			class = EnableIfDeleterConstructible<tLValRef<vDummy>>,
			class = EnableIfPointerConvertible<uPointer>>
		unique_ptr(uPointer p, tLValRef<vDummy> d) noexcept : pair(p, d)
		{}

		template<bool vDummy = true,
			class = EnableIfDeleterConstructible<tLValRef<vDummy>>>
		unique_ptr(nullptr_t, tLValRef<vDummy> d) noexcept : pair(nullptr, d)
		{}

		template<typename tPointer,
			bool vDummy = true,
			class = EnableIfDeleterConstructible<tGoodRValRef<vDummy>>,
			class = EnableIfPointerConvertible<tPointer>>
		unique_ptr(tPointer p, tGoodRValRef<vDummy> d) noexcept
			: pair(p, move(d))
		{
			static_assert(not_reference_v<deleter_type>,
				"rvalue deleter bound to reference");
		}

		template<bool vDummy = true,
			class = EnableIfDeleterConstructible<tGoodRValRef<vDummy>>>
		unique_ptr(nullptr_t, tGoodRValRef<vDummy> d) noexcept
			: pair(nullptr, move(d))
		{
			static_assert(not_reference_v<deleter_type>,
				"rvalue deleter bound to reference");
		}

		template<typename uPointer,
			bool vDummy = true,
			class = EnableIfDeleterConstructible<BadRValRefType<vDummy>>,
			class = EnableIfPointerConvertible<uPointer>>
		unique_ptr(uPointer p, BadRValRefType<vDummy> d) = delete;

		unique_ptr(unique_ptr&& u) noexcept
			: pair(u.release(), forward<deleter_type>(u.get_deleter()))
		{}

		auto
		operator=(unique_ptr&& u) noexcept -> unique_ptr&
		{
			reset(u.release());
			Faiz::get<1>(pair) = forward<deleter_type>(u.get_deleter());
			return *this;
		}

		template<typename uPointer,
			typename uDeleter,
			class = EnableIfMoveConvertible<unique_ptr<uPointer, uDeleter>,
				uPointer>,
			class = EnableIfDeleterConvertible<uDeleter>>
		unique_ptr(unique_ptr<uPointer, uDeleter>&& u) noexcept
			: pair(u.release(), forward<uDeleter>(u.get_deleter()))
		{}

		template<typename uPointer,
			typename uDeleter,
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

		auto operator=(nullptr_t) noexcept -> unique_ptr&
		{
			reset();
			return *this;
		}
		~unique_ptr() noexcept
		{
			reset();
		}

		template<typename tPointer>
		auto
		reset(tPointer p) noexcept
			-> enable_if_t<CheckArrayPointerConversion_v<tPointer>>
		{
			pointer tmp = Faiz::get<0>(pair);
			Faiz::get<0>(pair) = p;
			if(tmp)
			{
				Faiz::get<1>(pair)(tmp);
			}
		}


		auto reset(nullptr_t = nullptr) noexcept -> void
		{
			pointer tmp = Faiz::get<0>(pair);
			Faiz::get<0>(pair) = nullptr;
			if(tmp)
			{
				Faiz::get<1>(pair)(tmp);
			}
		}


		auto
		release() noexcept -> pointer
		{
			pointer tmp = Faiz::get<0>(pair);
			Faiz::get<0>(pair) = pointer();
			return tmp;
		}


		auto
		swap(unique_ptr& x) noexcept -> void
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

		auto
		get() const noexcept -> pointer
		{
			return Faiz::get<0>(pair);
		}

		auto
		get_deleter() noexcept -> deleter_type&
		{
			return Faiz::get<1>(pair);
		}

		auto
		get_deleter() const noexcept -> const deleter_type&
		{
			return Faiz::get<1>(pair);
		}

		explicit operator bool() const noexcept
		{
			return Faiz::get<0>(pair) != nullptr;
		}
	};


	template<typename T, typename D>
	auto
	swap(unique_ptr<T, D>& a, unique_ptr<T, D>& b) noexcept
	{
		a.swap(b);
	}


	template<typename T1, typename D1, typename T2, typename D2>
	auto
	operator==(const unique_ptr<T1, D1>& a, const unique_ptr<T2, D2>& b)
	{
		return (a.get() == b.get());
	}

	template<typename T, typename D>
	auto
	operator==(const unique_ptr<T, D>& a, nullptr_t) noexcept
	{
		return !a;
	}

	template<typename T, typename D>
	auto
	operator==(nullptr_t, const unique_ptr<T, D>& a) noexcept
	{
		return !a;
	}


	template<typename T1, typename D1, typename T2, typename D2>
	auto
	operator<(const unique_ptr<T1, D1>& a, const unique_ptr<T2, D2>& b)
	{
		using P1 = _p<Faiz::unique_ptr<T1, D1>>;
		using P2 = _p<Faiz::unique_ptr<T2, D2>>;
		using tCommon = common_type_t<P1, P2>;
		tCommon pT1 = a.get();
		tCommon pT2 = b.get();
		return std::less<tCommon>()(pT1, pT2);
	}


	template<typename T, typename D>
	auto
	operator<(const unique_ptr<T, D>& a, nullptr_t)
	{
		using pointer = _p<unique_ptr<T, D>>;
		return std::less<pointer>()(a.get(), nullptr);
	}

	template<typename T, typename D>
	auto
	operator<(nullptr_t, const unique_ptr<T, D>& b)
	{
		using pointer = _p<unique_ptr<T, D>>;
		pointer pT = b.get();
		return std::less<pointer>()(nullptr, pT);
	}

	// http://isocpp.org/files/papers/N3656.txt
	template<typename T>
	struct Unique_if
	{
		using Single_object = unique_ptr<T>;
	};

	template<typename T>
	struct Unique_if<T[]>
	{
		using Unknown_bound = unique_ptr<T[]>;
	};

	template<typename T, Faiz::size_t N>
	struct Unique_if<T[N]>
	{
		using Known_bound = void;
	};

	// lwg 2098
	template<typename T, typename... Args>
	typename Unique_if<T>::Single_object
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

	template<typename T>
	typename Unique_if<T>::Unknown_bound
	make_unique(size_t n)
	{
		using U = remove_extent_t<T>;
		return unique_ptr<T>(new U[n]());
	}

	template<typename T, typename... Args>
	typename Unique_if<T>::Known_bound
	make_unique(Args&&...)
		= delete;

	template<typename T, typename D>
	enable_if_t<std::is_swappable_v<D>, void>
	swap(unique_ptr<T, D>& x, unique_ptr<T, D>& y) noexcept
	{
		x.swap(y);
	}


} // namespace Rider::Faiz

#endif
