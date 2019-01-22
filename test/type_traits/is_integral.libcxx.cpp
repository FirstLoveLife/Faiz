//===----------------------------------------------------------------------===//
/// usr/bin/ld.gold:
/// CMakeFiles/type_traits-testsuite.dir/is_floating_point.libstdcxx.cpp.o:
/// previou
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_integral

#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"
#include <cstddef> // for std::nullptr_t

namespace
{

	namespace
	{

		template<class T>
		void
		test_is_integral()
		{
			STATIC_REQUIRE(Rider::Faiz::is_integral<T>::value);
			STATIC_REQUIRE(Rider::Faiz::is_integral<const T>::value);
			STATIC_REQUIRE(Rider::Faiz::is_integral<volatile T>::value);
			STATIC_REQUIRE(Rider::Faiz::is_integral<const volatile T>::value);
			STATIC_REQUIRE(Rider::Faiz::is_integral_v<T>);
			STATIC_REQUIRE(Rider::Faiz::is_integral_v<const T>);
			STATIC_REQUIRE(Rider::Faiz::is_integral_v<volatile T>);
			STATIC_REQUIRE(Rider::Faiz::is_integral_v<const volatile T>);
		}

		template<class T>
		void
		test_is_not_integral()
		{
			STATIC_REQUIRE(!Rider::Faiz::is_integral<T>::value);
			STATIC_REQUIRE(!Rider::Faiz::is_integral<const T>::value);
			STATIC_REQUIRE(!Rider::Faiz::is_integral<volatile T>::value);
			STATIC_REQUIRE(!Rider::Faiz::is_integral<const volatile T>::value);
			STATIC_REQUIRE(!Rider::Faiz::is_integral_v<T>);
			STATIC_REQUIRE(!Rider::Faiz::is_integral_v<const T>);
			STATIC_REQUIRE(!Rider::Faiz::is_integral_v<volatile T>);
			STATIC_REQUIRE(!Rider::Faiz::is_integral_v<const volatile T>);
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
} // namespace
TEST_CASE("is_integral.libcxx: ")
{
	test_is_integral<short>();
	test_is_integral<unsigned short>();
	test_is_integral<int>();
	test_is_integral<unsigned int>();
	test_is_integral<long>();
	test_is_integral<unsigned long>();
	test_is_integral<bool>();
	test_is_integral<char>();
	test_is_integral<signed char>();
	test_is_integral<unsigned char>();
	test_is_integral<wchar_t>();

	test_is_not_integral<Rider::Faiz::nullptr_t>();
	test_is_not_integral<void>();
	test_is_not_integral<int&>();
	test_is_not_integral<int&&>();
	test_is_not_integral<int*>();
	test_is_not_integral<double>();
	test_is_not_integral<const int*>();
	test_is_not_integral<char[3]>();
	test_is_not_integral<char[]>();
	test_is_not_integral<Union>();
	test_is_not_integral<Enum>();
	test_is_not_integral<FunctionPtr>();
	test_is_not_integral<Empty>();
	test_is_not_integral<bit_zero>();
	test_is_not_integral<NotEmpty>();
	test_is_not_integral<Abstract>();
	test_is_not_integral<incomplete_type>();
}
