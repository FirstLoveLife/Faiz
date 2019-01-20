#ifndef FAIZ_BIT
#define FAIZ_BIT
#include "rider/faiz/cstddef.hpp"
#include "rider/faiz/macros.hpp"
#include <iostream>
#include <string_view>
#include <vector>
namespace Rider::Faiz
{
	namespace detail
	{
		tpl<char Chs> cexp int
		toDecImpl()
		{
			return Chs > '9' ? Chs - 'A' + 10 : Chs - '0';
		}
	} // namespace detail
	tpl<int from, char... Chs> cexp int
	toDec()
	{
		int ret{};

		return ((ret *= from, ret += detail::toDecImpl<Chs>()), ...);
	}
	inline namespace literals
	{
		tpl<char... Chs> cexp int operator"" _B()
		{
			return toDec<2, Chs...>();
		}
		tpl<char... Chs> cexp int operator"" _O()
		{
			return toDec<8, Chs...>();
		}

		// I decide not to provide heximal, too ugly

	} // namespace literals
} // namespace Rider::Faiz
#endif
