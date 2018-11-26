//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_class

#include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"
#include <cstddef> // for rider::faiz::nullptr_t
template<class T>
void
test_is_class()
{
	static_assert(rider::faiz::is_class<T>::value, "");
	static_assert(rider::faiz::is_class<const T>::value, "");
	static_assert(rider::faiz::is_class<volatile T>::value, "");
	static_assert(rider::faiz::is_class<const volatile T>::value, "");
	static_assert(rider::faiz::is_class_v<T>, "");
	static_assert(rider::faiz::is_class_v<const T>, "");
	static_assert(rider::faiz::is_class_v<volatile T>, "");
	static_assert(rider::faiz::is_class_v<const volatile T>, "");
}

template<class T>
void
test_is_not_class()
{
	static_assert(!rider::faiz::is_class<T>::value, "");
	static_assert(!rider::faiz::is_class<const T>::value, "");
	static_assert(!rider::faiz::is_class<volatile T>::value, "");
	static_assert(!rider::faiz::is_class<const volatile T>::value, "");
	static_assert(!rider::faiz::is_class_v<T>, "");
	static_assert(!rider::faiz::is_class_v<const T>, "");
	static_assert(!rider::faiz::is_class_v<volatile T>, "");
	static_assert(!rider::faiz::is_class_v<const volatile T>, "");
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
	test_is_class<Empty>();
	test_is_class<bit_zero>();
	test_is_class<NotEmpty>();
	test_is_class<Abstract>();
	test_is_class<incomplete_type>();

	test_is_not_class<rider::faiz::nullptr_t>();
	test_is_not_class<void>();
	test_is_not_class<int>();
	test_is_not_class<int&>();
	test_is_not_class<int&&>();
	test_is_not_class<int*>();
	test_is_not_class<double>();
	test_is_not_class<const int*>();
	test_is_not_class<char[3]>();
	test_is_not_class<char[]>();
	test_is_not_class<Enum>();
	test_is_not_class<Union>();
	test_is_not_class<FunctionPtr>();
}
