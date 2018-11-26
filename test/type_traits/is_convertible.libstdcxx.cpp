#include "../testsuite_tr1.h"
#include "rider/faiz/type_traits.hpp"
using namespace __gnu_test;
using namespace rider::faiz;
void
test01()
{

	// Positive tests.
	static_assert(is_function<int(int)>(), "");
	static_assert(test_relationship<is_convertible, int, int>(true), "");
	static_assert(test_relationship<is_convertible, int, const int>(true), "");
	static_assert(
		test_relationship<is_convertible, volatile int, const int>(true), "");
	static_assert(test_relationship<is_convertible, int, float>(true), "");
	static_assert(test_relationship<is_convertible, double, float>(true), "");
	static_assert(test_relationship<is_convertible, float, int>(true), "");
	static_assert(
		test_relationship<is_convertible, int*, const int*>(true), "");
	static_assert(test_relationship<is_convertible, int*, void*>(true), "");
	static_assert(test_relationship<is_convertible, int[4], int*>(true), "");
	static_assert(test_relationship<is_convertible, float&, int>(true), "");
	static_assert(test_relationship<is_convertible, int, const int&>(true), "");
	static_assert(test_relationship<is_convertible, const int&, int>(true), "");
	static_assert(
		test_relationship<is_convertible, float, const int&>(true), "");
	static_assert(
		test_relationship<is_convertible, int(int), int (*)(int)>(true), "");
	static_assert(
		test_relationship<is_convertible, int (&)(int), int (*)(int)>(true),
		"");
	static_assert(test_relationship<is_convertible, EnumType, int>(true), "");
	static_assert(
		test_relationship<is_convertible, ClassType, ClassType>(true), "");
	static_assert(
		test_relationship<is_convertible, DerivedType, ClassType>(true), "");
	static_assert(
		test_relationship<is_convertible, DerivedType*, ClassType*>(true), "");
	static_assert(
		test_relationship<is_convertible, DerivedType&, ClassType&>(true), "");

	static_assert(
		test_relationship<is_convertible, const int, const int&>(true), "");

	static_assert(test_relationship<is_convertible, void, void>(true), "");
	static_assert(
		test_relationship<is_convertible, const void, void>(true), "");
	static_assert(
		test_relationship<is_convertible, void, volatile void>(true), "");
	static_assert(
		test_relationship<is_convertible, double&, ExplicitClass>(true), "");

	static_assert(
		test_relationship<is_convertible, int(int), int (&)(int)>(true), "");

	// Negative tests.
	static_assert(
		test_relationship<is_convertible, const int*, int*>(false), "");
	static_assert(test_relationship<is_convertible, int*, float*>(false), "");
	static_assert(
		test_relationship<is_convertible, const int[4], int*>(false), "");
	static_assert(test_relationship<is_convertible, int[4], int[4]>(false), "");
	static_assert(
		test_relationship<is_convertible, const int&, int&>(false), "");
	static_assert(test_relationship<is_convertible, float&, int&>(false), "");
	static_assert(
		test_relationship<is_convertible, float, volatile int&>(false), "");
	static_assert(
		test_relationship<is_convertible, int(int), int(int)>(false), "");
	static_assert(
		test_relationship<is_convertible, int(int), int (*)(void)>(false), "");
	static_assert(
		test_relationship<is_convertible, int (*)(int), int (&)(int)>(false),
		"");
	static_assert(test_relationship<is_convertible, int, EnumType>(false), "");
	static_assert(test_relationship<is_convertible, int, ClassType>(false), "");
	static_assert(
		test_relationship<is_convertible, ClassType, DerivedType>(false), "");
	static_assert(
		test_relationship<is_convertible, ClassType*, DerivedType*>(false), "");
	static_assert(
		test_relationship<is_convertible, ClassType&, DerivedType&>(false), "");

	static_assert(test_relationship<is_convertible, void, int>(false), "");
	static_assert(test_relationship<is_convertible, void, float>(false), "");
	static_assert(
		test_relationship<is_convertible, void, int (*)(int)>(false), "");

	// C++0x
	static_assert(test_relationship<is_convertible, int, void>(false), "");
	static_assert(test_relationship<is_convertible, int[4], void>(false), "");

	static_assert(test_relationship<is_convertible, int, int&>(false), "");
	static_assert(
		test_relationship<is_convertible, float, volatile float&>(false), "");
	static_assert(test_relationship<is_convertible,
					  const volatile int,
					  const volatile int&>(false),
		"");
	static_assert(
		test_relationship<is_convertible, volatile int, volatile int&>(false),
		"");
	static_assert(
		test_relationship<is_convertible, int&, ExplicitClass>(false), "");
	static_assert(
		test_relationship<is_convertible, void*, ExplicitClass>(false), "");
}

int
main()
{
	test01();
}
