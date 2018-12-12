#include "../testsuite_tr1.h"
#include "rider/faiz/type_traits.hpp"
using namespace Rider::Faiz;
using namespace __gnu_test;

void
test01()
{
	static_assert(!is_floating_point<char>::value);
	static_assert(!is_floating_point<signed char>::value, "");
	static_assert(!is_floating_point<unsigned char>::value, "");
	static_assert(!is_floating_point<wchar_t>::value, "");
	static_assert(!is_floating_point<short>::value, "");
	static_assert(!is_floating_point<unsigned short>::value, "");
	static_assert(!is_floating_point<int>::value, "");
	static_assert(!is_floating_point<unsigned int>::value, "");
	static_assert(!is_floating_point<long>::value, "");
	static_assert(!is_floating_point<unsigned long>::value, "");
	static_assert(!is_floating_point<long long>::value, "");
	static_assert(!is_floating_point<unsigned long long>::value, "");

	static_assert(is_floating_point<float>::value, "");
	static_assert(is_floating_point<double>::value, "");
	static_assert(is_floating_point<long double>::value, "");
	static_assert(is_floating_point<__float128>::value, "");
	static_assert(!is_floating_point<__int128>::value, "");
	static_assert(!is_floating_point<unsigned __int128>::value, "");
	static_assert(!is_floating_point<ClassType>::value, "");
}

int
main()
{
	test01();
}
