//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_union

// #include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>
#include <cstddef> // for std::nullptr_t

namespace
{

	template<class T>
	void
	test_is_union()
	{
		STATIC_REQUIRE(Rider::Faiz::is_union<T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_union<const T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_union<volatile T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_union<const volatile T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_union_v<T>);
		STATIC_REQUIRE(Rider::Faiz::is_union_v<const T>);
		STATIC_REQUIRE(Rider::Faiz::is_union_v<volatile T>);
		STATIC_REQUIRE(Rider::Faiz::is_union_v<const volatile T>);
	}

	template<class T>
	void
	test_is_not_union()
	{
		STATIC_REQUIRE(!Rider::Faiz::is_union<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_union<const T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_union<volatile T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_union<const volatile T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_union_v<T>);
		STATIC_REQUIRE(!Rider::Faiz::is_union_v<const T>);
		STATIC_REQUIRE(!Rider::Faiz::is_union_v<volatile T>);
		STATIC_REQUIRE(!Rider::Faiz::is_union_v<const volatile T>);
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
TEST_CASE("is_union.libcxx: ")
{
	test_is_union<Union>();

	test_is_not_union<Rider::Faiz::nullptr_t>();
	test_is_not_union<void>();
	test_is_not_union<int>();
	test_is_not_union<int&>();
	test_is_not_union<int&&>();
	test_is_not_union<int*>();
	test_is_not_union<double>();
	test_is_not_union<const int*>();
	test_is_not_union<char[3]>();
	test_is_not_union<char[]>();
	test_is_not_union<Enum>();
	test_is_not_union<FunctionPtr>();
	test_is_not_union<Empty>();
	test_is_not_union<bit_zero>();
	test_is_not_union<NotEmpty>();
	test_is_not_union<Abstract>();
	test_is_not_union<incomplete_type>();
}
