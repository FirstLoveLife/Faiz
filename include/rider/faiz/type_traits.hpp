#ifndef TYPE_TRAITS
#define TYPE_TRAITS
#include "../../../test/testsuite_tr1.h"
#include "rider/faiz/cstddef.hpp" // for size_t
#include <boost/predef.h>
#include <experimental/type_traits>
#include <type_traits>
/*
Don't implement:
 */

namespace rider::faiz
{
	template<class _type>
	using _t = typename _type::type;
	template<class...>
	using void_t = void;

	template<class T>
	struct type_identity
	{
		using type = T;
	};
	template<class T>
	using type_identity_t = _t<type_identity<T>>;

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
		constexpr value_type
		operator()() const noexcept
		{
			return value;
		} // since c++14
	};

#define ImplDeclIntT(_n, _t) \
	template<_t _vInt> \
	using _n = integral_constant<_t, _vInt>;
#define ImplDeclIntTDe(_t) ImplDeclIntT(_t##_, _t)

	// clang-format off

	ImplDeclIntTDe(bool)
	ImplDeclIntTDe(char)
	ImplDeclIntTDe(int)
	ImplDeclIntT(llong_, long long)
	ImplDeclIntTDe(long)
	ImplDeclIntTDe(short)
	ImplDeclIntT(ullong_, unsigned long long)
	ImplDeclIntT(ulong_, unsigned long)
	ImplDeclIntT(uint_, unsigned)
	ImplDeclIntT(ushort_, unsigned short)
	ImplDeclIntTDe(size_t)
	ImplDeclIntTDe(ptrdiff_t)

	// clang-format on
#undef ImplDeclIntTDe
#undef ImplDeclIntT


								using true_ = bool_<true>;
	using false_ = bool_<false>;
	template<bool B>
	using bool_constant = integral_constant<bool, B>;
	using true_type = true_;
	using false_type = false_;


	// Provides member typedef type, which is defined as T if B is true at
	// compile time, or as F if B is false.
	template<bool B, class T, class F>
	struct conditional : type_identity<T>
	{};

	// Provides member typedef type, which is defined as T if B is true at
	// compile time, or as F if B is false.
	template<class T, class F>
	struct conditional<false, T, F> : type_identity<F>
	{};

	//  Provides member typedef type, which is defined as T if B is true at
	//  compile time, or as F if B is false.
	template<bool B, class T, class F>
	using conditional_t = _t<conditional<B, T, F>>;


	template<typename...>
	using void_t = void;

	// forward declaration
	template<class From, class To>
	struct is_convertible;

	template<class T, class U>
	struct is_same;

	namespace detail
	{
		template<class Default,
			class AlwaysVoid,
			template<class...>
			class Op,
			class... Args>
		struct detector : type_identity<Default>
		{
			using value_t = faiz::false_;
		};

		template<class Default, template<class...> class Op, class... Args>
		struct detector<Default, faiz::void_t<Op<Args...>>, Op, Args...>
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
	template<template<class...> class Op, class... Args>
	using is_detected =
		typename detail::detector<nonesuch, void, Op, Args...>::value_t;


	// The alias template `detected_t` is equivalent to typename
	// `detected_or<faiz::nonesuch, Op, Args...>::type`. It is an
	// alias for `Op<Args...>` if that template-id denotes a valid type;
	// otherwise it is an alias for the class `faiz::nonesuch`.
	template<template<class...> class Op, class... Args>
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
	template<class Default, template<class...> class Op, class... Args>
	using detected_or = detail::detector<Default, void, Op, Args...>;

	template<template<class...> class Op, class... Args>
	constexpr bool is_detected_v = is_detected<Op, Args...>::value;

	template<class Default, template<class...> class Op, class... Args>
	using detected_or_t = _t<detected_or<Default, Op, Args...>>;
	//
	// The alias template is_detected_exact checks whether `detected_t<Op,
	// Args...>` is Expected.
	template<class Expected, template<class...> class Op, class... Args>
	using is_detected_exact = faiz::is_same<Expected, detected_t<Op, Args...>>;

	template<class Expected, template<class...> class Op, class... Args>
	constexpr bool is_detected_exact_v
		= is_detected_exact<Expected, Op, Args...>::value;
	//
	//  The alias template `is_detected_convertible` checks whether
	// `detected_t<Op, Args...>` is convertible to To.
	template<class To, template<class...> class Op, class... Args>
	using is_detected_convertible
		= faiz::is_convertible<detected_t<Op, Args...>, To>;

	template<class To, template<class...> class Op, class... Args>
	constexpr bool is_detected_convertible_v
		= is_detected_convertible<To, Op, Args...>::value;


	// FIXME: is_referenceable_aux depends on declval, while declval depends on
	// add_rvalue_reference, add_rvalue_reference depends on declval. So I have
	// to std::declval here as workaround, what a pity!
	//
	// declval<T> is enough here, because declval will check previous. But I
	// don't think this is a good solution. So, TODO: remove dependency of
	// declval.
	template<typename T>
	using is_referenceable_aux = decltype(std::declval<T>());
	template<typename T>
	// constexpr bool is_referenceable_v = is_detected_v<is_referenceable_aux,
	// T>;
	constexpr bool is_referenceable_v = true;

	template<class T>
	struct add_rvalue_reference;
	template<class T>
	using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

	template<class T>
	typename add_rvalue_reference<T>::type
	declval() noexcept;

} // namespace rider::faiz

