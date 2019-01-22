//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// remove_cv

// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{

	template<class T, class U>
	void
	test_remove_cv_imp()
	{
		STATIC_REQUIRE(
			Rider::Faiz::is_same<typename Rider::Faiz::remove_cv<T>::type,
				U>::value);

		STATIC_REQUIRE(
			Rider::Faiz::is_same<Rider::Faiz::remove_cv_t<T>, U>::value);
	}

	template<class T>
	void
	test_remove_cv()
	{
		test_remove_cv_imp<T, T>();
		test_remove_cv_imp<const T, T>();
		test_remove_cv_imp<volatile T, T>();
		test_remove_cv_imp<const volatile T, T>();
	}

} // namespace
TEST_CASE("remove_cv.libcxx: ")
{
	test_remove_cv<void>();
	test_remove_cv<int>();
	test_remove_cv<int[3]>();
	test_remove_cv<int&>();
	test_remove_cv<const int&>();
	test_remove_cv<int*>();
	test_remove_cv<const int*>();
}
