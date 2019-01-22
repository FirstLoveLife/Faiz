// { dg-options "-std=gnu++17" }
// { dg-do compile }

// Copyright (C) 2016-2018 Free Software Foundation, Inc.
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
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace funny
{
	struct T0
	{};

	void swap(T0, T0);

	struct T1
	{};
	struct T2
	{};

	void swap(T1, T2);
	void swap(T2, T1);

	struct BoolLike
	{};

	void
	swap(BoolLike, bool&);
	void
	swap(bool&, BoolLike);

	struct F0
	{};

	void swap(F0, F0) = delete;

	struct F1
	{};

	void swap(F0, F1) = delete;
	void swap(F1, F0) = delete;

	struct F2
	{};
	struct F3
	{};

	void swap(F2, F3);
	void swap(F3, F2) = delete;

	struct F4
	{
		friend void swap(F4, F4) = delete;
	};
} // namespace funny

void
test01()
{
	using Rider::Faiz::is_swappable_with;
	using namespace __gnu_test;
	// Positive tests.
	STATIC_REQUIRE(test_property<is_swappable_with, int&, int&>(true));
	STATIC_REQUIRE(
		test_property<is_swappable_with, funny::T0, funny::T0>(true));
	STATIC_REQUIRE(
		test_property<is_swappable_with, funny::T0, const funny::T0>(true));
	STATIC_REQUIRE(
		test_property<is_swappable_with, funny::T1, funny::T2>(true));
	STATIC_REQUIRE(
		test_property<is_swappable_with, funny::T2, funny::T1>(true));
	STATIC_REQUIRE(
		test_property<is_swappable_with, funny::BoolLike, bool&>(true));
	STATIC_REQUIRE(
		test_property<is_swappable_with, const funny::BoolLike, bool&>(true),


	// Negative tests.
	STATIC_REQUIRE(test_property<is_swappable_with, int, int>(false));
	STATIC_REQUIRE(test_property<is_swappable_with, int&, unsigned&>(false));
	STATIC_REQUIRE(
		test_property<is_swappable_with, const int&, const int&>(false));
	STATIC_REQUIRE(
		test_property<is_swappable_with, funny::F0, funny::F0>(false));
	STATIC_REQUIRE(
		test_property<is_swappable_with, funny::F0, const funny::F0>(false),

	STATIC_REQUIRE(
		test_property<is_swappable_with, funny::T0, funny::T1>(false));
	STATIC_REQUIRE(
		test_property<is_swappable_with, funny::F0, funny::F1>(false));
	STATIC_REQUIRE(
		test_property<is_swappable_with, funny::F0, const funny::F1>(false),

	STATIC_REQUIRE(
		test_property<is_swappable_with, const funny::F0, funny::F1>(false),

	STATIC_REQUIRE(
		test_property<is_swappable_with, funny::F2, funny::F3>(false));
	STATIC_REQUIRE(
		test_property<is_swappable_with, funny::F2, const funny::F3>(false),

	STATIC_REQUIRE(
		test_property<is_swappable_with, const funny::F2, funny::F3>(false),

	STATIC_REQUIRE(
		test_property<is_swappable_with, funny::F4, funny::F4>(false));
	STATIC_REQUIRE(
		test_property<is_swappable_with, funny::BoolLike, funny::BoolLike>(
			false),
}