namespace rider::faiz::logic
{
	template<class...>
	struct and_;

	template<>
	struct and_<> : true_
	{};

	template<class _b1>
	struct and_<_b1> : _b1
	{};

	template<class _b1, class _b2, class... _bn>
	struct and_<_b1, _b2, _bn...>
		: faiz::conditional_t<_b1::value, and_<_b2, _bn...>, _b1>
	{};

	template<class...>
	struct or_;

	template<>
	struct or_<> : false_
	{};

	template<class _b1>
	struct or_<_b1> : _b1
	{};

	template<class _b1, class _b2, class... _bn>
	struct or_<_b1, _b2, _bn...>
		: faiz::conditional<_b1::value, _b1, or_<_b2, _bn...>>::type
	{};


	template<class _b>
	struct not_ : bool_<!_b::value>
	{};


} // namespace rider::faiz::logic

namespace rider::faiz
{

	template<class... _b>
	using conjunction = logic::and_<_b...>;
	template<class... B>
	inline constexpr bool conjunction_v = conjunction<B...>::value;

	template<class... _b>
	using disjunction = logic::or_<_b...>;
	template<class... B>
	inline constexpr bool disjunction_v = disjunction<B...>::value;

	template<class _b>
	using negation = logic::not_<_b>;
	template<class B>
	inline constexpr bool negation_v = negation<B>::value;

	// clang-format on

	/******************** remove *********************/

	// If the type T is a reference type,
	// provides the member typedef type which is the type referred to by T.
	// Otherwise type is T.
	template<class T>
	struct remove_reference : type_identity<T>
	{};
	template<class T>
	struct remove_reference<T&> : type_identity<T>
	{};
	template<class T>
	struct remove_reference<T&&> : type_identity<T>
	{};
	template<class T>
	using remove_reference_t = _t<remove_reference<T>>;

	template<class T>
	struct remove_const : type_identity<T>
	{};
	template<class T>
	struct remove_const<const T> : type_identity<T>
	{};

	template<class T>
	struct remove_volatile : type_identity<T>
	{};

	template<class T>
	using remove_const_t = _t<remove_const<T>>;

	template<class T>
	using remove_volatile_t = _t<remove_volatile<T>>;

	template<class T>
	struct remove_volatile<volatile T> : type_identity<T>
	{};

	// If T is an array of some type X, provides the member typedef type
	// equal to X, otherwise type is T. Note that if T is a multidimensional
	// array, only the first dimension is removed.
	template<class T>
	struct remove_extent : type_identity<T>
	{};

	template<class T>
	struct remove_extent<T[]> : type_identity<T>
	{};

	template<class T, faiz::size_t N>
	struct remove_extent<T[N]> : type_identity<T>
	{};
	template<class T>
	using remove_extent_t = _t<remove_extent<T>>;

