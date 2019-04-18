#include "rider/faiz/container/array.hpp"
#include <array>
#include <catch2/catch.hpp>
#include <iostream>

TEST_CASE("array: cppreference operator[]")
{
	// // XXX: WTF I forget to put elem in public...
	Rider::Faiz::array<float, 4> numbers{2, 4, 6, 8};

	// std::cout << "Second element: " << numbers[1] << '\n';

	numbers[0] = 5;
	REQUIRE(numbers[0] == 5);
	REQUIRE(numbers[1] == 4);
	REQUIRE(numbers[2] == 6);
	REQUIRE(numbers[3] == 8);
}
