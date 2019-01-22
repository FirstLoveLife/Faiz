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

// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"
#include <cstddef> // for std::nullptr_t

namespace
{
	template<class T>
	void
	test_is_array()
	{
		STATIC_REQUIRE(Rider::Faiz::is_array<T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_array<const T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_array<volatile T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_array<const volatile T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_array_v<T>);
		STATIC_REQUIRE(Rider::Faiz::is_array_v<const T>);
		STATIC_REQUIRE(Rider::Faiz::is_array_v<volatile T>);
		STATIC_REQUIRE(Rider::Faiz::is_array_v<const volatile T>);
	}

	template<class T>
	void
	test_is_not_array()
	{
		STATIC_REQUIRE(!Rider::Faiz::is_array<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_array<const T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_array<volatile T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_array<const volatile T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_array_v<T>);
		STATIC_REQUIRE(!Rider::Faiz::is_array_v<const T>);
		STATIC_REQUIRE(!Rider::Faiz::is_array_v<volatile T>);
		STATIC_REQUIRE(!Rider::Faiz::is_array_v<const volatile T>);
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

} // namespace
TEST_CASE("is_array: <const int*>")
{
	test_is_not_array<const int*>();
}
TEST_CASE("is_array: <Enum>")
{
	test_is_not_array<Enum>();
}
TEST_CASE("is_array: <double>")
{
	test_is_not_array<double>();
}
TEST_CASE("is_array: <int&>")
{
	test_is_not_array<int&>();
}
TEST_CASE("is_array: <int&&>")
{
	test_is_not_array<int&&>();
}
TEST_CASE("is_array: <Rider::Faiz::nullptr_t>")
{
	test_is_not_array<Rider::Faiz::nullptr_t>();
}
TEST_CASE("is_arr<char[]>ay: ")
{
	test_is_array<char[]>();
}
TEST_CASE("is_arr<Union[]>ay: ")
{
	test_is_array<Union[]>();
}
TEST_CASE("is_array: <int*>")
{
	test_is_not_array<int*>();
}
TEST_CASE("is_array: <void>")
{
	test_is_not_array<void>();
}
TEST_CASE("is_array: <incomplete_type>")
{
	test_is_not_array<incomplete_type>(); //  LWG#2582
}
TEST_CASE("is_array: <NotEmpty>")
{
	test_is_not_array<NotEmpty>();
}
TEST_CASE("is_array: <FunctionPtr>")
{
	test_is_not_array<FunctionPtr>();
}
TEST_CASE("is_array: <Union>")
{
	test_is_not_array<Union>();
}
TEST_CASE("is_array: <Empty>")
{
	test_is_not_array<Empty>();
}
TEST_CASE("is_arr<char[3]>ay: ")
{
	test_is_array<char[3]>();
}

TEST_CASE("is_array: <bit_zero>")
{
	test_is_not_array<bit_zero>();
}
