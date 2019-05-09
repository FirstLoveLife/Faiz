#ifndef __BINARY_H_
#define __BINARY_H_
#include "rider/faiz/macros.hpp"

// namespace Rider::Faiz
// {
// 	Tpl<char high, char... digits> cfn
// 	operator"" _b()
// 	{
// 		static_assert(high == '0' or high == '1', "no binary number!");
// 		cIf(sizeof...(digits) == 0)
// 		{
// 			return high - '0';
// 		}
// 		cElse
// 		{
// 			return (high - '0') * (1 << sizeof...(digits))
// 				+ operator"" _b<digits...>();
// 		}
// 	}
// } // namespace Rider::Faiz

namespace Rider::Faiz
{
	namespace detail
	{
		Tpl<char Chs> cfn
		toDecImpl()
		{
			return Chs > '9' ? Chs - 'A' + 10 : Chs - '0';
		}
	} // namespace detail
	Tpl<int from, char... Chs> cfn
	toDec()
	{
		int ret{};

		return ((ret *= from, ret += detail::toDecImpl<Chs>()), ...);
	}
	inline namespace literals
	{
		Tpl<char... Chs> cfn
		operator"" _B()
		{
			return toDec<2, Chs...>();
		}
		Tpl<char... Chs> cfn
		operator"" _O()
		{
			return toDec<8, Chs...>();
		}

		Tpl<char... Chs> cfn
		operator"" _H()
		{
			return toDec<16, Chs...>();
		}

		// <del>I decide not to provide heximal, too ugly</del>
		// https://stackoverflow.com/questions/56053939/universal-number-system-conversion-to-decimal-with-udl?noredirect=1#comment98749517_56053939
		// TODO:operator ""_H(const char*, std::size_t)

	} // namespace literals
} // namespace Rider::Faiz

#endif // __BINARY_H_
