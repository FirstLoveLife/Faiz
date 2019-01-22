//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// conditional
#include "../test_macros.h"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

TEST_CASE("conditional libstdcxx")
{
	STATIC_REQUIRE(
		Rider::Faiz::is_same<Rider::Faiz::conditional<true, char, int>::type,
			char>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_same<Rider::Faiz::conditional<false, char, int>::type,
			int>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_same<Rider::Faiz::conditional_t<true, char, int>,
			char>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_same<Rider::Faiz::conditional_t<false, char, int>,
			int>::value);
}
