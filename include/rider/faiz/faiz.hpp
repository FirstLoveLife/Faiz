#ifndef FAIZ
#define FAIZ
#include "rider/faiz/algebra.hpp"
#include "rider/faiz/logic.hpp"
#include "rider/faiz/type_traits.hpp"
#include <iostream>
namespace rider
{
#define IMPL(...) __VA_ARGS__
#define COMMA ,

} // namespace rider

namespace rider::faiz
{
	template<typename...>
	struct empty_base
	{};

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
		struct impl
		{
			using type = _type;
		};

	public:
		template<typename... _types>
		using apply = impl<_types...>;
	};

	template<typename _type, typename... _types>
	using well_formed_t =
		typename always<_type>::template apply<_types...>::type;

	template<typename... _types>
	using when_valid = well_formed_t<when<true>, _types...>;

	template<typename _type, typename... _types>
	struct are_same : logic::and_<is_same<_type, _types>...>
	{};

	template<bool _bCond>
	using enable_when = enable_if_t<_bCond, when<true>>;

	template<typename _tCond, typename _tThen = true_, typename _tElse = false_>
	using cond_t = conditional_t<_tCond::value, _tThen, _tElse>;


} // namespace rider::faiz
#endif
