// { dg-do compile { target c++11 } }

// 2005-01-24  Paolo Carlini  <pcarlini@suse.de>
//
// Copyright (C) 2005-2018 Free Software Foundation, Inc.
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
		using Rider::Faiz::is_unsigned;

		STATIC_REQUIRE(test_category<is_unsigned, void>(false));

		STATIC_REQUIRE(test_category<is_unsigned, char>(char(-1) > char(0)));
		STATIC_REQUIRE(test_category<is_unsigned, signed char>(false));
		STATIC_REQUIRE(test_category<is_unsigned, unsigned char>(true));
		STATIC_REQUIRE(
			test_category<is_unsigned, wchar_t>(wchar_t(-1) > wchar_t(0)));
		STATIC_REQUIRE(test_category<is_unsigned, short>(false));
		STATIC_REQUIRE(test_category<is_unsigned, unsigned short>(true));
		STATIC_REQUIRE(test_category<is_unsigned, int>(false));
		STATIC_REQUIRE(test_category<is_unsigned, unsigned int>(true));
		STATIC_REQUIRE(test_category<is_unsigned, long>(false));
		STATIC_REQUIRE(test_category<is_unsigned, unsigned long>(true));
		STATIC_REQUIRE(test_category<is_unsigned, long long>(false));
		STATIC_REQUIRE(test_category<is_unsigned, unsigned long long>(true));

		STATIC_REQUIRE(test_category<is_unsigned, float>(false));
		STATIC_REQUIRE(test_category<is_unsigned, double>(false));
		STATIC_REQUIRE(test_category<is_unsigned, long double>(false));

		STATIC_REQUIRE(test_category<is_unsigned, unsigned __int128>(true));
		STATIC_REQUIRE(test_category<is_unsigned, __int128>(false));


#if _GLIBCXX_USE_FLOAT128
		STATIC_REQUIRE(test_category<is_unsigned, __float128>(false));
#endif
		// Sanity check.
		STATIC_REQUIRE(test_category<is_unsigned, ClassType>(false));
	}

} // namespace
TEST_CASE("is_unsigned.libstdcxx: ")
{
	test01();
}
