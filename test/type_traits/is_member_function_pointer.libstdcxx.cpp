// { dg-do compile { target c++11 } }

// Copyright (C) 2011-2018 Free Software Foundation, Inc.
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

#include "rider/faiz/type_traits.hpp"
#include <catch2/catch.hpp>

// #include "../testsuite_tr1.h"
#include "test-utilities.hpp"

namespace
{

	void
	test01()
	{
		using Rider::Faiz::is_member_function_pointer;

		// Positive tests.
		STATIC_REQUIRE(
			test_category<is_member_function_pointer, int (ClassType::*)(int)>(
				true));

		STATIC_REQUIRE(test_category<is_member_function_pointer,
			int (ClassType::*)(int) const>(true));

		STATIC_REQUIRE(test_category<is_member_function_pointer,
			int (ClassType::*)(float, ...)>(true));

		STATIC_REQUIRE(test_category<is_member_function_pointer,
			ClassType (ClassType::*)(ClassType)>(true));

		STATIC_REQUIRE(test_category<is_member_function_pointer,
			float (ClassType::*)(int, float, int[], int&)>(true));


		// Negative tests.
		STATIC_REQUIRE(
			test_category<is_member_function_pointer, int(ClassType::*)>(
				false));

		STATIC_REQUIRE(
			test_category<is_member_function_pointer, const int(ClassType::*)>(
				false));

		STATIC_REQUIRE(
			test_category<is_member_function_pointer, ClassType(ClassType::*)>(
				false));


		// Sanity check.
		STATIC_REQUIRE(
			test_category<is_member_function_pointer, ClassType>(false));
	}

} // namespace
TEST_CASE("is_member_function_pointer.libstdcxx: ")
{
	test01();
}
