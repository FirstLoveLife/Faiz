#ifndef __FAIZ_FWD_META_H_
#define __FAIZ_FWD_META_H_
#include "rider/faiz/macros.hpp"
#include <boost/preprocessor.hpp>
#include <initializer_list>
#include <iostream>
#include <type_traits>


namespace Rider
{

	tpl<typ T> using _t = typ T::type;

	tpl<typ T> using _p = typ T::pointer;

	tpl<typ T> using _e = typ T::element_type;

	tpl<typ T> using _p = typ T::pointer;

	tpl<typ T, bool b> struct dependent_type : public T
	{};

	tpl<typ T, bool b> using dependent_type_t = _t<dependent_type<T, b>>;

	tpl<typ...> using void_t = void;
	tpl<typ T> struct type_identity
	{
		using type = T;
	};
	tpl<typ T> using type_identity_t = _t<type_identity<T>>;

	using index = std::ptrdiff_t;


} // namespace Rider

namespace Rider::Faiz
{
	tpl<typ T, T v> struct integral_constant;
	tpl<typ T, T v> struct integral_constant
	{
		static cexp T value = v;
		using value_type = T;
		using type = integral_constant; // using injected-class-name
		cCNV operator value_type() const noexcept
		{
			return value;
		}
		cfn
		operator()() const noexcept->value_type
		{
			return value;
		} // since c++14
	};

#define ImplDeclIntT(_n, _t) \
	tpl<_t _vInt> using _n = integral_constant<_t, _vInt>;
#define ImplDeclIntTDe(_t) ImplDeclIntT(_t##_, _t)

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


	using true_ = bool_<true>;
	using false_ = bool_<false>;
	tpl<bool B> using bool_constant = bool_<B>;
	using true_type = true_;
	using false_type = false_;

	tpl<typ T> using sizeof_able = size_t_<sizeof(T)>;
} // namespace Rider::Faiz

namespace Rider::Faiz::detail
{
	tpl<tpl<typ tFirst, typ tSecond> class Trait,
		typ tFirst,
		typ tSecond,
		typ... tRest>
		cfn
	binaryTraitAre_impl()->bool
	{
		cIf(sizeof...(tRest) == 0)
		{
			return Trait<tFirst, tSecond>{}();
		}
		cElse
		{
			return Trait<tFirst, tSecond>{}()
				and binaryTraitAre_impl<Trait, tRest...>();
		}
	}

	tpl<tpl<typ tFirst, typ tSecond> class Trait,
		typ tFirst,
		typ tSecond,
		typ... tRest>
		cfn
	binaryTraitOr_impl()->bool
	{
		cIf(sizeof...(tRest) == 0)
		{
			return Trait<tFirst, tSecond>{}();
		}
		cElse
		{
			return Trait<tFirst, tSecond>{}()
				or binaryTraitOr_impl<Trait, tRest...>();
		}
	}
} // namespace Rider::Faiz::detail


namespace Rider::Faiz
{
	using std::has_virtual_destructor;
	using std::has_virtual_destructor_v;
	using std::endian;
	//  FIXME: below is traits that I am not able to implement yet.
	using std::has_unique_object_representations;
	using std::is_constructible;
	using std::is_constructible_v;
	using std::is_trivially_constructible;
	using std::is_trivially_constructible_v;

	using std::is_default_constructible;
	IS_NOT_ARE_ANY(default_constructible)

	using std::is_trivially_default_constructible;
	IS_NOT_ARE_ANY(trivially_default_constructible)

	using std::is_nothrow_default_constructible;
	IS_NOT_ARE_ANY(nothrow_default_constructible)

	using std::is_nothrow_constructible;
	using std::is_nothrow_constructible_v;

	using std::is_trivially_copy_constructible;
	using std::is_trivially_copy_constructible_v;

	using std::is_nothrow_copy_constructible;

	IS_NOT_ARE_ANY(nothrow_copy_constructible)

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
	IS_NOT_ARE_ANY(class)

	using std::is_trivially_assignable;
	using std::is_trivially_assignable_v;
	using std::is_enum;
	IS_NOT_ARE_ANY(enum);

	tpl<typ T> struct reference_wrapper;

	tpl<typ> struct is_reference_wrapper;

	tpl<typ T> struct add_cv;
	tpl<typ T> struct add_const;
	tpl<typ T> struct add_volatile;

	tpl<typ T> using add_cv_t = _t<add_cv<T>>;
	tpl<typ T> using add_const_t = _t<add_const<T>>;
	tpl<typ T> using add_volatile_t = _t<add_volatile<T>>;

	tpl<typ T> struct remove_const;
	tpl<typ T> struct remove_volatile;

	tpl<typ T> struct is_destructible;


	tpl<typ T> struct is_nothrow_destructible;

	tpl<typ T> using remove_const_t = _t<remove_const<T>>;

	tpl<typ T> using remove_volatile_t = _t<remove_volatile<T>>;

	tpl<typ T> using remove_cv = remove_volatile<remove_const_t<T>>;

	tpl<typ T> using remove_cv_t = _t<remove_cv<T>>;

	tpl<typ T> struct add_lvalue_reference;
	tpl<typ T> struct add_rvalue_reference;
	tpl<typ T> using add_lvalue_reference_t = _t<add_lvalue_reference<T>>;
	tpl<typ T> using add_rvalue_reference_t = _t<add_rvalue_reference<T>>;

