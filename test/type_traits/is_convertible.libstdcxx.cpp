// #include "../testsuite_tr1.h"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"
#include "test-utilities.hpp"
using namespace Rider::Faiz;
TEST_CASE(" is_convertible libstdcxx ")
{

	// Positive tests.
	STATIC_REQUIRE(is_function<int(int)>());
	STATIC_REQUIRE(test_relationship<is_convertible, int, int>(true));
	STATIC_REQUIRE(test_relationship<is_convertible, int, const int>(true));
	STATIC_REQUIRE(
		test_relationship<is_convertible, volatile int, const int>(true));
	STATIC_REQUIRE(test_relationship<is_convertible, int, float>(true));
	STATIC_REQUIRE(test_relationship<is_convertible, double, float>(true));
	STATIC_REQUIRE(test_relationship<is_convertible, float, int>(true));
	STATIC_REQUIRE(test_relationship<is_convertible, int*, const int*>(true));
	STATIC_REQUIRE(test_relationship<is_convertible, int*, void*>(true));
	STATIC_REQUIRE(test_relationship<is_convertible, int[4], int*>(true));
	STATIC_REQUIRE(test_relationship<is_convertible, float&, int>(true));
	STATIC_REQUIRE(test_relationship<is_convertible, int, const int&>(true));
	STATIC_REQUIRE(test_relationship<is_convertible, const int&, int>(true));
	STATIC_REQUIRE(test_relationship<is_convertible, float, const int&>(true));
	STATIC_REQUIRE(
		test_relationship<is_convertible, int(int), int (*)(int)>(true));
	STATIC_REQUIRE(
		test_relationship<is_convertible, int (&)(int), int (*)(int)>(true));

	STATIC_REQUIRE(test_relationship<is_convertible, EnumType, int>(true));
	STATIC_REQUIRE(
		test_relationship<is_convertible, ClassType, ClassType>(true));
	STATIC_REQUIRE(
		test_relationship<is_convertible, DerivedType, ClassType>(true));
	STATIC_REQUIRE(
		test_relationship<is_convertible, DerivedType*, ClassType*>(true));
	STATIC_REQUIRE(
		test_relationship<is_convertible, DerivedType&, ClassType&>(true));

	STATIC_REQUIRE(
		test_relationship<is_convertible, const int, const int&>(true));

	STATIC_REQUIRE(test_relationship<is_convertible, void, void>(true));
	STATIC_REQUIRE(test_relationship<is_convertible, const void, void>(true));
	STATIC_REQUIRE(
		test_relationship<is_convertible, void, volatile void>(true));
	STATIC_REQUIRE(
		test_relationship<is_convertible, double&, ExplicitClass>(true));

	STATIC_REQUIRE(
		test_relationship<is_convertible, int(int), int (&)(int)>(true));

	// Negative tests.
	STATIC_REQUIRE(test_relationship<is_convertible, const int*, int*>(false));
	STATIC_REQUIRE(test_relationship<is_convertible, int*, float*>(false));
	STATIC_REQUIRE(
		test_relationship<is_convertible, const int[4], int*>(false));
	STATIC_REQUIRE(test_relationship<is_convertible, int[4], int[4]>(false));
	STATIC_REQUIRE(test_relationship<is_convertible, const int&, int&>(false));
	STATIC_REQUIRE(test_relationship<is_convertible, float&, int&>(false));
	STATIC_REQUIRE(
		test_relationship<is_convertible, float, volatile int&>(false));
	STATIC_REQUIRE(
		test_relationship<is_convertible, int(int), int(int)>(false));
	STATIC_REQUIRE(
		test_relationship<is_convertible, int(int), int (*)(void)>(false));
	STATIC_REQUIRE(
		test_relationship<is_convertible, int (*)(int), int (&)(int)>(false));

	STATIC_REQUIRE(test_relationship<is_convertible, int, EnumType>(false));
	STATIC_REQUIRE(test_relationship<is_convertible, int, ClassType>(false));
	STATIC_REQUIRE(
		test_relationship<is_convertible, ClassType, DerivedType>(false));
	STATIC_REQUIRE(
		test_relationship<is_convertible, ClassType*, DerivedType*>(false));
	STATIC_REQUIRE(
		test_relationship<is_convertible, ClassType&, DerivedType&>(false));

	STATIC_REQUIRE(test_relationship<is_convertible, void, int>(false));
	STATIC_REQUIRE(test_relationship<is_convertible, void, float>(false));
	STATIC_REQUIRE(
		test_relationship<is_convertible, void, int (*)(int)>(false));

	// C++0x
	STATIC_REQUIRE(test_relationship<is_convertible, int, void>(false));
	STATIC_REQUIRE(test_relationship<is_convertible, int[4], void>(false));

	STATIC_REQUIRE(test_relationship<is_convertible, int, int&>(false));
	STATIC_REQUIRE(
		test_relationship<is_convertible, float, volatile float&>(false));
	STATIC_REQUIRE(test_relationship<is_convertible,
		const volatile int,
		const volatile int&>(false));

	STATIC_REQUIRE(
		test_relationship<is_convertible, volatile int, volatile int&>(false));

	STATIC_REQUIRE(
		test_relationship<is_convertible, int&, ExplicitClass>(false));
	STATIC_REQUIRE(
		test_relationship<is_convertible, void*, ExplicitClass>(false));
}
