#include "rider/faiz/algorithm.hpp"
#include "rider/faiz/container/array.hpp"
#include <algorithm>
#include <catch2/catch.hpp>

auto cmp = [](auto x, auto y) { return x + 1 == y; };

TEST_CASE("lexicographical_compare, array", "[lexicographical][array]", )
{
	Rider::Faiz::array a1{ 1, 2, 3, 4};
	auto a2 = Rider::Faiz::make_array(1, 2, 3, 4);
	Rider::Faiz::array a3{1, 2, 3, 4, 5};
	Rider::Faiz::array a4{2, 2, 3, 4, 5};
	Rider::Faiz::array a5{3, 2, 3, 4};
	Rider::Faiz::array a6{7};
	REQUIRE(not Rider::Faiz::lexicographical_compare(
		a1.begin(), a1.end(), a2.begin(), a2.end()));
	REQUIRE(Rider::Faiz::lexicographical_compare(
		a1.begin(), a1.end(), a3.begin(), a3.end()));
	REQUIRE(Rider::Faiz::lexicographical_compare(
		a1.begin(), a1.end(), a4.begin(), a4.end()));
	REQUIRE(Rider::Faiz::lexicographical_compare(
		a1.begin(), a1.end(), a5.begin(), a5.end()));
	REQUIRE(Rider::Faiz::lexicographical_compare(
		a3.begin(), a3.end(), a4.begin(), a4.end()));
	REQUIRE(Rider::Faiz::lexicographical_compare(
		a3.begin(), a3.end(), a5.begin(), a5.end()));
	REQUIRE(Rider::Faiz::lexicographical_compare(
		a3.begin(), a3.end(), a6.begin(), a6.end()));
}

TEST_CASE("equal", "[equal][array]", )
{
	Rider::Faiz::array a1{1, 2, 3, 4};
	auto a2 = Rider::Faiz::make_array(1, 2, 3, 4);
	Rider::Faiz::array a3{1, 2, 3, 4, 5};
	Rider::Faiz::array a4{2, 2, 3, 4, 5};
	Rider::Faiz::array a5{3, 2, 3, 4};
	Rider::Faiz::array a6{7};
	Rider::Faiz::array a7{2, 3, 4, 5};
	Rider::Faiz::array a8{2, 3, 4, 5, 6};
	REQUIRE(Rider::Faiz::equal(a1.begin(), a1.end(), a2.begin(), a2.end()));
	REQUIRE(
		Rider::Faiz::equal(a1.begin(), a1.end(), a7.begin(), a7.end(), cmp));
	REQUIRE(not Rider::Faiz::equal(
		a1.begin(), a1.end(), a8.begin(), a8.end(), cmp));
	REQUIRE(not Rider::Faiz::equal(a1.begin(), a1.end(), a3.begin(), a3.end()));
	REQUIRE(not Rider::Faiz::equal(a1.begin(), a1.end(), a4.begin(), a4.end()));
	REQUIRE(not Rider::Faiz::equal(a1.begin(), a1.end(), a5.begin(), a5.end()));
	REQUIRE(not Rider::Faiz::equal(a3.begin(), a3.end(), a4.begin(), a4.end()));
	REQUIRE(not Rider::Faiz::equal(a3.begin(), a3.end(), a5.begin(), a5.end()));
	REQUIRE(not Rider::Faiz::equal(a3.begin(), a3.end(), a6.begin(), a6.end()));

	REQUIRE(Rider::Faiz::equal(a1.begin(), a1.end(), a2.begin()));
	REQUIRE(Rider::Faiz::equal(a1.begin(), a1.end(), a7.begin(), cmp));
	REQUIRE(Rider::Faiz::equal(a1.begin(), a1.end(), a8.begin(), cmp));
	REQUIRE(Rider::Faiz::equal(a1.begin(), a1.end(), a3.begin()));
	REQUIRE(not Rider::Faiz::equal(a1.begin(), a1.end(), a4.begin()));
	REQUIRE(not Rider::Faiz::equal(a1.begin(), a1.end(), a5.begin()));
	REQUIRE(not Rider::Faiz::equal(a3.begin(), a3.end(), a4.begin()));
	REQUIRE(not Rider::Faiz::equal(a3.begin(), a3.end(), a5.begin()));
	REQUIRE(not Rider::Faiz::equal(a3.begin(), a3.end(), a6.begin()));

	REQUIRE(std::equal(a1.begin(), a1.end(), a2.begin(), a2.end()));
	REQUIRE(std::equal(a1.begin(), a1.end(), a7.begin(), a7.end(), cmp));
	REQUIRE(not std::equal(a1.begin(), a1.end(), a8.begin(), a8.end(), cmp));
	REQUIRE(not std::equal(a1.begin(), a1.end(), a3.begin(), a3.end()));
	REQUIRE(not std::equal(a1.begin(), a1.end(), a4.begin(), a4.end()));
	REQUIRE(not std::equal(a1.begin(), a1.end(), a5.begin(), a5.end()));
	REQUIRE(not std::equal(a3.begin(), a3.end(), a4.begin(), a4.end()));
	REQUIRE(not std::equal(a3.begin(), a3.end(), a5.begin(), a5.end()));
	REQUIRE(not std::equal(a3.begin(), a3.end(), a6.begin(), a6.end()));

	REQUIRE(std::equal(a1.begin(), a1.end(), a2.begin()));
	REQUIRE(std::equal(a1.begin(), a1.end(), a7.begin(), cmp));
	REQUIRE(std::equal(a1.begin(), a1.end(), a8.begin(), cmp));
	REQUIRE(std::equal(a1.begin(), a1.end(), a3.begin()));
	REQUIRE(not std::equal(a1.begin(), a1.end(), a4.begin()));
	REQUIRE(not std::equal(a1.begin(), a1.end(), a5.begin()));
	REQUIRE(not std::equal(a3.begin(), a3.end(), a4.begin()));
	REQUIRE(not std::equal(a3.begin(), a3.end(), a5.begin()));
	REQUIRE(not std::equal(a3.begin(), a3.end(), a6.begin()));
}
