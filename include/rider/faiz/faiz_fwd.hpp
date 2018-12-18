/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2014-present
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

#ifndef FAIZ_FWD_HPP
#define FAIZ_FWD_HPP
#include "rider/faiz/cstddef.hpp"
#include "rider/faiz/debug.hpp"
#include "rider/faiz/initializer_list.hpp"
#include <boost/preprocessor.hpp>
#include <iostream>
#include <type_traits>
namespace Rider
{

	template<typename T>
	using _t = typename T::type;
	template<typename...>
	using void_t = void;
	template<typename T>
	struct type_identity
	{
		using type = T;
	};
	template<typename T>
	using type_identity_t = _t<type_identity<T>>;

	using index = std::ptrdiff_t;

#define PAIR(A, B) A, B

#define INLINE_VARIABLE(type, name) inline constexpr type name{};


#define DEPRECATED(MSG) __attribute__((deprecated(MSG)))
#define IMPL(...) __VA_ARGS__
#define SEMICOLON ;
#define COMMA ,

#define DECLTYPE_AUTO_RETURN(...) \
	->decltype(__VA_ARGS__) \
	{ \
		return (__VA_ARGS__); \
	} \
	/**/

#define DECLTYPE_AUTO_RETURN_NOEXCEPT(...) \
	noexcept(noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))) \
		->decltype(__VA_ARGS__) \
	{ \
		return (__VA_ARGS__); \
	} \
	/**/

#define AUTO_RETURN_NOEXCEPT(...) \
	noexcept(noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))) \
	{ \
		return (__VA_ARGS__); \
	} \
	/**/

#define DECLTYPE_NOEXCEPT(...) \
	noexcept(noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))) \
		->decltype(__VA_ARGS__) /**/

} // namespace Rider

namespace Rider::Faiz
{
	template<typename T, T v>
	struct integral_constant;
	template<typename T, T v>
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

 #undef ImplDeclIntTDe
 #undef ImplDeclIntT

		// clang-format on

		using true_ = bool_<true>;
	using false_ = bool_<false>;
	template<bool B>
	using bool_constant = bool_<B>;
	using true_type = true_;
	using false_type = false_;

	template<typename T>
	using sizeof_able = size_t_<sizeof(T)>;
} // namespace Rider::Faiz

namespace Rider::Faiz::detail
{
	template<template<typename First, typename Second> class Trait,
		typename First,
		typename Second,
		typename... Rest>
	constexpr bool
	binaryTraitAre_impl()
	{
		if constexpr(sizeof...(Rest) == 0)
		{
			return Trait<First, Second>{}();
		}
		else
		{
			return Trait<First, Second>{}()
				and binaryTraitAre_impl<Trait, Rest...>();
		}
	}

	template<template<typename First, typename Second> class Trait,
		typename First,
		typename Second,
		typename... Rest>
	constexpr bool
	binaryTraitOr_impl()
	{
		if constexpr(sizeof...(Rest) == 0)
		{
			return Trait<First, Second>{}();
		}
		else
		{
			return Trait<First, Second>{}()
				or binaryTraitOr_impl<Trait, Rest...>();
		}
	}
} // namespace Rider::Faiz::detail

namespace Rider::Faiz
{
#define IS(name) \
	template<typename T> \
	inline constexpr bool is_##name##_v = is_##name<T>::value;

#define BI_IS(name) \
	template<typename From, typename To> \
	inline constexpr bool is_##name##_v = is_##name<From, To>::value;

#define NOT(name) \
	template<typename T> \
	inline constexpr bool not_##name##_v = not is_##name<T>::value; \
	template<typename T> \
	struct not_##name : bool_<not_##name##_v<T>> \
	{};

#define BI_NOT(name) \
	template<typename From, typename To> \
	inline constexpr bool not_##name##_v = not is_##name<From, To>::value; \
	template<typename From, typename To> \
	struct not_##name : bool_<not_##name##_v<From, To>> \
	{};

#define ARE(name) \
	template<typename... T> \
	inline constexpr bool are_##name##_v = (is_##name##_v<T> && ...); \
\
	template<typename... T> \
	struct are_##name : bool_<are_##name##_v<T...>> \
	{};

#define BI_ARE(name) \
	template<typename... T> \
	inline constexpr bool are_##name##_v \
		= detail::binaryTraitAre_impl<is_##name, T...>(); \
\
	template<typename... T> \
	struct are_##name : bool_<are_##name##_v<T...>> \
	{};

#define ANY(name) \
	template<typename... T> \
	inline constexpr bool any_##name##_v = (is_##name##_v<T> || ...); \
\
	template<typename... T> \
	struct any_##name : bool_<any_##name##_v<T...>> \
	{};

#define BI_ANY(name) \
	template<typename... T> \
	inline constexpr bool any_##name##_v \
		= detail::binaryTraitOr_impl<is_##name, T...>(); \
\
	template<typename... T> \
	struct any_##name : bool_<any_##name##_v<T...>> \
	{};


#define IS_NOT_ARE_ANY(name) \
	IS(name); \
	NOT(name); \
	ARE(name); \
	ANY(name);

#define BI_IS_NOT_ARE_ANY(name) \
	BI_IS(name); \
	BI_NOT(name); \
	BI_ARE(name); \
	BI_ANY(name);

} // namespace Rider::Faiz


// forwad declare type_traits
namespace Rider::Faiz
{
	// FIXME: below is traits that I am not able to implement yet.
	using std::has_unique_object_representations;
	using std::is_constructible;
	using std::is_constructible_v;
	using std::is_trivially_constructible;
	using std::is_trivially_constructible_v;

	using std::is_default_constructible;
	IS_NOT_ARE_ANY(default_constructible);

	using std::is_trivially_default_constructible;
	IS_NOT_ARE_ANY(trivially_default_constructible);

	using std::is_nothrow_default_constructible;
	IS_NOT_ARE_ANY(nothrow_default_constructible);

	using std::is_nothrow_constructible;
	using std::is_nothrow_constructible_v;

	//  using std::is_nothrow_copy_assignable;
	// using std::is_nothrow_copy_assignable_v;
	using std::is_standard_layout;
	using std::is_standard_layout_v;
	using std::is_trivially_copyable;
	using std::is_trivially_copyable_v;
	using std::is_trivial;
	using std::is_trivial_v;
	using std::is_empty;
	using std::is_empty_v;
	using std::is_trivially_destructible;
	using std::is_trivially_destructible_v;
	using std::is_final;
	using std::is_final_v;
	using std::is_union;
	using std::is_union_v;
	using std::is_class;
	using std::is_class_v;

	using std::is_trivially_assignable;
	using std::is_trivially_assignable_v;
	using std::is_enum;
	using std::is_enum_v;

	template<typename T>
	struct reference_wrapper;

	template<typename>
	struct is_reference_wrapper;

	template<class T>
	struct add_cv;
	template<class T>
	struct add_const;
	template<class T>
	struct add_volatile;

	template<class T>
	using add_cv_t = _t<add_cv<T>>;
	template<class T>
	using add_const_t = _t<add_const<T>>;
	template<class T>
	using add_volatile_t = _t<add_volatile<T>>;

	template<class T>
	struct remove_const;
	template<class T>
	struct remove_volatile;

	template<class T>
	struct is_destructible;


	template<class T>
	struct is_nothrow_destructible;

	template<class T>
	using remove_const_t = _t<remove_const<T>>;

	template<class T>
	using remove_volatile_t = _t<remove_volatile<T>>;

	template<typename T>
	using remove_cv = remove_volatile<remove_const_t<T>>;

	template<typename T>
	using remove_cv_t = _t<remove_cv<T>>;

	template<class T>
	struct add_lvalue_reference;
	template<class T>
	struct add_rvalue_reference;
	template<class T>
	using add_lvalue_reference_t = _t<add_lvalue_reference<T>>;
	template<class T>
	using add_rvalue_reference_t = _t<add_rvalue_reference<T>>;

	template<class T>
	add_rvalue_reference_t<T>
	declval() noexcept;

	//  If T is an object type (that is any possibly cv-qualified type other
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
	template<class T>
	struct is_object;
	template<class T>
	inline constexpr bool is_object_v = is_object<T>::value;

	template<class T, class U>
	struct is_assignable;

	template<class T, class U>
	struct is_nothrow_assignable;
	template<class T, class U>
	inline constexpr bool is_assignable_v = is_assignable<T, U>::value;

	template<class T, class U>
	inline constexpr bool is_nothrow_assignable_v
		= is_nothrow_assignable<T, U>::value;
	template<typename Base, typename Derived>
	struct is_base_of;
	template<typename T>
	struct remove_reference;
	template<typename T>
	using remove_reference_t = _t<remove_reference<T>>;

	template<class T>
	struct is_lvalue_reference;
	template<class T>
	inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

	template<typename T>
	struct remove_cvref;
	template<typename T>
	using remove_cvref_t = remove_cvref<T>;
	template<typename T>
	struct is_integral;
	template<bool B, typename T = void>
	struct enable_if;
	template<bool B, class T = void>
	using enable_if_t = _t<enable_if<B, T>>;
	template<bool B, typename T, typename F>
	struct conditional;

	template<typename T, class U>
	struct is_same;
	template<class T, class U>

	inline constexpr bool is_same_v = is_same<T, U>::value;

} // namespace Rider::Faiz


// implement integer_sequence here
// /*
// 39 level loop-unrolling is fastest on my os. nearly as fast as built_in
// integer_sequence
// https://stackoverflow.com/questions/53350706/how-to-understand-libcxxs-implementation-of-make-integer-sequence
namespace Rider::Faiz
{
	// implement make_integer_sequence. Recursively dividing
	// make_integer_sequence<N> into repeat_39_times<make_integer_sequence<N /
	// 39>>
	// /* + remainder */, so it's "recursion with a base case"
	// More or fewer than 39 will be slower on my machine.
	namespace detail
	{
		template<class Tindex, Tindex... V>
		struct integer_sequence_aux
		{
			template<template<class Tfrom, Tfrom...> class TtoIndexSeq,
				class Tto>
			using convert = TtoIndexSeq<Tto, V...>;
		};

