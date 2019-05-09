#include "rider/faiz/math/NumberSystem/literals.hpp"
#include <catch2/catch.hpp>
using namespace Rider::Faiz::literals;
TEST_CASE("operator _B")
{
	[[maybe_unused]] int a = 1000_B;
	REQUIRE(a == 8);
	REQUIRE(1001_B == 9);
}

// TEST_CASE("operator _H")
// {
	// [[maybe_unused]] int a = 82ABC_H;
// 	REQUIRE(a == 8);
// 	REQUIRE(1001_B == 9);
// }
