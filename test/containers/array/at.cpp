#include "rider/faiz/container/array.hpp"
#include <array>
#include <catch2/catch.hpp>

TEMPLATE_TEST_CASE("array.at(pos) should throw exception if out of bounds",
	"[array][out of bounds]",
	int,
	// std::string,
	(std::tuple<int, float>))
{
	Rider::Faiz::array<TestType, 10> a;
	REQUIRE_THROWS(a.at(10));
	REQUIRE_NOTHROW(a.at(9));
}

TEST_CASE("fill value")
{
	Rider::Faiz::array<int, 10> a{1, 2, 3, 4, 5, 6};
	a.fill(-10);
	REQUIRE(
		a == Rider::Faiz::array<int, 10>{-10, -10, -10, -10, -10, -10, -10, -10, -10, -10});
}
