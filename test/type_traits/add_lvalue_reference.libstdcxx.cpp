// { dg-do compile { target c++11 } }

// 2007-06-02  Paolo Carlini  <pcarlini@suse.de>
//
// Copyright (C) 2007-2018 Free Software Foundation, Inc.
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

TEST_CASE("add_lvalue_reference libstdcxx: test void")
{
	using Rider::Faiz::add_lvalue_reference;
	using std::is_same;
	// using namespace __gnu_test;

	STATIC_REQUIRE(is_same<add_lvalue_reference<int>::type, int&>::value);
	STATIC_REQUIRE(is_same<add_lvalue_reference<int&>::type, int&>::value);
	STATIC_REQUIRE(
		is_same<add_lvalue_reference<const int>::type, const int&>::value);
	STATIC_REQUIRE(is_same<add_lvalue_reference<int*>::type, int*&>::value);
	STATIC_REQUIRE(
		is_same<add_lvalue_reference<ClassType&>::type, ClassType&>::value);
	STATIC_REQUIRE(
		is_same<add_lvalue_reference<ClassType>::type, ClassType&>::value);
	STATIC_REQUIRE(
		is_same<add_lvalue_reference<int(int)>::type, int (&)(int)>::value);
	STATIC_REQUIRE(is_same<add_lvalue_reference<int&&>::type, int&>::value);
	STATIC_REQUIRE(
		is_same<add_lvalue_reference<ClassType&&>::type, ClassType&>::value);

	STATIC_REQUIRE(is_same<add_lvalue_reference<void>::type, void>::value);
	STATIC_REQUIRE(
		is_same<add_lvalue_reference<const void>::type, const void>::value);
	STATIC_REQUIRE(is_same<add_lvalue_reference<bool(int) const>::type,
		bool(int) const>::value);

	STATIC_REQUIRE(
		is_same<add_lvalue_reference<bool(int)&>::type, bool(int)&>::value);
	STATIC_REQUIRE(is_same<add_lvalue_reference<bool(int) const&&>::type,
		bool(int) const&&>::value);

	STATIC_REQUIRE(
		is_same<add_lvalue_reference<bool(int)>::type, bool (&)(int)>::value);
}
