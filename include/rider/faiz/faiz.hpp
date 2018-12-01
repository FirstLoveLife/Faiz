#ifndef FAIZ
#define FAIZ
#include "rider/faiz/cstddef.hpp"
#include "rider/faiz/faiz_fwd.hpp"
#include "rider/faiz/initializer_list.hpp"
#include <iostream>

namespace rider::faiz
{

	template<class... Args>
	void
	print(Args... args)
	{
		std::string sep = " ";
		((std::cout << sep << args), ...);
	}

	template<typename... List>
	inline constexpr unsigned int
	sum(const List&... list)
	{
		return (0 + ... + sizeof(List));
	}


} // namespace rider::faiz


#endif
