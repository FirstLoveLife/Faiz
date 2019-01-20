/// \file
// Range v3 library
//
//  Copyright Casey Carter 2018
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3

#ifndef RANGES_V3_DETAIL_ADL_GET_HPP
#define RANGES_V3_DETAIL_ADL_GET_HPP

#include "rider/faiz/faiz_fwd.hpp"
#include "rider/faiz/macros.hpp"
#include <cstddef>

namespace Rider::Faiz
{
	namespace detail
	{
		namespace _adl_get_
		{
			tpl<class> void
			get();

			tpl<std::size_t I, typ Tuple> cexp auto
			adl_get(Tuple&& t)
				DECLTYPE_AUTO_RETURN_NOEXCEPT(get<I>(static_cast<Tuple&&>(t)))

					tpl<typ T, typ Tuple> cexp auto adl_get(Tuple&& t)
						DECLTYPE_AUTO_RETURN_NOEXCEPT(
							get<T>(static_cast<Tuple&&>(t)))
		} // namespace _adl_get_
		using _adl_get_::adl_get;
	} // namespace detail
} // namespace Rider::Faiz

#endif // RANGES_V3_DETAIL_ADL_GET_HPP
