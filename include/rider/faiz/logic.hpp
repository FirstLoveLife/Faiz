#ifndef LOGIC
#define LOGIC
#include "rider/faiz/type_traits.hpp"
namespace rider::faiz::logic
{
	template<class...>
	struct and_;

	template<>
	struct and_<> : true_
	{};

	template<class _b1>
	struct and_<_b1> : _b1
	{};

	template<class _b1, class _b2, class... _bn>
	struct and_<_b1, _b2, _bn...>
		: faiz::conditional_t<_b1::value, and_<_b2, _bn...>, _b1>
	{};

	template<class...>
	struct or_;

	template<>
	struct or_<> : false_
	{};

	template<class _b1>
	struct or_<_b1> : _b1
	{};

	template<class _b1, class _b2, class... _bn>
	struct or_<_b1, _b2, _bn...>
		: faiz::conditional<_b1::value, _b1, or_<_b2, _bn...>>::type
	{};


	template<class _b>
	struct not_ : bool_<!_b::value>
	{};

	template<class... _b>
	using conjunction = and_<_b...>;

	template<class... _b>
	using disjunction = or_<_b...>;

	template<class _b>
	using negation = not_<_b>;

} // namespace rider::faiz::logic
#endif
