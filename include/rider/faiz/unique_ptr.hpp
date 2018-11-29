#ifndef UNIQUE_PTR
#	define UNIQUE_PTR
#	include "rider/faiz/compressed_pair.hpp"
#	include "rider/faiz/functional.hpp"
#	include "rider/faiz/ptr_traits.hpp"
#	include "rider/faiz/type_traits.hpp"
#	include "rider/faiz/utility.hpp"

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

namespace rider::faiz
{
	// faiz is a standard library like library

	// Tells if the Deleter type has a typedef for pointer to T. If so then
	// return it, else return T*. The large majority of the time the pointer
	// type will be T*. The C++11 Standard requires that scoped_ptr let the
	// deleter define the pointer type.
	//
	// Example usage:
	// ```
	//     typedef typename unique_pointer_type<int, SomeDeleter>::type
	//     pointer
	//
	template<typename T, typename Deleter>
	class unique_pointer_type
	{
		template<typename U>
		static typename U::pointer
		test(typename U::pointer*);

		template<typename U>
		static T*
		test(...);

	public:
		using type = decltype(test<faiz::remove_reference_t<Deleter>>(0));
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
		bool = faiz::is_same<typename faiz::remove_cv<typename pointer_traits<
								 P1>::element_type>::type,
			typename faiz::remove_cv<
				typename pointer_traits<P2>::element_type>::type>::value>
	struct is_array_cv_convertible_impl : public faiz::is_convertible<P1, P2>
	{}; // Return true if P1 is convertible to P2.

	template<typename P1, typename P2>
	struct is_array_cv_convertible_impl<P1, P2, false> : public faiz::false_type
	{}; // P1's underlying type is not the same as P2's, so it can't be
		// converted, even if P2 refers to a subclass of P1. Parent == Child,
		// but Parent[] != Child[]

	template<typename P1,
		typename P2,
		bool = faiz::is_scalar_v<P1> && !faiz::is_pointer_v<P1>>
	struct is_array_cv_convertible : public is_array_cv_convertible_impl<P1, P2>
	{};

	template<typename P1, typename P2>
	struct is_array_cv_convertible<P1, P2, true> : public faiz::false_type
	{}; // P1 is scalar not a pointer, so it can't be converted to a pointer.


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

	template<typename T, typename T_pointer, typename U, typename U_pointer>
	struct is_safe_array_conversion
		: public faiz::integral_constant<bool,
			  faiz::is_convertible_v<U_pointer,
				  T_pointer> && faiz::is_array_v<U> && (!faiz::is_pointer_v<U_pointer> || !is_pointer_v<T_pointer> || !faiz::is_derived_of_v<T, faiz::remove_extent_t<U>>)>
	{};


	template<class T>
	struct default_delete
	{
		static_assert(!is_function_v<T>,
			"Oops, default_delete cannot be instantiated for function "
			"types, please give me a new type");

		// Constructs a default_delete object.
		constexpr default_delete() noexcept = default;