		template<typename T, Faiz::size_t... Vextra>
		struct repeat;
		template<typename T, T... Vseq, Faiz::size_t... Vextra>
		struct repeat<integer_sequence_aux<T, Vseq...>, Vextra...>
		{
			// clang-format off
            using type = integer_sequence_aux<
                T,
                Vseq...,
                sizeof...(Vseq) + Vseq...,
 #define Aux(N)  N * sizeof...(Vseq) + Vseq...
                Aux(  2 ), Aux(  3 ), Aux(  4 ),
                Aux(  5 ), Aux(  6 ), Aux(  7 ),
                Aux(  8 ), Aux(  9 ), Aux( 10 ),
                Aux( 11 ), Aux( 12 ), Aux( 13 ),
                Aux( 14 ), Aux( 15 ), Aux( 16 ),
                Aux( 17 ), Aux( 18 ), Aux( 19 ),
                Aux( 20 ), Aux( 21 ), Aux( 22 ),
                Aux( 23 ), Aux( 24 ), Aux( 25 ),
                Aux( 26 ), Aux( 27 ), Aux( 28 ),
                Aux( 29 ), Aux( 30 ), Aux( 31 ),
                Aux( 32 ), Aux( 33 ), Aux( 34 ),
                Aux( 35 ), Aux( 36 ), Aux( 37 ),
                Aux( 38 ),
                Vextra...>;
 #undef Aux
			// clang-format on
		};

		template<Faiz::size_t V>
		struct parity;
		template<Faiz::size_t V>
		struct make : parity<V % 39>::template pmake<V>
		{};

		template<>
		struct make<0> : type_identity<integer_sequence_aux<Faiz::size_t>>
		{};
#define MAKE(N, ...) \
	template<> \
	struct make<N> \
		: type_identity<integer_sequence_aux<Faiz::size_t, __VA_ARGS__>> \
	{};
#define PARITY(N, ...) \
	template<> \
	struct parity<N> \
	{ \
		template<Faiz::size_t V> \
		struct pmake : repeat<_t<make<V / 39>>, __VA_ARGS__> \
		{}; \
	};
#define AppendV(z, n, data) (V - n)
#define VSequence(n) BOOST_PP_REPEAT(n, AppendV, )
#define ImplParityAux(INTEGER) \
	BOOST_PP_SEQ_ENUM( \
		BOOST_PP_SEQ_REVERSE(BOOST_PP_SEQ_TAIL(VSequence(INTEGER))))
#define ImplParity(N) ImplParityAux(BOOST_PP_INC(N))
#define TO_SEQ_ELEM(z, n, data) (n)
#define IntegerSequence(n) BOOST_PP_REPEAT(n, TO_SEQ_ELEM, )
#define ImplMakeAux(INTEGER) BOOST_PP_SEQ_ENUM(IntegerSequence(INTEGER))
#define ImplMake(N) ImplMakeAux(N)
#define ParityAndMake(N) \
	PARITY(N, ImplParity(N)) \
	MAKE(N, ImplMake(N))
#define MultipleParityAndMake(N)
		// clang-format off
        ParityAndMake(  1 ) ParityAndMake(  2 )
        ParityAndMake(  3 ) ParityAndMake(  4 )
        ParityAndMake(  5 ) ParityAndMake(  6 )
        ParityAndMake(  7 ) ParityAndMake(  8 )
        ParityAndMake(  9 ) ParityAndMake( 10 )
        ParityAndMake( 11 ) ParityAndMake( 12 )
        ParityAndMake( 13 ) ParityAndMake( 14 )
        ParityAndMake( 15 ) ParityAndMake( 16 )
        ParityAndMake( 17 ) ParityAndMake( 18 )
        ParityAndMake( 19 ) ParityAndMake( 20 )
        ParityAndMake( 21 ) ParityAndMake( 22 )
        ParityAndMake( 23 ) ParityAndMake( 24 )
        ParityAndMake( 25 ) ParityAndMake( 26 )
        ParityAndMake( 27 ) ParityAndMake( 28 )
        ParityAndMake( 29 ) ParityAndMake( 30 )
        ParityAndMake( 31 ) ParityAndMake( 32 )
        ParityAndMake( 33 ) ParityAndMake( 34 )
        ParityAndMake( 35 ) ParityAndMake( 36 )
        ParityAndMake( 37 ) ParityAndMake( 38 )
        ParityAndMake( 39 )
		// clang-format on

	} // namespace detail

	template<class T, T... Vseq>
	struct integer_sequence
	{
		typedef T value_type;
		static_assert(Faiz::is_integral<T>::value,
			"Faiz::integer_sequence can only be instantiated with an integral "
			"type");
		static constexpr Faiz::size_t
		size() noexcept
		{
			return sizeof...(Vseq);
		}
	};

	template<Faiz::size_t... Vseq>
	using index_sequence = integer_sequence<Faiz::size_t, Vseq...>;

	template<typename T, T V>
	using make_integer_sequence_aux_unchecked =
		typename detail::make<V>::type::template convert<integer_sequence, T>;

	template<class T, T V>
	struct make_integer_sequence_checked
		: type_identity<make_integer_sequence_aux_unchecked<T, 0 <= V ? V : 0>>
	{
		static_assert(std::is_integral<T>::value,
			"Faiz::make_integer_sequence can only be instantiated with an "
			"integral type");
		static_assert(0 <= V,
			"Faiz::make_integer_sequence must have a non-negative sequence "
			"length");
	};

	template<class T, T V>
	using make_integer_sequence_aux = _t<make_integer_sequence_checked<T, V>>;

	template<class T, T V>
	using make_integer_sequence = make_integer_sequence_aux<T, V>;

	template<Faiz::size_t V>
	using make_index_sequence = make_integer_sequence<Faiz::size_t, V>;

	template<class... T>
	using index_sequence_for = make_index_sequence<sizeof...(T)>;
} // namespace Rider::Faiz

// forward declare meta structs.
namespace Rider::Faiz::meta
{
	template<typename... Ts>
	struct list;

	template<typename T>
	struct id;

	template<template<typename...> class>
	struct quote;

	template<typename T, template<T...> class F>
	struct quote_i;

	template<typename... Fs>
	struct compose;

	template<template<typename...> class C, typename... Ts>
	struct defer;

	template<typename T, template<T...> class C, T... Is>
	struct defer_i;

	namespace extension
	{
		template<typename F, typename List>
		struct apply;
	}

} // namespace Rider::Faiz::meta


namespace Rider::Faiz::meta
{

	namespace detail
	{
		template<typename T>
		constexpr T*
		_nullptr_v()
		{
			return nullptr;
		}
	} // namespace detail

	struct nil_
	{};

	namespace lazy
	{
		template<typename T>
		using _t = defer<_t, T>;
	} // namespace lazy

	template<typename T>
	using inc
		= integral_constant<decltype(T::type::value + 1), T::type::value + 1>;

	template<typename T>
	using dec
		= integral_constant<decltype(T::type::value - 1), T::type::value - 1>;

	template<typename T, typename U>
	using plus = integral_constant<decltype(T::type::value + U::type::value),
		T::type::value + U::type::value>;

	template<typename T, typename U>
	using minus = integral_constant<decltype(T::type::value - U::type::value),
		T::type::value - U::type::value>;

	template<typename T, typename U>
	using multiplies
		= integral_constant<decltype(T::type::value * U::type::value),
			T::type::value * U::type::value>;

	template<typename T, typename U>
	using divides = integral_constant<decltype(T::type::value / U::type::value),
		T::type::value / U::type::value>;

	template<typename T>
	using negate
		= integral_constant<decltype(-T::type::value), -T::type::value>;

	template<typename T, typename U>
	using modulus = integral_constant<decltype(T::type::value % U::type::value),
		T::type::value % U::type::value>;

	template<typename T, typename U>
	using equal_to = bool_<T::type::value == U::type::value>;

	template<typename T, typename U>
	using not_equal_to = bool_<T::type::value != U::type::value>;

	template<typename T, typename U>
	using greater = bool_<(T::type::value > U::type::value)>;

	template<typename T, typename U>
	using less = bool_<(T::type::value < U::type::value)>;

	template<typename T, typename U>
	using greater_equal = bool_<(T::type::value >= U::type::value)>;

	template<typename T, typename U>
	using less_equal = bool_<(T::type::value <= U::type::value)>;

	template<typename T, typename U>
	using bit_and = integral_constant<decltype(T::type::value & U::type::value),
		T::type::value & U::type::value>;

	template<typename T, typename U>
	using bit_or = integral_constant<decltype(T::type::value | U::type::value),
		T::type::value | U::type::value>;

	template<typename T, typename U>
	using bit_xor = integral_constant<decltype(T::type::value ^ U::type::value),
		T::type::value ^ U::type::value>;

	template<typename T>
	using bit_not
		= integral_constant<decltype(~T::type::value), ~T::type::value>;

	namespace lazy
	{
		template<typename T>
		using inc = defer<inc, T>;

		template<typename T>
		using dec = defer<dec, T>;

		template<typename T, typename U>
		using plus = defer<plus, T, U>;

		template<typename T, typename U>
		using minus = defer<minus, T, U>;

		template<typename T, typename U>
		using multiplies = defer<multiplies, T, U>;

		template<typename T, typename U>
		using divides = defer<divides, T, U>;

		template<typename T>
		using negate = defer<negate, T>;

		template<typename T, typename U>
		using modulus = defer<modulus, T, U>;

		template<typename T, typename U>
		using equal_to = defer<equal_to, T, U>;

		template<typename T, typename U>
		using not_equal_to = defer<not_equal_to, T, U>;

		template<typename T, typename U>
		using greater = defer<greater, T, U>;

		template<typename T, typename U>
		using less = defer<less, T, U>;

		template<typename T, typename U>
		using greater_equal = defer<greater_equal, T, U>;

		template<typename T, typename U>
		using less_equal = defer<less_equal, T, U>;

		template<typename T, typename U>
		using bit_and = defer<bit_and, T, U>;

		template<typename T, typename U>
		using bit_or = defer<bit_or, T, U>;

		template<typename T, typename U>
		using bit_xor = defer<bit_xor, T, U>;

		template<typename T>
		using bit_not = defer<bit_not, T>;
	} // namespace lazy


	template<typename F, typename... Args>
	using invoke = typename F::template invoke<Args...>;

	namespace lazy
	{
		template<typename F, typename... Args>
		using invoke = defer<invoke, F, Args...>;
	} // namespace lazy

