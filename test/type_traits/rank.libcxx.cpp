//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// rank

#include "rider/faiz/type_traits.hpp"

#include "../test_macros.h"

template<class T, unsigned A>
void
test_rank()
{
	static_assert(Rider::Faiz::rank<T>::value == A, "");
	static_assert(Rider::Faiz::rank<const T>::value == A, "");
	static_assert(Rider::Faiz::rank<volatile T>::value == A, "");
	static_assert(Rider::Faiz::rank<const volatile T>::value == A, "");
	static_assert(Rider::Faiz::rank_v<T> == A, "");
	static_assert(Rider::Faiz::rank_v<const T> == A, "");
	static_assert(Rider::Faiz::rank_v<volatile T> == A, "");
	static_assert(Rider::Faiz::rank_v<const volatile T> == A, "");
}

class Class
{
public:
	~Class();
};

int
main()
{
	test_rank<void, 0>();
	test_rank<int&, 0>();
	test_rank<Class, 0>();
	test_rank<int*, 0>();
	test_rank<const int*, 0>();
	test_rank<int, 0>();
	test_rank<double, 0>();
	test_rank<bool, 0>();
	test_rank<unsigned, 0>();

	test_rank<char[3], 1>();
	test_rank<char[][3], 2>();
	test_rank<char[][4][3], 3>();
}
