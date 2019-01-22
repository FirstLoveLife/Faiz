// { dg-do compile { target c++11 } }

// Copyright (C) 2013-2018 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"


using Rider::Faiz::is_arithmetic;


TEST_CASE(
	"is_arithmetic.libstdcxx: (test_category<is_arithmetic, void>(false))")
{
	STATIC_REQUIRE(test_category<is_arithmetic, void>(false));
}
TEST_CASE("is_arithmetic.libstdcxx: (test_category<is_arithmetic, long "
		  "double>(true))")
{
	STATIC_REQUIRE(test_category<is_arithmetic, long double>(true));
}
TEST_CASE(
	"is_arithmetic.libstdcxx: (test_category<is_arithmetic, double>(true))")
{
	STATIC_REQUIRE(test_category<is_arithmetic, double>(true));
}
TEST_CASE("is_arithmetic.libstdcxx: (test_category<is_arithmetic, unsigned "
		  "long long>(true))")
{
	STATIC_REQUIRE(test_category<is_arithmetic, unsigned long long>(true));
}
TEST_CASE("is_arithmetic.libstdcxx: (test_category<is_arithmetic, char>(true))")
{
	STATIC_REQUIRE(test_category<is_arithmetic, char>(true));
}
TEST_CASE("is_arithmetic.libstdcxx: (test_category<is_arithmetic, "
		  "ClassType>(false))")
{
	STATIC_REQUIRE(test_category<is_arithmetic, ClassType>(false));
}
TEST_CASE("is_arithmetic.libstdcxx: (test_category<is_arithmetic, signed "
		  "char>(true))")
{
	STATIC_REQUIRE(test_category<is_arithmetic, signed char>(true));
}
TEST_CASE("is_arithmetic.libstdcxx: (test_category<is_arithmetic, unsigned "
		  "char>(true))")
{
	STATIC_REQUIRE(test_category<is_arithmetic, unsigned char>(true));
}
TEST_CASE(
	"is_arithmetic.libstdcxx: (test_category<is_arithmetic, short>(true))")
{
	STATIC_REQUIRE(test_category<is_arithmetic, short>(true));
}
TEST_CASE("is_arithmetic.libstdcxx: (test_category<is_arithmetic, long>(true))")
{
	STATIC_REQUIRE(test_category<is_arithmetic, long>(true));
}
TEST_CASE(
	"is_arithmetic.libstdcxx: (test_category<is_arithmetic, float>(true))")
{
	STATIC_REQUIRE(test_category<is_arithmetic, float>(true));
}
TEST_CASE("is_arithmetic.libstdcxx: (test_category<is_arithmetic, unsigned "
		  "int>(true))")
{
	STATIC_REQUIRE(test_category<is_arithmetic, unsigned int>(true));
}
TEST_CASE("is_arithmetic.libstdcxx: ")
{
	STATIC_REQUIRE(test_category<is_arithmetic, long long>(true));
}
TEST_CASE("is_arithmetic.libstdcxx: (test_category<is_arithmetic, unsigned "
		  "short>(true))")
{
	STATIC_REQUIRE(test_category<is_arithmetic, unsigned short>(true));
}
TEST_CASE("is_arithmetic.libstdcxx: (test_category<is_arithmetic, int>(true))")
{
	STATIC_REQUIRE(test_category<is_arithmetic, int>(true));
}
TEST_CASE("is_arithmetic.libstdcxx: (test_category<is_arithmetic, unsigned "
		  "long>(true))")
{
	STATIC_REQUIRE(test_category<is_arithmetic, unsigned long>(true));
}