	template<typename T>
	struct id : type_identity<T>
	{
		template<typename...>
		using invoke = T;
	};

	template<typename T>
	using id_t = _t<id<T>>;

	namespace lazy
	{
		template<typename T>
		using id = defer<id, T>;
	} // namespace lazy

	namespace detail
	{
		template<typename, typename = void>
		struct is_trait_ : type_identity<false_>
		{};

		template<typename T>
		struct is_trait_<T, void_t<typename T::type>> : type_identity<true_>
		{};

		template<typename, typename = void>
		struct is_callable_ : type_identity<false_>
		{};

		template<typename T>
		struct is_callable_<T, void_t<quote<T::template invoke>>>
			: type_identity<true_>
		{};

		template<template<typename...> class C,
			typename... Ts,
			template<typename...> class D = C>
		id<D<Ts...>>
		try_defer_(int);
		template<template<typename...> class C, typename... Ts>
		nil_
		try_defer_(long);

		template<template<typename...> class C, typename... Ts>
		using defer_ = decltype(detail::try_defer_<C, Ts...>(0));

		template<typename T,
			template<T...>
			class C,
			T... Is,
			template<T...> class D = C>
		id<D<Is...>>
		try_defer_i_(int);
		template<typename T, template<T...> class C, T... Is>
		nil_
		try_defer_i_(long);

		template<typename T, template<T...> class C, T... Is>
		using defer_i_ = decltype(detail::try_defer_i_<T, C, Is...>(0));

		template<typename T>
		using _t_t = _t<_t<T>>;
	} // namespace detail

	template<typename T>
	using is_trait = _t<detail::is_trait_<T>>;

	template<typename T>
	using is_callable = _t<detail::is_callable_<T>>;

	template<template<typename...> class C, typename... Ts>
	struct defer : detail::defer_<C, Ts...>
	{};

	template<typename T, template<T...> class C, T... Is>
	struct defer_i : detail::defer_i_<T, C, Is...>
	{};

	template<template<typename...> class C, typename... Ts>
	using defer_trait = defer<detail::_t_t, detail::defer_<C, Ts...>>;

	template<class T>
	using sizeof_ = size_t_<sizeof(T)>;

	template<class T>
	using alignof_ = size_t_<alignof(T)>;

	namespace lazy
	{
		template<typename T>
		using sizeof_ = defer<sizeof_, T>;

		template<typename T>
		using alignof_ = defer<alignof_, T>;
	} // namespace lazy


	namespace detail
	{
		template<typename, template<typename...> class>
		struct is_ : false_
		{};

		template<typename... Ts, template<typename...> class C>
		struct is_<C<Ts...>, C> : true_
		{};
	} // namespace detail


	template<typename T, template<typename...> class C>
	using is = _t<detail::is_<T, C>>;


	template<typename... Fs>
	struct compose
	{};

	template<typename F0>
	struct compose<F0>
	{
		template<typename... Ts>
		using invoke = invoke<F0, Ts...>;
	};

	template<typename F0, typename... Fs>
	struct compose<F0, Fs...>
	{
		template<typename... Ts>
		using invoke = invoke<F0, invoke<compose<Fs...>, Ts...>>;
	};

	namespace lazy
	{
		template<typename... Fns>
		using compose = defer<compose, Fns...>;
	} // namespace lazy

	template<template<typename...> class C>
	struct quote
	{
		// Indirection through defer here needed to avoid Core issue 1430
		// http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
		template<typename... Ts>
		using invoke = _t<detail::defer_<C, Ts...>>;
	};

	template<typename T, template<T...> class C>
	struct quote_i
	{
		// Indirection through defer_i here needed to avoid Core issue 1430
		// http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
		template<typename... Ts>
		using invoke = _t<detail::defer_i_<T, C, Ts::type::value...>>;
	};

	template<template<typename...> class C>
	using quote_trait = compose<quote<_t>, quote<C>>;

	template<typename T, template<T...> class C>
	using quote_trait_i = compose<quote<_t>, quote_i<T, C>>;

	template<typename F, typename... Ts>
	struct bind_front
	{
		template<typename... Us>
		using invoke = invoke<F, Ts..., Us...>;
	};

	template<typename F, typename... Us>
	struct bind_back
	{
		template<typename... Ts>
		using invoke = invoke<F, Ts..., Us...>;
	};

	namespace lazy
	{
		template<typename Fn, typename... Ts>
		using bind_front = defer<bind_front, Fn, Ts...>;

		template<typename Fn, typename... Ts>
		using bind_back = defer<bind_back, Fn, Ts...>;
	} // namespace lazy

	namespace extension
	{
		template<typename F, typename List>
		struct apply
		{};

		template<typename F, typename Ret, typename... Args>
		struct apply<F, Ret(Args...)> : lazy::invoke<F, Ret, Args...>
		{};

		template<typename F, template<typename...> class T, typename... Ts>
		struct apply<F, T<Ts...>> : lazy::invoke<F, Ts...>
		{};

		template<typename F, typename T, T... Is>
		struct apply<F, integer_sequence<T, Is...>>
			: lazy::invoke<F, integral_constant<T, Is>...>
		{};
	} // namespace extension

	template<typename C, typename List>
	using apply = _t<extension::apply<C, List>>;

	namespace lazy
	{
		template<typename F, typename List>
		using apply = defer<apply, F, List>;
	}

	template<typename F, typename Q = quote<list>>
	using curry = compose<F, Q>;

	template<typename F>
	using uncurry = bind_front<quote<apply>, F>;

	namespace lazy
	{
		template<typename F, typename Q = quote<list>>
		using curry = defer<curry, F, Q>;

		template<typename F>
		using uncurry = defer<uncurry, F>;
	} // namespace lazy

	template<typename F>
	struct flip
	{
	private:
		template<typename... Ts>
		struct impl
		{};
		template<typename A, typename B, typename... Ts>
		struct impl<A, B, Ts...> : lazy::invoke<F, B, A, Ts...>
		{};

	public:
		template<typename... Ts>
		using invoke = _t<impl<Ts...>>;
	};

	namespace lazy
	{
		template<typename F>
		using flip = defer<flip, F>;
	} // namespace lazy

	namespace detail
	{
		template<typename...>
		struct on_
		{};
		template<typename F, typename... Gs>
		struct on_<F, Gs...>
		{
			template<typename... Ts>
			using invoke = invoke<F, invoke<compose<Gs...>, Ts>...>;
		};
	} // namespace detail

	template<typename... Fs>
	using on = detail::on_<Fs...>;

	namespace lazy
	{
		template<typename F, typename G>
		using on = defer<on, F, G>;
	} // namespace lazy

	namespace detail
	{
		template<typename, typename = bool>
		struct _if_
		{};

		template<typename If>
		struct _if_<list<If>, decltype(bool(If::type::value))>
			: enable_if<If::type::value>
		{};

		template<typename If, typename Then>
		struct _if_<list<If, Then>, decltype(bool(If::type::value))>
			: enable_if<If::type::value, Then>
		{};

		template<typename If, typename Then, typename Else>
		struct _if_<list<If, Then, Else>, decltype(bool(If::type::value))>
			: conditional<If::type::value, Then, Else>
		{};
	} // namespace detail

	template<typename... Args>
	using if_ = _t<detail::_if_<list<Args...>>>;

	template<bool If, typename... Args>
	using if_c = _t<detail::_if_<list<bool_<If>, Args...>>>;

	namespace lazy
	{
		template<typename... Args>
		using if_ = defer<if_, Args...>;

		template<bool If, typename... Args>
		using if_c = if_<bool_<If>, Args...>;
	} // namespace lazy

	namespace detail
	{
		template<bool>
		struct _and_
		{
			template<class...>
			using invoke = true_;
		};

		template<>
		struct _and_<false>
		{
			template<typename Bool, typename... Bools>
			using invoke = invoke<if_c<!Bool::type::value,
									  id<false_>,
									  _and_<0 == sizeof...(Bools)>>,
				Bools...>;
		};

		template<bool>
		struct _or_
		{
			template<class = void>
			using invoke = false_;
		};

		template<>
		struct _or_<false>
		{
			template<typename Bool_, typename... Bools>
			using invoke = invoke<
				if_c<Bool_::type::value, id<true_>, _or_<0 == sizeof...(Bools)>>,
				Bools...>;
		};
	} // namespace detail

	template<bool Bool_>
	using not_c = bool_<!Bool_>;

	template<typename Bool_>
	using not_ = not_c<Bool_::type::value>;

	// template<bool... Bools>
	// using and_c = is_same<integer_sequence<bool, Bools...>,
	// 	integer_sequence<bool, (Bools || true)...>>;
	template<bool... B>
	using and_c = bool_<(B && ...)>;
	template<bool... B>
	inline constexpr bool and_t = bool_<(B && ...)>::value;

	template<typename... Bools>
	using strict_and = and_c<Bools::type::value...>;
	template<typename... Bools>
	inline constexpr bool strict_and_v = and_c<Bools::type::value...>::value;

	template<typename... Bools>
	using and_ = _t<
		defer<detail::_and_<0 == sizeof...(Bools)>::template invoke, Bools...>>;


	template<bool... B>
	struct fold_and : bool_<(B && ...)>
	{};

	template<bool... Bools>
	using or_c = not_<is_same<integer_sequence<bool, Bools...>,
		integer_sequence<bool, (Bools && false)...>>>;

	template<typename... Bools>
	using strict_or = or_c<Bools::type::value...>;

	template<typename... Bools>
	using or_ = _t<
		defer<detail::_or_<0 == sizeof...(Bools)>::template invoke, Bools...>>;

	namespace lazy
	{
		template<typename... Bools>
		using and_ = defer<and_, Bools...>;

		template<typename... Bools>
		using or_ = defer<or_, Bools...>;

		template<typename Bool_>
		using not_ = defer<not_, Bool_>;

		template<typename... Bools>
		using strict_and = defer<strict_and, Bools...>;

		template<typename... Bools>
		using strict_or = defer<strict_or, Bools...>;
	} // namespace lazy

	namespace detail
	{
		template<typename Fun, typename T0>
		struct compose1_
		{
			template<typename X>
			using invoke = invoke<Fun, _t<X>, T0>;
		};

