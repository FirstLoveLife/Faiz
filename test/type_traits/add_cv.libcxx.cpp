//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// add_cv

#include "rider/faiz/type_traits.hpp"
#include <catch2/catch.hpp>


// #include "../test_macros.h"
#include "test-utilities.hpp"

namespace
{
	template<class T, class U>
	void
	test_add_cv_imp()
	{
		STATIC_REQUIRE(
			Rider::Faiz::is_same<typename Rider::Faiz::add_cv<T>::type,
				const volatile U>::value);
		STATIC_REQUIRE(
			Rider::Faiz::is_same<Rider::Faiz::add_cv_t<T>, U>::value);
	}

	template<class T>
	void
	test_add_cv()
	{
		test_add_cv_imp<T, const volatile T>();
		test_add_cv_imp<const T, const volatile T>();
		test_add_cv_imp<volatile T, volatile const T>();
		test_add_cv_imp<const volatile T, const volatile T>();
	}
} // namespace
TEST_CASE("add_cv: int")
{
	test_add_cv<int>();
}
TEST_CASE("add_cv: int[3]")
{
	test_add_cv<int[3]>();
}
TEST_CASE("add_cv: int&")
{
	test_add_cv<int&>();
}
TEST_CASE("add_cv: const int*")
{
	test_add_cv<const int*>();
}
TEST_CASE("add_cv: const int&")
{
	test_add_cv<const int&>();
}
TEST_CASE("add_cv: int*")
{
	test_add_cv<int*>();
}
TEST_CASE("add_cv: void")
{
	test_add_cv<void>();
}
