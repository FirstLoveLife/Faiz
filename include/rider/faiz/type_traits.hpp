 #ifndef TYPE_TRAITS
 #define TYPE_TRAITS
 #include "rider/faiz/cstddef.hpp" // for size_t
 #include "rider/faiz/faiz.hpp"
 #include "rider/faiz/faiz_fwd.hpp"
 #include <boost/predef.h>
 #include <type_traits>
 /*
 Don't implement:
 */

 // Most here are forward declaration here.

 namespace rider::faiz
 {
     // Provides member typedef type, which is defined as T if B is true at
     // compile time, or as F if B is false.
     template<bool B, typename T, typename F>
     struct conditional : type_identity<T>
     {};

     // Provides member typedef type, which is defined as T if B is true at
     // compile time, or as F if B is false.
     template<typename T, typename F>
     struct conditional<false, T, F> : type_identity<F>
     {};

     //  Provides member typedef type, which is defined as T if B is true at
     //  compile time, or as F if B is false.
     template<bool B, class T, typename F>
     using conditional_t = _t<conditional<B, T, F>>;

     // forward declaration
     // If the imaginary function definition To `test() { return
     // std::declval<From>(); }` is well-formed, (that is, either
     // `faiz::declval<From>()` can be converted to To using implicit
     // conversions, or both From and To are possibly `cv-qualified void`),
     // provides the member constant value equal to true. Otherwise value is
     // false. For the purposes of this check, the use of `faiz::declval` in the
     // return statement is not considered an odr-use. Access checks are
     // performed as if from a context unrelated to either type. Only the
     // validity of the immediate context of the expression in the return
     // statement (including conversions to the return type) is considered.
     template<typename From, class To>
     struct is_convertible;
     template<typename From, class To>
     inline constexpr bool is_convertible_v = is_convertible<From, To>::value;

     namespace detail
     {

         template<typename Default,
             class AlwaysVoid,
             template<typename...>
             class Op,
             class... Args>
         struct detector : type_identity<Default>
         {
             using value_t = faiz::false_;
         };

         template<typename Default, template<typename...> class Op, class... Args>
         struct detector<Default, void_t<Op<Args...>>, Op, Args...>
 #if BOOST_COMP_GNUC
             : type_identity<Op<Args...>>
 #endif
         {
             using value_t = faiz::true_;
 #if BOOST_COMP_CLANG
             using type = Op<Args...>;
 #endif
         };

     } // namespace detail

     // nonesuch cannot be constructed, destroyed, or copied in the usual way.
     // However, it is an aggregate and therefore can be constructed (presumably
     // unintentionally) via aggregate initialization in contexts where the
     // destructor's availability is not an issue, such as a new-expression: new
     // std::experimental::nonesuch{}.
     //
     // This type was inspired by, and patterned after, the internal type __nat
     // (which we believe is an acronym for “not a type”) found in libc++.
     struct nonesuch
     {
         ~nonesuch() = delete;
         nonesuch(nonesuch const&) = delete;
         void
         operator=(nonesuch const&)
             = delete;
     };


     // The alias template `is_detected` is equivalent to typename
     // `detected_or<faiz::nonesuch, Op, Args...>::value_t`. It is an
     // alias for `faiz::true_type` if the `template-id Op<Args...>` denotes a
     // valid type; otherwise it is an alias for `faiz::false_type`.
     template<template<typename...> class Op, class... Args>
     using is_detected =
         typename detail::detector<nonesuch, void, Op, Args...>::value_t;


     // The alias template `detected_t` is equivalent to typename
     // `detected_or<faiz::nonesuch, Op, Args...>::type`. It is an
     // alias for `Op<Args...>` if that template-id denotes a valid type;
     // otherwise it is an alias for the class `faiz::nonesuch`.
     template<template<typename...> class Op, class... Args>
     using detected_t = _t<detail::detector<nonesuch, void, Op, Args...>>;

     // The alias template `detected_or` is an alias for an unspecified class
     // type with two public member typedefs `value_t` and type, which are
     // defined as follows:
     //
     // - If the template-id `Op<Args...>` denotes a valid type,
     // then `value_t`
     // is an alias for `faiz::true_type`, and type is an alias for
     // `Op<Args...>`;
     // - Otherwise, `value_t` is an alias for `faiz::false_type` and type is
     // an alias for `Default`.
     template<typename Default, template<typename...> class Op, class... Args>
     using detected_or = detail::detector<Default, void, Op, Args...>;

     template<template<typename...> class Op, class... Args>
     constexpr bool is_detected_v = is_detected<Op, Args...>::value;

     template<typename Default, template<typename...> class Op, class... Args>
     using detected_or_t = _t<detected_or<Default, Op, Args...>>;
     //
     // The alias template is_detected_exact checks whether `detected_t<Op,
     // Args...>` is Expected.
     template<typename Expected, template<typename...> class Op, class... Args>
     using is_detected_exact = faiz::is_same<Expected, detected_t<Op, Args...>>;

     template<typename T>
     struct remove_cv;

     template<typename T>
     using remove_cv_t = _t<remove_cv<T>>;


     template<typename Expected, template<typename...> class Op, class... Args>
     constexpr bool is_detected_exact_v
         = is_detected_exact<Expected, Op, Args...>::value;
     //
     //  The alias template `is_detected_convertible` checks whether
     // `detected_t<Op, Args...>` is convertible to To.
     template<typename To, template<typename...> class Op, class... Args>
     using is_detected_convertible
         = faiz::is_convertible<detected_t<Op, Args...>, To>;

     template<typename To, template<typename...> class Op, class... Args>
     constexpr bool is_detected_convertible_v
         = is_detected_convertible<To, Op, Args...>::value;

     // Checks whether T is a void type. Provides the member constant value
     // that is equal to true, if T is the type void, const void, volatile
     // void, or const volatile void. Otherwise, value is equal to false.
     template<typename T>
     struct is_void : is_same<void, faiz::remove_cv_t<T>>
     {};
     template<typename T>
     inline constexpr bool is_void_v = is_void<T>::value;


     template<typename T>
     using is_referenceable_aux = is_void<T&>;
     template<typename T>
     constexpr bool is_referenceable_v
         = is_detected_v<is_referenceable_aux, T> && !is_void_v<T>;
     template<typename T>
     struct is_referenceable : bool_<is_referenceable_v<T>>
     {};


     // If `T` is an object type or a function type that has no `cv-` or `ref-
     // qualifier` (since C++17), provides a member typedef type which is `T&`.
     // If `T` is an **rvalue reference** to some type `U`, then type is `U&`.
     // Otherwise, type is `T`.
     template<typename T>
     struct add_lvalue_reference;

     template<typename T>
     using add_lvalue_reference_t = _t<add_lvalue_reference<T>>;

     template<typename T>
     struct is_enum : public bool_<__is_enum(T)>
     {};

     template<typename T>
     inline constexpr bool is_enum_v = is_enum<T>();

     template<typename T>
     struct is_rvalue_reference;
     template<typename T>
     inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

 } // namespace rider::faiz


 // forward declaration
 namespace rider::faiz
 {
     template<typename... Ts>
     struct common_type;

     template<typename... Ts>
     using common_type_t = _t<common_type<Ts...>>;

     template<typename T, typename U, typename TQual, typename UQual>
     struct basic_common_reference;

     template<typename... Ts>
     struct common_reference;

     template<typename... Ts>
     using common_reference_t = _t<common_reference<Ts...>>;

 } // namespace rider::faiz


 namespace rider::faiz::detail
 {
     template<typename Base>
     faiz::true_
     is_base_of_test_func(const volatile Base*);
     template<typename Base>
     faiz::false_
     is_base_of_test_func(const volatile void*);
     template<typename Base, typename Derived>
     using pre_is_base_of
         = decltype(is_base_of_test_func<Base>(faiz::declval<Derived*>()));

     // with <experimental/type_traits>:
     // template <typename Base, typename Derived>
     // using pre_is_base_of2 =
     // detected_or_t<faiz::true_, pre_is_base_of,
     // Base, Derived>;
     template<typename Base, typename Derived, typename = void>
     struct pre_is_base_of2 : public faiz::true_
     {};
     // note faiz::void_t is a C++17 feature
     template<typename Base, typename Derived>
     struct pre_is_base_of2<Base, Derived, void_t<pre_is_base_of<Base, Derived>>>
         : public pre_is_base_of<Base, Derived>
     {};

     template<typename T, typename>
     struct is_polymorphic_impl : false_
     {};
     template<typename T>
     struct is_polymorphic_impl<T,
         decltype(dynamic_cast<void*>(declval<remove_cv_t<T>*>()))> : true_
     {};


     // NOTE: !rider::faiz::is_assignable<bool &, std::nullptr_t>::value'
     // "Error", clang++ has a bug here.
     template<typename T, typename Arg, typename>
     struct is_assignable_imp : false_
     {};
     template<typename T, typename Arg>
     struct is_assignable_imp<T,
         Arg,
         void_t<decltype(declval<T>() = declval<Arg>())>>
         : bool_<!is_void_v<Arg>>
     {};

     template<bool IsArithmetic, typename T>
     struct is_signed_impl : bool_<T(-1) < T(0)>
     {};
     template<typename T>
     struct is_signed_impl<false, T> : false_
     {};

     template<bool IsArithmetic, typename T>
     struct is_unsigned_impl : bool_<(T(-1) > T(0))>
     {};
     template<typename T>
     struct is_unsigned_impl<false, T> : false_
     {};

     template<typename T, typename U = remove_cv_t<T>>
     struct is_floating_point_aux : is_any<U,
                                        float,
                                        double,
                                        __float128, // add gcc specific
                                        long double>
     {};

     template<typename T>
     constexpr bool not_enum_rvalue_reference_v
         = logic::and_<logic::not_<is_enum<T>>,
             logic::not_<is_rvalue_reference<T>>>::value;

     template<typename T>
     using is_integral_arith
         = void_t<decltype(T{} * T{}), decltype(+T{})&, decltype(T{} % 1)>;
     template<typename T>
     constexpr bool is_integral_impl
         = meta::and_<is_detected<is_integral_arith, T>,
             bool_<not_enum_rvalue_reference_v<T>>>::value;

     template<typename T, bool = is_integral_impl<T>>
     struct is_integral_aux : false_
     {};
     template<typename T>
     struct is_integral_aux<T, true> : true_
     {};

     template<typename T>
     using is_arithmetic_arith = void_t<decltype(T{} * T{}), decltype(+T{})&>;

     template<typename T>
     constexpr bool is_arithmetic_impl
         = logic::and_<is_detected<is_arithmetic_arith, T>,
             bool_<not_enum_rvalue_reference_v<T>>>::value;

     template<typename T, bool = is_arithmetic_impl<T>>
     struct is_arithmetic_aux : false_
     {};

     template<typename T>
     struct is_arithmetic_aux<T, true> : true_
     {};

     template<typename T, bool = is_referenceable_v<T>>
     struct add_rvalue_reference_impl : type_identity<T&&>
     {};
     template<typename T>
     struct add_rvalue_reference_impl<T, false> : type_identity<T>
     {};

     template<typename T, bool = is_referenceable_v<T>>
     struct add_lvalue_reference_impl : type_identity<T&>
     {};
     template<typename T>
     struct add_lvalue_reference_impl<T, false> : type_identity<T>
     {};

     template<typename T>
     char
     test(int T::*);
     struct two
     {
         char c[2];
     };
     template<typename T>
     two
     test(...);

     template<typename T>
     struct is_pointer_helper : false_
     {};
     template<typename T>
     struct is_pointer_helper<T*> : true_
     {};


     template<typename T, bool is_function_type = false>
     struct add_pointer : type_identity<remove_reference_t<T>*>
     {};

     template<typename T>
     struct add_pointer<T, true> : type_identity<T>
     {};

     template<typename T, class... Args>
     struct add_pointer<T(Args...), true> : type_identity<T (*)(Args...)>
     {};

     template<typename T, class... Args>
     struct add_pointer<T(Args..., ...), true>
         : type_identity<T (*)(Args..., ...)>
     {};


 } // namespace rider::faiz::detail

 namespace rider::faiz
 {

     template<typename... _b>
     using conjunction = logic::and_<_b...>;
     template<typename... B>
     inline constexpr bool conjunction_v = conjunction<B...>::value;

     template<typename... _b>
     using disjunction = logic::or_<_b...>;
     template<typename... B>
     inline constexpr bool disjunction_v = disjunction<B...>::value;

     template<typename _b>
     using negation = logic::not_<_b>;
     template<typename B>
     inline constexpr bool negation_v = negation<B>::value;

     // clang-format on

     /******************** remove *********************/

     // If the type T is a reference type,
     // provides the member typedef type which is the type referred to by T.
     // Otherwise type is T.
     template<typename T>
     struct remove_reference : type_identity<T>
     {};
     template<typename T>
     struct remove_reference<T&> : type_identity<T>
     {};
     template<typename T>
     struct remove_reference<T&&> : type_identity<T>
     {};

     template<typename T>
     struct remove_const : type_identity<T>
     {};
     template<typename T>
     struct remove_const<const T> : type_identity<T>
     {};

     template<typename T>
     struct remove_volatile : type_identity<T>
     {};

     template<typename T>
     struct remove_volatile<volatile T> : type_identity<T>
     {};

     // If T is an array of some type X, provides the member typedef type
     // equal to X, otherwise type is T. Note that if T is a multidimensional
     // array, only the first dimension is removed.
     template<typename T>
     struct remove_extent : type_identity<T>
     {};

     template<typename T>
     struct remove_extent<T[]> : type_identity<T>
     {};

     template<typename T, faiz::size_t N>
     struct remove_extent<T[N]> : type_identity<T>
     {};
     template<typename T>
     using remove_extent_t = _t<remove_extent<T>>;

     template<typename T>
     struct remove_cv : type_identity<faiz::remove_volatile_t<remove_const_t<T>>>
     {};
     template<typename T>
     using remove_cv_t = _t<remove_cv<T>>;

     // If T is a multidimensional array of some type X, provides the member
     // typedef type equal to X, otherwise type is T.
     template<typename T>
     struct remove_all_extents;
     template<typename T>
     using remove_all_extents_t = _t<remove_all_extents<T>>;

     template<typename T>
     struct remove_all_extents : type_identity<T>
     {};
     template<typename T>
     struct remove_all_extents<T[]> : type_identity<remove_all_extents_t<T>>
     {};
     template<typename T, size_t V>
     struct remove_all_extents<T[V]> : type_identity<remove_all_extents_t<T>>
     {};


     // Provides the member typedef type which is the type pointed to by T,
     // or, if T is not a pointer, then type is the same as T.
     // ```cpp
     // print_is_same<int, remove_pointer<int*>::type>();  // true
     // print_is_same<int, remove_pointer<int**>::type>(); // false
     // ```
     template<typename T>
     struct remove_pointer : type_identity<T>
     {};
     template<typename T>
     struct remove_pointer<T*> : type_identity<T>
     {};
     template<typename T>
     struct remove_pointer<T* const> : type_identity<T>
     {};
     template<typename T>
     struct remove_pointer<T* volatile> : type_identity<T>
     {};
     template<typename T>
     struct remove_pointer<T* const volatile> : type_identity<T>
     {};

     template<typename T>
     using remove_cv_t = _t<remove_cv<T>>;
     template<typename T>
     using remove_const_t = _t<remove_const<T>>;
     template<typename T>
     using remove_volatile_t = _t<remove_volatile<T>>;
     template<typename T>
     using remove_extent_t = _t<remove_extent<T>>;
     template<typename T>
     using remove_pointer_t = _t<remove_pointer<T>>;

     // If the type **T** is a reference type, provides the member typedef
     // type which is the type referred to by
     // **T** with its topmost cv-qualifiers removed. Otherwise type is **T**
     // with its topmost cv-qualifiers removed.
     //
     // Removing cvref does most of what decay does, but doesn't convert
     // function types and array types to pointers.

     template<typename T>
     struct remove_cvref : type_identity<remove_cv_t<remove_reference_t<T>>>
     {};

     /******************** is *********************/

     // If T and U name the same type (including const/volatile
     // qualifications), provides the member constant value equal to true.
     // Otherwise value is false.
     template<typename T, class U>
     struct is_same : false_
     {};

     // If T and U name the same type (including const/volatile
     // qualifications), provides the member constant value equal to true.
     // Otherwise value is false.
     template<typename T>
     struct is_same<T, T> : true_
     {};


     template<typename T>
     struct is_null_pointer
         : faiz::is_same<faiz::nullptr_t, faiz::remove_cv_t<T>>
     {};
     template<typename T>
     constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

     template<typename T>
     struct is_const : false_
     {};
     template<typename T>
     struct is_const<const T> : true_
     {};

     template<typename T>
     inline constexpr bool is_const_v = is_const<T>::value;

     template<typename T>
     struct is_reference : false_
     {};
     template<typename T>
     struct is_reference<T&> : true_
     {};
     template<typename T>
     struct is_reference<T&&> : true_
     {};
     template<typename T>
     inline constexpr bool is_reference_v = is_reference<T>::value;

     template<typename T>
     struct is_function : logic::and_<logic::not_<is_const<T const>>,
                              logic::not_<is_reference<T>>>
     {};

     template<typename T>
     struct is_volatile : public false_
     {};
     template<typename T>
     struct is_volatile<T volatile> : public true_
     {};
     template<typename T, size_t N>
     struct is_volatile<T volatile[N]> : public true_
     {};
     template<typename T>
     struct is_volatile<T volatile[]> : public true_
     {};

     template<typename T>
     constexpr bool is_volatile_v = is_volatile<T>::value;

     template<typename T>
     inline constexpr bool is_function_v = is_function<T>::value;

     template<typename T>
     struct is_floating_point : detail::is_floating_point_aux<T>
     {};

     template<typename T>
     inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

     // Checks whether T is an integral type. Provides the member constant
     // value which is equal to true, if T is the type bool, char, char16_t,
     // char32_t, wchar_t, short, int, long, long long, or any
     // implementation-defined extended integer types, including any signed,
     // unsigned, and cv-qualified variants. Otherwise, value is equal to
     // false.

     template<typename T>
     struct is_integral : detail::is_integral_aux<T>
     {};

     template<typename T>
     inline constexpr bool is_integral_v = is_integral<T>::value;
     //  If T is an arithmetic type (that is, an integral type or a
     // floating-point type) or a cv-qualified version thereof, provides the
     // member constant value equal true. For any other type, value is false.
     //
     // gcc may complains, latest msvc/clang compiles happily
     template<typename T>
     struct is_arithmetic : detail::is_arithmetic_aux<T>
     {};

     template<typename T>
     inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;


     // Checks whether T is **a pointer to object** or **a pointer to
     // function** (but not a pointer to member/member function). Provides
     // the member constant value which is equal to true, if T is a
     // object/function pointer type. Otherwise, value is equal to false.
     template<typename T>
     struct is_pointer : public detail::is_pointer_helper<remove_cv_t<T>>
     {};

     template<typename T>
     inline constexpr bool is_pointer_v = is_pointer<T>::value;

     template<typename T>
     struct is_empty : public bool_<__is_empty(T)>
     {};

     template<typename T>
     inline constexpr bool is_empty_v = is_empty<T>::value;

     template<typename T>
     struct is_lvalue_reference : false_
     {};
     template<typename T>
     struct is_lvalue_reference<T&> : true_
     {};

     template<typename T>
     struct is_rvalue_reference : false_
     {};
     template<typename T>
     struct is_rvalue_reference<T&&> : true_
     {};


     template<typename T>
     struct is_union : public bool_<__is_union(T)>
     {};
     template<typename T>
     constexpr bool is_union_v = is_union<T>::value;

     // TODO: implement is_class without ugly sizeof
     template<typename T>
     struct is_class
         : bool_<sizeof(detail::test<T>(0)) == 1 and !faiz::is_union<T>::value
               and !faiz::is_null_pointer<T>::value>
     {};
     template<typename T>
     inline constexpr bool is_class_v = is_class<T>::value;


     template<typename T>
     struct is_object
         : bool_<!is_reference_v<T> and !is_void_v<T> and !is_function_v<T>>
     {};


     template<typename T, unsigned N = 0>
     struct extent : size_t_<0>
     {};

     template<typename T>
     struct extent<T[], 0> : size_t_<0>
     {};

     template<typename T, unsigned N>
     struct extent<T[], N> : extent<T, N - 1>
     {};

     template<typename T, faiz::size_t I>
     struct extent<T[I], 0> : size_t_<I>
     {};

     template<typename T, faiz::size_t I, unsigned N>
     struct extent<T[I], N> : extent<T, N - 1>
     {};
     template<typename T, unsigned N = 0>
     inline constexpr faiz::size_t extent_v = extent<T, N>::value;

     //  If T is an object type or a function type that has no cv- or ref-
     //  qualifier (since C++17), provides a member typedef type which is T&. If
     //  T is an rvalue reference to some type U, then type is U&. Otherwise,
     //  type is T.


     // FIXME: add_rvalue_reference_impl is a workaround here, otherwise forward
     // declaration can also not solve type dependency.
     template<typename T>
     struct add_rvalue_reference : detail::add_rvalue_reference_impl<T>
     {};

     template<typename T>
     struct add_lvalue_reference : detail::add_lvalue_reference_impl<T>
     {};


     // Converts any type **T** to a reference type, making it possible to
     // use member functions in *decltype* expressions without the need to go
     // through constructors.
     //
     // *declval* is commonly used in templates where acceptable template
     // parameters may have no constructor in common, but have the same
     // member function whose return type is needed.
     //
     // Note that because no definition exists for *declval*, it can only be
     // used in unevaluated contexts; it is an error to evaluate an
     // expression that contains this function. Formally, the program is
     // *ill-formed* if this function is *odr-used*.
     //
     // Cannot be called and thus never returns a value. The return type is
     // T&& unless **T** is (possibly cv-qualified) void, in which case the
     // return type is **T**.
     //
     // return `add_rvalue_reference_t<T>` is for reference collapsing

     template<typename T>
     struct add_cv : type_identity<const volatile T>
     {};

     template<typename T>
     struct add_const : type_identity<const T>
     {};

     template<typename T>
     struct add_volatile : type_identity<volatile T>
     {};

     template<typename T>
     using add_cv_t = _t<add_cv<T>>;
     template<typename T>
     using add_const_t = _t<add_const<T>>;
     template<typename T>
     using add_volatile_t = _t<add_volatile<T>>;

     // p0007r0
     // https://stackoverflow.com/questions/34566063/why-is-the-const-overload-of-as-const-deleted
     // Forms lvalue reference to const type of t
     template<typename T>
     constexpr add_const_t<T>&
     as_const(T& t) noexcept;

     //  const rvalue reference overload is deleted to disallow rvalue arguments
     template<typename T>
     void
     as_const(const T&&)
         = delete;

     // TODO: check whether standard has as_cref_t
     template<typename T, typename R = _t<remove_reference<T>>>
     using as_cref_t = _t<add_lvalue_reference<_t<add_const<R>>>>;

     template<class T>
     struct is_signed : detail::is_signed_impl<is_arithmetic_v<T>, T>
     {};

     template<typename T>
     constexpr bool is_signed_v = is_signed<T>::value;
     template<typename T>
     constexpr bool is_unsigned_v
         = not is_signed<T>::value and is_arithmetic_v<T>;
     template<class T>
     struct is_unsigned : bool_<is_unsigned_v<T>>
     {};

     // TODO: add support for compiler extension larger signed.
     template<typename T>
     struct make_signed
     {
     private:
         static_assert((is_integral_v<T> or is_enum_v<T>),
             "The template argument to make_signed must be an integer or enum "
             "type.");
         static_assert(not(is_same_v<remove_cv_t<T>, bool>),
             "The template argument to make_signed must not be the type bool.");

         using t_no_cv = remove_cv_t<T>;
         using base_integer_type = meta::if_<
             // if
             logic::and_<is_signed<T>,
                 is_integral<T>,
                 logic::not_<is_same<t_no_cv, char>>,
                 logic::not_<is_same<t_no_cv, wchar_t>>,
                 logic::not_<is_same<t_no_cv, bool>>>,
             // then
             T,
             // else
             //    if
             meta::if_<logic::and_<is_integral<T>,
                           logic::not_<is_same<t_no_cv, char>>,
                           logic::not_<is_same<t_no_cv, wchar_t>>,
                           logic::not_<is_same<t_no_cv, bool>>>,
                 // then
                 meta::if_<is_same<t_no_cv, unsigned char>,
                     signed char,
                     meta::if_<is_same<t_no_cv, unsigned short>,
                         signed short,
                         meta::if_<is_same<t_no_cv, unsigned int>,
                             int,
                             meta::if_<is_same<t_no_cv, unsigned long>,
                                 long,
                                 long long>>>>,
                 //  else
                 // Not a regular integer type:
                 meta::if_c<sizeof(t_no_cv) == sizeof(unsigned char),
                     signed char,
                     meta::if_c<sizeof(t_no_cv) == sizeof(unsigned short),
                         signed short,
                         meta::if_c<sizeof(t_no_cv) == sizeof(unsigned int),
                             int,
                             meta::if_c<sizeof(t_no_cv) == sizeof(unsigned long),
                                 long,
                                 long long>>>>>>;

         // Add back any const qualifier:
         using const_base_integer_type = meta::
             if_<is_const<T>, add_const_t<base_integer_type>, base_integer_type>;

     public:
         // Add back any volatile qualifier:
         using type = meta::if_<is_volatile<T>,
             add_volatile_t<const_base_integer_type>,
             const_base_integer_type>;
     };

     template<class T>
     struct make_unsigned
     {
     private:
         static_assert(disjunction_v<is_integral<T>, is_enum<T>>,
             "The template argument to make_unsigned must be an integer or enum "
             "type.");
         static_assert(negation_v<is_same<remove_cv_t<T>, bool>>,
             "The template argument to make_unsigned must not be the type "
             "bool");

         using t_no_cv = remove_cv_t<T>;
         using base_integer_type = meta::if_<
             meta::strict_and<is_unsigned<T>,
                 is_integral<T>,
                 meta::not_<is_same<t_no_cv, char>>,
                 meta::not_<is_same<t_no_cv, wchar_t>>,
                 meta::not_<is_same<t_no_cv, bool>>>,
             T,
             meta::if_<meta::strict_and<is_integral<T>,
                           meta::not_<is_same<t_no_cv, char>>,
                           meta::not_<is_same<t_no_cv, wchar_t>>,
                           meta::not_<is_same<t_no_cv, bool>>>,
                 meta::if_<is_same<t_no_cv, signed char>,
                     unsigned char,
                     meta::if_<is_same<t_no_cv, short>,
                         unsigned short,
                         meta::if_<is_same<t_no_cv, int>,
                             unsigned int,
                             meta::if_<is_same<t_no_cv, long>,
                                 unsigned long,
                                 unsigned long long>>>>,
                 // Not a regular integer type:
                 meta::if_c<sizeof(t_no_cv) == sizeof(unsigned char),
                     unsigned char,
                     meta::if_c<sizeof(t_no_cv) == sizeof(unsigned short),
                         unsigned short,
                         meta::if_c<sizeof(t_no_cv) == sizeof(unsigned int),
                             unsigned int,
                             meta::if_c<sizeof(t_no_cv) == sizeof(unsigned long),
                                 unsigned long,
                                 unsigned long long>>>>>>;

         // Add back any const qualifier:
         using const_base_integer_type = _t<meta::if_<is_const<T>,
             add_const_t<base_integer_type>,
             base_integer_type>>;

     public:
         // Add back any volatile qualifier:
         using type = _t<meta::if_<is_volatile<T>,
             add_volatile_t<const_base_integer_type>,
             const_base_integer_type>>;
     };

     template<class T>
     using make_unsigned_t = _t<make_unsigned<T>>;

     template<class T>
     using make_signed_t = _t<make_signed<T>>;

     template<bool B, typename T>
     struct enable_if
     {};

     template<typename T>
     struct enable_if<true, T> : type_identity<T>
     {};


     template<typename T>
     inline constexpr bool is_array_v = false;
     template<typename T>
     inline constexpr bool is_array_v<T[]> = true;
     template<typename T, size_t N>
     inline constexpr bool is_array_v<T[N]> = true;

     template<typename T>
     struct is_array : bool_<is_array_v<T>>
     {};

     // If the imaginary function definition To test() { return
     // std::declval<From>(); } is well-formed, (that is, either
     // std::declval<From>() can be converted to To using implicit conversions,
     // or both From and To are possibly cv-qualified void), provides the member
     // constant value equal to true. Otherwise value is false. For the purposes
     // of this check, the use of std::declval in the return statement is not
     // considered an odr-use.
     //
     //  Access checks are performed as if from a context unrelated to either
     // type. Only the validity of the immediate context of the expression in the
     // return statement (including conversions to the return type) is
     // considered.
     // // using namespace logic;
     // template<typename From, typename To>
     // constexpr bool is_convertible_v = or_<
     // 	and_<or_<is_void<From>, is_function<To>, is_array<To>>, is_void<To>>,
     // 	is_detected<is_convertible_helper, From, To>>::value;
     //
     template<typename To1>
     static void test_aux(To1) noexcept;


     template<typename From, typename To>
     using is_convertible_helper = decltype(test_aux<To>(faiz::declval<From>()));


     template<typename From, typename To>
     constexpr bool
     my_is_convertible()
     {
         if constexpr(disjunction<is_void<From>, is_function<To>, is_array<To>>::
                          value)
         {
             return is_void_v<To>;
         }
         else
         {
             return is_detected<is_convertible_helper, From, To>::value;
         }
     }

     template<typename From, typename To>
     struct is_convertible : bool_<my_is_convertible<From, To>()>
     {};

     // TODO: use detection idiom implement is_convertible
     // If T is an object or reference type and the variable definition T
     // obj(std::declval<Args>()...); is well-formed, provides the member
     // constant value equal to true. In all other cases, value is false.
     // template<typename Tp, class... Args>
     // struct is_constructible : public bool_<__is_constructible(Tp, Args...)>
     // {};

     template<typename T, class, class...>
     struct is_constructible_impl : false_
     {};
     template<typename T, class... Us>
     struct is_constructible_impl<T,
         decltype(void(::new(declval<void*>()) T(declval<Us>()...))),
         Us...> : true_
     {};
     template<typename T, class... Us>
     struct is_constructible : is_constructible_impl<T, void, Us...>
     {};

     template<typename T, class... Args>
     inline constexpr bool is_constructible_v
         = is_constructible<T, Args...>::value;

     template<typename Tp>
     struct is_copy_constructible
         : public is_constructible<Tp, add_lvalue_reference_t<add_const_t<Tp>>>
     {};

     template<typename Tp>
     inline constexpr bool is_copy_constructible_v
         = is_copy_constructible<Tp>::value;


     // If T is an array type, provides the member constant value equal to
     // the number of dimensions of the array. For any other type, value is
     // 0.
     template<typename T>
     struct rank : public size_t_<0>
     {};
     // If T is an array type, provides the member constant value equal to
     // the number of dimensions of the array. For any other type, value is
     // 0.
     template<typename T>
     struct rank<T[]> : public size_t_<rank<T>::value + 1>
     {};
     // If T is an array type, provides the member constant value equal to
     // the number of dimensions of the array. For any other type, value is
     // 0.
     template<typename T, faiz::size_t N>
     struct rank<T[N]> : public size_t_<rank<T>::value + 1>
     {};
     template<typename T>
     inline constexpr faiz::size_t rank_v = rank<T>::value;


     // If **T** is a reference type, then provides the member typedef type
     // which is a pointer to the referred type. Otherwise, if **T** names an
     // object type, a function type that is not **cv-** or **ref-qualified**
     // (since C++17), or a (possibly **cv-qualified**) void type, provides
     // the member typedef type which is the type T*. Otherwise (if T is a
     // **cv-** or **ref-qualified** function type), provides the member
     // typedef type which is the type **T**
     //
     // * type:	pointer to T or to the type referenced by T
     template<typename T>
     struct add_pointer : detail::add_pointer<T, is_function_v<T>>
     {};
     template<typename T>
     using add_pointer_t = _t<add_pointer<T>>;

     // Applies lvalue-to-rvalue, array-to-pointer,
     // and function-to-pointer implicit conversions to the type **T**,
     // removes cv-qualifiers, and defines the resulting type as the member
     // typedef type. Formally:
     // * If **T** names the type "**array of U**" or "**reference to array
     // of U**", the member typedef type is
     // **U***.
     // * Otherwise, if **T** is a function type **F** or a reference
     // thereto, the member typedef type is
     // **add_pointer<F>::type**.
     // * Otherwise, the member typedef type is
     // **remove_cv<remove_reference<T>::type>::type**
     //
     //```cpp
     // #include <iostream>
     // #include <type_traits>
     // template <typename T, typename U>
     // struct decay_equiv :
     //     is_same<typename decay<T>::type, U>::type
     // {};
     // int main()
     // {
     //     faiz::cout << faiz::boolalpha
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
     template<typename T>
     struct decay
     {
     private:
         using U = remove_reference_t<T>;

     public:
         using type = meta::if_<is_array<U>,
             remove_extent_t<U>*,
             meta::if_<is_function<U>, add_pointer_t<U>, remove_cv_t<U>>>;
     };
     template<typename T>
     using decay_t = _t<decay<T>>;

     // is_assignable


     template<typename T, typename Arg>
     struct is_assignable : public detail::is_assignable_imp<T, Arg, void>
     {};

     // If Derived is derived from Base or if both are the same non-union class
     // (in both cases ignoring cv-qualification), provides the member constant
     // value equal to true. Otherwise value is false.
     //
     //  If both Base and Derived are non-union class types, and they are not the
     // same type (ignoring cv-qualification), Derived shall be a complete type;
     // otherwise the behavior is undefined.
     //
     // faiz::is_base_of<A, B>::value is true even if A is a private, protected,
     // or ambiguous base class of B. In many situations,
     // faiz::is_convertible<B*, A*> is the more appropriate test.
     //
     // Although no class is its own base, faiz::is_base_of<T, T>::value is true
     // because the intent of the trait is to model the "is-a" relationship, and
     // T is a T. Despite that, faiz::is_base_of<int, int>::value is false
     // because only classes participate in the relationship that this trait
     // models.
     template<typename Base, typename Derived>
     struct is_base_of
         : public meta::if_<logic::and_<is_class<Base>, is_class<Derived>>,
               detail::pre_is_base_of2<Base, Derived>,
               false_>
     {};
     template<typename Base, class Derived>
     inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

     template<typename T>
     struct is_member_pointer_helper : faiz::false_
     {};

     template<typename T, class U>
     struct is_member_pointer_helper<T U::*> : faiz::true_
     {};

     template<typename T>
     struct is_member_pointer : is_member_pointer_helper<_t<faiz::remove_cv<T>>>
     {};
     template<typename T>
     inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

     // If `T` is a scalar type (that is a possibly **cv-qualified** arithmetic,
     // pointer, pointer to member, enumeration, or `faiz::nullptr_t` type),
     // provides the member constant value equal `true`. For any other type,
     // value is `false`.
     //
     // Each individual memory location in the C++ memory model, including the
     // hidden memory locations used by language features (e.g virtual table
     // pointer), has scalar type (or is a sequence of adjacent bit-fields of
     // non-zero length). Sequencing of side-effects in expression evaluation,
     // interthread synchronization, and dependency ordering are all defined in
     // terms of individual scalar objects.
     //
     template<typename T>
     struct is_scalar : faiz::disjunction<faiz::is_arithmetic<T>,
                            faiz::is_enum<T>,
                            faiz::is_pointer<T>,
                            faiz::is_member_pointer<T>,
                            faiz::is_null_pointer<T>>
     {};
     template<typename T>
     inline constexpr bool is_scalar_v = is_scalar<T>::value;

     template<typename T>
     struct is_member_function_pointer_helper : faiz::false_
     {};

     template<typename T, class U>
     struct is_member_function_pointer_helper<T U::*> : faiz::is_function<T>
     {};
     //
     //  Checks whether T is a non-static member function pointer. Provides the
     // member constant value which is equal to true, if T is a non-static member
     // function pointer type. Otherwise, value is equal to false.
     template<typename T>
     struct is_member_function_pointer
         : is_member_function_pointer_helper<faiz::remove_cv_t<T>>
     {};
     template<typename T>
     inline constexpr bool is_member_function_pointer_v
         = is_member_function_pointer<T>::value;

     // Checks whether T is a non-static member object. Provides the member
     // constant value which is equal to true, if T is a non-static member object
     // type. Otherwise, value is equal to false.
     template<typename T>
     struct is_member_object_pointer
         : bool_<faiz::is_member_pointer<T>::value
               && !faiz::is_member_function_pointer<T>::value>
     {};
     template<typename T>
     inline constexpr bool is_member_object_pointer_v
         = is_member_object_pointer<T>::value;


     // Given two (possibly identical) types Base and Derived,
     // is_base_of<Base, Derived>::value == true if and only if Base is a
     // direct or indirect base class of Derived. This is like
     // is_base_of<Base, Derived> but returns false if Derived is the same as
     // Base. So is_derived is true only if Derived is actually a subclass of
     // Base and not Base itself.
     //
     // is_derived may only be applied to complete types.
     //
     // Example usage:
     //     is_derived<int, int>::value             => false
     //     is_derived<int, bool>::value            => false
     //     is_derived<Parent, Child>::value        => true
     //     is_derived<Child, Parent>::value        => false
     template<typename Base, typename Derived>
     struct is_derived
         : public bool_<
               faiz::is_base_of_v<Base,
                   Derived> && !faiz::is_same_v<faiz::remove_cv_t<Base>, faiz::remove_cv_t<Derived>>>
     {};
     template<typename Base, class Derived>
     inline constexpr bool is_derived_of_v = is_base_of<Base, Derived>::value;

     template<typename T>
     struct is_final : public bool_<__is_final(T)>
     {};

     template<typename T>
     constexpr bool is_final_v = is_final<T>::value;

     template<typename T>
     struct is_polymorphic : public detail::is_polymorphic_impl<T, void*>
     {};

     template<typename T>
     constexpr bool is_polymorphic_v = is_polymorphic<T>::value;

     template<typename T>
     struct is_copy_assignable
         : faiz::is_assignable<_t<faiz::add_lvalue_reference<T>>,
               faiz::add_lvalue_reference_t<const T>>
     {};

     template<typename T, typename _Up>
     struct is_trivially_assignable
         : public bool_<__is_trivially_assignable(T, _Up)>
     {};

     template<typename T>
     struct is_trivially_copy_assignable
         : faiz::is_trivially_assignable<_t<faiz::add_lvalue_reference<T>>,
               faiz::add_lvalue_reference_t<const T>>
     {};

     template<bool, class T, class A>
     struct is_nothrow_assignable_aux;

     template<typename T, class A>
     struct is_nothrow_assignable_aux<false, T, A> : public false_
     {};

     template<typename T, class A>
     struct is_nothrow_assignable_aux<true, T, A>
         : public bool_<noexcept(faiz::declval<T>() = faiz::declval<A>())>
     {};

     template<typename T, class A>
     struct is_nothrow_assignable
         : public is_nothrow_assignable_aux<is_assignable<T, A>::value, T, A>
     {};

     template<typename T>
     struct is_nothrow_copy_assignable
         : faiz::is_nothrow_assignable<_t<faiz::add_lvalue_reference<T>>,
               _t<faiz::add_lvalue_reference<const T>>>
     {};

     template<typename T>
     inline constexpr bool is_nothrow_copy_assignable_v
         = is_nothrow_copy_assignable<T>::value;

     // std::is_standard_layout is not something you can implement without
     // compiler intrinsics. As you've correctly pointed out, it needs more
     // information than the C++ type system can express.
     //
     // If T is a standard layout type (that is, a scalar type, a standard-layout
     // class, or an array of such type/class, possibly cv-qualified), provides
     // the member constant value equal true. For any other type, value is false.
     //
     // A standard-layout class is a class that satisfies StandardLayoutType.
     //
     // The behavior is undefined if std::remove_all_extents_t<T> is an
     // incomplete type and not (possibly cv-qualified) void.
     template<typename Tp>
     struct is_standard_layout : public bool_<__is_standard_layout(Tp)>
     {};

     template<typename T>
     inline constexpr bool is_standard_layout_v = is_standard_layout<T>::value;

     template<typename _Tp>
     struct is_trivially_copyable : bool_<is_scalar_v<remove_all_extents_t<_Tp>>>
     {};

     template<typename _Tp>
     inline constexpr bool is_trivially_copyable_v
         = is_trivially_copyable<_Tp>::value;


     template<typename Tp>
     struct is_trivial : public bool_<__is_trivial(Tp)>
     {};

     template<typename T>
     inline constexpr bool is_trivial_v = is_trivial<T>::value;


     template<typename T>
     struct is_unknown_bound_array : false_
     {};
     template<typename T>
     struct is_unknown_bound_array<T[]> : true_
     {};

     template<typename T>
     inline constexpr bool is_unknown_bound_array_v
         = is_unknown_bound_array<T>::value;

     template<typename T, typename U = faiz::remove_all_extents_t<T>>
     using has_dtor = decltype(declval<U>().~U());

     // clang-format off
    // FIXME \\\ fix compiler.....  gcc has bug here, I post a thread here: https://stackoverflow.com/questions/53456848/implement-is-destructible-with-detected-idiomhttps://stackoverflow.com/questions/53456848/implement-is-destructible-with-detected-idiom
    // template<typename T>
    // constexpr bool is_destructible_v =
    // 	is_reference_v<T> || (!(is_void_v<T> || is_function_v<T> || is_unknown_bound_array_v<T>) and is_object_v<T> and is_detected_v<has_dtor, T>);

 #if BOOST_COMP_CLANG
    template<typename T>
    constexpr bool is_destructible_v =
        (faiz::is_detected_v<has_dtor,
           T>
       or faiz::is_reference_v<T>)
    and not is_unknown_bound_array_v<T>
        and not faiz::is_function_v<T>;

 #elif BOOST_COMP_GNUC
    template<typename T>
    constexpr bool
    my_is_destructible()
    {
        if constexpr(is_reference_v<T>)
        {
            return true;
        }
        else if constexpr(
            is_same_v<remove_cv_t<T>,
                void> || is_function_v<T> || is_unknown_bound_array<T>::value)
        {
            return false;
        }
        else if constexpr(is_object_v<T>)
        {
            return is_detected_v<has_dtor, T>;
        }
        else
        {
            return false;
        }
    }
    template<typename T>
    constexpr bool is_destructible_v = my_is_destructible<T>();
 #endif
     // clang-format on
     template<typename T>
     struct is_destructible : bool_<is_destructible_v<T>>
     {};

     template<typename _Tp>
     struct __libcpp_trivial_destructor
         : public bool_<is_scalar<_Tp>::value || is_reference<_Tp>::value>
     {};

     template<typename _Tp>
     struct is_trivially_destructible
         : public __libcpp_trivial_destructor<
               typename remove_all_extents<_Tp>::type>
     {};

     template<typename _Tp>
     struct is_trivially_destructible<_Tp[]> : public false_
     {};

     template<typename _Tp>
     inline constexpr bool is_trivially_destructible_v
         = is_trivially_destructible<_Tp>::value;


 } // namespace rider::faiz

 #endif