		template<typename Fun,
			typename T0,
			typename T1,
			typename T2,
			typename T3,
			typename T4,
			typename T5,
			typename T6,
			typename T7,
			typename T8,
			typename T9>
		struct compose10_
		{
			template<typename X, typename Y>
			using F = invoke<Fun, X, Y>;

			template<typename S>
			using invoke = F<
				F<F<F<F<F<F<F<F<F<_t<S>, T0>, T1>, T2>, T3>, T4>, T5>, T6>, T7>,
					T8>,
				T9>;
		};

		template<typename, typename, typename>
		struct fold_
		{};

		template<typename State, typename Fun>
		struct fold_<list<>, State, Fun> : State
		{};

		template<typename Head, typename... List, typename State, typename Fun>
		struct fold_<list<Head, List...>, State, Fun>
			: fold_<list<List...>,
				  lazy::invoke<compose1_<Fun, Head>, State>,
				  Fun>
		{};

		template<typename T0,
			typename T1,
			typename T2,
			typename T3,
			typename T4,
			typename T5,
			typename T6,
			typename T7,
			typename T8,
			typename T9,
			typename... List,
			typename State,
			typename Fun>
		struct fold_<list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, List...>,
			State,
			Fun>
			: fold_<list<List...>,
				  lazy::invoke<
					  compose10_<Fun, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>,
					  State>,
				  Fun>
		{};
	} // namespace detail

	template<typename List, typename State, typename Fun>
	using fold = _t<detail::fold_<List, id<State>, Fun>>;

	template<typename List, typename State, typename Fun>
	using accumulate = fold<List, State, Fun>;

	namespace lazy
	{
		template<typename List, typename State, typename Fun>
		using fold = defer<fold, List, State, Fun>;

		template<typename List, typename State, typename Fun>
		using accumulate = defer<accumulate, List, State, Fun>;
	} // namespace lazy

	namespace detail
	{
		template<typename List, typename State, typename Fun>
		struct reverse_fold_
		{};

		template<typename State, typename Fun>
		struct reverse_fold_<list<>, State, Fun> : type_identity<State>
		{};

		template<typename Head, typename... List, typename State, typename Fun>
		struct reverse_fold_<list<Head, List...>, State, Fun>
			: lazy::invoke<compose1_<Fun, Head>,
				  reverse_fold_<list<List...>, State, Fun>>
		{};

		template<typename T0,
			typename T1,
			typename T2,
			typename T3,
			typename T4,
			typename T5,
			typename T6,
			typename T7,
			typename T8,
			typename T9,
			typename... List,
			typename State,
			typename Fun>
		struct reverse_fold_<
			list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, List...>,
			State,
			Fun>
			: lazy::invoke<
				  compose10_<Fun, T9, T8, T7, T6, T5, T4, T3, T2, T1, T0>,
				  reverse_fold_<list<List...>, State, Fun>>
		{};
	} // namespace detail

	template<typename List, typename State, typename Fun>
	using reverse_fold = _t<detail::reverse_fold_<List, State, Fun>>;

	namespace lazy
	{
		template<typename List, typename State, typename Fun>
		using reverse_fold = defer<reverse_fold, List, State, Fun>;
	} // namespace lazy

	using npos = size_t_<Faiz::size_t(-1)>;

	template<typename... Ts>
	struct list
	{
		using type = list;
		/// \return `sizeof...(Ts)`
		static constexpr Faiz::size_t
		size() noexcept
		{
			return sizeof...(Ts);
		}
	};

	template<typename List>
	using size = size_t_<List::size()>;

	namespace lazy
	{
		template<typename List>
		using size = defer<size, List>;
	} // namespace lazy

	namespace detail
	{
		template<typename... Lists>
		struct concat_
		{};

		template<>
		struct concat_<> : type_identity<list<>>
		{};

		template<typename... List1>
		struct concat_<list<List1...>> : type_identity<list<List1...>>
		{};

		template<typename... List1, typename... List2, typename... Rest>
		struct concat_<list<List1...>, list<List2...>, Rest...>
			: concat_<list<List1..., List2...>, Rest...>
		{};

		template<typename... List1,
			typename... List2,
			typename... List3,
			typename... List4,
			typename... List5,
			typename... List6,
			typename... List7,
			typename... List8,
			typename... List9,
			typename... List10,
			typename... Rest>
		struct concat_<list<List1...>,
			list<List2...>,
			list<List3...>,
			list<List4...>,
			list<List5...>,
			list<List6...>,
			list<List7...>,
			list<List8...>,
			list<List9...>,
			list<List10...>,
			Rest...> : concat_<list<List1...,
								   List2...,
								   List3...,
								   List4...,
								   List5...,
								   List6...,
								   List7...,
								   List8...,
								   List9...,
								   List10...>,
						   Rest...>
		{};
	} // namespace detail

	template<typename... Lists>
	using concat = _t<detail::concat_<Lists...>>;

	namespace lazy
	{
		template<typename... Lists>
		using concat = defer<concat, Lists...>;
	} // namespace lazy

	template<typename ListOfLists>
	using join = apply<quote<concat>, ListOfLists>;

	namespace lazy
	{
		template<typename ListOfLists>
		using join = defer<join, ListOfLists>;
	} // namespace lazy

	namespace detail
	{
		template<typename, typename = void>
		struct transform_
		{};

		template<typename... Ts, typename Fun>
		struct transform_<list<list<Ts...>, Fun>, void_t<invoke<Fun, Ts>...>>
			: type_identity<list<invoke<Fun, Ts>...>>
		{};

		template<typename... Ts0, typename... Ts1, typename Fun>
		struct transform_<list<list<Ts0...>, list<Ts1...>, Fun>,
			void_t<invoke<Fun, Ts0, Ts1>...>>
			: type_identity<list<invoke<Fun, Ts0, Ts1>...>>
		{};
	} // namespace detail

	template<typename... Args>
	using transform = _t<detail::transform_<list<Args...>>>;

	namespace lazy
	{
		template<typename... Args>
		using transform = defer<transform, Args...>;
	} // namespace lazy

	namespace detail
	{
		template<typename T, Faiz::size_t>
		using first_ = T;

		template<typename T, typename Ints>
		struct repeat_n_c_
		{};

		template<typename T, Faiz::size_t... Is>
		struct repeat_n_c_<T, index_sequence<Is...>>
			: type_identity<list<first_<T, Is>...>>
		{};
	} // namespace detail

	template<Faiz::size_t N, typename T = void>
	using repeat_n_c = _t<detail::repeat_n_c_<T, make_index_sequence<N>>>;

	template<typename N, typename T = void>
	using repeat_n = repeat_n_c<N::type::value, T>;

	namespace lazy
	{
		template<typename N, typename T = void>
		using repeat_n = defer<repeat_n, N, T>;

		template<Faiz::size_t N, typename T = void>
		using repeat_n_c = defer<repeat_n, size_t_<N>, T>;
	} // namespace lazy

	namespace detail
	{
		template<typename VoidPtrs>
		struct at_impl_;

		template<typename... VoidPtrs>
		struct at_impl_<list<VoidPtrs...>>
		{
			static nil_
			eval(...);

			template<typename T, typename... Us>
			static T
			eval(VoidPtrs..., T*, Us*...);
		};

		template<typename List, Faiz::size_t N>
		struct at_
		{};

		template<typename... Ts, Faiz::size_t N>
		struct at_<list<Ts...>, N>
			: decltype(at_impl_<repeat_n_c<N, void*>>::eval(
				  static_cast<id<Ts>*>(nullptr)...))
		{};
	} // namespace detail

	template<typename List, Faiz::size_t N>
	using at_c = _t<detail::at_<List, N>>;

	template<typename List, typename N>
	using at = at_c<List, N::type::value>;

	namespace lazy
	{
		template<typename List, typename N>
		using at = defer<at, List, N>;
	} // namespace lazy

	namespace detail
	{
		template<typename VoidPtrs>
		struct drop_impl_
		{
			static nil_
			eval(...);
		};

		template<typename... VoidPtrs>
		struct drop_impl_<list<VoidPtrs...>>
		{
			static nil_
			eval(...);

			template<typename... Ts>
			static id<list<Ts...>>
			eval(VoidPtrs..., id<Ts>*...);
		};

		template<>
		struct drop_impl_<list<>>
		{
			template<typename... Ts>
			static id<list<Ts...>>
			eval(id<Ts>*...);
		};

		template<typename List, typename N>
		struct drop_
		{};

		template<typename... Ts, typename N>
		struct drop_<list<Ts...>, N>
			: decltype(drop_impl_<repeat_n<N, void*>>::eval(
				  detail::_nullptr_v<id<Ts>>()...))
		{};
	} // namespace detail

	template<typename List, typename N>
	using drop = _t<detail::drop_<List, N>>;

	template<typename List, Faiz::size_t N>
	using drop_c = _t<detail::drop_<List, size_t_<N>>>;

	namespace lazy
	{
		template<typename List, typename N>
		using drop = defer<drop, List, N>;
	} // namespace lazy

	namespace detail
	{
		template<typename List>
		struct front_
		{};

		template<typename Head, typename... List>
		struct front_<list<Head, List...>> : type_identity<Head>
		{};
	} // namespace detail

	template<typename List>
	using front = _t<detail::front_<List>>;

	namespace lazy
	{
		template<typename List>
		using front = defer<front, List>;
	} // namespace lazy

	namespace detail
	{
		template<typename List>
		struct back_
		{};

		template<typename Head, typename... List>
		struct back_<list<Head, List...>>
			: type_identity<at_c<list<Head, List...>, sizeof...(List)>>
		{};
	} // namespace detail

	template<typename List>
	using back = _t<detail::back_<List>>;

	namespace lazy
	{
		template<typename List>
		using back = defer<back, List>;
	} // namespace lazy

	namespace detail
	{
		template<typename List, typename T>
		struct push_front_
		{};

		template<typename... List, typename T>
		struct push_front_<list<List...>, T> : type_identity<list<T, List...>>
		{};
	} // namespace detail

	template<typename List, typename T>
	using push_front = _t<detail::push_front_<List, T>>;

	namespace lazy
	{
		template<typename List, typename T>
		using push_front = defer<push_front, List, T>;
	} // namespace lazy

	namespace detail
	{
		template<typename List>
		struct pop_front_
		{};