	template<class T>
	struct remove_cv : type_identity<faiz::remove_volatile_t<remove_const_t<T>>>
	{};
	template<class T>
	using remove_cv_t = _t<remove_cv<T>>;

	// If T is a multidimensional array of some type X, provides the member
	// typedef type equal to X, otherwise type is T.
	template<class T>
	struct remove_all_extents;
	template<class T>
	using remove_all_extents_t = _t<remove_all_extents<T>>;

	template<class T>
	struct remove_all_extents : type_identity<T>
	{};
	template<class T>
	struct remove_all_extents<T[]> : type_identity<remove_all_extents_t<T>>
	{};
	template<class T, size_t V>
	struct remove_all_extents<T[V]> : type_identity<remove_all_extents_t<T>>
	{};


	// Provides the member typedef type which is the type pointed to by T,
	// or, if T is not a pointer, then type is the same as T.
	// ```cpp
	// print_is_same<int, remove_pointer<int*>::type>();  // true
	// print_is_same<int, remove_pointer<int**>::type>(); // false
	// ```
	template<class T>
	struct remove_pointer : type_identity<T>
	{};
	template<class T>
	struct remove_pointer<T*> : type_identity<T>
	{};
	template<class T>
	struct remove_pointer<T* const> : type_identity<T>
	{};
	template<class T>
	struct remove_pointer<T* volatile> : type_identity<T>
	{};
	template<class T>
	struct remove_pointer<T* const volatile> : type_identity<T>
	{};

	template<class T>
	using remove_cv_t = _t<remove_cv<T>>;
	template<class T>
	using remove_const_t = _t<remove_const<T>>;
	template<class T>
	using remove_volatile_t = _t<remove_volatile<T>>;
	template<class T>
	using remove_extent_t = _t<remove_extent<T>>;
	template<class T>
	using remove_pointer_t = _t<remove_pointer<T>>;

	// If the type **T** is a reference type, provides the member typedef
	// type which is the type referred to by
	// **T** with its topmost cv-qualifiers removed. Otherwise type is **T**
	// with its topmost cv-qualifiers removed.
	//
	// Removing cvref does most of what decay does, but doesn't convert
	// function types and array types to pointers.

	template<class T>
	struct remove_cvref : type_identity<remove_cv_t<remove_reference_t<T>>>
	{};
	template<class T>
	using remove_cvref_t = _t<remove_cvref<T>>;

	/******************** is *********************/

	// If T and U name the same type (including const/volatile
	// qualifications), provides the member constant value equal to true.
	// Otherwise value is false.
	template<class T, class U>
	struct is_same : false_
	{};

	// If T and U name the same type (including const/volatile
	// qualifications), provides the member constant value equal to true.
	// Otherwise value is false.
	template<class T>
	struct is_same<T, T> : true_
	{};
	template<class T, class U>
	inline constexpr bool is_same_v = is_same<T, U>::value;

	// Checks whether T is a void type. Provides the member constant value
	// that is equal to true, if T is the type void, const void, volatile
	// void, or const volatile void. Otherwise, value is equal to false.
	template<class T>
	struct is_void : is_same<void, faiz::remove_cv_t<T>>
	{};
	template<class T>
	inline constexpr bool is_void_v = is_void<T>::value;

	template<class T>
	struct is_null_pointer
		: faiz::is_same<faiz::nullptr_t, faiz::remove_cv_t<T>>
	{};

	template<class T>
	struct is_const : false_
	{};
	template<class T>
	struct is_const<const T> : true_
	{};

	template<class T>
	inline constexpr bool is_const_v = is_const<T>::value;

	template<class T>
	struct is_reference : false_
	{};
	template<class T>
	struct is_reference<T&> : true_
	{};
	template<class T>
	struct is_reference<T&&> : true_
	{};
	template<class T>
	inline constexpr bool is_reference_v = is_reference<T>::value;

	template<class T>
	struct is_function : logic::and_<logic::not_<is_const<T const>>,
							 logic::not_<is_reference<T>>>
	{};