		// Constructs a default_delete object from another
		// default_delete object. This constructor will only
		// participate in overload resolution if U* is implicitly
		// convertible to T*.
		template<class U>
		default_delete(const default_delete<U>&,
			enable_if_t<is_convertible_v<U*, T*>>* = 0) noexcept;

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
			static_assert(!is_void_v<T>,
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
	template<class T>
	struct default_delete<T[]>
	{
	private:
		// helper struct for default_delete<T[]>
		template<class U>
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
		template<class U>
		default_delete(const default_delete<U[]>&,
			typename EnableIfConvertible<U>::type* = 0) noexcept
		{}

		//  At the point in the code the operator() is called, the type must
		// be complete. In some implementations a static_assert is used to
		// make sure this is the case. The reason for this requirement is
		// that calling delete on an incomplete type is undefined behavior
		// in C++ if the complete class type has a nontrivial destructor or
		// a deallocation function, as the compiler has no way of knowing
		// whether such functions exist and must be invoked.
		template<class U>
		typename EnableIfConvertible<U>::type
		operator()(U* ptr) const noexcept
		{
			static_assert(
				sizeof(T) > 0, "default_delete can not delete incomplete type");
			static_assert(
				!is_void_v<T>, "default_delete can not delete void type");
			delete[] ptr;
		}
	};

	// "I'm returning a managed resource to you. If you don't explicitly
	// capture the return value, then the resource will be cleaned up. If
	// you do, then you now have exclusive ownership of that resource."
	//
	// unique_ptr is a smart pointer that owns and manages another
	// object through a pointer and disposes of that object when the
	// unique_ptr goes out of scope.
	//```c++
	// unique_ptr<T> myPtr(new T);       // Okay
	// unique_ptr<T> myOtherPtr = myPtr; // Error: Can't copy unique_ptr
	//
	// unique_ptr<T> myPtr(new T);                  // Okay
	// unique_ptr<T> myOtherPtr = move(myPtr); // Okay, resource now
	// stored in myOtherPtr
	//```
	// Similarly, you can do something like this:
	// ```c++
	// unique_ptr<T> MyFunction() {
	//     unique_ptr<T> myPtr(/* ... */);
	//     /* ... */
	//     return myPtr;
	// }
	//```
	//
	// for custom deleter, just With:
	//```c++
	// unique_ptr<Bar, decltype(&destroy)> ptr_;
	//```
	/* https://stackoverflow.com/a/6089065/6949852:
	Complete type requirements for
	//                           unique_ptr and shared_ptr
	+------------------------+---------------+---------------+
	|          P()           |      I        |      I        |
	|  default constructor   |               |               |
	+------------------------+---------------+---------------+
	|      P(const P&)       |     N/A       |      I        |
	|    copy constructor    |               |               |
	+------------------------+---------------+---------------+
	|         P(P&&)         |      I        |      I        |
	|    move constructor    |               |               |
	+------------------------+---------------+---------------+
	|         ~P()           |      C        |      I        |
	|       destructor       |               |               |
	+------------------------+---------------+---------------+
	|         P(A*)          |      I        |      C        |
	+------------------------+---------------+---------------+
	|  operator=(const P&)   |     N/A       |      I        |
	|    copy assignment     |               |               |
	+------------------------+---------------+---------------+
	|    operator=(P&&)      |      C        |      I        |
	|    move assignment     |               |               |
	+------------------------+---------------+---------------+
	|        reset()         |      C        |      I        |
	+------------------------+---------------+---------------+
	|       reset(A*)        |      C        |      C        |
	+------------------------+---------------+---------------+
	// std::unique_ptr has memory overhead only if you provide it with some
	non-trivial deleter.
	*/
	template<typename T, class Deleter = default_delete<T>>
	class unique_ptr
	{
	public:
		// Deleter, the function object or lvalue reference to function or
		// to function object, to be called from the destructor
		using deleter_type = Deleter;
		// A synonym for the template parameter T.
		using element_type = T;
		// A synonym for `Delete::pointer` if defined, otherwise `T *`.
		// IOW, `faiz::remove_reference<Deleter>::type::pointer` if that
		// type exists, otherwise `T*`. Must satisfy *NullablePointer*
		using pointer = typename rider::faiz::unique_pointer_type<element_type,
			deleter_type>::type;

		static_assert(!rider::faiz::is_rvalue_reference_v<deleter_type>,
			"Come on, rvalue_reference is not what I want");

		unique_ptr(unique_ptr& up) = delete;

		unique_ptr&
		operator=(unique_ptr& up)
			= delete;

		unique_ptr&
		operator=(pointer p)
			= delete;


	protected:
		compressed_pair<pointer, deleter_type> pair;

	public:
		constexpr unique_ptr() noexcept : pair{pointer{}}
		{
			static_assert(!faiz::is_pointer_v<deleter_type>,
				"WTF, If this constructor is instantiated with a pointer "
				"type or reference type, for the template argument D, the "
				"program is ill-formed.");
			static_assert(!is_reference_v<deleter_type>,
				"WTF, If this constructor is "
				"instantiated with a pointer "
				"type or reference type, for "
				"the template argument D, the "
				"program is ill-formed.");
		}

		// If this constructor is instantiated with a pointer type or
		// reference type for the template argument D, the program is
		// ill-formed.
		constexpr unique_ptr(faiz::nullptr_t) noexcept : pair{}
		{
			static_assert(!faiz::is_pointer_v<deleter_type>,
				"WTF, If this constructor is instantiated with a pointer "
				"type or reference type, for the template argument D, the "
				"program is ill-formed.");
			static_assert(!is_reference_v<deleter_type>,
				"WTF, If this constructor is "
				"instantiated with a pointer "
				"type or reference type, for "
				"the template argument D, the "
				"program is ill-formed.");
		}

		explicit unique_ptr(pointer ptr) noexcept : pair{ptr}
		{
			static_assert(!faiz::is_pointer_v<deleter_type>,
				"WTF, If this constructor is instantiated with a pointer "
				"type or reference type, for the template argument D, the "
				"program is ill-formed.");
			static_assert(!is_reference_v<deleter_type>,
				"WTF, If this constructor is "
				"instantiated with a pointer "
				"type or reference type, for "
				"the template argument D, the "
				"program is ill-formed.");
		}

		template<class U, class E>
		unique_ptr(unique_ptr<U, E>&& up) noexcept
			: pair(up.release(),
				  rider::faiz::forward<deleter_type>(up.get_deleter()))
		{}

		unique_ptr(pointer ptr,
			typename faiz::conditional_t<faiz::is_reference_v<deleter_type>,
				deleter_type,
				typename faiz::add_lvalue_reference_t<const deleter_type>>
				deleter) noexcept
			: pair(ptr, deleter)
		{}

		unique_ptr(pointer ptr,
			faiz::remove_reference_t<deleter_type>&& deleter) noexcept
			: pair(ptr, faiz::move(deleter))
		{}

		template<typename U, typename E>
		unique_ptr(unique_ptr<U, E>&& u,
			faiz::enable_if_t<
				!faiz::is_array_v<
					U> && faiz::is_convertible_v<typename unique_ptr<U, E>::pointer, pointer> && faiz::is_convertible_v<E, deleter_type> && (faiz::is_same_v<deleter_type, E> || !faiz::is_reference_v<deleter_type>),
				E>* = 0) noexcept
			: pair(u.release(), faiz::forward<E>(u.get_deleter()))
		{}

		// Returns the deleter object which would be used for destruction of
		// the managed object.
		deleter_type&
		get_deleter() noexcept
		{
			return pair.second();
		}
		// Returns the deleter object which would be used for destruction of
		// the managed object.
		const Deleter&
		get_deleter() const noexcept
		{
			return pair.second();
		}

		// Releases the ownership of the managed object if any. **get()**
		// returns nullptr after the call.
		pointer
		release() noexcept
		{
			pointer const pTemp = pair.first();
			pair.first() = pointer();
			return pTemp;
		}

		// Returns a pointer to the managed object or **nullptr** if no
		// object is owned.
		pointer
		get() const noexcept
		{
			return pair.first();
		}

		void
		reset(pointer ptr = pointer()) noexcept
		{
			if(ptr != pair.first())
			{
				get_deleter()(pair.first());
				pair.first() = ptr;
			}
		}

		// Swaps the managed objects and associated deleters of *this and
		// another unique_ptr object other.
		template<class U, class E>
		void
		swap(unique_ptr<U, E>& x) noexcept
		{
			pair.swap(x.pair);
		}

		// Checks whether *this owns an object, i.e. whether get() !=
		// nullptr.
		operator bool() const noexcept
		{
			return pair.get() != nullptr;
		}

		// may throw, e.g. if pointer defines a throwing operator*
		add_lvalue_reference_t<T> operator*() const

		{
			return *pair.first();
		}

		pointer operator->() const noexcept
		{
			return pair.first();
		}


		template<typename U, typename E>
		enable_if_t<
			!is_array_v<
				U> && is_convertible_v<typename unique_ptr<U, E>::pointer, pointer> && faiz::is_assignable_v<deleter_type&, E&&>,
			unique_ptr&>
		operator=(unique_ptr<U, E>&& u) noexcept
		{
			reset(u.release());
			pair.second() = faiz::move(faiz::forward<E>(u.get_deleter()));
			return *this;
		}

		unique_ptr&
		operator=(unique_ptr&& u) noexcept
		{
			reset(u.release());
			pair.second() = faiz::forward<deleter_type>(u.get_deleter());
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
	template<typename T, typename Deleter>
	class unique_ptr<T[], Deleter>
	{

	public:
		using deleter_type = Deleter;
		using element_type = T;
		using pointer =
			typename unique_pointer_type<element_type, deleter_type>::type;
		/// These functions are private in order to prevent copying, for safety.
		unique_ptr(const unique_ptr&) = delete;
		unique_ptr&
		operator=(const unique_ptr&)
			= delete;
		unique_ptr&
		operator=(pointer arr)
			= delete;

	protected:
		faiz::compressed_pair<pointer, deleter_type> pair;

	public:
		constexpr unique_ptr() noexcept : pair(pointer())
		{
			static_assert(!faiz::is_pointer_v<deleter_type>,
				"unique_ptr deleter default-constructed with null pointer. Use "
				"a different constructor or "
				"change your deleter to a class.");
		}

		constexpr unique_ptr(faiz::nullptr_t) noexcept : pair(pointer())
		{
			static_assert(!faiz::is_pointer_v<deleter_type>,
				"unique_ptr deleter default-constructed with null pointer. Use "
				"a different constructor or "
				"change your deleter to a class.");
		}

		template<typename P> // Pointers to types derived from T are rejected by
							 // the constructors, and by reset.
		explicit unique_ptr(P arr,
			typename faiz::enable_if<
				is_array_cv_convertible<P, pointer>::value>::type* = 0) noexcept
			: pair(arr)
		{
			static_assert(!faiz::is_pointer_v<deleter_type>,
				"unique_ptr deleter default-constructed with null pointer. Use "
				"a different constructor or "
				"change your deleter to a class.");
		}

		template<typename P>
		unique_ptr(P arr,
			typename faiz::conditional_t<faiz::is_reference_v<deleter_type>,
				deleter_type,
				faiz::add_lvalue_reference_t<const deleter_type>> deleter,
			typename faiz::enable_if<
				is_array_cv_convertible<P, pointer>::value>::type* = 0) noexcept
			: pair(arr, deleter)
		{}

		template<typename P>
		unique_ptr(P arr,
			faiz::remove_reference_t<deleter_type>&& deleter,
			typename faiz::enable_if<
				is_array_cv_convertible<P, pointer>::value>::type* = 0) noexcept
			: pair(arr, faiz::move(deleter))
		{
			static_assert(!faiz::is_reference_v<deleter_type>,
				"deleter_type reference refers to an rvalue deleter. The "
				"reference will probably become invalid before "
				"used. Change the deleter_type to not be a reference or "
				"construct with permanent deleter.");
		}

		unique_ptr(unique_ptr&& x) noexcept
			: pair(x.release(), faiz::forward<deleter_type>(x.get_deleter()))
		{}

		template<typename U, typename E>
		unique_ptr(unique_ptr<U, E>&& u,
			enable_if_t<is_safe_array_conversion<T,
							pointer,
							U,
							typename unique_ptr<U, E>::pointer>::value
				&& faiz::is_convertible_v<E,
					   deleter_type> && (!faiz::is_reference_v<deleter_type> || faiz::is_same_v<E, deleter_type>)>* = 0) noexcept
			: pair(u.release(), faiz::forward<E>(u.get_deleter()))
		{}

		unique_ptr&
		operator=(unique_ptr&& x) noexcept
		{
			reset(x.release());
			pair.second()
				= faiz::move(faiz::forward<deleter_type>(x.get_deleter()));
			return *this;
		}

		template<typename U, typename E>
		enable_if_t<is_safe_array_conversion<T,
						pointer,
						U,
						typename unique_ptr<U, E>::pointer>::value
				&& is_assignable_v<deleter_type&, E&&>,
			unique_ptr&>
		operator=(unique_ptr<U, E>&& u) noexcept
		{
			reset(u.release());
			pair.second() = faiz::move(faiz::forward<E>(u.get_deleter()));
			return *this;
		}

		unique_ptr& operator=(faiz::nullptr_t) noexcept
		{
			reset();
			return *this;
		}

		~unique_ptr() noexcept
		{
			reset();
		}

		void reset(nullptr_t = nullptr) noexcept
		{
			pointer tmp = pair.first();
			pair.first() = nullptr;
			if(tmp)
				pair.second()(tmp);
		}


		pointer
		release() noexcept
		{
			pointer const tmp = pair.first();
			pair.first() = pointer();
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
			static_assert(faiz::extent_v<T> == 0 || i < faiz::extent_v<T>);
			static_assert(pair.first);
			return pair.first()[i];
		}

		pointer
		get() const noexcept
		{
			return pair.first();
		}

		deleter_type&
		get_deleter() noexcept
		{
			return pair.second();
		}

		const deleter_type&
		get_deleter() const noexcept
		{
			return pair.second();
		}

		explicit operator bool() const noexcept
		{
			return (pair.first() != pointer());
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
		using P1 = typename faiz::unique_ptr<T1, D1>::pointer;
		using P2 = typename faiz::unique_ptr<T2, D2>::pointer;
		using PCommon = faiz::common_type_t<P1, P2>;
		PCommon pT1 = a.get();
		PCommon pT2 = b.get();
		return faiz::less<PCommon>()(pT1, pT2);
	}

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
	operator==(const unique_ptr<T, D>& a, faiz::nullptr_t) noexcept
	{
		return !a;
	}

	template<typename T, typename D>
	inline bool
	operator==(faiz::nullptr_t, const unique_ptr<T, D>& a) noexcept
	{
		return !a;
	}

	template<typename T, typename D>
	inline bool
	operator!=(const unique_ptr<T, D>& a, faiz::nullptr_t) noexcept
	{
		return static_cast<bool>(a);
	}

	template<typename T, typename D>
	inline bool
	operator!=(faiz::nullptr_t, const unique_ptr<T, D>& a) noexcept
	{
		return static_cast<bool>(a);
	}

	template<typename T, typename D>
	inline bool
	operator<(const unique_ptr<T, D>& a, faiz::nullptr_t)
	{
		using pointer = typename unique_ptr<T, D>::pointer;
		return faiz::less<pointer>()(a.get(), nullptr);
	}

	template<typename T, typename D>
	inline bool
	operator<(faiz::nullptr_t, const unique_ptr<T, D>& b)
	{
		using pointer = typename unique_ptr<T, D>::pointer;
		pointer pT = b.get();
		return faiz::less<pointer>()(nullptr, pT);
	}

	template<typename T, typename D>
	inline bool
	operator>(const unique_ptr<T, D>& a, faiz::nullptr_t)
	{
		return (nullptr < a);
	}

	template<typename T, typename D>
	inline bool
	operator>(faiz::nullptr_t, const unique_ptr<T, D>& b)
	{
		return (b < nullptr);
	}

	template<typename T, typename D>
	inline bool
	operator<=(const unique_ptr<T, D>& a, faiz::nullptr_t)
	{
		return !(nullptr < a);
	}

	template<typename T, typename D>
	inline bool
	operator<=(faiz::nullptr_t, const unique_ptr<T, D>& b)
	{
		return !(b < nullptr);
	}

	template<typename T, typename D>
	inline bool
	operator>=(const unique_ptr<T, D>& a, faiz::nullptr_t)
	{
		return !(a < nullptr);
	}

	template<typename T, typename D>
	inline bool
	operator>=(faiz::nullptr_t, const unique_ptr<T, D>& b)
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

	template<class T, faiz::size_t N>
	struct Unique_if<T[N]>
	{
		using Known_bound = void;
	};

	// lwg 2098
	template<class T, class... Args>
	typename Unique_if<T>::Single_object
	make_unique(Args&&... args)
	{
		if constexpr(faiz::is_constructible<T, Args...>())
		{
			return unique_ptr<T>(new T(faiz::forward<Args>(args)...));
		}
		else
		{
			return unique_ptr<T>(new T{faiz::forward<Args>(args)...});
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
#endif

} // namespace rider::faiz
