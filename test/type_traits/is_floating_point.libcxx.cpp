//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_floating_point

#include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"

template<class T>
void
test_is_floating_point()
{

	static_assert(Rider::Faiz::is_floating_point<T>::value, "");
	static_assert(Rider::Faiz::is_floating_point<const T>::value, "");
	static_assert(Rider::Faiz::is_floating_point<volatile T>::value, "");
	static_assert(Rider::Faiz::is_floating_point<const volatile T>::value, "");
	static_assert(Rider::Faiz::is_floating_point_v<T>, "");
	static_assert(Rider::Faiz::is_floating_point_v<const T>, "");
	static_assert(Rider::Faiz::is_floating_point_v<volatile T>, "");
	static_assert(Rider::Faiz::is_floating_point_v<const volatile T>, "");
}

template<class T>
void
test_is_not_floating_point()
{
	static_assert(!Rider::Faiz::is_floating_point<T>::value, "");
	static_assert(!Rider::Faiz::is_floating_point<const T>::value, "");
	static_assert(!Rider::Faiz::is_floating_point<volatile T>::value, "");
	static_assert(!Rider::Faiz::is_floating_point<const volatile T>::value, "");
	static_assert(!Rider::Faiz::is_floating_point_v<T>, "");
	static_assert(!Rider::Faiz::is_floating_point_v<const T>, "");
	static_assert(!Rider::Faiz::is_floating_point_v<volatile T>, "");
	static_assert(!Rider::Faiz::is_floating_point_v<const volatile T>, "");
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
	test_is_floating_point<float>();
	test_is_floating_point<double>();
	test_is_floating_point<long double>();

	test_is_not_floating_point<short>();
	test_is_not_floating_point<unsigned short>();
	test_is_not_floating_point<int>();
	test_is_not_floating_point<unsigned int>();
	test_is_not_floating_point<long>();
	test_is_not_floating_point<unsigned long>();

	test_is_not_floating_point<Rider::Faiz::nullptr_t>();
	test_is_not_floating_point<void>();
	test_is_not_floating_point<int&>();
	test_is_not_floating_point<int&&>();
	test_is_not_floating_point<int*>();
	test_is_not_floating_point<const int*>();
	test_is_not_floating_point<char[3]>();
	test_is_not_floating_point<char[]>();
	test_is_not_floating_point<Union>();
	test_is_not_floating_point<Empty>();
	test_is_not_floating_point<bit_zero>();
	test_is_not_floating_point<NotEmpty>();
	test_is_not_floating_point<Abstract>();
	test_is_not_floating_point<Enum>();
	test_is_not_floating_point<FunctionPtr>();
	test_is_not_floating_point<incomplete_type>();
}
