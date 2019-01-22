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

#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"
#include <cstddef> // for Rider::Faiz::nullptr_t

namespace
{
	template<class T>
	void
	test_is_class()
	{
		STATIC_REQUIRE(Rider::Faiz::is_class<T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_class<const T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_class<volatile T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_class<const volatile T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_class_v<T>);
		STATIC_REQUIRE(Rider::Faiz::is_class_v<const T>);
		STATIC_REQUIRE(Rider::Faiz::is_class_v<volatile T>);
		STATIC_REQUIRE(Rider::Faiz::is_class_v<const volatile T>);
	}

	template<class T>
	void
	test_is_not_class()
	{
		STATIC_REQUIRE(!Rider::Faiz::is_class<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_class<const T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_class<volatile T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_class<const volatile T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_class_v<T>);
		STATIC_REQUIRE(!Rider::Faiz::is_class_v<const T>);
		STATIC_REQUIRE(!Rider::Faiz::is_class_v<volatile T>);
		STATIC_REQUIRE(!Rider::Faiz::is_class_v<const volatile T>);
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
TEST_CASE("is_class.libcxx: ")
{
	test_is_class<Empty>();
	test_is_class<bit_zero>();
	test_is_class<NotEmpty>();
	test_is_class<Abstract>();
	test_is_class<incomplete_type>();

	test_is_not_class<Rider::Faiz::nullptr_t>();
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
