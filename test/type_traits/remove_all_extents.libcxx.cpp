//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// remove_all_extents

#include "rider/faiz/type_traits.hpp"
#include <catch2/catch.hpp>


// #include "../test_macros.h"
#include "test-utilities.hpp"

namespace
{
	enum Enum
	{
		zero,
		one_
	};

	template<class T, class U>
	void
	test_remove_all_extents()
	{
		STATIC_REQUIRE(Rider::Faiz::is_same<
			typename Rider::Faiz::remove_all_extents<T>::type,
			U>::value);

		STATIC_REQUIRE(
			Rider::Faiz::is_same<Rider::Faiz::remove_all_extents_t<T>,
				U>::value);
	}

} // namespace
TEST_CASE("remove_all_extents.libcxx: ")
{
	test_remove_all_extents<int, int>();
	test_remove_all_extents<const Enum, const Enum>();
	test_remove_all_extents<int[], int>();
	test_remove_all_extents<const int[], const int>();
	test_remove_all_extents<int[3], int>();
	test_remove_all_extents<const int[3], const int>();
	test_remove_all_extents<int[][3], int>();
	test_remove_all_extents<const int[][3], const int>();
	test_remove_all_extents<int[2][3], int>();
	test_remove_all_extents<const int[2][3], const int>();
	test_remove_all_extents<int[1][2][3], int>();
	test_remove_all_extents<const int[1][2][3], const int>();
}
