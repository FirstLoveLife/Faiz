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

	namespace detail
	{
		template<typename T, size_t... Vextra>
		struct repeat;
		template<typename T, T... Tp, size_t... Vextra>
		struct repeat<integer_sequence_aux<T, Tp...>, Vextra...>
		{
			// clang-format off
			using type = integer_sequence_aux<
				T,
				Tp...,
				sizeof...(Tp) + Tp...,
#define Aux(N)  N * sizeof...(Tp) + Tp...
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

		template<size_t Tp>
		struct parity;
		template<size_t Tp>
		struct make : parity<Tp % 39>::template pmake<Tp>
		{};

		template<>
		struct make<0> : type_identity<integer_sequence_aux<size_t>>
		{};
#define MAKE(N, ...) \
	template<> \
	struct make<N> : type_identity<integer_sequence_aux<size_t, __VA_ARGS__>> \
	{};
#define PARITY(N, ...) \
	template<> \
	struct parity<N> \
	{ \
		template<size_t Tp> \
		struct pmake : repeat<typename make<Tp / 39>::type, __VA_ARGS__> \
		{}; \
	};
#define AppendTp(z, n, data) (Tp - n)
#define TpSequence(n) BOOST_PP_REPEAT(n, AppendTp, )
#define ImplParityAux(INTEGER) \
	BOOST_PP_SEQ_ENUM( \
		BOOST_PP_SEQ_REVERSE(BOOST_PP_SEQ_TAIL(TpSequence(INTEGER))))
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

	template<class T, T... _Ip>
	struct integer_sequence
	{
		typedef T value_type;
		static_assert(faiz::is_integral<T>::value,
			"std::integer_sequence can only be instantiated with an integral "
			"type");
		static constexpr size_t
		size() noexcept
		{
			return sizeof...(_Ip);
		}
	};

	template<size_t... _Ip>
	using index_sequence = integer_sequence<size_t, _Ip...>;

	template<typename T, T Tp>
	using make_integer_sequence_aux_unchecked =
		typename detail::make<Tp>::type::template convert<integer_sequence, T>;

	template<class T, T _Ep>
	struct make_integer_sequence_checked
		: type_identity<
			  make_integer_sequence_aux_unchecked<T, 0 <= _Ep ? _Ep : 0>>
	{
		static_assert(faiz::is_integral<T>::value,
			"std::make_integer_sequence can only be instantiated with an "
			"integral type");
		static_assert(0 <= _Ep,
			"std::make_integer_sequence must have a non-negative sequence "
			"length");
	};

	template<class T, T _Ep>
	using make_integer_sequence_aux =
		typename make_integer_sequence_checked<T, _Ep>::type;

	template<class T, T Tp>
	using make_integer_sequence = make_integer_sequence_aux<T, Tp>;

	template<size_t Tp>
	using make_index_sequence = make_integer_sequence<size_t, Tp>;

	template<class... T>
	using index_sequence_for = make_index_sequence<sizeof...(T)>;
} // namespace rider::faiz
#endif
