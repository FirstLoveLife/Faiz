#ifndef INTEGER_SEQUENCE
#define INTEGER_SEQUENCE
#include "rider/faiz/cstddef.hpp"
#include "rider/faiz/type_traits.hpp"
#include <boost/preprocessor.hpp>
/*
39 level loop-unrolling is fastest on my os. nearly as fast as built_in
integer_sequence
https://stackoverflow.com/questions/53350706/how-to-understand-libcxxs-implementation-of-make-integer-sequence
 */

namespace rider::faiz
{
	template<class Tindex, Tindex... V>
	struct integer_sequence_aux
	{
		template<template<class Tfrom, Tfrom...> class TtoIndexSeq, class Tto>
		using convert = TtoIndexSeq<Tto, V...>;
	};
} // namespace rider::faiz

namespace rider::faiz::detail
{
	template<typename T, faiz::size_t... Vextra>
	struct repeat;
	template<typename T, T... Vseq, faiz::size_t... Vextra>
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

	template<faiz::size_t V>
	struct parity;
	template<faiz::size_t V>
	struct make : parity<V % 39>::template pmake<V>
	{};

	template<>
	struct make<0> : type_identity<integer_sequence_aux<faiz::size_t>>
	{};
#define MAKE(N, ...) \
	template<> \
	struct make<N> \
		: type_identity<integer_sequence_aux<faiz::size_t, __VA_ARGS__>> \
	{};
#define PARITY(N, ...) \
	template<> \
	struct parity<N> \
	{ \
		template<faiz::size_t V> \
		struct pmake : repeat<typename make<V / 39>::type, __VA_ARGS__> \
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

} // namespace rider::faiz::detail
namespace rider::faiz
{
	template<class T, T... Vseq>
	struct integer_sequence
	{
		typedef T value_type;
		static_assert(faiz::is_integral<T>::value,
			"faiz::integer_sequence can only be instantiated with an integral "
			"type");
		static constexpr faiz::size_t
		size() noexcept
		{
			return sizeof...(Vseq);
		}
	};

	template<faiz::size_t... Vseq>
	using index_sequence = integer_sequence<faiz::size_t, Vseq...>;

	template<typename T, T V>
	using make_integer_sequence_aux_unchecked =
		typename detail::make<V>::type::template convert<integer_sequence, T>;

	template<class T, T V>
	struct make_integer_sequence_checked
		: type_identity<make_integer_sequence_aux_unchecked<T, 0 <= V ? V : 0>>
	{
		static_assert(faiz::is_integral<T>::value,
			"faiz::make_integer_sequence can only be instantiated with an "
			"integral type");
		static_assert(0 <= V,
			"faiz::make_integer_sequence must have a non-negative sequence "
			"length");
	};

	template<class T, T V>
	using make_integer_sequence_aux =
		typename make_integer_sequence_checked<T, V>::type;

	template<class T, T V>
	using make_integer_sequence = make_integer_sequence_aux<T, V>;

	template<faiz::size_t V>
	using make_index_sequence = make_integer_sequence<faiz::size_t, V>;

	template<class... T>
	using index_sequence_for = make_index_sequence<sizeof...(T)>;
} // namespace rider::faiz
#endif