	template<class T>
	inline constexpr bool is_function_v = is_function<T>::value;

#undef ImplIsFun


	// Checks whether T is a floating-point type. Provides the member
	// constant value which is equal to true, if T is the type float,
	// double, long double, including any cv-qualified variants. Otherwise,
	// value is equal to false.
	template<class T>
	struct is_floating_point
		: integral_constant<bool,
			  is_same_v<float,
				  remove_cv_t<
					  T>> || is_same_v<double, remove_cv_t<T>> || is_same_v<long double, faiz::remove_cv_t<T>>>
	{};
	template<class T>
	inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

	// Checks whether T is an integral type. Provides the member constant
	// value which is equal to true, if T is the type bool, char, char16_t,
	// char32_t, wchar_t, short, int, long, long long, or any
	// implementation-defined extended integer types, including any signed,
	// unsigned, and cv-qualified variants. Otherwise, value is equal to
	// false.

	template<typename T, class = void>
	struct is_integral : false_
	{};
	template<typename T>
	struct is_integral<T,
		void_t<decltype(T{} * T{}), decltype(+T{})&, decltype(T{} % 1)>> : true_
	{};
	template<class T>
	inline constexpr bool is_integral_v = is_integral<T>::value;
	//  If T is an arithmetic type (that is, an integral type or a
	// floating-point type) or a cv-qualified version thereof, provides the
	// member constant value equal true. For any other type, value is false.
	//
	// gcc may complains, latest msvc/clang compiles happily
	template<class T, class = void>
	struct is_arithmetic : false_
	{};

	template<class T>
	struct is_arithmetic<T, void_t<decltype(T{} * T{}), decltype(+T{})&>>
		: true_
	{};

	template<class T>
	inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

	template<typename T>
	struct is_pointer_helper : false_
	{};
	template<typename T>
	struct is_pointer_helper<T*> : true_
	{};
	// Checks whether T is **a pointer to object** or **a pointer to
	// function** (but not a pointer to member/member function). Provides
	// the member constant value which is equal to true, if T is a
	// object/function pointer type. Otherwise, value is equal to false.
	template<typename T>
	struct is_pointer : public is_pointer_helper<remove_cv_t<T>>
	{};

	template<class T>
	inline constexpr bool is_pointer_v = is_pointer<T>::value;

	template<typename T>
	struct is_empty : public integral_constant<bool, __is_empty(T)>
	{};

	template<class T>
	inline constexpr bool is_empty_v = is_empty<T>::value;

	template<class T>
	struct is_lvalue_reference : false_
	{};
	template<class T>
	struct is_lvalue_reference<T&> : true_
	{};

	template<class T>
	inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;
	template<class T>
	struct is_rvalue_reference : false_
	{};
	template<class T>
	struct is_rvalue_reference<T&&> : true_
	{};
	template<class T>
	inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;


	template<class T>
	struct is_union : public integral_constant<bool, __is_union(T)>
	{};

	template<class T>
	struct is_enum : public integral_constant<bool, __is_enum(T)>
	{};

	namespace detail
	{
		template<class T>
		char
		test(int T::*);
		struct two
		{
			char c[2];
		};
		template<class T>
		two
		test(...);
	} // namespace detail

	template<class T>
	struct is_class
		: faiz::integral_constant<bool,
			  sizeof(detail::test<T>(0)) == 1 and !faiz::is_union<T>::value>
	{};
	template<class T>
	inline constexpr bool is_class_v = is_class<T>::value;

	// If T is an object type (that is any possibly cv-qualified type other
	// than function, reference, or void types), provides the member
	// constant value equal true. For any other type, value is false.
	// ```cpp
	// !is_reference<T>::value and !is_void<T>::value and
	// !is_function<T>::value
	// ```
	// OR
	// ```cpp
	// is_scalar<T>::value || is_array<T>::value  || is_union<T>::value  ||
	// is_class<T>::value>
	// ```
	template<typename T>
	struct is_object
		: integral_constant<bool,
			  !is_reference_v<T> and !is_void_v<T> and !is_function_v<T>>
	{};
	template<class T>
	inline constexpr bool is_object_v = is_object<T>::value;


