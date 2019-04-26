#include "rider/faiz/container/array.hpp"
#include <array>
#include <catch2/catch.hpp>
#include <iostream>

TEST_CASE("array: cppreference operator[]")
{
	// // XXX: WTF I forget to put elem in public...
	Rider::Faiz::FixedArray<float, 4> numbers{2, 4, 6, 8};
	Rider::Faiz::FixedArray<float, 5> numbers_;
	numbers[0] = 5, numbers[1] = 4, numbers[2] = 6, numbers[3] = 8;

	numbers[0] = 5;
	REQUIRE(numbers[0] == 5);
	REQUIRE(numbers[1] == 4);
	REQUIRE(numbers[2] == 6);
	REQUIRE(numbers[3] == 8);

	REQUIRE(numbers_[0] == 5);
	REQUIRE(numbers_[1] == 4);
	REQUIRE(numbers_[2] == 6);
	REQUIRE(numbers_[3] == 8);
}
