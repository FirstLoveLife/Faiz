//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_signed

// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"
#include <complex>

namespace
{
	template<class T>
	void
	test_is_signed()
	{
		STATIC_REQUIRE(Rider::Faiz::is_signed<T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_signed<const T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_signed<volatile T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_signed<const volatile T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_signed_v<T>);
		STATIC_REQUIRE(Rider::Faiz::is_signed_v<const T>);
		STATIC_REQUIRE(Rider::Faiz::is_signed_v<volatile T>);
		STATIC_REQUIRE(Rider::Faiz::is_signed_v<const volatile T>);
	}

	template<class T>
	void
	test_is_not_signed()
	{
		STATIC_REQUIRE(!Rider::Faiz::is_signed<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_signed<const T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_signed<volatile T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_signed<const volatile T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_signed_v<T>);
		STATIC_REQUIRE(!Rider::Faiz::is_signed_v<const T>);
		STATIC_REQUIRE(!Rider::Faiz::is_signed_v<volatile T>);
		STATIC_REQUIRE(!Rider::Faiz::is_signed_v<const volatile T>);
	}

	template<class T>
	void
	test_is_not_signed_v()
	{
		STATIC_REQUIRE(Rider::Faiz::not_signed_v<T>);
		STATIC_REQUIRE(Rider::Faiz::not_signed_v<const T>);
		STATIC_REQUIRE(Rider::Faiz::not_signed_v<volatile T>);
		STATIC_REQUIRE(Rider::Faiz::not_signed_v<const volatile T>);
		STATIC_REQUIRE(Rider::Faiz::not_signed_v<T>);
		STATIC_REQUIRE(Rider::Faiz::not_signed_v<const T>);
		STATIC_REQUIRE(Rider::Faiz::not_signed_v<volatile T>);
		STATIC_REQUIRE(Rider::Faiz::not_signed_v<const volatile T>);
	}


	class Class
	{
	public:
		~Class();
	};

	struct A; // incomplete

} // namespace
TEST_CASE("is_signed: ")
{
	test_is_not_signed<void>();
	test_is_not_signed<int&>();
	test_is_not_signed<Class>();
	test_is_not_signed<int*>();
	test_is_not_signed<const int*>();
	test_is_not_signed<char[3]>();
	test_is_not_signed<char[]>();
	test_is_not_signed<bool>();
	test_is_not_signed<unsigned>();
	test_is_not_signed<A>();
	// test_is_signed<Rider::Faiz::complex<double>>();
	test_is_signed<int>();
	test_is_signed<double>();

	// #ifndef _LIBCPP_HAS_NO_INT128
	// 	test_is_signed<__int128_t>();
	// 	test_is_not_signed<__uint128_t>();
	// #endif
}

TEST_CASE("not_signed_v: ")
{
	test_is_not_signed_v<void>();
	test_is_not_signed_v<int&>();
	test_is_not_signed_v<Class>();
	test_is_not_signed_v<int*>();
	test_is_not_signed_v<const int*>();
	test_is_not_signed_v<char[3]>();
	test_is_not_signed_v<char[]>();
	test_is_not_signed_v<bool>();
	test_is_not_signed_v<unsigned>();
	test_is_not_signed_v<A>();
	// test_is_signed<Rider::Faiz::complex<double>>();
	test_is_signed<int>();
	test_is_signed<double>();

	// #ifndef _LIBCPP_HAS_NO_INT128
	// 	test_is_signed<__int128_t>();
	// 	test_is_not_signed_v<__uint128_t>();
	// #endif
}