	template<class T, unsigned N = 0>
	struct extent : integral_constant<faiz::size_t, 0>
	{};

	template<class T>
	struct extent<T[], 0> : integral_constant<faiz::size_t, 0>
	{};

	template<class T, unsigned N>
	struct extent<T[], N> : extent<T, N - 1>
	{};

	template<class T, faiz::size_t I>
	struct extent<T[I], 0> : integral_constant<faiz::size_t, I>
	{};

	template<class T, faiz::size_t I, unsigned N>
	struct extent<T[I], N> : extent<T, N - 1>
	{};
	template<class T, unsigned N = 0>
	inline constexpr faiz::size_t extent_v = extent<T, N>::value;

	template<typename T, bool = is_referenceable_v<T>>
	struct add_lvalue_reference : type_identity<T>
	{};

	template<typename T>
	struct add_lvalue_reference<T, true> : type_identity<T&>
	{};

	template<typename T>
	using add_lvalue_reference_t = _t<add_lvalue_reference<T>>;

	namespace detail
	{
		template<typename T, bool = is_referenceable_v<T>>
		struct add_rvalue_reference_impl
		{
			using type = T&&;
		};
		template<typename T>
		struct add_rvalue_reference_impl<T, false>
		{
			using type = T;
		};
	} // namespace detail

	// FIXME: add_rvalue_reference_impl is a workaround here, otherwise forward
	// declaration can also not solve type dependency.
	template<typename T>
	struct add_rvalue_reference : detail::add_rvalue_reference_impl<T>
	{};

	template<typename T>
	using add_rvalue_reference_t = _t<add_rvalue_reference<T>>;

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

	template<class T>
	struct add_cv : type_identity<const volatile T>
	{};

	template<class T>
	struct add_const : type_identity<const volatile T>
	{};

	template<class T>
	struct add_volatile : type_identity<volatile T>
	{};

	template<class T>
	using add_cv_t = _t<add_cv<T>>;
	template<class T>
	using add_const_t = _t<add_const<T>>;
	template<class T>
	using add_volatile_t = _t<add_volatile<T>>;

	// p0007r0
	// https://stackoverflow.com/questions/34566063/why-is-the-const-overload-of-as-const-deleted
	// Forms lvalue reference to const type of t
	template<class T>
	constexpr add_const_t<T>&
	as_const(T& t) noexcept;

	//  const rvalue reference overload is deleted to disallow rvalue arguments
	template<class T>
	void
	as_const(const T&&)
		= delete;

	template<bool B, class T = void>
	struct enable_if
	{};

	template<class T>
	struct enable_if<true, T> : type_identity<T>
	{};

	template<bool B, class T = void>
	using enable_if_t = _t<enable_if<B, T>>;

	template<class T>
	struct is_array : false_
	{};

	template<class T>
	struct is_array<T[]> : true_
	{};

	template<class T, faiz::size_t N>
	struct is_array<T[N]> : true_
	{};
	template<class T>
	inline constexpr bool is_array_v = is_array<T>::value;

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
	using is_convertible_helper = decltype(test_aux<To>(declval<From>()));


	template<typename From, typename To>
	constexpr bool
	my_is_convertible()
	{
		if constexpr(std::disjunction<std::is_void<From>,
						 std::is_function<To>,
						 std::is_array<To>>::value)
		{
			return std::is_void_v<To>;
		}
		else
		{
			return std::experimental::
				is_detected<is_convertible_helper, From, To>::value;
		}
	}
	template<typename From, typename To>
	constexpr bool is_convertible_v = my_is_convertible<From, To>();

	template<typename From, typename To>
	struct is_convertible : std::bool_constant<is_convertible_v<From, To>>
	{};

	static_assert(std::experimental::
			is_detected<is_convertible_helper, int(int), int (*)(int)>::value);
	static_assert(is_function<int(int)>(), "");
	// template<typename F,
	// 	typename T,
	// 	bool = is_void<F>::value || is_function<T>::value ||
	// is_array<T>::value> class is_convertible_aux : public
	// integral_constant<bool, is_void<T>::value>
	// {};