		template<typename Head, typename... List>
		struct pop_front_<list<Head, List...>> : type_identity<list<List...>>
		{};
	} // namespace detail

	template<typename List>
	using pop_front = _t<detail::pop_front_<List>>;

	namespace lazy
	{
		template<typename List>
		using pop_front = defer<pop_front, List>;
	} // namespace lazy

	namespace detail
	{
		template<typename List, typename T>
		struct push_back_
		{};

		template<typename... List, typename T>
		struct push_back_<list<List...>, T> : type_identity<list<List..., T>>
		{};
	} // namespace detail

	template<typename List, typename T>
	using push_back = _t<detail::push_back_<List, T>>;

	namespace lazy
	{
		template<typename List, typename T>
		using push_back = defer<push_back, List, T>;
	} // namespace lazy

	namespace detail
	{
		template<typename T, typename U>
		using min_ = if_<less<U, T>, U, T>;

		template<typename T, typename U>
		using max_ = if_<less<U, T>, T, U>;
	} // namespace detail

	template<typename... Ts>
	using min
		= fold<pop_front<list<Ts...>>, front<list<Ts...>>, quote<detail::min_>>;

	template<typename... Ts>
	using max
		= fold<pop_front<list<Ts...>>, front<list<Ts...>>, quote<detail::max_>>;

	namespace lazy
	{
		template<typename... Ts>
		using min = defer<min, Ts...>;

		template<typename... Ts>
		using max = defer<max, Ts...>;
	} // namespace lazy

	template<typename List>
	using empty = bool_<0 == size<List>::type::value>;

	namespace lazy
	{
		template<typename List>
		using empty = defer<empty, List>;
	} // namespace lazy

	template<typename F, typename S>
	using pair = list<F, S>;

	template<typename Pair>
	using first = front<Pair>;

	template<typename Pair>
	using second = front<pop_front<Pair>>;

	namespace lazy
	{
		template<typename Pair>
		using first = defer<first, Pair>;

		template<typename Pair>
		using second = defer<second, Pair>;
	} // namespace lazy

	namespace detail
	{
		constexpr Faiz::size_t
		find_index_i_(
			bool const* const first, bool const* const last, Faiz::size_t N = 0)
		{
			return first == last ?
				npos::value :
				*first ? N : find_index_i_(first + 1, last, N + 1);
		}

		template<typename List, typename T>
		struct find_index_
		{};

		template<typename V>
		struct find_index_<list<>, V> : type_identity<npos>
		{};

		template<typename... T, typename V>
		struct find_index_<list<T...>, V>
		{
			static constexpr bool s_v[] = {is_same<T, V>::value...};
			using type = size_t_<find_index_i_(s_v, s_v + sizeof...(T))>;
		};
	} // namespace detail

	template<typename List, typename T>
	using find_index = _t<detail::find_index_<List, T>>;

	namespace lazy
	{
		template<typename List, typename T>
		using find_index = defer<find_index, List, T>;
	} // namespace lazy

	namespace detail
	{
		constexpr Faiz::size_t
		reverse_find_index_i_(
			bool const* const first, bool const* const last, Faiz::size_t N)
		{
			return first == last ?
				npos::value :
				*(last - 1) ? N - 1 :
							  reverse_find_index_i_(first, last - 1, N - 1);
		}

		template<typename List, typename T>
		struct reverse_find_index_
		{};

		template<typename V>
		struct reverse_find_index_<list<>, V> : type_identity<npos>
		{};

		template<typename... T, typename V>
		struct reverse_find_index_<list<T...>, V>
		{
			static constexpr bool s_v[] = {is_same<T, V>::value...};
			using type = size_t_<reverse_find_index_i_(
				s_v, s_v + sizeof...(T), sizeof...(T))>;
		};
	} // namespace detail

	template<typename List, typename T>
	using reverse_find_index = _t<detail::reverse_find_index_<List, T>>;

	namespace lazy
	{
		template<typename List, typename T>
		using reverse_find_index = defer<reverse_find_index, List, T>;
	} // namespace lazy

	template<typename List, typename T>
	using find = drop<List, min<find_index<List, T>, size<List>>>;

	namespace lazy
	{
		template<typename List, typename T>
		using find = defer<find, List, T>;
	} // namespace lazy

	template<typename List, typename T>
	using reverse_find
		= drop<List, min<reverse_find_index<List, T>, size<List>>>;

	namespace lazy
	{
		template<typename List, typename T>
		using reverse_find = defer<reverse_find, List, T>;
	} // namespace lazy

	namespace detail
	{
		constexpr bool const*
		find_if_i_(bool const* const begin, bool const* const end)
		{
			return begin == end || *begin ? begin : find_if_i_(begin + 1, end);
		}

		template<typename List, typename Fun, typename = void>
		struct find_if_
		{};

		template<typename Fun>
		struct find_if_<list<>, Fun> : type_identity<list<>>
		{};

		template<typename... List, typename Fun>
		struct find_if_<list<List...>,
			Fun,
			void_t<integer_sequence<bool,
				bool(invoke<Fun, List>::type::value)...>>>
		{
			static constexpr bool s_v[] = {invoke<Fun, List>::type::value...};
			using type = drop_c<list<List...>,
				detail::find_if_i_(s_v, s_v + sizeof...(List)) - s_v>;
		};
	} // namespace detail

	template<typename List, typename Fun>
	using find_if = _t<detail::find_if_<List, Fun>>;

	namespace lazy
	{
		template<typename List, typename Fun>
		using find_if = defer<find_if, List, Fun>;
	} // namespace lazy

	namespace detail
	{
		constexpr bool const*
		reverse_find_if_i_(bool const* const begin,
			bool const* const pos,
			bool const* const end)
		{
			return begin == pos ?
				end :
				*(pos - 1) ? pos - 1 : reverse_find_if_i_(begin, pos - 1, end);
		}

		template<typename List, typename Fun, typename = void>
		struct reverse_find_if_
		{};

		template<typename Fun>
		struct reverse_find_if_<list<>, Fun> : type_identity<list<>>
		{};

		template<typename... List, typename Fun>
		struct reverse_find_if_<list<List...>,
			Fun,
			void_t<integer_sequence<bool,
				bool(invoke<Fun, List>::type::value)...>>>
		{
			static constexpr bool s_v[] = {invoke<Fun, List>::type::value...};
			using type = drop_c<list<List...>,
				detail::reverse_find_if_i_(
					s_v, s_v + sizeof...(List), s_v + sizeof...(List))
					- s_v>;
		};
	} // namespace detail

	template<typename List, typename Fun>
	using reverse_find_if = _t<detail::reverse_find_if_<List, Fun>>;

	namespace lazy
	{
		template<typename List, typename Fun>
		using reverse_find_if = defer<reverse_find_if, List, Fun>;
	} // namespace lazy

	namespace detail
	{
		template<typename List, typename T, typename U>
		struct replace_
		{};

		template<typename... List, typename T, typename U>
		struct replace_<list<List...>, T, U>
			: type_identity<list<if_<is_same<T, List>, U, List>...>>
		{};
	} // namespace detail

	template<typename List, typename T, typename U>
	using replace = _t<detail::replace_<List, T, U>>;

	namespace lazy
	{
		template<typename List, typename T, typename U>
		using replace = defer<replace, T, U>;
	} // namespace lazy

	namespace detail
	{
		template<typename List, typename C, typename U, typename = void>
		struct replace_if_
		{};

		template<typename... List, typename C, typename U>
		struct replace_if_<list<List...>,
			C,
			U,
			void_t<
				integer_sequence<bool, bool(invoke<C, List>::type::value)...>>>
		{
			using type = list<if_<invoke<C, List>, U, List>...>;
		};
	} // namespace detail

	template<typename List, typename C, typename U>
	using replace_if = _t<detail::replace_if_<List, C, U>>;

	namespace lazy
	{
		template<typename List, typename C, typename U>
		using replace_if = defer<replace_if, C, U>;
	} // namespace lazy

	namespace detail
	{
		constexpr Faiz::size_t
		count_i_(bool const* const begin, bool const* const end, Faiz::size_t n)
		{
			return begin == end ? n :
								  detail::count_i_(begin + 1, end, n + *begin);
		}

		template<typename List, typename T, typename = void>
		struct count_
		{};

		template<typename T>
		struct count_<list<>, T> : type_identity<size_t_<0>>
		{};

		template<typename... List, typename T>
		struct count_<list<List...>, T>
		{
			static constexpr bool s_v[] = {is_same<T, List>::value...};
			using type
				= size_t_<detail::count_i_(s_v, s_v + sizeof...(List), 0u)>;
		};
	} // namespace detail

	template<typename List, typename T>
	using count = _t<detail::count_<List, T>>;

	namespace lazy
	{
		template<typename List, typename T>
		using count = defer<count, List, T>;
	} // namespace lazy

	namespace detail
	{
		template<typename List, typename Fn, typename = void>
		struct count_if_
		{};

		template<typename Fn>
		struct count_if_<list<>, Fn> : type_identity<size_t_<0>>
		{};

		template<typename... List, typename Fn>
		struct count_if_<list<List...>,
			Fn,
			void_t<
				integer_sequence<bool, bool(invoke<Fn, List>::type::value)...>>>
		{
			static constexpr bool s_v[] = {invoke<Fn, List>::type::value...};
			using type
				= size_t_<detail::count_i_(s_v, s_v + sizeof...(List), 0u)>;
		};
	} // namespace detail

	template<typename List, typename Fn>
	using count_if = _t<detail::count_if_<List, Fn>>;

	namespace lazy
	{
		template<typename List, typename Fn>
		using count_if = defer<count_if, List, Fn>;
	} // namespace lazy

	namespace detail
	{
		template<typename Pred>
		struct filter_
		{
			template<typename A>
			using invoke = if_c<invoke<Pred, A>::type::value, list<A>, list<>>;
		};
	} // namespace detail

	template<typename List, typename Pred>
	using filter = join<transform<List, detail::filter_<Pred>>>;

	namespace lazy
	{
		template<typename List, typename Pred>
		using filter = defer<filter, List, Pred>;
	} // namespace lazy

	namespace detail
	{
		template<typename T>
		struct static_const
		{
			static constexpr T value{};
		};

