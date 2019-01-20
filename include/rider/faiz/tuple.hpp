#ifndef TUPLEE
#define TUPLEE

#include "rider/faiz/macros.hpp"
namespace Rider::Faiz
{
	struct ignore_t
	{
		ignore_t()
		{}

		tpl<typ T> const ignore_t&
		operator=(const T&) const
		{
			return *this;
		}
	};

} // namespace Rider::Faiz

#endif
