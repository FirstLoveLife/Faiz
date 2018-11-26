#include "../testsuite_tr1.h"
#include "rider/faiz/type_traits.hpp"
using namespace rider::faiz;
using namespace __gnu_test;

void
test01()
{
	static_assert(test_category<rider::faiz::is_floating_point, char>(false));
	static_assert(test_category<is_floating_point, signed char>(false), "");
	static_assert(test_category<is_floating_point, unsigned char>(false), "");
	static_assert(test_category<is_floating_point, wchar_t>(false), "");
	static_assert(test_category<is_floating_point, short>(false), "");
	static_assert(test_category<is_floating_point, unsigned short>(false), "");
	static_assert(test_category<is_floating_point, int>(false), "");
	static_assert(test_category<is_floating_point, unsigned int>(false), "");
	static_assert(test_category<is_floating_point, long>(false), "");
	static_assert(test_category<is_floating_point, unsigned long>(false), "");
	static_assert(test_category<is_floating_point, long long>(false), "");
	static_assert(
		test_category<is_floating_point, unsigned long long>(false), "");

	static_assert(test_category<is_floating_point, float>(true), "");
	static_assert(test_category<is_floating_point, double>(true), "");
	static_assert(test_category<is_floating_point, long double>(true), "");
	static_assert(test_category<is_floating_point, __float128>(true), "");
	static_assert(test_category<is_floating_point, __int128>(false), "");
	static_assert(
		test_category<is_floating_point, unsigned __int128>(false), "");
	static_assert(test_category<is_floating_point, ClassType>(false), "");
}

int
main()
{
	test01();
}