		// Avoid potential ODR violations with global objects:
		template<typename T>
		constexpr T static_const<T>::value;
	} // namespace detail

	namespace detail
	{
		struct for_each_fn
		{
			template<class UnaryFunction, class... Args>
			constexpr auto
			operator()(list<Args...>, UnaryFunction f) const -> UnaryFunction
			{
				return (void)initializer_list<int>{((void)f(Args{}), 0)...}, f;
			}
		};
	} // namespace detail

	namespace
	{
		constexpr auto&& for_each
			= detail::static_const<detail::for_each_fn>::value;
	} // namespace

	template<typename ListOfLists>
	using transpose = fold<ListOfLists,
		repeat_n<size<front<ListOfLists>>, list<>>,
		bind_back<quote<transform>, quote<push_back>>>;

	namespace lazy
	{
		template<typename ListOfLists>
		using transpose = defer<transpose, ListOfLists>;
	} // namespace lazy

	template<typename Fun, typename ListOfLists>
	using zip_with = transform<transpose<ListOfLists>, uncurry<Fun>>;

	namespace lazy
	{
		template<typename Fun, typename ListOfLists>
		using zip_with = defer<zip_with, Fun, ListOfLists>;
	} // namespace lazy

	template<typename ListOfLists>
	using zip = transpose<ListOfLists>;

	namespace lazy
	{
		template<typename ListOfLists>
		using zip = defer<zip, ListOfLists>;
	} // namespace lazy

	namespace detail
	{
		// Indirection here needed to avoid Core issue 1430
		// http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
		template<typename Sequence>
		struct as_list_ : lazy::invoke<uncurry<quote<list>>, Sequence>
		{};
	} // namespace detail

	template<typename Sequence>
	using as_list = _t<detail::as_list_<remove_cvref_t<Sequence>>>;

	namespace lazy
	{
		template<typename Sequence>
		using as_list = defer<as_list, Sequence>;
	} // namespace lazy

	namespace detail
	{
		template<typename List, typename State = list<>>
		struct reverse_ : lazy::fold<List, State, quote<push_front>>
		{};

		template<typename T0,
			typename T1,
			typename T2,
			typename T3,
			typename T4,
			typename T5,
			typename T6,
			typename T7,
			typename T8,
			typename T9,
			typename... Ts,
			typename... Us>
		struct reverse_<list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, Ts...>,
			list<Us...>>
			: reverse_<list<Ts...>,
				  list<T9, T8, T7, T6, T5, T4, T3, T2, T1, T0, Us...>>
		{};
	} // namespace detail

	template<typename List>
	using reverse = _t<detail::reverse_<List>>;

	namespace lazy
	{
		template<typename List>
		using reverse = defer<reverse, List>;
	} // namespace lazy

	template<typename F>
	using not_fn = compose<quote<not_>, F>;

	namespace lazy
	{
		template<typename F>
		using not_fn = defer<not_fn, F>;
	} // namespace lazy

	template<typename List, typename F>
	using all_of = empty<find_if<List, not_fn<F>>>;

	namespace lazy
	{
		/// \sa 'meta::all_of'
		/// \ingroup lazy_query
		template<typename List, typename Fn>
		using all_of = defer<all_of, List, Fn>;
	} // namespace lazy

	template<typename List, typename F>
	using any_of = not_<empty<find_if<List, F>>>;

	namespace lazy
	{
		template<typename List, typename Fn>
		using any_of = defer<any_of, List, Fn>;
	} // namespace lazy

	template<typename List, typename F>
	using none_of = empty<find_if<List, F>>;

	namespace lazy
	{
		template<typename List, typename Fn>
		using none_of = defer<none_of, List, Fn>;
	} // namespace lazy

	template<typename List, typename T>
	using in = not_<empty<find<List, T>>>;

	namespace lazy
	{
		template<typename List, typename T>
		using in = defer<in, List, T>;
	} // namespace lazy

	namespace detail
	{
		template<typename List>
		struct inherit_
		{};

		template<typename... List>
		struct inherit_<list<List...>> : List...
		{
			using type = inherit_;
		};
	} // namespace detail

	template<typename List>
	using inherit = _t<detail::inherit_<List>>;

	namespace lazy
	{
		template<typename List>
		using inherit = defer<inherit, List>;
	} // namespace lazy

	namespace detail
	{
		template<typename Set, typename T>
		struct in_
		{};

		template<typename... Set, typename T>
		struct in_<list<Set...>, T>
			: Faiz::is_base_of<id<T>, inherit<list<id<Set>...>>>
		{};

		template<typename Set, typename T>
		struct insert_back_
		{};

		template<typename... Set, typename T>
		struct insert_back_<list<Set...>, T>
		{
			using type
				= if_<in_<list<Set...>, T>, list<Set...>, list<Set..., T>>;
		};
	} // namespace detail

	template<typename List>
	using unique = fold<List, list<>, quote_trait<detail::insert_back_>>;

	namespace lazy
	{
		template<typename List>
		using unique = defer<unique, List>;
	} // namespace lazy

	namespace detail
	{
		template<typename Pred>
		struct partition_
		{
			template<typename, typename, typename = void>
			struct impl
			{};
			template<typename... Yes, typename... No, typename A>
			struct impl<pair<list<Yes...>, list<No...>>,
				A,
				void_t<bool_<invoke<Pred, A>::type::value>>>
			{
				using type = if_<invoke<Pred, A>,
					pair<list<Yes..., A>, list<No...>>,
					pair<list<Yes...>, list<No..., A>>>;
			};
			template<typename State, typename A>
			using invoke = _t<impl<State, A>>;
		};
	} // namespace detail

	template<typename List, typename Pred>
	using partition
		= fold<List, pair<list<>, list<>>, detail::partition_<Pred>>;

	namespace lazy
	{
		template<typename List, typename Pred>
		using partition = defer<partition, List, Pred>;
	} // namespace lazy

	namespace detail
	{
		template<typename, typename, typename = void>
		struct sort_
		{};

		template<typename Pred>
		struct sort_<list<>, Pred> : type_identity<list<>>
		{};

		template<typename A, typename Pred>
		struct sort_<list<A>, Pred> : type_identity<list<A>>
		{};

		template<typename A, typename B, typename... List, typename Pred>
		struct sort_<list<A, B, List...>,
			Pred,
			void_t<
				_t<sort_<first<partition<list<B, List...>, bind_back<Pred, A>>>,
					Pred>>>>
		{
			using P = partition<list<B, List...>, bind_back<Pred, A>>;
			using type = concat<_t<sort_<first<P>, Pred>>,
				list<A>,
				_t<sort_<second<P>, Pred>>>;
		};
	} // namespace detail

	template<typename List, typename Pred>
	using sort = _t<detail::sort_<List, Pred>>;

	namespace lazy
	{
		template<typename List, typename Pred>
		using sort = defer<sort, List, Pred>;
	} // namespace lazy

	namespace detail
	{
		template<typename T, int = 0>
		struct protect_;

		template<typename, int = 0>
		struct vararg_;

		template<typename T, int = 0>
		struct is_valid_;

		// Returns which branch to evaluate
		template<typename If, typename... Ts>
		using lazy_if_ = lazy::_t<defer<_if_, list<If, protect_<Ts>...>>>;

		template<typename A, typename T, typename F, typename Ts>
		struct subst1_ : type_identity<list<list<T>>>
		{};
		template<typename T, typename F, typename Ts>
		struct subst1_<F, T, F, Ts> : type_identity<list<>>
		{};
		template<typename A, typename T, typename F, typename Ts>
		struct subst1_<vararg_<A>, T, F, Ts> : type_identity<list<Ts>>
		{};

		template<typename As, typename Ts>
		using substitutions_ = push_back<
			join<transform<
				concat<As, repeat_n_c<size<Ts>{} + 2 - size<As>{}, back<As>>>,
				concat<Ts, repeat_n_c<2, back<As>>>,
				bind_back<quote_trait<subst1_>,
					back<As>,
					drop_c<Ts, size<As>{} - 2>>>>,
			list<back<As>>>;

		template<typename As, typename Ts>
		using substitutions
			= invoke<if_c<(size<Ts>{} + 2 >= size<As>{}), quote<substitutions_>>,
				As,
				Ts>;

		template<typename T>
		struct is_vararg_ : false_
		{};
		template<typename T>
		struct is_vararg_<vararg_<T>> : true_
		{};

		template<typename Tags>
		using is_variadic_
			= is_vararg_<at<push_front<Tags, void>, dec<size<Tags>>>>;

		template<typename Tags, bool IsVariadic = is_variadic_<Tags>::value>
		struct lambda_;

