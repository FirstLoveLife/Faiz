#include "rider/faiz/compare_functor.hpp"
#include <catch2/catch.hpp>

#include <functional>
#include <iostream>

template<typename A, typename B, typename U>
bool
f(A a, B b, U u = U())
{
	return u(a, b);
}


TEST_CASE("test compre functor: less")
{
	REQUIRE(f(5, 20, Rider::Faiz::less<int>()));
	REQUIRE(not f(100, 10, Rider::Faiz::less<int>()));
	REQUIRE(not f(100, 100, Rider::Faiz::less<int>()));
}

TEST_CASE("test compre functor: greater")
{
	REQUIRE(not f(5, 20, Rider::Faiz::greater<int>()));
	REQUIRE(f(100, 10, Rider::Faiz::greater<int>()));
	REQUIRE(not f(100, 100, Rider::Faiz::greater<int>()));
}

TEST_CASE("test compre functor: greater_equal")
{
	REQUIRE(not f(5, 20, Rider::Faiz::greater_equal<int>()));
	REQUIRE(f(100, 10, Rider::Faiz::greater_equal<int>()));
	REQUIRE(f(100, 100, Rider::Faiz::greater_equal<int>()));
}

TEST_CASE("test compre functor: equal_to")
{
	REQUIRE(not f(100, 10, Rider::Faiz::equal_to<int>()));
	REQUIRE(f(100, 100, Rider::Faiz::equal_to<int>()));
}

TEST_CASE("test compre functor: not_equal_to")
{
	REQUIRE(f(100, 10, Rider::Faiz::not_equal_to<int>()));
	REQUIRE(not f(100, 100, Rider::Faiz::not_equal_to<int>()));
}
