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

#include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"

template<class T, class U>
void
test_add_const_imp()
{
	static_assert(
		(Rider::Faiz::is_same<typename Rider::Faiz::add_const<T>::type,
			const U>::value),
		"");
	static_assert(
		(Rider::Faiz::is_same<Rider::Faiz::add_const_t<T>, U>::value), "");
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

int
main()
{
	test_add_const<void>();
	test_add_const<int>();
	test_add_const<int[3]>();
	test_add_const<int&>();
	test_add_const<const int&>();
	test_add_const<int*>();
	test_add_const<const int*>();
}
