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

	template<bool _bCond>
	struct when;

	template<typename _type>
	struct always
	{
	private:
		template<typename...>
		struct impl : type_identity<_type>
		{};

	public:
		template<typename... _types>
		using apply = impl<_types...>;
	};

	template<typename _type, typename... _types>
	using well_formed_t =
		typename always<_type>::template apply<_types...>::type;

	template<typename... _types>
	using when_valid = well_formed_t<when<true>, _types...>;


} // namespace rider::faiz


#endif
