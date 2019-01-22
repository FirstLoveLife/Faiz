//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// add_const
// in one cpp file


// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{

	template<class T, class U>
	void
	test_add_const_imp()
	{
		STATIC_REQUIRE(
			Rider::Faiz::is_same<typename Rider::Faiz::add_const<T>::type,
				const U>::value);
		STATIC_REQUIRE(
			Rider::Faiz::is_same<Rider::Faiz::add_const_t<T>, U>::value);
	}

	template<class T>
	void
	test_add_const()
	{
		test_add_const_imp<T, const T>();
		test_add_const_imp<const T, const T>();
		test_add_const_imp<volatile T, volatile const T>();
		test_add_const_imp<const volatile T, const volatile T>();
	}
} // namespace
TEST_CASE("add_const: test void")
{
	test_add_const<void>();
}
TEST_CASE("add_const: test int")
{
	test_add_const<int>();
}
TEST_CASE("add_const: test int[3]")
{
	test_add_const<int[3]>();
}
TEST_CASE("add_const test int&")
{
	test_add_const<int&>();
}
TEST_CASE("add_const: test int&")
{
	test_add_const<const int&>();
}
TEST_CASE("add_const: test int*")
{
	test_add_const<int*>();
}
TEST_CASE("add_const: test const int*")
{
	test_add_const<const int*>();
}