		// Non-variadic lambda implementation
		template<typename... As>
		struct lambda_<list<As...>, false>
		{
		private:
			static constexpr Faiz::size_t arity = sizeof...(As) - 1;
			using Tags
				= list<As...>; // Includes the lambda body as the last arg!
			using F = back<Tags>;
			template<typename T, typename Args>
			struct impl;
			template<typename T, typename Args>
			using lazy_impl_ = lazy::_t<defer<impl, T, protect_<Args>>>;
			template<typename, typename, typename = void>
			struct subst_
			{};
			template<template<typename...> class C,
				typename... Ts,
				typename Args>
			struct subst_<defer<C, Ts...>,
				Args,
				void_t<C<_t<impl<Ts, Args>>...>>>
				: type_identity<C<_t<impl<Ts, Args>>...>>
			{};
			template<typename T, template<T...> class C, T... Is, typename Args>
			struct subst_<defer_i<T, C, Is...>, Args, void_t<C<Is...>>>
				: type_identity<C<Is...>>
			{};
			template<typename T, typename Args>
			struct impl : if_c<(reverse_find_index<Tags, T>() != npos()),
							  lazy::at<Args, reverse_find_index<Tags, T>>,
							  id<T>>
			{};
			template<typename T, typename Args>
			struct impl<protect_<T>, Args> : type_identity<T>
			{};
			template<typename T, typename Args>
			struct impl<is_valid_<T>, Args>
				: type_identity<is_trait<impl<T, Args>>>
			{};
			template<typename If, typename... Ts, typename Args>
			struct impl<defer<if_, If, Ts...>, Args> // Short-circuit if_
				: impl<lazy_impl_<lazy_if_<If, Ts...>, Args>, Args>
			{};
			template<typename Bool_, typename... Ts, typename Args>
			struct impl<defer<and_, Bool_, Ts...>, Args> // Short-circuit
														 // and_
				: impl<lazy_impl_<
						   lazy_if_<Bool_, lazy::and_<Ts...>, protect_<false_>>,
						   Args>,
					  Args>
			{};
			template<typename Bool_, typename... Ts, typename Args>
			struct impl<defer<or_, Bool_, Ts...>, Args> // Short-circuit or_
				: impl<lazy_impl_<
						   lazy_if_<Bool_, protect_<true_>, lazy::or_<Ts...>>,
						   Args>,
					  Args>
			{};
			template<template<typename...> class C,
				typename... Ts,
				typename Args>
			struct impl<defer<C, Ts...>, Args> : subst_<defer<C, Ts...>, Args>
			{};
			template<typename T, template<T...> class C, T... Is, typename Args>
			struct impl<defer_i<T, C, Is...>, Args>
				: subst_<defer_i<T, C, Is...>, Args>
			{};
			template<template<typename...> class C,
				typename... Ts,
				typename Args>
			struct impl<C<Ts...>, Args> : subst_<defer<C, Ts...>, Args>
			{};
			template<typename... Ts, typename Args>
			struct impl<lambda_<list<Ts...>, false>, Args>
			{
				using type
					= compose<uncurry<lambda_<list<As..., Ts...>, false>>,
						curry<bind_front<quote<concat>, Args>>>;
			};
			template<typename... Bs, typename Args>
			struct impl<lambda_<list<Bs...>, true>, Args>
			{
				using type
					= compose<typename lambda_<list<As..., Bs...>, true>::thunk,
						bind_front<quote<concat>, transform<Args, quote<list>>>,
						curry<bind_front<quote<substitutions>, list<Bs...>>>>;
			};

		public:
			template<typename... Ts>
			using invoke
				= _t<if_c<sizeof...(Ts) == arity, impl<F, list<Ts..., F>>>>;
		};

		template<typename... As>
		struct lambda_<list<As...>, true>
		{
		private:
			template<typename T, bool IsVar>
			friend struct lambda_;
			using Tags
				= list<As...>; // Includes the lambda body as the last arg!
			template<typename T, typename Args>
			struct impl;
			template<typename Args>
			using eval_impl_ = bind_back<quote_trait<impl>, Args>;
			template<typename T, typename Args>
			using lazy_impl_ = lazy::_t<defer<impl, T, protect_<Args>>>;
			template<template<typename...> class C, typename Args, typename Ts>
			using try_subst_
				= apply<quote<C>, join<transform<Ts, eval_impl_<Args>>>>;
			template<typename, typename, typename = void>
			struct subst_
			{};
			template<template<typename...> class C,
				typename... Ts,
				typename Args>
			struct subst_<defer<C, Ts...>,
				Args,
				void_t<try_subst_<C, Args, list<Ts...>>>>
				: type_identity<list<try_subst_<C, Args, list<Ts...>>>>
			{};
			template<typename T, template<T...> class C, T... Is, typename Args>
			struct subst_<defer_i<T, C, Is...>, Args, void_t<C<Is...>>>
				: type_identity<list<C<Is...>>>
			{};
			template<typename T, typename Args>
			struct impl : if_c<(reverse_find_index<Tags, T>() != npos()),
							  lazy::at<Args, reverse_find_index<Tags, T>>,
							  id<list<T>>>
			{};
			template<typename T, typename Args>
			struct impl<protect_<T>, Args> : type_identity<list<T>>
			{};
			template<typename T, typename Args>
			struct impl<is_valid_<T>, Args>
				: type_identity<list<is_trait<impl<T, Args>>>>
			{};
			template<typename If, typename... Ts, typename Args>
			struct impl<defer<if_, If, Ts...>, Args> // Short-circuit if_
				: impl<lazy_impl_<lazy_if_<If, Ts...>, Args>, Args>
			{};
			template<typename Bool_, typename... Ts, typename Args>
			struct impl<defer<and_, Bool_, Ts...>, Args> // Short-circuit
														 // and_
				: impl<lazy_impl_<
						   lazy_if_<Bool_, lazy::and_<Ts...>, protect_<false_>>,
						   Args>,
					  Args>
			{};
			template<typename Bool_, typename... Ts, typename Args>
			struct impl<defer<or_, Bool_, Ts...>, Args> // Short-circuit or_
				: impl<lazy_impl_<
						   lazy_if_<Bool_, protect_<true_>, lazy::or_<Ts...>>,
						   Args>,
					  Args>
			{};
			template<template<typename...> class C,
				typename... Ts,
				typename Args>
			struct impl<defer<C, Ts...>, Args> : subst_<defer<C, Ts...>, Args>
			{};
			template<typename T, template<T...> class C, T... Is, typename Args>
			struct impl<defer_i<T, C, Is...>, Args>
				: subst_<defer_i<T, C, Is...>, Args>
			{};
			template<template<typename...> class C,
				typename... Ts,
				typename Args>
			struct impl<C<Ts...>, Args> : subst_<defer<C, Ts...>, Args>
			{};
			template<typename... Bs, bool IsVar, typename Args>
			struct impl<lambda_<list<Bs...>, IsVar>, Args>
			{
				using type = list<
					compose<typename lambda_<list<As..., Bs...>, true>::thunk,
						bind_front<quote<concat>, Args>,
						curry<bind_front<quote<substitutions>, list<Bs...>>>>>;
			};
			struct thunk
			{
				template<typename S, typename R = _t<impl<back<Tags>, S>>>
				using invoke = if_c<size<R>{} == 1, front<R>>;
			};

		public:
			template<typename... Ts>
			using invoke = invoke<thunk, substitutions<Tags, list<Ts...>>>;
		};
	} // namespace detail

	template<typename... Ts>
	using lambda = if_c<(sizeof...(Ts) > 0), detail::lambda_<list<Ts...>>>;

	template<typename T>
	using is_valid = detail::is_valid_<T>;

	template<typename T>
	using vararg = detail::vararg_<T>;

	template<typename T>
	using protect = detail::protect_<T>;

	template<typename Tag, typename Value>
	struct var;

	namespace detail
	{
		template<typename... As>
		struct let_
		{};
		template<typename Fn>
		struct let_<Fn> : type_identity<lazy::invoke<lambda<Fn>>>
		{};
		template<typename Tag, typename Value, typename... Rest>
		struct let_<var<Tag, Value>, Rest...>
			: type_identity<lazy::invoke<lambda<Tag, _t<let_<Rest...>>>, Value>>
		{};
	} // namespace detail

	template<typename... As>
	using let = _t<_t<detail::let_<As...>>>;

	namespace lazy
	{
		template<typename... As>
		using let = defer<let, As...>;
	} // namespace lazy

	inline namespace placeholders
	{
		struct _a;
		struct _b;
		struct _c;
		struct _d;
		struct _e;
		struct _f;
		struct _g;
		struct _h;
		struct _i;

		using _args = vararg<void>;
		using _args_a = vararg<_a>;
		using _args_b = vararg<_b>;
		using _args_c = vararg<_c>;
	} // namespace placeholders

	namespace detail
	{
		template<typename M2, typename M>
		struct cartesian_product_fn
		{
			template<typename X>
			struct lambda0
			{
				template<typename Xs>
				using lambda1 = list<push_front<Xs, X>>;
				using type = join<transform<M2, quote<lambda1>>>;
			};
			using type = join<transform<M, quote_trait<lambda0>>>;
		};
	} // namespace detail

	template<typename ListOfLists>
	using cartesian_product = reverse_fold<ListOfLists,
		list<list<>>,
		quote_trait<detail::cartesian_product_fn>>;

	namespace lazy
	{
		template<typename ListOfLists>
		using cartesian_product = defer<cartesian_product, ListOfLists>;
	} // namespace lazy

	namespace detail
	{
		template<bool>
		struct add_const_if
		{
			template<typename T>
			using invoke = T const;
		};
		template<>
		struct add_const_if<false>
		{
			template<typename T>
			using invoke = T;
		};
	} // namespace detail
	template<bool If>
	using add_const_if_c = detail::add_const_if<If>;
	template<typename If>
	using add_const_if = add_const_if_c<If::type::value>;

	template<bool If, typename T>
	using const_if_c = invoke<add_const_if_c<If>, T>;
	template<typename If, typename T>
	using const_if = invoke<add_const_if<If>, T>;

	namespace detail
	{
		template<typename State, typename Ch>
		using atoi_ = if_c<(Ch::value >= '0' && Ch::value <= '9'),
			integral_constant<typename State::value_type,
				State::value * 10 + (Ch::value - '0')>>;
	}

	inline namespace literals
	{
		template<char... Chs>
		constexpr fold<list<char_<Chs>...>, size_t_<0>, quote<detail::atoi_>>
		operator"" _z()
		{
			return {};
		}
	} // namespace literals

	template<typename... Ts>
	constexpr int
	ignore_unused(Ts&&...)
	{
		return 42;
	}

} // namespace Rider::Faiz::meta


namespace Rider::Faiz::range
{
	inline namespace CPOs
	{}

	/// \cond
	namespace _end_
	{
		struct fn;
	}
	using end_fn = _end_::fn;

	namespace _size_
	{
		struct fn;
	}
	/// \endcond

	template<typename...>
	struct variant;

	template<typename I = void>
	struct dangling;

	struct make_pipeable_fn;

	template<typename Derived>
	struct pipeable;

	template<typename First, typename Second>
	struct composed;

	template<typename... Fns>
	struct overloaded;

	namespace action
	{
		template<typename Action>
		struct action;
	}

	namespace view
	{
		template<typename View>
		struct view;
	}

	namespace adl_advance_detail
	{
		struct advance_fn;
	}
	using adl_advance_detail::advance_fn;

	struct advance_to_fn;

	struct advance_bounded_fn;

	struct next_fn;

	struct prev_fn;

	struct distance_fn;

	struct iter_size_fn;

	template<typename T>
	struct difference_type;

	template<typename T>
	struct value_type;

	template<typename T>
	struct iterator_category;

	template<typename T>
	struct size_type;

	struct view_base
	{};

	/// \cond
	namespace detail
	{
		template<typename T = void>
		struct any_
		{
			any_() = default;
			any_(T&&)
			{}
		};

