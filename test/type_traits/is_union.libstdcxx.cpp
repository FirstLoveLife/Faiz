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

#include "../testsuite_tr1.h"
#include "rider/faiz/type_traits.hpp"

void
test01()
{
	using rider::faiz::is_union;
	using namespace __gnu_test;

	// Positive tests.
	static_assert(test_category<is_union, UnionType>(true), "");

	// Negative tests.
	static_assert(test_category<is_union, ClassType>(false), "");
	static_assert(test_category<is_union, DerivedType>(false), "");
	static_assert(test_category<is_union, ConvType>(false), "");
	static_assert(test_category<is_union, AbstractClass>(false), "");
	static_assert(test_category<is_union, PolymorphicClass>(false), "");
	static_assert(test_category<is_union, DerivedPolymorphic>(false), "");
	static_assert(test_category<is_union, void>(false), "");
	static_assert(test_category<is_union, int>(false), "");
	static_assert(test_category<is_union, float>(false), "");
	static_assert(test_category<is_union, int[2]>(false), "");
	static_assert(test_category<is_union, int*>(false), "");
	static_assert(test_category<is_union, int (*)(int)>(false), "");
	static_assert(test_category<is_union, float&>(false), "");
	static_assert(test_category<is_union, float (&)(float)>(false), "");
	static_assert(test_category<is_union, int(ClassType::*)>(false), "");
	static_assert(test_category<is_union, int (ClassType::*)(int)>(false), "");
	static_assert(test_category<is_union, int(int)>(false), "");
	static_assert(test_category<is_union, EnumType>(false), "");
}
