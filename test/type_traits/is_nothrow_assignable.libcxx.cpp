//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_nothrow_assignable

#include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"

template<class T, class U>
void
test_is_nothrow_assignable()
{
	static_assert((Rider::Faiz::is_nothrow_assignable<T, U>::value), "");
	static_assert((Rider::Faiz::is_nothrow_assignable_v<T, U>), "");
}

template<class T, class U>
void
test_is_not_nothrow_assignable()
{
	static_assert((!Rider::Faiz::is_nothrow_assignable<T, U>::value), "");
	static_assert((!Rider::Faiz::is_nothrow_assignable_v<T, U>), "");
}

struct A
{};

struct B
{
	void operator=(A);
};

struct C
{
	void
	operator=(C&); // not const
};

int
main()
{
	test_is_nothrow_assignable<int&, int&>();
	test_is_nothrow_assignable<int&, int>();
	test_is_nothrow_assignable<int&, double>();

	test_is_not_nothrow_assignable<int, int&>();
	test_is_not_nothrow_assignable<int, int>();
	test_is_not_nothrow_assignable<B, A>();
	test_is_not_nothrow_assignable<A, B>();
	test_is_not_nothrow_assignable<C, C&>();
}
