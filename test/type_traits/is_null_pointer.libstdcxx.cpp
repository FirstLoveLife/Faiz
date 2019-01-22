// { dg-do compile { target c++11 } }

// 2013-05-02  Paolo Carlini  <pcarlini@suse.de>
//
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

TEST_CASE("is_null_pointer libstdcxx")
{
	using Rider::Faiz::is_null_pointer;
	// using namespace __gnu_test;

	STATIC_REQUIRE(
		test_category<is_null_pointer, Rider::Faiz::nullptr_t>(true));

	STATIC_REQUIRE(test_category<is_null_pointer, int>(false));
	STATIC_REQUIRE(test_category<is_null_pointer, float>(false));
	STATIC_REQUIRE(test_category<is_null_pointer, EnumType>(false));
	STATIC_REQUIRE(test_category<is_null_pointer, int*>(false));
	STATIC_REQUIRE(test_category<is_null_pointer, int (*)(int)>(false));
	STATIC_REQUIRE(test_category<is_null_pointer, int(ClassType::*)>(false));
	STATIC_REQUIRE(
		test_category<is_null_pointer, int (ClassType::*)(int)>(false));
	STATIC_REQUIRE(test_category<is_null_pointer, int[2]>(false));
	STATIC_REQUIRE(test_category<is_null_pointer, float[][3]>(false));
	STATIC_REQUIRE(test_category<is_null_pointer, EnumType[2][3][4]>(false));
	STATIC_REQUIRE(test_category<is_null_pointer, int * [3]>(false));
	STATIC_REQUIRE(test_category<is_null_pointer, int (*[][2])(int)>(false));
	STATIC_REQUIRE(
		test_category<is_null_pointer, int(ClassType:: * [2][3])>(false));
	STATIC_REQUIRE(
		test_category<is_null_pointer, int (ClassType::*[][2][3])(int)>(false));

	STATIC_REQUIRE(test_category<is_null_pointer, ClassType>(false));
	STATIC_REQUIRE(test_category<is_null_pointer, PODType>(false));
	STATIC_REQUIRE(test_category<is_null_pointer, void>(false));
	STATIC_REQUIRE(test_category<is_null_pointer, NType>(false));
	STATIC_REQUIRE(test_category<is_null_pointer, TType>(false));
	STATIC_REQUIRE(test_category<is_null_pointer, SLType>(false));
}