	tpl<typ T> add_rvalue_reference_t<T>
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
	tpl<typ T> struct is_object;
	IS_NOT_ARE_ANY(object)
	// FIXME: error: incomplete type ‘Ride r::Faiz::is_integral<long unsigned
	// int>’ used in nested name specifier. No good solution now. Maybe module
	// can help me

	tpl<typ T, typ U> struct is_assignable;

	tpl<typ T, typ U> struct is_nothrow_assignable;
	tpl<typ T, typ U> inline cexp bool is_assignable_v
		= is_assignable<T, U>::value;

	tpl<typ T, typ U> inline cexp bool is_nothrow_assignable_v
		= is_nothrow_assignable<T, U>::value;
	tpl<typ Base, typ Derived> struct is_base_of;
	tpl<typ T> struct remove_reference;
	tpl<typ T> using remove_reference_t = _t<remove_reference<T>>;

	tpl<typ T> struct is_lvalue_reference;

	IS_NOT_ARE_ANY(lvalue_reference);

	tpl<typ T> struct remove_cvref;
	tpl<typ T> using remove_cvref_t = _t<remove_cvref<T>>;
	tpl<typ T> struct is_integral;
	tpl<bool B, typ T = void> struct enable_if;
	tpl<bool B, typ T = void> using enable_if_t = _t<enable_if<B, T>>;

	tpl<bool B, typ T = void> using disable_if = enable_if<not B, T>;

	tpl<bool B, typ T, typ F> struct conditional;

	tpl<typ T, typ U> struct is_same;

	BI_IS(same);
	BI_NOT(same);

	namespace detail
	{
		tpl<typ A0, typ... Rest> cfn
		are_same_aux()->bool
		{
			return (is_same_v<Rest, A0> and ... and true);
		}
	} // namespace detail

	tpl<typ... Types> inline cexp bool are_same_v
		= detail::are_same_aux<Types...>();

	tpl<typ... Types> struct are_same : bool_<are_same_v<Types...>>
	{};

	tpl<typ T, typ... Rest> inline cexp bool is_any_v
		= (is_same_v<T, Rest> or ...);

	tpl<typ T, typ... Rest> struct is_any : bool_<is_any_v<T, Rest...>>
	{};

	tpl<typ T, typ... Rest> inline cexp bool not_any_v
		= (not_same_v<T, Rest> and ...);

	tpl<typ T, typ... Rest> struct not_any : bool_<not_any_v<T, Rest...>>
	{};


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
		tpl<typ tIndex, tIndex... V> struct integer_sequence_aux
		{
			tpl<tpl<typ tFrom, tFrom...> typ tToIndexSeq, typ tTo> using convert
				= tToIndexSeq<tTo, V...>;
		};

		tpl<typ T, size_t... vExtra> struct repeat;
		tpl<typ T, T... vSeq, size_t... vExtra> struct repeat<
			integer_sequence_aux<T, vSeq...>,
			vExtra...>
		{
			// clang-format off
            using type = integer_sequence_aux<
                T,
                vSeq...,
                sizeof...(vSeq) + vSeq...,
 #define Aux(N)  N * sizeof...(vSeq) + vSeq...
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
                vExtra...>;
 #undef Aux
			// clang-format on
		};

		tpl<size_t V> struct parity;
		tpl<size_t V> struct make : parity<V % 39>::tpl pmake<V>
		{};

		tpl<> struct make<0> : type_identity<integer_sequence_aux<size_t>>
		{};
#define MAKE(N, ...) \
	tpl<> struct make<N> \
		: type_identity<integer_sequence_aux<size_t, __VA_ARGS__>> \
	{};
#define PARITY(N, ...) \
	tpl<> struct parity<N> \
	{ \
		tpl<size_t V> struct pmake : repeat<_t<make<V / 39>>, __VA_ARGS__> \
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

	tpl<typ T, T... vSeq> struct integer_sequence
	{
		using value_type = T;
		static_assert(std::is_integral_v<T>,
			"Faiz::integer_sequence can only be instantiated with an integral "
			"type");

		static cfn
		size() noexcept->size_t
		{
			return sizeof...(vSeq);
		}
	};

	tpl<size_t... vSeq> using index_sequence
		= integer_sequence<size_t, vSeq...>;

	tpl<typ T, T V> using make_integer_sequence_aux_unchecked
		= typ detail::make<V>::type::tpl convert<integer_sequence, T>;

	tpl<typ T, T V> struct make_integer_sequence_checked
		: type_identity<make_integer_sequence_aux_unchecked<T, 0 <= V ? V : 0>>
	{
		static_assert(std::is_integral_v<T>,
			"Faiz::make_integer_sequence can only be instantiated with an "
			"integral type");
		static_assert(0 <= V,
			"Faiz::make_integer_sequence must have a non-negative sequence "
			"length");
	};

	tpl<typ T, T V> using make_integer_sequence_aux
		= _t<make_integer_sequence_checked<T, V>>;

	tpl<typ T, T V> using make_integer_sequence
		= make_integer_sequence_aux<T, V>;

	tpl<size_t V> using make_index_sequence = make_integer_sequence<size_t, V>;

	tpl<typ... T> using index_sequence_for = make_index_sequence<sizeof...(T)>;
} // namespace Rider::Faiz


#endif // __FAIZ_FWD_META_H_
