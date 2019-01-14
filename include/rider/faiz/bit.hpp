#ifndef FAIZ_BIT
#define FAIZ_BIT
#include "rider/faiz/cstddef.hpp"
#include <iostream>
#include <string_view>
#include <vector>
namespace Rider::Faiz
{
	namespace detail
	{
		template<char Chs>
		constexpr int
		toDecImpl()
		{
			return Chs > '9' ? Chs - 'A' + 10 : Chs - '0';
		}
	} // namespace detail
	template<int from, char... Chs>
	constexpr int
	toDec()
	{
		int ret{};

		return ((ret *= from, ret += detail::toDecImpl<Chs>()), ...);
	}
	inline namespace literals
	{
		template<char... Chs>
		constexpr int operator"" _B()
		{
			return toDec<2, Chs...>();
		}
		template<char... Chs>
		constexpr int operator"" _O()
		{
			return toDec<8, Chs...>();
		}

		// I decide not to provide heximal, too ugly

	} // namespace literals
} // namespace Rider::Faiz
#endif
