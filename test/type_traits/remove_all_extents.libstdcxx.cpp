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

// #include "../testsuite_tr1.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{}
TEST_CASE("remove_all_extents libstdcxx")
{
	using Rider::Faiz::remove_all_extents;
	using Rider::Faiz::is_same;
	// using namespace __gnu_test;

	STATIC_REQUIRE(is_same<remove_all_extents<int>::type, int>::value);
	STATIC_REQUIRE(is_same<remove_all_extents<int[2]>::type, int>::value);
	STATIC_REQUIRE(is_same<remove_all_extents<int[2][3]>::type, int>::value);
	STATIC_REQUIRE(is_same<remove_all_extents<int[][3]>::type, int>::value);
	STATIC_REQUIRE(
		is_same<remove_all_extents<const int[2][3]>::type, const int>::value);

	STATIC_REQUIRE(
		is_same<remove_all_extents<ClassType>::type, ClassType>::value);
	STATIC_REQUIRE(
		is_same<remove_all_extents<ClassType[2]>::type, ClassType>::value);
	STATIC_REQUIRE(
		is_same<remove_all_extents<ClassType[2][3]>::type, ClassType>::value);

	STATIC_REQUIRE(
		is_same<remove_all_extents<ClassType[][3]>::type, ClassType>::value);

	STATIC_REQUIRE(is_same<remove_all_extents<const ClassType[2][3]>::type,
		const ClassType>::value);
}
