#ifndef SCTDDEF
#define SCTDDEF
#include "rider/faiz/macros.hpp"
#include <cstddef>
namespace Rider::Faiz
{

	using std::nullptr_t;
	using std::size_t;

	// Faiz::ptrdiff_t is the signed integer type of the result of subtracting
	// two pointers.
	// 	using ptrdiff_t = __PTRDIFF_TYPE__;
	// #	if __cplusplus >= 201103L
	// 	struct nullptr_t
	// 	{
	// 		void* lx;

	// 		struct nat
	// 		{
	// 			int for_bool_;
	// 		};
	// 		operator std::nullptr_t()
	// 		{
	// 			return nullptr;
	// 		}

	// 		cexp nullptr_t() : lx(0)
	// 		{}
	// 		cexp nullptr_t(int nat::*) : lx(0)
	// 		{}

	// 		cexp operator int nat::*() const
	// 		{
	// 			return 0;
	// 		}

	// 		Tpl<class T>
	// 		cexp operator T*() const
	// 		{
	// 			return 0;
	// 		}

	// 		Tpl<class T, class U>
	// 		operator T U::*() const
	// 		{
	// 			return 0;
	// 		}

	// 		friend cexp bool operator==(nullptr_t, nullptr_t)
	// 		{
	// 			return true;
	// 		}
	// 		friend cexp bool operator!=(nullptr_t, nullptr_t)
	// 		{
	// 			return false;
	// 		}
	// #	else
	// 	using nullptr_t = decltype(nullptr);
	// #	endif

	// 	}; // namespace Rider::Faiz

	// 	inline cexp nullptr_t
	// 	get_nullptr_t()
	// 	{
	// 		return nullptr_t(0);
	// 	}
	// #	define nullptr Rider::Faiz::get_nullptr_t()
} // namespace Rider::Faiz
#endif
