#include "rider/faiz/math/NumberSystem/binary.hpp"
#include <catch2/catch.hpp>
using Rider::Faiz::operator""_b;
TEST_CASE("operator"
		  "_b")
{
	[[maybe_unused]] int a = 1000_b;
	REQUIRE(a == 8);
	REQUIRE(1001_b == 9);
}
