//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// decay


#include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"

template<class T, class U>
void
test_decay()
{
	static_assert(
		(Rider::Faiz::is_same<typename Rider::Faiz::decay<T>::type, U>::value),
		"");
	static_assert(
		(Rider::Faiz::is_same<Rider::Faiz::decay_t<T>, U>::value), "");
}

int
main()
{
	test_decay<void, void>();
	test_decay<int, int>();
	test_decay<const volatile int, int>();
	test_decay<int*, int*>();
	test_decay<int[3], int*>();
	test_decay<const int[3], const int*>();
	test_decay<void(), void (*)()>();
	test_decay<int(int) const, int(int) const>();
	test_decay<int(int) volatile, int(int) volatile>();
	test_decay<int(int)&, int(int)&>();
	test_decay<int(int)&&, int(int) &&>();
}
