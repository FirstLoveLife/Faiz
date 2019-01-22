// { dg-do compile { target c++11 } }

// 2008-05-20  Paolo Carlini  <paolo.carlini@oracle.com>
//
// Copyright (C) 2008-2018 Free Software Foundation, Inc.
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

namespace
{

	void
	test01()
	{
		using Rider::Faiz::is_integral;

		STATIC_REQUIRE(test_category<is_integral, void>(false));

		STATIC_REQUIRE(test_category<is_integral, char>(true));
		STATIC_REQUIRE(test_category<is_integral, signed char>(true));
		STATIC_REQUIRE(test_category<is_integral, unsigned char>(true));
		STATIC_REQUIRE(test_category<is_integral, wchar_t>(true));
		STATIC_REQUIRE(test_category<is_integral, char16_t>(true));
		STATIC_REQUIRE(test_category<is_integral, char32_t>(true));
		STATIC_REQUIRE(test_category<is_integral, short>(true));
		STATIC_REQUIRE(test_category<is_integral, unsigned short>(true));
		STATIC_REQUIRE(test_category<is_integral, int>(true));
		STATIC_REQUIRE(test_category<is_integral, unsigned int>(true));
		STATIC_REQUIRE(test_category<is_integral, long>(true));
		STATIC_REQUIRE(test_category<is_integral, unsigned long>(true));
		STATIC_REQUIRE(test_category<is_integral, long long>(true));
		STATIC_REQUIRE(test_category<is_integral, unsigned long long>(true));

		STATIC_REQUIRE(test_category<is_integral, float>(false));
		STATIC_REQUIRE(test_category<is_integral, double>(false));
		STATIC_REQUIRE(test_category<is_integral, long double>(false));

#ifndef __STRICT_ANSI__
		// GNU Extensions.
#	ifdef _GLIBCXX_USE_INT128
		STATIC_REQUIRE(test_category<is_integral, __int128>(true));
		STATIC_REQUIRE(test_category<is_integral, unsigned __int128>(true));
#	endif

#	ifdef _GLIBCXX_USE_FLOAT128
		STATIC_REQUIRE(test_category<is_integral, __float128>(false));
#	endif
#endif

		// Sanity check.
		STATIC_REQUIRE(test_category<is_integral, ClassType>(false));
	}
} // namespace
TEST_CASE("is_integral.libstdcxx: ")
{
	test01();
}
