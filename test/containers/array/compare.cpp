#include "rider/faiz/container/array.hpp"
#include <array>
// #include <experimental/array>

#include <catch2/catch.hpp>
namespace fix
{
	Rider::Faiz::FixedArray<int, 4> a0{1, 2, 3, 4};
	Rider::Faiz::FixedArray a1{1, 2, 3, 4};
	auto a2 = Rider::Faiz::MakeFixedArray(1, 2, 3, 4);
	Rider::Faiz::FixedArray a3{1, 2, 3, 4, 5};
	Rider::Faiz::FixedArray a4{2, 2, 3, 4, 5};
	Rider::Faiz::FixedArray a5{3, 2, 3, 4};
	Rider::Faiz::FixedArray a6{7};
} // namespace f
namespace s
{
	std::array a1{1, 2, 3, 4};
	std::array a2{1, 2, 3, 4};
	std::array a3{1, 2, 3, 4, 5};
	std::array a4{2, 2, 3, 4, 5};
	std::array a5{3, 2, 3, 4};
	std::array a6{7};
} // namespace s
namespace f
{
	Rider::Faiz::array<int, 4> a0{1, 2, 3, 4};
	Rider::Faiz::array a1{1, 2, 3, 4};
	auto a2 = Rider::Faiz::make_array(1, 2, 3, 4);
	Rider::Faiz::array a3{1, 2, 3, 4, 5};
	Rider::Faiz::array a4{2, 2, 3, 4, 5};
	Rider::Faiz::array a5{3, 2, 3, 4};
	Rider::Faiz::array a6{7};
} // namespace f
TEST_CASE("compare functions for fixed array")
{

	REQUIRE((fix::a1 == fix::a2) == (s::a1 == s::a2));
	REQUIRE((fix::a2 != fix::a5) == (s::a2 != s::a5));
	REQUIRE((fix::a2 > fix::a5) == (s::a2 > s::a5));
	REQUIRE((fix::a2 < fix::a5) == (s::a2 < s::a5));
	REQUIRE((fix::a2 <= fix::a5) == (s::a2 <= s::a5));
	REQUIRE((fix::a2 >= fix::a5) == (s::a2 >= s::a5));
	REQUIRE((fix::a3 >= fix::a4) == (s::a3 >= s::a4));
}

TEST_CASE("compare functions for non-fixed array")
{

	REQUIRE((f::a1 == f::a2) == (s::a1 == s::a2));
	REQUIRE((f::a2 != f::a5) == (s::a2 != s::a5));
	REQUIRE((f::a2 > f::a5) == (s::a2 > s::a5));
	REQUIRE((f::a2 < f::a5) == (s::a2 < s::a5));
	REQUIRE((f::a2 <= f::a5) == (s::a2 <= s::a5));
	REQUIRE((f::a2 >= f::a5) == (s::a2 >= s::a5));
	REQUIRE((f::a3 >= f::a4) == (s::a3 >= s::a4));
}
