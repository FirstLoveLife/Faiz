//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// extent

#include "rider/faiz/type_traits.hpp"

#include "../test_macros.h"

template<class T, unsigned A>
void
test_extent()
{
	static_assert((rider::faiz::extent<T>::value == A), "");
	static_assert((rider::faiz::extent<const T>::value == A), "");
	static_assert((rider::faiz::extent<volatile T>::value == A), "");
	static_assert((rider::faiz::extent<const volatile T>::value == A), "");
	static_assert((rider::faiz::extent_v<T> == A), "");
	static_assert((rider::faiz::extent_v<const T> == A), "");
	static_assert((rider::faiz::extent_v<volatile T> == A), "");
	static_assert((rider::faiz::extent_v<const volatile T> == A), "");
}

template<class T, unsigned A>
void
test_extent1()
{
	static_assert((rider::faiz::extent<T, 1>::value == A), "");
	static_assert((rider::faiz::extent<const T, 1>::value == A), "");
	static_assert((rider::faiz::extent<volatile T, 1>::value == A), "");
	static_assert((rider::faiz::extent<const volatile T, 1>::value == A), "");
	static_assert((rider::faiz::extent_v<T, 1> == A), "");
	static_assert((rider::faiz::extent_v<const T, 1> == A), "");
	static_assert((rider::faiz::extent_v<volatile T, 1> == A), "");
	static_assert((rider::faiz::extent_v<const volatile T, 1> == A), "");
}

class Class
{
public:
	~Class();
};

int
main()
{
	test_extent<void, 0>();
	test_extent<int&, 0>();
	test_extent<Class, 0>();
	test_extent<int*, 0>();
	test_extent<const int*, 0>();
	test_extent<int, 0>();
	test_extent<double, 0>();
	test_extent<bool, 0>();
	test_extent<unsigned, 0>();

	test_extent<int[2], 2>();
	test_extent<int[2][4], 2>();
	test_extent<int[][4], 0>();

	test_extent1<int, 0>();
	test_extent1<int[2], 0>();
	test_extent1<int[2][4], 4>();
	test_extent1<int[][4], 4>();
}