		template<>
		struct any_<void>
		{
			any_() = default;
			template<typename T>
			any_(T&&)
			{}
		};

		using any = any_<>;

		struct value_init
		{
			template<typename T>
			operator T() const
			{
				return T{};
			}
		};

		struct make_compressed_pair_fn;

		template<typename T, typename R = _t<remove_reference<T>>>
		using as_ref_t = _t<add_lvalue_reference<_t<remove_const<R>>>>;

		template<typename T, typename R = _t<remove_reference<T>>>
		using as_cref_t = _t<add_lvalue_reference<_t<add_const<R>>>>;

		struct get_first;
		struct get_second;

		template<typename Val1, typename Val2>
		struct replacer_fn;

		template<typename Pred, typename Val>
		struct replacer_if_fn;

		template<typename I>
		struct move_into_cursor;

		template<typename Int>
		struct from_end_;

		template<typename... Ts>
		constexpr int
		ignore_unused(Ts&&...)
		{
			return 42;
		}

		template<int I>
		struct priority_tag : priority_tag<I - 1>
		{};

		template<>
		struct priority_tag<0>
		{};

	} // namespace detail
	/// \endcond

	namespace concepts
	{
		template<typename Concept, typename... Ts>
		struct models;
	}

	struct begin_tag
	{};
	struct end_tag
	{};
	struct copy_tag
	{};
	struct move_tag
	{};

	struct not_equal_to;
	struct equal_to;
	struct less;
	struct ordered_less;
	struct ident;

	enum cardinality : std::ptrdiff_t
	{
		infinite = -3,
		unknown = -2,
		finite = -1
	};

	template<typename Rng, typename Void = void>
	struct range_cardinality;

	template<typename Rng>
	using is_finite = bool_<range_cardinality<Rng>::value >= finite>;

	template<typename Rng>
	using is_infinite = bool_<range_cardinality<Rng>::value == infinite>;

	template<typename T>
	struct enable_view;

	template<typename R>
	struct disable_sized_range;

	template<typename S, typename I>
	struct disable_sized_sentinel;

	template<typename Cur>
	struct basic_mixin;

	template<typename Cur>
	struct basic_iterator;

	template<cardinality>
	struct basic_view : view_base
	{};

	template<typename Derived, cardinality C = finite>
	struct view_facade;

	template<typename Derived,
		typename BaseRng,
		cardinality C = range_cardinality<BaseRng>::value>
	struct view_adaptor;

	template<typename I, typename S>
	struct common_iterator;


	template<typename First, typename Second>
	struct compressed_pair;

	template<typename T>
	struct bind_element;

	template<typename T>
	using bind_element_t = _t<bind_element<T>>;

	template<typename Derived, cardinality = finite>
	struct view_interface;

	template<typename T>
	struct istream_range;

	template<typename T>
	istream_range<T>
	istream(std::istream& sin);

	template<typename I, typename S = I>
	struct iterator_range;

	template<typename I, typename S = I>
	struct sized_iterator_range;


	// Views
	//
	template<typename Rng, typename Pred>
	struct adjacent_filter_view;

	namespace view
	{
		struct adjacent_filter_fn;
	}

	template<typename Rng, typename Pred>
	struct adjacent_remove_if_view;

	namespace view
	{
		struct adjacent_remove_if_fn;
	}

	namespace view
	{
		struct all_fn;
	}

	template<typename Rng>
	struct const_view;

	namespace view
	{
		struct const_fn;
	}

	template<typename I>
	struct counted_view;

	namespace view
	{
		struct counted_fn;
	}

	struct default_sentinel
	{};

	template<typename I>
	struct move_iterator;

	template<typename I>
	using move_into_iterator = basic_iterator<detail::move_into_cursor<I>>;

	template<typename Rng, bool = (bool)is_infinite<Rng>()>
	struct cycled_view;

	namespace view
	{
		struct cycle_fn;
	}

	/// \cond
	namespace detail
	{
		template<typename I>
		struct reverse_cursor;
	}
	/// \endcond

	template<typename I>
	using reverse_iterator = basic_iterator<detail::reverse_cursor<I>>;

	template<typename T>
	struct empty_view;

	namespace view
	{
		struct empty_fn;
	}

	template<typename Rng, typename Pred>
	struct filter_view;

	namespace view
	{
		struct filter_fn;
	}

	template<typename Rng, typename Fun>
	struct group_by_view;

	namespace view
	{
		struct group_by_fn;
	}

	template<typename Rng>
	struct indirect_view;

	namespace view
	{
		struct indirect_fn;
	}

	template<typename From, typename To = void>
	struct iota_view;

	template<typename From, typename To = void>
	struct closed_iota_view;

	namespace view
	{
		struct iota_fn;
		struct closed_iota_fn;
	} // namespace view

	template<typename Rng, typename ValRng = void>
	struct join_view;

	namespace view
	{
		struct join_fn;
	}

	template<typename... Rngs>
	struct concat_view;

	namespace view
	{
		struct concat_fn;
	}

	template<typename Rng, typename Fun>
	struct partial_sum_view;

	namespace view
	{
		struct partial_sum_fn;
	}

	template<typename Rng>
	struct move_view;

	namespace view
	{
		struct move_fn;
	}

	template<typename Val>
	struct repeat_view;

	namespace view
	{
		struct repeat_fn;
	}

	template<typename Rng>
	struct reverse_view;

	namespace view
	{
		struct reverse_fn;
	}

	template<typename Rng>
	struct slice_view;

	namespace view
	{
		struct slice_fn;
	}

	template<typename Rng, typename Fun>
	struct split_view;

	namespace view
	{
		struct split_fn;
	}

	template<typename Rng>
	struct single_view;

	namespace view
	{
		struct single_fn;
	}

	template<typename Rng>
	struct stride_view;

	namespace view
	{
		struct stride_fn;
	}

	template<typename Rng>
	struct take_view;

	namespace view
	{
		struct take_fn;
	}

	/// \cond
	namespace detail
	{
		template<typename Rng>
		struct is_random_access_bounded_;

		template<typename Rng,
			bool IsRandomAccessBounded = is_random_access_bounded_<Rng>::value>
		struct take_exactly_view_;
	} // namespace detail
	/// \endcond

	template<typename Rng>
	using take_exactly_view = detail::take_exactly_view_<Rng>;

	namespace view
	{
		struct take_exactly_fn;
	}

	template<typename Rng, typename Pred>
	struct iter_take_while_view;

	template<typename Rng, typename Pred>
	struct take_while_view;

	namespace view
	{
		struct iter_take_while_fn;
		struct take_while_fn;
	} // namespace view

	template<typename Rng, typename Regex, typename SubMatchRange>
	struct tokenize_view;

	namespace view
	{
		struct tokenize_fn;
	}

	template<typename Rng, typename Fun>
	struct iter_transform_view;

	template<typename Rng, typename Fun>
	struct transform_view;

	namespace view
	{
		struct transform_fn;
	}

	template<typename Rng, typename Val1, typename Val2>
	using replace_view
		= iter_transform_view<Rng, detail::replacer_fn<Val1, Val2>>;

	template<typename Rng, typename Pred, typename Val>
	using replace_if_view
		= iter_transform_view<Rng, detail::replacer_if_fn<Pred, Val>>;

	namespace view
	{
		struct replace_fn;

		struct replace_if_fn;
	} // namespace view

	template<typename I>
	struct unbounded_view;

	namespace view
	{
		struct unbounded_fn;
	}

	template<typename Rng>
	using unique_view = adjacent_filter_view<Rng, not_equal_to>;

	namespace view
	{
		struct unique_fn;
	}

	template<typename Rng>
	using keys_range_view = transform_view<Rng, detail::get_first>;

	template<typename Rng>
	using values_view = transform_view<Rng, detail::get_second>;

	namespace view
	{
		struct keys_fn;

		struct values_fn;
	} // namespace view

	template<typename Fun, typename... Rngs>
	struct iter_zip_with_view;

	template<typename Fun, typename... Rngs>
	struct zip_with_view;

	template<typename... Rngs>
	struct zip_view;

	namespace view
	{
		struct iter_zip_with_fn;

		struct zip_with_fn;

		struct zip_fn;
	} // namespace view
} // namespace Rider::Faiz::range


namespace Rider::Faiz::logic
{
	template<typename...>
	struct and_;

	template<>
	struct and_<> : true_
	{};

	template<typename _b1>
	struct and_<_b1> : _b1
	{};

	template<typename _b1, class _b2, class... _bn>
	struct and_<_b1, _b2, _bn...> : meta::if_<_b1, and_<_b2, _bn...>, _b1>
	{};

	template<typename...>
	struct or_;

	template<>
	struct or_<> : false_
	{};

	template<typename _b1>
	struct or_<_b1> : _b1
	{};

	template<typename _b1, class _b2, class... _bn>
	struct or_<_b1, _b2, _bn...> : meta::if_<_b1, _b1, or_<_b2, _bn...>>
	{};


	template<typename _b>
	struct not_ : bool_<!_b::value>
	{};


} // namespace Rider::Faiz::logic

namespace Rider::Faiz
{
	template<class...>
	struct empty_struct_template
	{};
	using empty_struct = empty_struct_template<>;

	template<bool _bCond>
	struct when;

	template<typename _type>
	struct always
	{
	private:
		template<typename...>
		struct impl : type_identity<_type>
		{};

	public:
		template<typename... _types>
		using apply = impl<_types...>;
	};


	template<typename _type, typename...>
	using well_formed_t = _type;

	template<typename... _types>
	using when_valid = well_formed_t<when<true>, _types...>;

	template<bool _bCond>
	using enable_when = enable_if_t<_bCond, when<true>>;

	struct any_constructible
	{
		any_constructible(...);
	};

	template<class T, class... Rest>
	inline constexpr bool are_same_v = (is_same_v<T, Rest> && ...);

	template<typename T, typename... Rest>
	struct are_same : bool_<are_same_v<T, Rest...>>
	{};

	template<class T, class... Rest>
	inline constexpr bool is_any_v = (is_same_v<T, Rest> || ...);

	template<typename T, typename... Rest>
	struct is_any : bool_<is_any_v<T, Rest...>>
	{};

} // namespace Rider::Faiz

#endif