	// template<typename F, typename T>
	// class is_convertible_aux<F, T, false>
	// {
	// 	template<typename TT>
	// 	static void __test_aux(TT);
	// 	template<typename FF,
	// 		typename TT,
	// 		typename = decltype(__test_aux<TT>(declval<FF>()))>
	// 	static true_type
	// 	__test(int);
	// 	template<typename FF, typename TT>
	// 	static false_type
	// 	__test(...);

	// public:
	// 	using type = decltype(__test<F, T>(0));
	// };
	// template<typename F, typename T>
	// struct is_convertible : public is_convertible_aux<F, T>::type
	// {};


	// If T is an object or reference type and the variable definition T
	// obj(std::declval<Args>()...); is well-formed, provides the member
	// constant value equal to true. In all other cases, value is false.
	template<class Tp, class... Args>
	struct is_constructible
		: public integral_constant<bool, __is_constructible(Tp, Args...)>
	{};

	template<class T, class... Args>
	inline constexpr bool is_constructible_v
		= is_constructible<T, Args...>::value;

	template<class Tp>
	struct is_copy_constructible
		: public is_constructible<Tp, add_lvalue_reference_t<add_const_t<Tp>>>
	{};

	template<typename Tp>
	inline constexpr bool is_copy_constructible_v
		= is_copy_constructible<Tp>::value;


	// If T is an array type, provides the member constant value equal to
	// the number of dimensions of the array. For any other type, value is
	// 0.
	template<class T>
	struct rank : public integral_constant<faiz::size_t, 0>
	{};
	// If T is an array type, provides the member constant value equal to
	// the number of dimensions of the array. For any other type, value is
	// 0.
	template<class T>
	struct rank<T[]>
		: public integral_constant<faiz::size_t, rank<T>::value + 1>
	{};
	// If T is an array type, provides the member constant value equal to
	// the number of dimensions of the array. For any other type, value is
	// 0.
	template<class T, faiz::size_t N>
	struct rank<T[N]>
		: public integral_constant<faiz::size_t, rank<T>::value + 1>
	{};
	template<class T>
	inline constexpr faiz::size_t rank_v = rank<T>::value;

	namespace detail
	{
		template<class T, bool is_function_type = false>
		struct add_pointer : type_identity<remove_reference_t<T>*>
		{};

		template<class T>
		struct add_pointer<T, true> : type_identity<T>
		{};

		template<class T, class... Args>
		struct add_pointer<T(Args...), true> : type_identity<T (*)(Args...)>
		{};

		template<class T, class... Args>
		struct add_pointer<T(Args..., ...), true>
			: type_identity<T (*)(Args..., ...)>
		{};

	} // namespace detail

	// If **T** is a reference type, then provides the member typedef type
	// which is a pointer to the referred type. Otherwise, if **T** names an
	// object type, a function type that is not **cv-** or **ref-qualified**
	// (since C++17), or a (possibly **cv-qualified**) void type, provides
	// the member typedef type which is the type T*. Otherwise (if T is a
	// **cv-** or **ref-qualified** function type), provides the member
	// typedef type which is the type **T**
	//
	// * type:	pointer to T or to the type referenced by T
	template<class T>
	struct add_pointer : detail::add_pointer<T, is_function_v<T>>
	{};
	template<class T>
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
	template<class T>
	struct decay
	{
	private:
		using U = remove_reference_t<T>;

	public:
		using type = conditional_t<is_array_v<U>,
			remove_extent_t<U>*,
			conditional_t<is_function_v<U>, add_pointer_t<U>, remove_cv_t<U>>>;
	};
	template<class T>
	using decay_t = _t<decay<T>>;

	// is_assignable

	template<typename, typename T>
	struct select_2nd : type_identity<T>
	{};

	template<class T, class Arg>
	_t<select_2nd<decltype((faiz::declval<T>() = faiz::declval<Arg>())), true_>>
	is_assignable_test(int);

	template<class, class>
	false_
	is_assignable_test(...);

