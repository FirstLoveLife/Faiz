#ifndef ALGEBRA
#define ALGEBRA
#include "rider/faiz/type_traits.hpp" // for _t,
namespace rider::faiz
{
	template<typename T>
	struct identity
	{
		using type = T;
	};

	template<typename T>
	using identity_t = _t<identity<T>>;

} // namespace rider::faiz
#endif
