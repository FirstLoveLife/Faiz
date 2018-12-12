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

#ifndef RANGES_V3_UTILITY_MOVE_HPP
#define RANGES_V3_UTILITY_MOVE_HPP

#include "rider/faiz/associated_types.hpp"
#include "rider/faiz/faiz_fwd.hpp"
#include "rider/faiz/utility.hpp"

namespace Rider::Faiz
{
	namespace aux
	{
		/// \ingroup group-utility
		struct move_fn : range::move_tag
		{
			template<typename T, typename U = _t<remove_reference<T>>>
			U&&
			operator()(T&& t) const noexcept
			{
				return static_cast<U&&>(t);
			}
		};

		/// \ingroup group-utility
		/// \sa `move_fn`
		inline constexpr move_fn move{};

		/// \ingroup group-utility
		/// \sa `move_fn`
		template<typename T>
		_t<remove_reference<T>>&&
		operator|(T&& t, move_fn move) noexcept
		{
			return move(t);
		}

		/// \ingroup group-utility
		/// \sa `move_fn`
		template<typename R>
		using move_t
			= meta::if_<is_reference<R>, _t<remove_reference<R>>&&, decay_t<R>>;
	} // namespace aux

	/// \cond
	namespace adl_move_detail
	{
#if RANGES_BROKEN_CPO_LOOKUP
		void
		iter_move(); // unqualified name lookup block
#endif

		template<typename T, typename = decltype(iter_move(declval<T>()))>
		true_
		try_adl_iter_move_(int);

		template<typename T>
		false_
		try_adl_iter_move_(long);

		template<typename T>
		struct is_adl_indirectly_movable_
			: meta::id_t<decltype(adl_move_detail::try_adl_iter_move_<T>(42))>
		{};

		// TODO: investigate the breakage when these are made constexpr.
		// (Results in ODR-use of projected_readable::operator*)
		struct iter_move_fn
		{
			template<typename I,
				typename = meta::if_c<is_adl_indirectly_movable_<I&>::value>>
			auto
			operator()(I&& i) const DECLTYPE_AUTO_RETURN_NOEXCEPT(iter_move(i))

				template<typename I,
					typename
					= meta::if_c<!is_adl_indirectly_movable_<I&>::value>,
					typename R = reference_t<I>>
				auto
				operator()(I&& i) const DECLTYPE_AUTO_RETURN_NOEXCEPT(
					static_cast<aux::move_t<R>>(aux::move(*i)))
		};
	} // namespace adl_move_detail
	/// \endcond

	inline namespace CPOs
	{
		inline constexpr adl_move_detail::iter_move_fn iter_move{};
	}

	/// \cond
	struct indirect_move_fn
	{
		template<typename I>
		DEPRECATED("Please replace uses of ranges::indirect_move "
				   "with ranges::iter_move.")
		void
		operator()(I&& i) const AUTO_RETURN_NOEXCEPT(iter_move((I &&) i))
	};

	inline constexpr indirect_move_fn indirect_move{};

	namespace detail
	{
		template<typename I, typename O>
		using is_indirectly_movable_
			= bool_<is_constructible<_t<Faiz::value_type<I>>,
						decltype(iter_move(declval<I&>()))>::value
				&& is_assignable<_t<value_type<I>>&,
					   decltype(iter_move(declval<I&>()))>::value
				&& is_assignable<reference_t<O>, _t<value_type<I>>>::value
				&& is_assignable<reference_t<O>,
					   decltype(iter_move(declval<I&>()))>::value>;

		template<typename I, typename O>
		using is_nothrow_indirectly_movable_
			= bool_<noexcept(iter_move(declval<I&>()))
				&& std::is_nothrow_constructible<_t<value_type<I>>,
					   decltype(iter_move(declval<I&>()))>::value
				&& Rider::Faiz::is_nothrow_assignable<_t<value_type<I>>&,
					   decltype(iter_move(declval<I&>()))>::value
				&& Rider::Faiz::is_nothrow_assignable<reference_t<O>,
					   _t<value_type<I>>>::value
				&& Rider::Faiz::is_nothrow_assignable<reference_t<O>,
					   decltype(iter_move(declval<I&>()))>::value>;
	} // namespace detail
	/// \endcond

	template<typename I, typename O>
	struct is_indirectly_movable
		: _t<meta::if_<
			  meta::is_trait<meta::defer<detail::is_indirectly_movable_, I, O>>,
			  meta::defer<detail::is_indirectly_movable_, I, O>,
			  false_>>
	{};

	template<typename I, typename O>
	struct is_nothrow_indirectly_movable
		: _t<meta::if_<
			  meta::is_trait<
				  meta::defer<detail::is_nothrow_indirectly_movable_, I, O>>,
			  meta::defer<detail::is_nothrow_indirectly_movable_, I, O>,
			  false_>>
	{};
} // namespace Rider::Faiz

#endif
