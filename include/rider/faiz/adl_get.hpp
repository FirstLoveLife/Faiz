#ifndef RANGES_V3_DETAIL_ADL_GET_HPP
#define RANGES_V3_DETAIL_ADL_GET_HPP

#include "rider/faiz/faiz_fwd.hpp"
#include <cstddef>

namespace rider::faiz
{
	namespace detail
	{
		namespace _adl_get_
		{
			template<class>
			void
			get();

			template<std::size_t I, typename Tuple>
			constexpr auto
			adl_get(Tuple&& t)
				DECLTYPE_AUTO_RETURN_NOEXCEPT(get<I>(static_cast<Tuple&&>(t)))

					template<typename T, typename Tuple>
					constexpr auto adl_get(Tuple&& t)
						DECLTYPE_AUTO_RETURN_NOEXCEPT(
							get<T>(static_cast<Tuple&&>(t)))
		} // namespace _adl_get_
		using _adl_get_::adl_get;
	} // namespace detail
} // namespace rider::faiz

#endif // RANGES_V3_DETAIL_ADL_GET_HPP
