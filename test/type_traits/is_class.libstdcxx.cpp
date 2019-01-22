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

TEST_CASE("is_class")
{
	using Rider::Faiz::is_class;

	// Positive tests.
	STATIC_REQUIRE(test_category<is_class, ClassType>(true));
	STATIC_REQUIRE(test_category<is_class, DerivedType>(true));
	STATIC_REQUIRE(test_category<is_class, ConvType>(true));
	STATIC_REQUIRE(test_category<is_class, AbstractClass>(true));
	STATIC_REQUIRE(test_category<is_class, PolymorphicClass>(true));
	STATIC_REQUIRE(test_category<is_class, DerivedPolymorphic>(true));

	// Negative tests.
	STATIC_REQUIRE(test_category<is_class, UnionType>(false));
	STATIC_REQUIRE(test_category<is_class, void>(false));
	STATIC_REQUIRE(test_category<is_class, int>(false));
	STATIC_REQUIRE(test_category<is_class, float>(false));
	STATIC_REQUIRE(test_category<is_class, int[2]>(false));
	STATIC_REQUIRE(test_category<is_class, int*>(false));
	STATIC_REQUIRE(test_category<is_class, int (*)(int)>(false));
	STATIC_REQUIRE(test_category<is_class, float&>(false));
	STATIC_REQUIRE(test_category<is_class, float (&)(float)>(false));
	STATIC_REQUIRE(test_category<is_class, int(ClassType::*)>(false));
	STATIC_REQUIRE(test_category<is_class, int (ClassType::*)(int)>(false));
	STATIC_REQUIRE(test_category<is_class, int(int)>(false));
	STATIC_REQUIRE(test_category<is_class, EnumType>(false));
}
