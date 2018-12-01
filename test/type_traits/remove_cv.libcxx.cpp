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

#include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"

template<class T, class U>
void
test_remove_cv_imp()
{
	static_assert(
		(rider::faiz::is_same<typename rider::faiz::remove_cv<T>::type,
			U>::value),
		"");
	static_assert(
		(rider::faiz::is_same<rider::faiz::remove_cv_t<T>, U>::value), "");
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

int
main()
{
	test_remove_cv<void>();
	test_remove_cv<int>();
	test_remove_cv<int[3]>();
	test_remove_cv<int&>();
	test_remove_cv<const int&>();
	test_remove_cv<int*>();
	test_remove_cv<const int*>();
}
