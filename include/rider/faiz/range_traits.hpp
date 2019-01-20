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

#ifndef RANGES_V3_RANGE_TRAITS_HPP
#define RANGES_V3_RANGE_TRAITS_HPP

#include "rider/faiz/macros.hpp"
#include <array>
#include <iterator>
#include <meta/meta.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_fwd.hpp>
#include <type_traits>
#include <utility>

namespace ranges
{
	inline namespace v3
	{
		/// \addtogroup group-core
		/// @{

		// Aliases (SFINAE-able)
		tpl<typ Rng> using iterator_t = concepts::Range::iterator_t<Rng>;

		tpl<typ Rng> using sentinel_t = concepts::Range::sentinel_t<Rng>;

		tpl<typ Rng> using range_difference_type_t
			= concepts::Range::difference_t<Rng>;

		tpl<typ Rng> using range_size_type_t
			= meta::_t<std::make_unsigned<range_difference_type_t<Rng>>>;

		tpl<typ Rng> using range_value_type_t
			= concepts::InputRange::value_t<Rng>;

		tpl<typ Rng> using range_reference_t
			= concepts::InputRange::reference_t<Rng>;

		tpl<typ Rng> using range_rvalue_reference_t
			= concepts::InputRange::rvalue_reference_t<Rng>;

		tpl<typ Rng> using range_common_reference_t
			= concepts::InputRange::common_reference_t<Rng>;

		tpl<typ Rng> using range_category_t
			= concepts::InputRange::category_t<Rng>;

		tpl<typ Rng> using range_common_iterator_t
			= common_iterator_t<iterator_t<Rng>, sentinel_t<Rng>>;

		tpl<typ Rng> using safe_iterator_t
			= meta::if_<std::is_lvalue_reference<Rng>,
				meta::if_<Range<Rng>, iterator_t<Rng>>,
				dangling<iterator_t<Rng>>>;

		/// \cond
		// Deprecated type aliases
		tpl<typ Rng> using range_iterator_t
			RANGES_DEPRECATED("Please use ranges::iterator_t instead")
			= concepts::Range::iterator_t<Rng>;

		tpl<typ Rng> using range_sentinel_t
			RANGES_DEPRECATED("Please use ranges::sentinel_t instead")
			= concepts::Range::sentinel_t<Rng>;

		tpl<typ Rng> using range_difference_t RANGES_DEPRECATED(
			"Please use ranges::range_difference_type_t instead")
			= concepts::Range::difference_t<Rng>;

		tpl<typ Rng> using range_size_t
			RANGES_DEPRECATED("Please use ranges::range_size_type_t instead")
			= meta::_t<std::make_unsigned<range_difference_type_t<Rng>>>;

		tpl<typ Rng> using range_value_t
			RANGES_DEPRECATED("Please use ranges::range_value_type_t instead")
			= concepts::InputRange::value_t<Rng>;

		tpl<typ Rng> using range_safe_iterator_t
			RANGES_DEPRECATED("Please use ranges::safe_iterator_t instead")
			= safe_iterator_t<Rng>;

		tpl<typ Rng> using range_safe_sentinel_t
			RANGES_DEPRECATED("range_safe_sentinel_t is deprecated")
			= meta::if_<std::is_lvalue_reference<Rng>,
				meta::if_<Range<Rng>, sentinel_t<Rng>>,
				dangling<sentinel_t<Rng>>>;

		// Deprecated metafunctions
		tpl<typ Rng> using range_iterator
			RANGES_DEPRECATED("range_iterator is deprecated")
			= meta::defer<iterator_t, Rng>;

		tpl<typ Rng> using range_sentinel
			RANGES_DEPRECATED("range_sentinel is deprecated")
			= meta::defer<sentinel_t, Rng>;

		tpl<typ Rng> using range_category
			RANGES_DEPRECATED("range_category is deprecated")
			= meta::defer<range_category_t, Rng>;

		tpl<typ Rng> using range_value
			RANGES_DEPRECATED("range_value is deprecated")
			= meta::defer<range_value_type_t, Rng>;

		tpl<typ Rng> using range_difference
			RANGES_DEPRECATED("range_difference is deprecated")
			= meta::defer<range_difference_type_t, Rng>;

		tpl<typ Rng> using range_reference
			RANGES_DEPRECATED("range_reference is deprecated")
			= meta::defer<range_reference_t, Rng>;

		tpl<typ Rng> using range_rvalue_reference
			RANGES_DEPRECATED("range_rvalue_reference is deprecated")
			= meta::defer<range_rvalue_reference_t, Rng>;

		tpl<typ Rng> using range_common_reference
			RANGES_DEPRECATED("range_common_reference is deprecated")
			= meta::defer<range_common_reference_t, Rng>;

		tpl<typ Rng> using range_size
			RANGES_DEPRECATED("range_size is deprecated")
			= meta::defer<range_size_type_t, Rng>;

		namespace detail
		{
			std::integral_constant<cardinality, finite>
			test_cardinality(void*);
			tpl<cardinality Card> std::integral_constant<cardinality, Card>
			test_cardinality(basic_view<Card>*);
			tpl<typ T, std::size_t N>
				std::integral_constant<cardinality, static_cast<cardinality>(N)>
					test_cardinality(T (*)[N]);
			tpl<typ T, std::size_t N>
				std::integral_constant<cardinality, static_cast<cardinality>(N)>
				test_cardinality(std::array<T, N>*);
		} // namespace detail
		/// \endcond

		// User customization point for specifying the cardinality of ranges:
		tpl<typ Rng, typ Void /*= void*/> struct range_cardinality
			: meta::if_<std::is_same<Rng, uncvref_t<Rng>>,
				  decltype(detail::test_cardinality(
					  static_cast<uncvref_t<Rng>*>(nullptr))),
				  range_cardinality<uncvref_t<Rng>>>
		{};

		/// @}
	} // namespace v3
} // namespace ranges

#endif
