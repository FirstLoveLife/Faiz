//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// remove_pointer

// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{

	template<class T, class U>
	void
	test_remove_pointer()
	{
		STATIC_REQUIRE(
			Rider::Faiz::is_same<typename Rider::Faiz::remove_pointer<T>::type,
				U>::value);

		STATIC_REQUIRE(
			Rider::Faiz::is_same<Rider::Faiz::remove_pointer_t<T>, U>::value);
	}

} // namespace
TEST_CASE("remove_pointerlibcxx: ")
{
	test_remove_pointer<void, void>();
	test_remove_pointer<int, int>();
	test_remove_pointer<int[3], int[3]>();
	test_remove_pointer<int*, int>();
	test_remove_pointer<const int*, const int>();
	test_remove_pointer<int**, int*>();
	test_remove_pointer<int** const, int*>();
	test_remove_pointer<int* const*, int* const>();
	test_remove_pointer<const int**, const int*>();

	test_remove_pointer<int&, int&>();
	test_remove_pointer<const int&, const int&>();
	test_remove_pointer<int(&)[3], int(&)[3]>();
	test_remove_pointer<int(*)[3], int[3]>();
	test_remove_pointer<int*&, int*&>();
	test_remove_pointer<const int*&, const int*&>();
}
