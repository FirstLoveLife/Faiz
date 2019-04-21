#include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"
#include <catch2/catch.hpp>
#include <type_traits>
using Rider::Faiz::are_nothrow_constructible;
using Rider::Faiz::are_nothrow_constructible_v;

class Empty
{};

class NotEmpty
{
	virtual ~NotEmpty();
};

union Union
{};

struct bit_zero
{
	int : 0;
};

class Abstract
{
	virtual ~Abstract() = 0;
};

struct A
{
	A(const A&);
};

struct C
{
	C(C&); // not const
	void
	operator=(C&); // not const
};

struct Tuple
{
	Tuple(Empty&&) noexcept
	{}
};


TEST_CASE("are_nothrow_constructible: ")
{
	REQUIRE(are_nothrow_constructible_v<Rider::Faiz::Pack<int>>);
	REQUIRE(are_nothrow_constructible_v<Rider::Faiz::Pack<int, const int&>>);
	REQUIRE(are_nothrow_constructible_v<Rider::Faiz::Pack<Empty>>);
	REQUIRE(
		are_nothrow_constructible_v<Rider::Faiz::Pack<Empty, const Empty&>>);

	REQUIRE_FALSE(are_nothrow_constructible_v<Rider::Faiz::Pack<A, int>>);
	REQUIRE_FALSE(
		are_nothrow_constructible_v<Rider::Faiz::Pack<A, int, double>>);
	REQUIRE_FALSE(are_nothrow_constructible_v<Rider::Faiz::Pack<A>>);
	REQUIRE_FALSE(are_nothrow_constructible_v<Rider::Faiz::Pack<A>>);
	REQUIRE_FALSE(are_nothrow_constructible_v<Rider::Faiz::Pack<C>>);

	STATIC_REQUIRE(
		are_nothrow_constructible_v<Rider::Faiz::Pack<Tuple&&, Empty>>);

	STATIC_REQUIRE(
		are_nothrow_constructible_v<Rider::Faiz::Pack<Tuple&&, Empty>, Rider::Faiz::Pack<Empty, const Empty&>>);

	STATIC_REQUIRE_FALSE(
		are_nothrow_constructible<Rider::Faiz::Pack<Tuple&, Empty>>::value);
	STATIC_REQUIRE_FALSE(
		are_nothrow_constructible_v<Rider::Faiz::Pack<Tuple&, Empty>>);

	STATIC_REQUIRE_FALSE(
		are_nothrow_constructible<Rider::Faiz::Pack<Tuple&&, Empty>, Rider::Faiz::Pack<Empty, const Empty&>, Rider::Faiz::Pack<Tuple&, Empty>>::value);
}
