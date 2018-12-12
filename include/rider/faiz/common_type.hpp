/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2013-present
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//


#ifndef COMMON_TYPE
#define COMMON_TYPE

#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"

// TODO: use my own tuple and pair
#include <tuple>

namespace Rider::Faiz
{
	namespace detail
	{
		template<typename From, typename To>
		struct _copy_cv_ : type_identity<To>
		{};
		template<typename From, typename To>
		struct _copy_cv_<From const, To> : type_identity<To const>
		{};
		template<typename From, typename To>
		struct _copy_cv_<From volatile, To> : type_identity<To volatile>
		{};
		template<typename From, typename To>
		struct _copy_cv_<From const volatile, To>
			: type_identity<To const volatile>
		{};
		template<typename From, typename To>
		using _copy_cv = _t<_copy_cv_<From, To>>;

		template<typename T, typename U, typename = void>
		struct _builtin_common;

		template<typename T, typename U>
		using _builtin_common_t = _t<_builtin_common<T, U>>;

		template<typename T, typename U>
		using _cond_res = decltype(true ? declval<T>() : declval<U>());

		template<typename T, typename U, typename R = _builtin_common_t<T&, U&>>
		using _rref_res
			= meta::if_<is_reference<R>, _t<remove_reference<R>>&&, R>;

		template<class T, class U>
		using _lref_res = _cond_res<_copy_cv<T, U>&, _copy_cv<U, T>&>;

		template<typename T, typename U, typename /*= void*/>
		struct _builtin_common
			: meta::lazy::let<meta::defer<decay_t,
				  meta::defer<_cond_res, as_cref_t<T>, as_cref_t<U>>>>
		{};
		template<typename T, typename U>
		struct _builtin_common<T&&,
			U&&,
			meta::if_<meta::and_<is_convertible<T, _rref_res<T, U>>,
				is_convertible<U, _rref_res<T, U>>>>>
			: type_identity<_rref_res<T, U>>
		{};
		template<typename T, typename U>
		struct _builtin_common<T&, U&> : meta::defer<_lref_res, T, U>
		{};
		template<typename T, typename U>
		struct _builtin_common<T&,
			U&&,
			meta::if_<is_convertible<U, _builtin_common_t<T&, U const&>>>>
			: _builtin_common<T&, U const&>
		{};
		template<typename T, typename U>
		struct _builtin_common<T&&, U&> : _builtin_common<U&, T&&>
		{};
	} // namespace detail

	template<typename... Ts>
	struct common_type
	{};

	template<typename T>
	struct common_type<T> : decay<T>
	{};

	template<typename T, typename U>
	struct common_type<T, U>
		: meta::if_c<(is_same_v<decay_t<T>, T> and is_same_v<decay_t<U>, U>),
			  meta::defer<detail::_builtin_common_t, T, U>,
			  common_type<decay_t<T>, decay_t<U>>>
	{};

	template<typename... Ts>
	using common_type_t = _t<common_type<Ts...>>;


	template<typename T, typename U, typename... Vs>
	struct common_type<T, U, Vs...>
		: meta::lazy::fold<meta::list<U, Vs...>, T, meta::quote<common_type_t>>
	{};

	template<typename T, typename U, typename TQual, typename UQual>
	struct basic_common_reference
	{};

	/// \cond
	namespace detail
	{
		using _rref = meta::quote_trait<add_rvalue_reference>;
		using _lref = meta::quote_trait<add_lvalue_reference>;

		template<typename T>
		struct _xref : type_identity<meta::quote_trait<meta::id>>
		{};
		template<typename T>
		struct _xref<T&&> : type_identity<meta::compose<_rref, _t<_xref<T>>>>
		{};
		template<typename T>
		struct _xref<T&> : type_identity<meta::compose<_lref, _t<_xref<T>>>>
		{};
		template<typename T>
		struct _xref<T const> : type_identity<meta::quote_trait<add_const>>
		{};
		template<typename T>
		struct _xref<T volatile>
			: type_identity<meta::quote_trait<add_volatile>>
		{};
		template<typename T>
		struct _xref<T const volatile>
			: type_identity<meta::quote_trait<add_cv>>
		{};

		template<typename T, typename U>
		using _basic_common_reference
			= basic_common_reference<remove_cvref_t<T>,
				remove_cvref_t<U>,
				_t<_xref<T>>,
				_t<_xref<U>>>;

		template<typename T, typename U, typename = void>
		struct _common_reference2
			: meta::if_<meta::is_trait<_basic_common_reference<T, U>>,
				  _basic_common_reference<T, U>,
				  common_type<T, U>>
		{};

		template<typename T, typename U>
		struct _common_reference2<T,
			U,
			meta::if_<is_reference<_builtin_common_t<T, U>>>>
			: _builtin_common<T, U>
		{};
	} // namespace detail

	template<typename... Ts>
	struct common_reference
	{};

	template<typename... Ts>
	using common_reference_t = _t<common_reference<Ts...>>;


	template<typename T>
	struct common_reference<T> : type_identity<T>
	{};

	template<typename T, typename U>
	struct common_reference<T, U> : detail::_common_reference2<T, U>
	{};

	template<typename T, typename U, typename... Vs>
	struct common_reference<T, U, Vs...>
		: meta::lazy::
			  fold<meta::list<U, Vs...>, T, meta::quote<common_reference_t>>
	{};

	template<typename F, typename S>
	struct common_pair;

	template<typename... Ts>
	struct common_tuple;

	template<typename F1, typename S1, typename F2, typename S2>
	struct common_type<std::pair<F1, S1>, common_pair<F2, S2>>;

	template<typename F1, typename S1, typename F2, typename S2>
	struct common_type<common_pair<F1, S1>, std::pair<F2, S2>>;

	template<typename F1, typename S1, typename F2, typename S2>
	struct common_type<common_pair<F1, S1>, common_pair<F2, S2>>;

	template<typename... Ts, typename... Us>
	struct common_type<common_tuple<Ts...>, std::tuple<Us...>>;

	template<typename... Ts, typename... Us>
	struct common_type<std::tuple<Ts...>, common_tuple<Us...>>;

	template<typename... Ts, typename... Us>
	struct common_type<common_tuple<Ts...>, common_tuple<Us...>>;

	template<typename F1,
		typename S1,
		typename F2,
		typename S2,
		typename Qual1,
		typename Qual2>
	struct basic_common_reference<common_pair<F1, S1>,
		std::pair<F2, S2>,
		Qual1,
		Qual2>;

	template<typename F1,
		typename S1,
		typename F2,
		typename S2,
		typename Qual1,
		typename Qual2>
	struct basic_common_reference<std::pair<F1, S1>,
		common_pair<F2, S2>,
		Qual1,
		Qual2>;

	template<typename F1,
		typename S1,
		typename F2,
		typename S2,
		typename Qual1,
		typename Qual2>
	struct basic_common_reference<common_pair<F1, S1>,
		common_pair<F2, S2>,
		Qual1,
		Qual2>;

	template<typename... Ts, typename... Us, typename Qual1, typename Qual2>
	struct basic_common_reference<common_tuple<Ts...>,
		std::tuple<Us...>,
		Qual1,
		Qual2>;

	template<typename... Ts, typename... Us, typename Qual1, typename Qual2>
	struct basic_common_reference<std::tuple<Ts...>,
		common_tuple<Us...>,
		Qual1,
		Qual2>;

	template<typename... Ts, typename... Us, typename Qual1, typename Qual2>
	struct basic_common_reference<common_tuple<Ts...>,
		common_tuple<Us...>,
		Qual1,
		Qual2>;
} // namespace Rider::Faiz

#endif
