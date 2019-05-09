#ifndef __BINARY_H_
#define __BINARY_H_
#include "rider/faiz/macros.hpp"

namespace Rider::Faiz
{
	Tpl<char high, char... digits> cfn
	operator"" _b()
	{
		static_assert(high == '0' or high == '1', "no binary number!");
		cIf(sizeof...(digits) == 0)
		{
			return high - '0';
		}
		cElse
		{
			return (high - '0') * (1 << sizeof...(digits))
				+ operator"" _b<digits...>();
		}
	}
} // namespace Rider::Faiz

#endif // __BINARY_H_
