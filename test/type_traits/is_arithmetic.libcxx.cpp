//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_arithmetic

// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"
#include <complex>
#include <cstddef> // for std::nullptr_t

namespace
{

	template<class T>
	void
	test_is_arithmetic()
	{
		STATIC_REQUIRE(Rider::Faiz::is_arithmetic<T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_arithmetic<const T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_arithmetic<volatile T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_arithmetic<const volatile T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_arithmetic_v<T>);
		STATIC_REQUIRE(Rider::Faiz::is_arithmetic_v<const T>);
		STATIC_REQUIRE(Rider::Faiz::is_arithmetic_v<volatile T>);
		STATIC_REQUIRE(Rider::Faiz::is_arithmetic_v<const volatile T>);
	}

	template<class T>
	void
	test_is_not_arithmetic()
	{
		STATIC_REQUIRE(!Rider::Faiz::is_arithmetic<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_arithmetic<const T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_arithmetic<volatile T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_arithmetic<const volatile T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_arithmetic_v<T>);
		STATIC_REQUIRE(!Rider::Faiz::is_arithmetic_v<const T>);
		STATIC_REQUIRE(!Rider::Faiz::is_arithmetic_v<volatile T>);
		STATIC_REQUIRE(!Rider::Faiz::is_arithmetic_v<const volatile T>);
	}

	class incomplete_type;

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

	typedef void (*FunctionPtr)();


} // namespace
TEST_CASE("is_arithmetic.libcxx: <Union>")
{
	test_is_not_arithmetic<Union>();
}
TEST_CASE("is_arithmetic.libcxx: <NotEmpty>")
{
	test_is_not_arithmetic<NotEmpty>();
}
TEST_CASE("is_arithmetic.libcxx: <Enum>")
{
	test_is_not_arithmetic<Enum>();
}
TEST_CASE("is_arithmetic.libcxx: <char[]>")
{
	test_is_not_arithmetic<char[]>();
}
TEST_CASE("is_arithmetic.libcxx: <unsigned char>")
{
	test_is_arithmetic<unsigned char>();
}
TEST_CASE("is_arithmetic.libcxx: <std::complex<double>>")
{
	test_is_not_arithmetic<std::complex<double>>();
}
TEST_CASE("is_arithmetic.libcxx: <char[3]>")
{
	test_is_not_arithmetic<char[3]>();
}
TEST_CASE("is_arithmetic.libcxx: <double>")
{
	test_is_arithmetic<double>();
}
TEST_CASE("is_arithmetic.libcxx: <wchar_t>")
{
	test_is_arithmetic<wchar_t>();
}


TEST_CASE("is_arithmetic.libcxx: <bool>")
{
	test_is_arithmetic<bool>();
}
TEST_CASE("is_arithmetic.libcxx: <unsigned long>")
{
	test_is_arithmetic<unsigned long>();
}
TEST_CASE("is_arithmetic.libcxx: <const int*>")
{
	test_is_not_arithmetic<const int*>();
}
TEST_CASE("is_arithmetic.libcxx: <int*>")
{
	test_is_not_arithmetic<int*>();
}
TEST_CASE("is_arithmetic.libcxx: <int&&>")
{
	test_is_not_arithmetic<int&&>();
}
TEST_CASE("is_arithmetic.libcxx: <int>")
{
	test_is_arithmetic<int>();
}
TEST_CASE("is_arithmetic.libcxx: <FunctionPtr>")
{
	test_is_not_arithmetic<FunctionPtr>();
}
TEST_CASE("is_arithmetic.libcxx: <signed char>")
{
	test_is_arithmetic<signed char>();
}
TEST_CASE("is_arithmetic.libcxx: <char>")
{
	test_is_arithmetic<char>();
}
TEST_CASE("is_arithmetic.libcxx: <long>")
{
	test_is_arithmetic<long>();
}
TEST_CASE("is_arithmetic.libcxx: <unsigned int>")
{
	test_is_arithmetic<unsigned int>();
}


TEST_CASE("is_arithmetic.libcxx: <unsigned short>")
{
	test_is_arithmetic<unsigned short>();
}
TEST_CASE("is_arithmetic.libcxx: <int&>")
{
	test_is_not_arithmetic<int&>();
}
TEST_CASE("is_arithmetic.libcxx: <void>")
{
	test_is_not_arithmetic<void>();
}
TEST_CASE("is_arithmetic.libcxx: <Rider::Faiz::nullptr_t>")
{
	test_is_not_arithmetic<Rider::Faiz::nullptr_t>();
}
TEST_CASE("is_arithmetic.libcxx: <bit_zero>")
{
	test_is_not_arithmetic<bit_zero>();
}
TEST_CASE("is_arithmetic.libcxx: <incomplete_type>")
{
	test_is_not_arithmetic<incomplete_type>();
}
TEST_CASE("is_arithmetic.libcxx: <Empty>")
{
	test_is_not_arithmetic<Empty>();
}
TEST_CASE("is_arithmetic.libcxx: <short>")
{
	test_is_arithmetic<short>();
}

TEST_CASE("is_arithmetic.libcxx: <Abstract>")
{
	test_is_not_arithmetic<Abstract>();
}
