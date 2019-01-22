#include "rider/faiz/type_traits.hpp"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>


TEST_CASE("add_rvalue_reference libstdcxx")
{
	using Rider::Faiz::add_rvalue_reference;
	using std::is_same;

	STATIC_REQUIRE(is_same<add_rvalue_reference<int>::type, int&&>::value);
	STATIC_REQUIRE(is_same<add_rvalue_reference<int&&>::type, int&&>::value);
	STATIC_REQUIRE(is_same<add_rvalue_reference<int&>::type, int&>::value);
	STATIC_REQUIRE(
		is_same<add_rvalue_reference<const int>::type, const int&&>::value);
	STATIC_REQUIRE(is_same<add_rvalue_reference<int*>::type, int*&&>::value);
	STATIC_REQUIRE(
		is_same<add_rvalue_reference<ClassType&&>::type, ClassType&&>::value);

	STATIC_REQUIRE(
		is_same<add_rvalue_reference<ClassType>::type, ClassType&&>::value);
	STATIC_REQUIRE(
		is_same<add_rvalue_reference<int(int)>::type, int(&&)(int)>::value);
	STATIC_REQUIRE(is_same<add_rvalue_reference<void>::type, void>::value);
	STATIC_REQUIRE(
		is_same<add_rvalue_reference<const void>::type, const void>::value);
	STATIC_REQUIRE(is_same<add_rvalue_reference<bool(int) const>::type,
		bool(int) const>::value);

	STATIC_REQUIRE(
		is_same<add_rvalue_reference<bool(int)&>::type, bool(int)&>::value);
	STATIC_REQUIRE(is_same<add_rvalue_reference<bool(int) const&&>::type,
		bool(int) const&&>::value);

	STATIC_REQUIRE(
		is_same<add_rvalue_reference<bool(int)>::type, bool(&&)(int)>::value);
}
