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

#include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"

template<class T, class U>
void
test_remove_pointer()
{
	static_assert(
		(Rider::Faiz::is_same<typename Rider::Faiz::remove_pointer<T>::type,
			U>::value),
		"");
	static_assert(
		(Rider::Faiz::is_same<Rider::Faiz::remove_pointer_t<T>, U>::value), "");
}

int
main()
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