	template<class T, class Arg, bool = is_void_v<T> || is_void_v<Arg>>
	struct is_assignable_imp
		: public decltype((faiz::is_assignable_test<T, Arg>(0)))
	{};

	template<class T, class Arg>
	struct is_assignable_imp<T, Arg, true> : public false_
	{};

	template<class T, class Arg>
	struct is_assignable : public is_assignable_imp<T, Arg>
	{};

	template<class T, class Arg>
	inline constexpr bool is_assignable_v = is_assignable<T, Arg>::value;

	template<typename... T>
	struct common_type;

	template<typename T>
	struct common_type<T> : type_identity<decay_t<T>>
	{};
	// Question: Should we use T or decay_t<T> here? The C++11 Standard
	// specifically (20.9.7.6,p3) specifies that it be without decay, but
	// libc++ uses decay.

	template<typename T, typename U>
	struct common_type<T, U>
	{
		using type
			= decay_t<decltype(true ? faiz::declval<T>() : faiz::declval<U>())>;
		// The type of a tertiary
		// expression is set by
		// the compiler to be the
		// common type of the two
		// result types.
	};

	template<typename T, typename U, typename... V>
	struct common_type<T, U, V...>
		: type_identity<_t<common_type<_t<common_type<T, U>>, V...>>>
	{};

	template<typename... T>
	using common_type_t = _t<common_type<T...>>;


