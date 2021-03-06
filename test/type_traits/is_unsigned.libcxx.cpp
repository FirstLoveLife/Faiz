//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_unsigned

// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{

	template<class T>
	void
	test_is_unsigned()
	{
		STATIC_REQUIRE(Rider::Faiz::is_unsigned<T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_unsigned<const T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_unsigned<volatile T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_unsigned<const volatile T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_unsigned_v<T>);
		STATIC_REQUIRE(Rider::Faiz::is_unsigned_v<const T>);
		STATIC_REQUIRE(Rider::Faiz::is_unsigned_v<volatile T>);
		STATIC_REQUIRE(Rider::Faiz::is_unsigned_v<const volatile T>);
	}

	template<class T>
	void
	test_is_not_unsigned()
	{
		STATIC_REQUIRE(!Rider::Faiz::is_unsigned<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_unsigned<const T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_unsigned<volatile T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_unsigned<const volatile T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_unsigned_v<T>);
		STATIC_REQUIRE(!Rider::Faiz::is_unsigned_v<const T>);
		STATIC_REQUIRE(!Rider::Faiz::is_unsigned_v<volatile T>);
		STATIC_REQUIRE(!Rider::Faiz::is_unsigned_v<const volatile T>);
	}

	class Class
	{
	public:
		~Class();
	};

	struct A; // incomplete

} // namespace
TEST_CASE("is_unsigned.libcxx: ")
{
	test_is_not_unsigned<void>();
	test_is_not_unsigned<int&>();
	test_is_not_unsigned<Class>();
	test_is_not_unsigned<int*>();
	test_is_not_unsigned<const int*>();
	test_is_not_unsigned<char[3]>();
	test_is_not_unsigned<char[]>();
	test_is_not_unsigned<int>();
	test_is_not_unsigned<double>();
	test_is_not_unsigned<A>();

	test_is_unsigned<bool>();
	test_is_unsigned<unsigned>();

#ifndef _LIBCPP_HAS_NO_INT128
	test_is_unsigned<__uint128_t>();
	test_is_not_unsigned<__int128_t>();
#endif
}
