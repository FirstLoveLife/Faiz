#include "rider/faiz/container/array.hpp"
#include <array>
// #include <experimental/array>

#include <catch2/catch.hpp>
namespace f
{
	Rider::Faiz::array a1{1, 2, 3, 4};
	auto a2 = Rider::Faiz::make_array(1, 2, 3, 4);
	Rider::Faiz::array a3{1, 2, 3, 4, 5};
	Rider::Faiz::array a4{2, 2, 3, 4, 5};
	Rider::Faiz::array a5{3, 2, 3, 4};
	Rider::Faiz::array a6{7};
} // namespace f
namespace s
{
	std::array a1{1, 2, 3, 4};
	std::array a2 {1, 2, 3, 4};
	std::array a3{1, 2, 3, 4, 5};
	std::array a4{2, 2, 3, 4, 5};
	std::array a5{3, 2, 3, 4};
	std::array a6{7};
} // namespace s
TEST_CASE("compare functions")
{

	REQUIRE((f::a1 == f::a2) == (s::a1 == s::a2));
	REQUIRE((f::a2 != f::a5) == (s::a2 != s::a5));
	REQUIRE((f::a2 > f::a5) == (s::a2 > s::a5));
	REQUIRE((f::a2 < f::a5) == (s::a2 < s::a5));
	REQUIRE((f::a2 <= f::a5) == (s::a2 <= s::a5));
	REQUIRE((f::a2 >= f::a5) == (s::a2 >= s::a5));
	REQUIRE((f::a3 >= f::a4) == (s::a3 >= s::a4));
}
