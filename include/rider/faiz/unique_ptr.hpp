#ifndef UNIQUE_PTR
#define UNIQUE_PTR
#include "rider/faiz/common_type.hpp"
#include "rider/faiz/compressed_pair.hpp"
#include "rider/faiz/functional.hpp"
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

	// faiz is a standard library like library

	// Tells if the tDeleter type has a typedef for pointer to T. If so then
	// return it, else return T*. The large majority of the time the pointer
	// type will be T*. The C++11 Standard requires that scoped_ptr let the
	// deleter define the pointer type.
	//
	// Example usage:
	// ```
	//     typedef typename unique_pointer_type<int, SomeDeleter>::type
	//     pointer
	//
	template<typename T, typename tDeleter>
	class unique_pointer_type
	{
		template<typename U>
		static typename U::pointer
		test(typename U::pointer*);

		template<typename U>
		static T*
		test(...);

	public:
		using type = decltype(test<remove_reference_t<tDeleter>>(0));
	};


	///////////////////////////////////////////////////////////////////////
	// is_array_cv_convertible
	//
	// Tells if the array pointer P1 is cv-convertible to array pointer P2.
	// The two types have two be equivalent pointer types and be convertible
	// when you consider const/volatile properties of them.
	//
	// Example usage:
	//     is_array_cv_convertible<int, Base*>::value             => false
	//     is_array_cv_convertible<Base, Base*>::value            => false
	//     is_array_cv_convertible<double*, bool*>::value         => false
	//     is_array_cv_convertible<Subclass*, Base*>::value       => false
	//     is_array_cv_convertible<const Base*, Base*>::value     => false
	//     is_array_cv_convertible<Base*, Base*>::value           => true
	//     is_array_cv_convertible<Base*, const Base*>::value     => true
	//     is_array_cv_convertible<Base*, volatile Base*>::value  => true
	///////////////////////////////////////////////////////////////////////

	template<typename P1,
		typename P2,
		bool = is_same_v<remove_cv_t<typename pointer_traits<P1>::element_type>,
			remove_cv_t<typename pointer_traits<P2>::element_type>>>
	struct is_array_cv_convertible_impl : public is_convertible<P1, P2>
	{}; // Return true if P1 is convertible to P2.

	template<typename P1, typename P2>
	struct is_array_cv_convertible_impl<P1, P2, false> : public false_
	{}; // P1's underlying type is not the same as P2's, so it can't be
		// converted, even if P2 refers to a subclass of P1. Parent == Child,
		// but Parent[] != Child[]

	template<typename P1,
		typename P2,
		bool = is_scalar_v<P1>and not_pointer_v<P1>>
	struct is_array_cv_convertible : public is_array_cv_convertible_impl<P1, P2>
	{};

	template<typename P1, typename P2>
	struct is_array_cv_convertible<P1, P2, true> : public false_
	{}; // P1 is scalar not a pointer, so it can't be converted to a pointer.

	BI_IS_NOT_ARE_ANY(array_cv_convertible)

	// is_safe_array_conversion
	//
	// Say you have two array types: T* t and U* u. You want to assign the u
	// to t but only if that's a safe thing to do. As shown in the logic
	// below, the array conversion is safe if U* and T* are convertible, if
	// U is an array, and if either U or T is not a pointer or U is not
	// derived from T.
	//
	// Note: Usage of this class could be replaced with
	// is_array_cv_convertible usage. To do: Do this replacement and test
	// it.
	//
	///////////////////////////////////////////////////////////////////////
	// clang-format on
	template<typename T, typename T_pointer, typename U, typename U_pointer>
	struct is_safe_array_conversion
		: public bool_<
			  is_convertible_v<U_pointer,
				  T_pointer> and is_array_v<U> and (not_pointer_v<U_pointer> || not_pointer_v<T_pointer> || not_derived_v<T, remove_extent_t<U>>)>
	{};


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
		template<typename U>
		default_delete(const default_delete<U>&,
			enable_if_t<is_convertible_v<U*, T*>>* = nullptr) noexcept
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
		_t<EnableIfConvertible<U>>
		operator()(U* ptr) const noexcept
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
		using enable_rval_overload = false_type;
	};

	template<typename tDeleter>
	struct unique_ptr_deleter_sfinae<tDeleter&>
	{
		using lval_ref_type = tDeleter&;
		using bad_rval_ref_type = tDeleter&&;
		using enable_rval_overload = false_type;
	};


	template<typename T, class Tdeleter = default_delete<T>>
	class unique_ptr
	{
	public:
		// Tdeleter, the function object or lvalue reference to function or
		// to function object, to be called from the destructor
		using deleter_type = Tdeleter;
		// A synonym for the template parameter T.
		using element_type = T;
		// A synonym for `Delete::pointer` if defined, otherwise `T *`.
		// IOW, `remove_reference<Tdeleter>::type::pointer` if that
		// type exists, otherwise `T*`. Must satisfy *NullablePointer*
		using pointer = _t<unique_pointer_type<element_type, deleter_type>>;

		static_assert(not_rvalue_reference_v<deleter_type>,
			"Come on, rvalue_reference is not what I want");

		unique_ptr(unique_ptr& up) = delete;

		unique_ptr&
		operator=(unique_ptr&)
			= delete;

		template<typename U, typename Udeleter>
		unique_ptr&
		operator=(unique_ptr<U, Udeleter>&)
			= delete;

	private:
		struct nat
		{
			int for_bool_;
		};

		using TdeleterSFINAE = unique_ptr_deleter_sfinae<Tdeleter>;

		template<bool Vdummy>
		using LValRefType =
			typename dependent_type<TdeleterSFINAE, Vdummy>::lval_ref_type;

		template<bool Vdummy>
		using GoodRValRefType =
			typename dependent_type<TdeleterSFINAE, Vdummy>::good_rval_ref_type;

		template<bool Vdummy>
		using BadRValRefType =
			typename dependent_type<TdeleterSFINAE, Vdummy>::bad_rval_ref_type;


		template<bool Vdummy,
			typename tUDeleter
			= dependent_type_t<type_identity<deleter_type>, Vdummy>>
		using EnableIfDeleterDefaultConstructible = enable_if_t<
			is_default_constructible_v<tUDeleter> and not_pointer_v<tUDeleter>>;

		template<typename ArgType>
		using EnableIfDeleterConstructible
			= enable_if_t<is_constructible_v<deleter_type, ArgType>>;

		template<typename UPtr, typename Up>
		using EnableIfMoveConvertible = enable_if_t<
			is_convertible_v<_p<UPtr>, pointer> and not_array_v<Up>>;

		template<typename tUDel>
		using EnableIfDeleterConvertible = enable_if_t<
			(is_reference_v<Tdeleter> and is_same_v<Tdeleter, tUDel>)
			or (not_reference_v<
					Tdeleter> and is_convertible_v<tUDel, Tdeleter>)>;

		template<typename tUDel>
		using EnableIfDeleterAssignable
			= enable_if_t<is_assignable_v<Tdeleter&, tUDel&&>>;

	protected:
		tight_pair<pointer, deleter_type> pair;

	public:
		template<bool Vdummy = true,
			class = EnableIfDeleterDefaultConstructible<Vdummy>>
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
		template<bool Vdummy = true,
			typename = EnableIfDeleterDefaultConstructible<Vdummy>>
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

		template<bool Vdummy = true,
			typename = EnableIfDeleterDefaultConstructible<Vdummy>>
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

		template<bool Vdummy = true,
			class = EnableIfDeleterConstructible<LValRefType<Vdummy>>>
		unique_ptr(pointer ptr, LValRefType<Vdummy> d) noexcept : pair(ptr, d)
		{}

		template<bool Vdummy = true,
			class = EnableIfDeleterConstructible<GoodRValRefType<Vdummy>>>
		unique_ptr(pointer ptr, GoodRValRefType<Vdummy> d) noexcept
			: pair(ptr, move(d))
		{
			static_assert(not_reference_v<deleter_type>,
				"rvalue deleter bound to reference");
		}

		template<bool Vdummy = true,
			class = EnableIfDeleterConstructible<BadRValRefType<Vdummy>>>
		unique_ptr(pointer ptr, BadRValRefType<Vdummy> d) = delete;


		unique_ptr(unique_ptr&& u) noexcept
			: pair(u.release(), forward<deleter_type>(u.get_deleter()))
		{}

		template<typename Up,
			typename Ep,
			class = EnableIfMoveConvertible<unique_ptr<Up, Ep>, Up>,
			class = EnableIfDeleterConvertible<Ep>>
		unique_ptr(unique_ptr<Up, Ep>&& u) noexcept
			: pair(u.release(), forward<Ep>(u.get_deleter()))
		{}


		// Returns the deleter object which would be used for destruction of
		// the managed object.
		deleter_type&
		get_deleter() noexcept
		{
			return Faiz::get<1>(pair);
		}
		// Returns the deleter object which would be used for destruction of
		// the managed object.
		const deleter_type&
		get_deleter() const noexcept
		{
			return Faiz::get<1>(pair);
		}

		pointer
		get() const noexcept
		{
			return Faiz::get<0>(pair);
		}

		void
		reset(pointer p = pointer()) noexcept
		{
			pointer tmp = Faiz::get<0>(pair);
			Faiz::get<0>(pair) = p;
			if(tmp)
			{
				Faiz::get<1>(pair)(tmp);
			}
		}

		void
		swap(unique_ptr& u) noexcept
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

		pointer
		release() noexcept
		{
			pointer t = Faiz::get<0>(pair);
			Faiz::get<0>(pair) = pointer();
			return t;
		}


		// may throw, e.g. if pointer defines a throwing operator*
		add_lvalue_reference_t<T> operator*() const
		{
			return *Faiz::get<0>(pair);
		}

		pointer operator->() const noexcept
		{
			return Faiz::get<0>(pair);
		}

		unique_ptr&
		operator=(unique_ptr&& u) noexcept
		{
			reset(u.release());
			Faiz::get<1>(pair) = std::forward<deleter_type>(u.get_deleter());
			return *this;
		}

		template<typename Up,
			typename Ep,
			class = EnableIfMoveConvertible<unique_ptr<Up, Ep>, Up>,
			class = EnableIfDeleterAssignable<Ep>>
		unique_ptr&
		operator=(unique_ptr<Up, Ep>&& u) noexcept
		{
			reset(u.release());
			Faiz::get<1>(pair) = forward<Ep>(u.get_deleter());
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
	template<typename T, typename Tdeleter>
	class unique_ptr<T[], Tdeleter>
	{

	public:
		using deleter_type = Tdeleter;
		using element_type = T;
		using pointer =
			typename unique_pointer_type<element_type, deleter_type>::type;
		/// These functions are private in order to prevent copying, for safety.

		unique_ptr&
		operator=(unique_ptr&)
			= delete;
		template<class _Up>
		unique_ptr&
		operator=(unique_ptr<_Up>&)
			= delete;

	private:
		template<class _From>
		struct CheckArrayPointerConversion : is_same<_From, pointer>
		{};

		template<class _FromElem>
		struct CheckArrayPointerConversion<_FromElem*>
			: bool_<
				  is_same_v<_FromElem*,
					  pointer> or (is_same_v<pointer, element_type*> and is_convertible_v<_FromElem (*)[], element_type (*)[]>)>
		{};

		template<typename From>
		static constexpr bool CheckArrayPointerConversion_v
			= CheckArrayPointerConversion<From>::value;


		typedef unique_ptr_deleter_sfinae<deleter_type> DeleterSFINAE;

		template<bool Vdummy>
		using LValRefType =
			typename dependent_type<DeleterSFINAE, Vdummy>::lval_ref_type;

		template<bool Vdummy>
		using GoodRValRefType =
			typename dependent_type<DeleterSFINAE, Vdummy>::good_rval_ref_type;

		template<bool Vdummy>
		using BadRValRefType =
			typename dependent_type<DeleterSFINAE, Vdummy>::bad_rval_ref_type;

		template<bool Vdummy,
			class tUDeleter
			= dependent_type_t<type_identity<deleter_type>, Vdummy>>
		using EnableIfDeleterDefaultConstructible = enable_if_t<
			is_default_constructible_v<tUDeleter> and not_pointer_v<tUDeleter>>;

		template<class ArgType>
		using EnableIfDeleterConstructible
			= enable_if_t<is_constructible_v<deleter_type, ArgType>>;

		template<class Tp>
		using EnableIfPointerConvertible
			= enable_if_t<CheckArrayPointerConversion_v<Tp>>;

		// clang-format off
		template<typename Uptr,
			typename Up,
				 typename ElemT = _e<Uptr>>
		using EnableIfMoveConvertible = enable_if_t<is_array_v<Up>
			and is_same_v<pointer, element_type*>
			and is_same_v<_p<Uptr>, ElemT*>
			and is_convertible_v<ElemT (*)[], element_type (*)[]>>;
		// clang-format on

		template<typename Udel>
		using EnableIfDeleterConvertible = enable_if_t<
			(is_reference_v<deleter_type> and is_same_v<deleter_type, Udel>)
			or (not_reference_v<
					deleter_type> and is_convertible_v<Udel, deleter_type>)>;

		template<class Udel>
		using EnableIfDeleterAssignable
			= enable_if_t<is_assignable_v<deleter_type&, Udel&&>>;


	protected:
		Faiz::tight_pair<pointer, deleter_type> pair;

	public:
		template<bool Udummy = true,
			class = EnableIfDeleterDefaultConstructible<Udummy>>
		constexpr unique_ptr() noexcept : pair(pointer(), {})
		{}

		template<bool Udummy = true,
			class = EnableIfDeleterDefaultConstructible<Udummy>>
		constexpr unique_ptr(nullptr_t) noexcept : pair(pointer(), {})
		{}

		template<class _Pp,
			bool Udummy = true,
			class = EnableIfDeleterDefaultConstructible<Udummy>,
			class = EnableIfPointerConvertible<_Pp>>
		explicit unique_ptr(_Pp __p) noexcept : pair(__p, {})
		{}

		template<class _Pp,
			bool Udummy = true,
			class = EnableIfDeleterConstructible<LValRefType<Udummy>>,
			class = EnableIfPointerConvertible<_Pp>>
		unique_ptr(_Pp p, LValRefType<Udummy> d) noexcept : pair(p, d)
		{}

		template<bool Udummy = true,
			class = EnableIfDeleterConstructible<LValRefType<Udummy>>>
		unique_ptr(nullptr_t, LValRefType<Udummy> d) noexcept : pair(nullptr, d)
		{}

		template<class Pp,
			bool Udummy = true,
			class = EnableIfDeleterConstructible<GoodRValRefType<Udummy>>,
			class = EnableIfPointerConvertible<Pp>>
		unique_ptr(Pp p, GoodRValRefType<Udummy> d) noexcept : pair(p, move(d))
		{
			static_assert(not_reference<deleter_type>::value,
				"rvalue deleter bound to reference");
		}

		template<bool Udummy = true,
			class = EnableIfDeleterConstructible<GoodRValRefType<Udummy>>>
		unique_ptr(nullptr_t, GoodRValRefType<Udummy> d) noexcept
			: pair(nullptr, move(d))
		{
			static_assert(not_reference<deleter_type>::value,
				"rvalue deleter bound to reference");
		}

		template<class Pp,
			bool Udummy = true,
			class = EnableIfDeleterConstructible<BadRValRefType<Udummy>>,
			class = EnableIfPointerConvertible<Pp>>
		unique_ptr(Pp p, BadRValRefType<Udummy> d) = delete;

		unique_ptr(unique_ptr&& u) noexcept
			: pair(u.release(), forward<deleter_type>(u.get_deleter()))
		{}

		unique_ptr&
		operator=(unique_ptr&& u) noexcept
		{
			reset(u.release());
			Faiz::get<1>(pair) = forward<deleter_type>(u.get_deleter());
			return *this;
		}

		template<class Up,
			class Ep,
			class = EnableIfMoveConvertible<unique_ptr<Up, Ep>, Up>,
			class = EnableIfDeleterConvertible<Ep>>
		unique_ptr(unique_ptr<Up, Ep>&& u) noexcept
			: pair(u.release(), forward<Ep>(u.get_deleter()))
		{}

		template<class Up,
			class Ep,
			class = EnableIfMoveConvertible<unique_ptr<Up, Ep>, Up>,
			class = EnableIfDeleterAssignable<Ep>>
		unique_ptr&
		operator=(unique_ptr<Up, Ep>&& u) noexcept
		{
			reset(u.release());
			Faiz::get<1>(pair) = forward<Ep>(u.get_deleter());
			return *this;
		}

		unique_ptr& operator=(nullptr_t) noexcept
		{
			reset();
			return *this;
		}
		~unique_ptr() noexcept
		{
			reset();
		}

		template<class Pp>
		enable_if_t<CheckArrayPointerConversion_v<Pp>>
		reset(Pp p) noexcept
		{
			pointer tmp = Faiz::get<0>(pair);
			Faiz::get<0>(pair) = p;
			if(tmp)
			{
				Faiz::get<1>(pair)(tmp);
			}
		}


		void reset(nullptr_t = nullptr) noexcept
		{
			pointer tmp = Faiz::get<0>(pair);
			Faiz::get<0>(pair) = nullptr;
			if(tmp)
			{
				Faiz::get<1>(pair)(tmp);
			}
		}


		pointer
		release() noexcept
		{
			pointer tmp = Faiz::get<0>(pair);
			Faiz::get<0>(pair) = pointer();
			return tmp;
		}


		void
		swap(unique_ptr& x) noexcept
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

		pointer
		get() const noexcept
		{
			return Faiz::get<0>(pair);
		}

		deleter_type&
		get_deleter() noexcept
		{
			return Faiz::get<1>(pair);
		}

		const deleter_type&
		get_deleter() const noexcept
		{
			return Faiz::get<1>(pair);
		}

		explicit operator bool() const noexcept
		{
			return Faiz::get<0>(pair) != nullptr;
		}
	};


	template<typename T, typename D>
	inline void
	swap(unique_ptr<T, D>& a, unique_ptr<T, D>& b) noexcept
	{
		a.swap(b);
	}


	template<typename T1, typename D1, typename T2, typename D2>
	inline bool
	operator==(const unique_ptr<T1, D1>& a, const unique_ptr<T2, D2>& b)
	{
		return (a.get() == b.get());
	}

	template<typename T1, typename D1, typename T2, typename D2>
	inline bool
	operator!=(const unique_ptr<T1, D1>& a, const unique_ptr<T2, D2>& b)
	{
		return !(a == b);
	}

	template<typename T1, typename D1, typename T2, typename D2>
	inline bool
	operator<(const unique_ptr<T1, D1>& a, const unique_ptr<T2, D2>& b)
	{
		using P1 = typename Faiz::unique_ptr<T1, D1>::pointer;
		using P2 = typename Faiz::unique_ptr<T2, D2>::pointer;
		using PCommon = common_type_t<P1, P2>;
		PCommon pT1 = a.get();
		PCommon pT2 = b.get();
		return std::less<PCommon>()(pT1, pT2);
	}

	// FIXME gcc dosn't support friend comparation functions here somehow, so I
	// have to use free function.
	template<typename T1, typename D1, typename T2, typename D2>
	inline bool
	operator>(const unique_ptr<T1, D1>& a, const unique_ptr<T2, D2>& b)
	{
		return (b < a);
	}

	template<typename T1, typename D1, typename T2, typename D2>
	inline bool
	operator<=(const unique_ptr<T1, D1>& a, const unique_ptr<T2, D2>& b)
	{
		return !(b < a);
	}

	template<typename T1, typename D1, typename T2, typename D2>
	inline bool
	operator>=(const unique_ptr<T1, D1>& a, const unique_ptr<T2, D2>& b)
	{
		return !(a < b);
	}


	template<typename T, typename D>
	inline bool
	operator==(const unique_ptr<T, D>& a, nullptr_t) noexcept
	{
		return !a;
	}

	template<typename T, typename D>
	inline bool
	operator==(nullptr_t, const unique_ptr<T, D>& a) noexcept
	{
		return !a;
	}

	template<typename T, typename D>
	inline bool
	operator!=(const unique_ptr<T, D>& a, nullptr_t) noexcept
	{
		return static_cast<bool>(a);
	}

	template<typename T, typename D>
	inline bool
	operator!=(nullptr_t, const unique_ptr<T, D>& a) noexcept
	{
		return static_cast<bool>(a);
	}

	template<typename T, typename D>
	inline bool
	operator<(const unique_ptr<T, D>& a, nullptr_t)
	{
		using pointer = typename unique_ptr<T, D>::pointer;
		return std::less<pointer>()(a.get(), nullptr);
	}

	template<typename T, typename D>
	inline bool
	operator<(nullptr_t, const unique_ptr<T, D>& b)
	{
		using pointer = typename unique_ptr<T, D>::pointer;
		pointer pT = b.get();
		return std::less<pointer>()(nullptr, pT);
	}

	template<typename T, typename D>
	inline bool
	operator>(const unique_ptr<T, D>& a, nullptr_t)
	{
		return (nullptr < a);
	}

	template<typename T, typename D>
	inline bool
	operator>(nullptr_t, const unique_ptr<T, D>& b)
	{
		return (b < nullptr);
	}

	template<typename T, typename D>
	inline bool
	operator<=(const unique_ptr<T, D>& a, nullptr_t)
	{
		return !(nullptr < a);
	}

	template<typename T, typename D>
	inline bool
	operator<=(nullptr_t, const unique_ptr<T, D>& b)
	{
		return !(b < nullptr);
	}

	template<typename T, typename D>
	inline bool
	operator>=(const unique_ptr<T, D>& a, nullptr_t)
	{
		return !(a < nullptr);
	}

	template<typename T, typename D>
	inline bool
	operator>=(nullptr_t, const unique_ptr<T, D>& b)
	{
		return !(nullptr < b);
	}

	// http://isocpp.org/files/papers/N3656.txt
	template<class T>
	struct Unique_if
	{
		using Single_object = unique_ptr<T>;
	};

	template<class T>
	struct Unique_if<T[]>
	{
		using Unknown_bound = unique_ptr<T[]>;
	};

	template<class T, Faiz::size_t N>
	struct Unique_if<T[N]>
	{
		using Known_bound = void;
	};

	// lwg 2098
	template<class T, class... Args>
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

	template<class T>
	typename Unique_if<T>::Unknown_bound
	make_unique(size_t n)
	{
		using U = remove_extent_t<T>;
		return unique_ptr<T>(new U[n]());
	}

	template<class T, class... Args>
	typename Unique_if<T>::Known_bound
	make_unique(Args&&...)
		= delete;

	template<class T, class D>
	typename enable_if<std::is_swappable_v<D>, void>::type
	swap(unique_ptr<T, D>& x, unique_ptr<T, D>& y) noexcept
	{
		x.swap(y);
	}


} // namespace Rider::Faiz

#endif
