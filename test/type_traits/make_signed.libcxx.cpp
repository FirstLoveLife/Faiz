//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// make_signed

// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{

	enum Enum
	{
		zero,
		one_
	};

	enum BigEnum : unsigned long long // MSVC's ABI doesn't follow the Standard
	{
		bigzero,
		big = 0xFFFFFFFFFFFFFFFFULL
	};

	// enum HugeEnum : __uint128_t
	// {
	// 	hugezero
	// };

	template<class T, class U>
	void
	test_make_signed()
	{
		STATIC_REQUIRE(
			std::is_same<typename Rider::Faiz::make_signed<T>::type, U>::value);

		STATIC_REQUIRE(std::is_same<Rider::Faiz::make_signed_t<T>, U>::value);
	}

} // namespace
TEST_CASE("make_signed.libcxx: ")
{
	test_make_signed<signed char, signed char>();
	test_make_signed<unsigned char, signed char>();
	test_make_signed<char, signed char>();
	test_make_signed<short, signed short>();
	test_make_signed<unsigned short, signed short>();
	test_make_signed<int, signed int>();
	test_make_signed<unsigned int, signed int>();
	test_make_signed<long, signed long>();
	test_make_signed<unsigned long, long>();
	test_make_signed<long long, signed long long>();
	test_make_signed<unsigned long long, signed long long>();
	test_make_signed<wchar_t,
		std::conditional<sizeof(wchar_t) == 4, int, short>::type>();
	test_make_signed<const wchar_t,
		std::conditional<sizeof(wchar_t) == 4, const int, const short>::type>();
	test_make_signed<const Enum,
		std::conditional<sizeof(Enum) == sizeof(int),
			const int,
			const signed char>::type>();
	test_make_signed<BigEnum,
		std::conditional<sizeof(long) == 4, long long, long>::type>();
	// test_make_signed<__int128_t, __int128_t>();
	// test_make_signed<__uint128_t, __int128_t>();
	// test_make_signed<HugeEnum, __int128_t>();
}