	namespace details
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
		// faiz::experimental::detected_or_t<faiz::true_, pre_is_base_of,
		// Base, Derived>;
		template<typename Base, typename Derived, typename = void>
		struct pre_is_base_of2 : public faiz::true_
		{};
		// note faiz::void_t is a C++17 feature
		template<typename Base, typename Derived>
		struct pre_is_base_of2<Base,
			Derived,
			faiz::void_t<pre_is_base_of<Base, Derived>>>
			: public pre_is_base_of<Base, Derived>
		{};
	} // namespace details

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
	struct is_base_of : public faiz::conditional_t<faiz::is_class<Base>::value
								&& faiz::is_class<Derived>::value,
							details::pre_is_base_of2<Base, Derived>,
							faiz::false_>
	{};
	template<class Base, class Derived>
	inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

	template<class T>
	struct is_member_pointer_helper : faiz::false_
	{};

	template<class T, class U>
	struct is_member_pointer_helper<T U::*> : faiz::true_
	{};

	template<class T>
	struct is_member_pointer : is_member_pointer_helper<_t<faiz::remove_cv<T>>>
	{};
	template<class T>
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
	template<class T>
	struct is_scalar : faiz::disjunction<faiz::is_arithmetic<T>,
						   faiz::is_enum<T>,
						   faiz::is_pointer<T>,
						   faiz::is_member_pointer<T>,
						   faiz::is_null_pointer<T>>
	{};
	template<class T>
	inline constexpr bool is_scalar_v = is_scalar<T>::value;

	template<class T>
	struct is_member_function_pointer_helper : faiz::false_
	{};

	template<class T, class U>
	struct is_member_function_pointer_helper<T U::*> : faiz::is_function<T>
	{};
	//
	//  Checks whether T is a non-static member function pointer. Provides the
	// member constant value which is equal to true, if T is a non-static member
	// function pointer type. Otherwise, value is equal to false.
	template<class T>
	struct is_member_function_pointer
		: is_member_function_pointer_helper<faiz::remove_cv_t<T>>
	{};
	template<class T>
	inline constexpr bool is_member_function_pointer_v
		= is_member_function_pointer<T>::value;

	// Checks whether T is a non-static member object. Provides the member
	// constant value which is equal to true, if T is a non-static member object
	// type. Otherwise, value is equal to false.
	template<class T>
	struct is_member_object_pointer
		: faiz::integral_constant<bool,
			  faiz::is_member_pointer<T>::value
				  && !faiz::is_member_function_pointer<T>::value>
	{};
	template<class T>
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
		: public faiz::integral_constant<bool,
			  faiz::is_base_of_v<Base,
				  Derived> && !faiz::is_same_v<faiz::remove_cv_t<Base>, faiz::remove_cv_t<Derived>>>
	{};
	template<class Base, class Derived>
	inline constexpr bool is_derived_of_v = is_base_of<Base, Derived>::value;

	template<class T>
	struct is_final : public integral_constant<bool, __is_final(T)>
	{};

	template<class T>
	constexpr bool is_final_v = is_final<T>::value;

	struct two
	{
		char lx[2];
	};

	template<typename T>
	char& is_polymorphic_impl_aux(enable_if_t<
		sizeof((T*)dynamic_cast<const volatile void*>(declval<T*>())) != 0,
		int>);
	template<typename T>
	two&
	is_polymorphic_impl_aux(...);

	template<class T>
	struct is_polymorphic : public integral_constant<bool,
								sizeof(is_polymorphic_impl_aux<T>(0)) == 1>
	{};

	template<class T>
	constexpr bool is_polymorphic_v = is_final<T>::value;

	template<class T>
	struct is_copy_assignable
		: faiz::is_assignable<_t<faiz::add_lvalue_reference<T>>,
			  faiz::add_lvalue_reference_t<const T>>
	{};

	template<typename T, typename _Up>
	struct is_trivially_assignable
		: public integral_constant<bool, __is_trivially_assignable(T, _Up)>
	{};

	template<class T, class U>
	inline constexpr bool is_trivially_assignable_v
		= is_trivially_assignable<T, U>::value;

	template<class T>
	struct is_trivially_copy_assignable
		: faiz::is_trivially_assignable<_t<faiz::add_lvalue_reference<T>>,
			  faiz::add_lvalue_reference_t<const T>>
	{};

	template<bool, class T, class A>
	struct is_nothrow_assignable_aux;

	template<class T, class A>
	struct is_nothrow_assignable_aux<false, T, A> : public false_
	{};

	template<class T, class A>
	struct is_nothrow_assignable_aux<true, T, A>
		: public integral_constant<bool,
			  noexcept(faiz::declval<T>() = faiz::declval<A>())>
	{};

	template<class T, class A>
	struct is_nothrow_assignable
		: public is_nothrow_assignable_aux<is_assignable<T, A>::value, T, A>
	{};

	template<class T, class U>
	inline constexpr bool is_nothrow_assignable_v
		= is_nothrow_assignable<T, U>::value;

	template<class T>
	struct is_nothrow_copy_assignable
		: faiz::is_nothrow_assignable<_t<faiz::add_lvalue_reference<T>>,
			  _t<faiz::add_lvalue_reference<const T>>>
	{};

	template<class T>
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
	struct is_standard_layout
		: public integral_constant<bool, __is_standard_layout(Tp)>
	{};

	template<class T>
	inline constexpr bool is_standard_layout_v = is_standard_layout<T>::value;

	template<class _Tp>
	struct is_trivially_copyable : bool_<is_scalar_v<remove_all_extents_t<_Tp>>>
	{};

	template<class _Tp>
	inline constexpr bool is_trivially_copyable_v
		= is_trivially_copyable<_Tp>::value;


	template<class Tp>
	struct is_trivial : public integral_constant<bool, __is_trivial(Tp)>
	{};

	template<class T>
	inline constexpr bool is_trivial_v = is_trivial<T>::value;


	template<class T>
	struct is_unknown_bound_array : false_
	{};
	template<class T>
	struct is_unknown_bound_array<T[]> : true_
	{};

	template<class T>
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
	and not is_unknown_bound_array<T>::value
		and not faiz::is_function_v<T>;

#elif BOOST_COMP_GNUC
	template<class T>
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

	template<class _Tp>
	struct __libcpp_trivial_destructor
		: public integral_constant<bool,
			  is_scalar<_Tp>::value || is_reference<_Tp>::value>
	{};

	template<class _Tp>
	struct is_trivially_destructible
		: public __libcpp_trivial_destructor<
			  typename remove_all_extents<_Tp>::type>
	{};

	template<class _Tp>
	struct is_trivially_destructible<_Tp[]> : public false_
	{};

	template<class _Tp>
	inline constexpr bool is_trivially_destructible_v
		= is_trivially_destructible<_Tp>::value;

} // namespace rider::faiz

#endif
