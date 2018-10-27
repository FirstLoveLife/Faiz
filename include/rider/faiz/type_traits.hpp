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

        //
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

        template<class T>
        struct remove_cv
        {
            using type = typename remove_volatile<typename remove_const<T>::type>::type;
        };

        template<class T>
        using remove_cv_t = typename remove_cv<T>::type;
        template<class T>
        using remove_const_t = typename remove_const<T>::type;
        template<class T>
        using remove_volatile_t = typename remove_volatile<T>::type;


        /******************** is *********************/

        //
        template<class T, class U>
        struct is_same : false_type
        {
        };

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
            typedef T type;
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

        template<typename T, typename U>
        struct is_swappable_with
            : public integral_constant<bool, is_convertible<U, T>::value && is_convertible<T, U>::value>
        {
        };
        template<typename T, typename U>
        struct is_nothrow_swappable_with
            : public integral_constant<bool, is_convertible<U, T>::value && is_convertible<T, U>::value>
        {
        };


#    pragma clang diagnostic pop
    } // namespace faiz
} // namespace rider

#endif
