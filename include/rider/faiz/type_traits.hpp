#include <cstddef>

#ifndef TYPE_TRAITS
#    define TYPE_TRAITS

namespace rider
{
    namespace faiz
    {
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wambiguous-ellipsis"

        template<class T, T v>
        struct integral_constant
        {
            static constexpr T value = v;
            using value_type = T;
            using type = integral_constant; // using injected-class-name
            constexpr operator value_type() const noexcept
            {
                return value;
            }
            constexpr value_type operator()() const noexcept
            {
                return value;
            } // since c++14
        };
        template<bool B>
        using bool_constant = integral_constant<bool, B>;
        using true_type = integral_constant<bool, true>;
        using false_type = integral_constant<bool, false>;


        /******************** is_function *********************/

        // primary template
        template<class>
        struct is_function : false_type
        {
        };

        // specialization for regular functions
        template<class Ret, class... Args>
        struct is_function<Ret(Args...)> : true_type
        {
        };

        // specialization for variadic functions such as printf
        template<class Ret, class... Args>
        struct is_function<Ret(Args......)> : true_type
        {
        };

        // specialization for function types that have cv-qualifiers
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) const> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) volatile> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) const volatile> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args..., ...) const> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) volatile> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args..., ...) const volatile> : true_type
        {
        };

        // specialization for function types that have ref-qualifiers
        template<class Ret, class... Args>
        struct is_function<Ret(Args...)&> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) const&> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) volatile&> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) const volatile&> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......)&> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) const&> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) volatile&> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) const volatile&> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) &&> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) const&&> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) volatile&&> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) const volatile&&> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) &&> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) const&&> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) volatile&&> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) const volatile&&> : true_type
        {
        };

        // specializations for noexcept versions of all the above (C++17 and
        // later)
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) const noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) volatile noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) const volatile noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) const noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args..., ...) volatile noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) const volatile noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) & noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) const& noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) volatile& noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) const volatile& noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) & noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) const& noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) volatile& noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) const volatile& noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) && noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) const&& noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) volatile&& noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args...) const volatile&& noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) && noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) const&& noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) volatile&& noexcept> : true_type
        {
        };
        template<class Ret, class... Args>
        struct is_function<Ret(Args......) const volatile&& noexcept> : true_type
        {
        };

        template<class T>
        inline constexpr bool is_function_v = is_function<T>::value;

        /******************** remove *********************/


        // If the type T is a reference type,
        // provides the member typedef type which is the type referred to by T.
        // Otherwise type is T.
        template<class T>
        struct remove_reference
        {
            using type = T;
        };
        template<class T>
        struct remove_reference<T&>
        {
            using type = T;
        };
        template<class T>
        struct remove_reference<T&&>
        {
            using type = T;
        };
        template<class T>
        using remove_reference_t = typename remove_reference<T>::type;

        template<class T>
        struct remove_const
        {
            using type = T;
        };
        template<class T>
        struct remove_const<const T>
        {
            using type = T;
        };

        template<class T>
        struct remove_volatile
        {
            using type = T;
        };
        template<class T>
        struct remove_volatile<volatile T>
        {
            using type = T;
        };

        // If T is an array of some type X, provides the member typedef type equal to X, otherwise type is T.
        // Note that if T is a multidimensional array, only the first dimension is removed.
        template<class T>
        struct remove_extent
        {
            using type = T;
        };

        template<class T>
        struct remove_extent<T[]>
        {
            using type = T;
        };

        template<class T, size_t N>
        struct remove_extent<T[N]>
        {
            using type = T;
        };

        template<class T>
        struct remove_cv
        {
            using type = typename remove_volatile<typename remove_const<T>::type>::type;
        };


        // Provides the member typedef type which is the type pointed to by T,
        // or, if T is not a pointer, then type is the same as T.
        // ```cpp
        // print_is_same<int, remove_pointer<int*>::type>();  // true
        // print_is_same<int, remove_pointer<int**>::type>(); // false
        // ```
        template<class T>
        struct remove_pointer
        {
            using type = T;
        };
        template<class T>
        struct remove_pointer<T*>
        {
            using type = T;
        };
        template<class T>
        struct remove_pointer<T* const>
        {
            using type = T;
        };
        template<class T>
        struct remove_pointer<T* volatile>
        {
            using type = T;
        };
        template<class T>
        struct remove_pointer<T* const volatile>
        {
            using type = T;
        };


        template<class T>
        using remove_cv_t = typename remove_cv<T>::type;
        template<class T>
        using remove_const_t = typename remove_const<T>::type;
        template<class T>
        using remove_volatile_t = typename remove_volatile<T>::type;
        template<class T>
        using remove_extent_t = typename remove_extent<T>::type;
        template<class T>
        using remove_pointer_t = typename remove_pointer<T>::type;

        // If the type **T** is a reference type, provides the member typedef type which is the type referred to by
        // **T** with its topmost cv-qualifiers removed. Otherwise type is **T** with its topmost cv-qualifiers removed.
        //
        // Removing cvref does most of what decay does, but doesn't convert function types and array types to pointers.

        template<class T>
        struct remove_cvref
        {
            using type = remove_cv_t<remove_reference_t<T>>;
        };
        template<class T>
        using remove_cvref_t = typename remove_cvref<T>::type;

        /******************** is *********************/

        // If T and U name the same type (including const/volatile qualifications), provides the member constant value
        // equal to true. Otherwise value is false.
        template<class T, class U>
        struct is_same : false_type
        {
        };

        // If T and U name the same type (including const/volatile qualifications), provides the member constant value
        // equal to true. Otherwise value is false.
        template<class T>
        struct is_same<T, T> : true_type
        {
        };

        // Checks whether T is a void type. Provides the member constant value
        // that is equal to true, if T is the type void, const void, volatile
        // void, or const volatile void. Otherwise, value is equal to false.
        template<class T>
        struct is_void : is_same<void, typename remove_cv<T>::type>
        {
        };

        template<typename T>
        struct is_pointer_helper : public false_type
        {
        };
        template<typename T>
        struct is_pointer_helper<T*> : public true_type
        {
        };
        // Checks whether T is **a pointer to object** or **a pointer to function** (but not a pointer to member/member
        // function). Provides the member constant value which is equal to true, if T is a object/function pointer type.
        // Otherwise, value is equal to false.
        template<typename T>
        struct is_pointer : public is_pointer_helper<remove_cv_t<T>>
        {
        };

        template<class T>
        struct is_lvalue_reference : false_type
        {
        };
        template<class T>
        struct is_lvalue_reference<T&> : true_type
        {
        };

        template<class T>
        inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;
        template<class T>
        struct is_rvalue_reference : false_type
        {
        };
        template<class T>
        struct is_rvalue_reference<T&&> : true_type
        {
        };
        template<class T>
        inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

        template<class T>
        struct is_reference : false_type
        {
        };
        template<class T>
        struct is_reference<T&> : true_type
        {
        };
        template<class T>
        struct is_reference<T&&> : true_type
        {
        };
        template<class T>
        inline constexpr bool is_reference_v = is_reference<T>::value;

        // If T is an object type (that is any possibly cv-qualified type other than function, reference, or void
        // types), provides the member constant value equal true. For any other type, value is false.
        // ```cpp
        // !is_reference<T>::value && !is_void<T>::value && !is_function<T>::value
        // ```
        // OR
        // ```cpp
        // is_scalar<T>::value || is_array<T>::value  || is_union<T>::value  || is_class<T>::value>
        // ```
        template<typename T>
        struct is_object
            : integral_constant<bool, !is_reference<T>::value && !is_void<T>::value && !is_function<T>::value>
        {
        };

        template<typename T>
        struct is_referenceable : integral_constant<bool, is_reference<T>::value || is_object<T>::value>
        {
        };
        template<typename R, typename... Args>
        struct is_referenceable<R(Args...)> : public true_type
        {
        };
        template<typename R, typename... Args>
        struct is_referenceable<R(Args..., ...)> : public true_type
        {
        };


        template<class T, unsigned N = 0>
        struct extent : integral_constant<size_t, 0>
        {
        };

        template<class T>
        struct extent<T[], 0> : integral_constant<size_t, 0>
        {
        };

        template<class T, unsigned N>
        struct extent<T[], N> : extent<T, N - 1>
        {
        };

        template<class T, size_t I>
        struct extent<T[I], 0> : integral_constant<size_t, I>
        {
        };

        template<class T, size_t I, unsigned N>
        struct extent<T[I], N> : extent<T, N - 1>
        {
        };

        template<typename T, bool = is_referenceable<T>::value>
        struct add_lvalue_reference
        {
            using type = T;
        };
        template<typename T>
        struct add_lvalue_reference<T, true>
        {
            using type = T&;
        };

        template<typename T>
        using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

        template<typename T, bool = is_referenceable<T>::value>
        struct add_rvalue_reference
        {
            using type = T;
        };
        template<typename T>
        struct add_rvalue_reference<T, true>
        {
            using type = T&&;
        };

        template<typename T>
        using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

        /// Unusable default value for T. Use with decltype.
        template<typename T>
        add_rvalue_reference_t<T> declval(void) noexcept;

        template<bool B, class T = void>
        struct enable_if
        {
        };

        template<class T>
        struct enable_if<true, T>
        {
            using type = T;
        };

        template<class T>
        struct is_array : false_type
        {
        };

        template<class T>
        struct is_array<T[]> : true_type
        {
        };

        template<class T, size_t N>
        struct is_array<T[N]> : true_type
        {
        };

        template<typename F, typename T, bool = is_void<F>::value || is_function<T>::value || is_array<T>::value>
        class is_convertible : public integral_constant<bool, is_void<T>::value>
        {
        };
        template<typename F, typename T>
        class is_convertible<F, T, false>
        {
            template<typename TT>
            static void test_aux(TT);
            template<typename FF, typename TT, typename = decltype(test_aux<TT>(declval<FF>()))>
            static true_type test(int);
            template<typename FF, typename TT>
            static false_type test(...);

        public:
            using type = decltype(test<F, T>(0));
        };


        // Provides member typedef type, which is defined as T if B is true at compile time, or as F if B is false.
        template<bool B, class T, class F>
        struct conditional
        {
            using type = T;
        };

        // Provides member typedef type, which is defined as T if B is true at compile time, or as F if B is false.
        template<class T, class F>
        struct conditional<false, T, F>
        {
            using type = F;
        };

        //  Provides member typedef type, which is defined as T if B is true at compile time, or as F if B is false.
        template<bool B, class T, class F>
        using conditional_t = typename conditional<B, T, F>::type;

        // If T is an array type, provides the member constant value equal to the number of dimensions of the array.
        // For any other type, value is 0.
        template<class T>
        struct rank : public integral_constant<size_t, 0>
        {
        };
        // If T is an array type, provides the member constant value equal to the number of dimensions of the array.
        // For any other type, value is 0.
        template<class T>
        struct rank<T[]> : public integral_constant<size_t, rank<T>::value + 1>
        {
        };
        // If T is an array type, provides the member constant value equal to the number of dimensions of the array.
        // For any other type, value is 0.
        template<class T, size_t N>
        struct rank<T[N]> : public integral_constant<size_t, rank<T>::value + 1>
        {
        };
        template<class T>
        inline constexpr size_t rank_v = rank<T>::value;


        namespace detail
        {
            template<class T, bool is_function_type = false>
            struct add_pointer
            {
                using type = typename remove_reference<T>::type*;
            };

            template<class T>
            struct add_pointer<T, true>
            {
                using type = T;
            };

            template<class T, class... Args>
            struct add_pointer<T(Args...), true>
            {
                using type = T (*)(Args...);
            };

            template<class T, class... Args>
            struct add_pointer<T(Args..., ...), true>
            {
                using type = T (*)(Args..., ...);
            };

        } // namespace detail

        // If **T** is a reference type, then provides the member typedef type which is a pointer to the referred type.
        // Otherwise, if **T** names an object type, a function type that is not **cv-** or **ref-qualified** (since
        // C++17), or a (possibly **cv-qualified**) void type, provides the member typedef type which is the type T*.
        // Otherwise (if T is a **cv-** or **ref-qualified** function type), provides the member typedef type which is
        // the type **T**
        //
        // * Name:	Definition
        // * type:	pointer to T or to the type referenced by T
        template<class T>
        struct add_pointer : detail::add_pointer<T, is_function<T>::value>
        {
        };
        template<class T>
        using add_pointer_t = typename add_pointer<T>::type;

        // Applies lvalue-to-rvalue, array-to-pointer,
        // and function-to-pointer implicit conversions to the type **T**,
        // removes cv-qualifiers, and defines the resulting type as the member typedef type. Formally:
        // * If **T** names the type "**array of U**" or "**reference to array of U**", the member typedef type is
        // **U***.
        // * Otherwise, if **T** is a function type **F** or a reference thereto, the member typedef type is
        // **add_pointer<F>::type**.
        // * Otherwise, the member typedef type is **remove_cv<remove_reference<T>::type>::type**
        //
        //```cpp
        // #include <iostream>
        // #include <type_traits>
        // template <typename T, typename U>
        // struct decay_equiv :
        //     is_same<typename std::decay<T>::type, U>::type
        // {};
        // int main()
        // {
        //     std::cout << std::boolalpha
        //               << decay_equiv<int, int>::value << '\n'
        //               << decay_equiv<int&, int>::value << '\n'
        //               << decay_equiv<int&&, int>::value << '\n'
        //               << decay_equiv<const int&, int>::value << '\n'
        //               << decay_equiv<int[2], int*>::value << '\n'
        //               << decay_equiv<int(int), int(*)(int)>::value << '\n';
        // }
        // ```
        // true
        // true
        // true
        // true
        // true
        // true
        //
        template<class T>
        struct decay
        {
        private:
            using U = typename remove_reference<T>::type;

        public:
            using type = typename conditional<is_array<U>::value,
                typename remove_extent<U>::type*,
                typename conditional<is_function<U>::value,
                    typename add_pointer<U>::type,
                    typename remove_cv<U>::type>::type>::type;
        };
        template<class T>
        using decay_t = typename decay<T>::type;
#    pragma clang diagnostic pop
    } // namespace faiz
} // namespace rider

#endif
