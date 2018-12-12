//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_array

#include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"
#include <cstddef> // for std::nullptr_t
template<class T>
void
test_is_array()
{
	static_assert(Rider::Faiz::is_array<T>::value, "");
	static_assert(Rider::Faiz::is_array<const T>::value, "");
	static_assert(Rider::Faiz::is_array<volatile T>::value, "");
	static_assert(Rider::Faiz::is_array<const volatile T>::value, "");
	static_assert(Rider::Faiz::is_array_v<T>, "");
	static_assert(Rider::Faiz::is_array_v<const T>, "");
	static_assert(Rider::Faiz::is_array_v<volatile T>, "");
	static_assert(Rider::Faiz::is_array_v<const volatile T>, "");
}

template<class T>
void
test_is_not_array()
{
	static_assert(!Rider::Faiz::is_array<T>::value, "");
	static_assert(!Rider::Faiz::is_array<const T>::value, "");
	static_assert(!Rider::Faiz::is_array<volatile T>::value, "");
	static_assert(!Rider::Faiz::is_array<const volatile T>::value, "");
	static_assert(!Rider::Faiz::is_array_v<T>, "");
	static_assert(!Rider::Faiz::is_array_v<const T>, "");
	static_assert(!Rider::Faiz::is_array_v<volatile T>, "");
	static_assert(!Rider::Faiz::is_array_v<const volatile T>, "");
}

class Empty
{};

class NotEmpty
{
	virtual ~NotEmpty();
};

union Union
{};

struct bit_zero
{
	int : 0;
};

class Abstract
{
	virtual ~Abstract() = 0;
};

enum Enum
{
	zero,
	one
};
struct incomplete_type;

typedef void (*FunctionPtr)();

int
main()
{
	test_is_array<char[3]>();
	test_is_array<char[]>();
	test_is_array<Union[]>();

	test_is_not_array<Rider::Faiz::nullptr_t>();
	test_is_not_array<void>();
	test_is_not_array<int&>();
	test_is_not_array<int&&>();
	test_is_not_array<int*>();
	test_is_not_array<double>();
	test_is_not_array<const int*>();
	test_is_not_array<Enum>();
	test_is_not_array<Union>();
	test_is_not_array<FunctionPtr>();
	test_is_not_array<Empty>();
	test_is_not_array<bit_zero>();
	test_is_not_array<NotEmpty>();
	test_is_not_array<incomplete_type>(); //  LWG#2582
}
