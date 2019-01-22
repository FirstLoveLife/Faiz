#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
// Copyright (C) 2015-2018 Free Software Foundation, Inc.
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
#include "rider/faiz/utility/swap.hpp"
#include <array>
#include <queue>
#include <stack>
#include <tuple>
#include <utility>

namespace
{
	using Rider::Faiz::range::is_swappable;

	namespace funny
	{
		struct F
		{};
		void
		swap(F&, F&)
			= delete;
		void swap(F (&)[5], F (&)[5]);

		struct F2
		{
			friend void
			swap(F2&, F2&)
				= delete;
		};

		struct F3
		{
			friend void
			swap(F3&, F3)
			{}
		};
	} // namespace funny
} // namespace
TEST_CASE("is_swappable.libstdcxx")
{
	// using namespace __gnu_test;
	// Positive tests.
	STATIC_REQUIRE(is_swappable<int>::value);
	STATIC_REQUIRE(is_swappable<bool>::value);
	STATIC_REQUIRE(is_swappable<decltype(nullptr)>::value);
	STATIC_REQUIRE(is_swappable<int&>::value);
	STATIC_REQUIRE(is_swappable<int&&>::value);
	STATIC_REQUIRE(is_swappable<int[1]>::value);
	STATIC_REQUIRE(is_swappable<int[1][2]>::value);
	STATIC_REQUIRE(is_swappable<int[1][2][3]>::value);
	STATIC_REQUIRE(is_swappable<int(&)[1]>::value);
	STATIC_REQUIRE(is_swappable<funny::F[5]>::value);
	STATIC_REQUIRE(is_swappable<funny::F3>::value);
	STATIC_REQUIRE(is_swappable<funny::F3[1]>::value);
	STATIC_REQUIRE(is_swappable<funny::F3[1][2]>::value);
	STATIC_REQUIRE(is_swappable<funny::F3[1][2][3]>::value);
	STATIC_REQUIRE(is_swappable<ThrowCopyConsClass>::value);
	STATIC_REQUIRE(is_swappable<EnumType>::value);
	STATIC_REQUIRE(is_swappable<PODType>::value);
	STATIC_REQUIRE(is_swappable<UnionType>::value);
	STATIC_REQUIRE(is_swappable<construct::SE>::value);
	STATIC_REQUIRE(is_swappable<construct::Empty>::value);
	STATIC_REQUIRE(is_swappable<void*>::value);
	STATIC_REQUIRE(is_swappable<int const*>::value);
	STATIC_REQUIRE(is_swappable<ClassType*>::value);
	STATIC_REQUIRE(is_swappable<int ClassType::*>::value);
	STATIC_REQUIRE(is_swappable<void (ClassType::*)()>::value);
	STATIC_REQUIRE(is_swappable<construct::Nontrivial>::value);
	STATIC_REQUIRE(is_swappable<construct::Any>::value);
	STATIC_REQUIRE(is_swappable<construct::nAny>::value);
	STATIC_REQUIRE(is_swappable<std::pair<int, int>>::value);
	STATIC_REQUIRE(is_swappable<std::pair<int, int>[1]>::value);
	STATIC_REQUIRE(is_swappable<std::pair<int, int>[1][2]>::value);
	STATIC_REQUIRE(is_swappable<std::pair<int, int>[1][2][3]>::value);
	STATIC_REQUIRE(
		is_swappable<std::pair<construct::Nontrivial, construct::Nontrivial>>(
			true));

	STATIC_REQUIRE(is_swappable<std::tuple<int>>::value);
	STATIC_REQUIRE(is_swappable<std::tuple<int>[1]>::value);
	STATIC_REQUIRE(is_swappable<std::tuple<int>[1][2]>::value);
	STATIC_REQUIRE(is_swappable<std::tuple<int>[1][2][3]>::value);
	STATIC_REQUIRE(is_swappable<std::tuple<>>::value);
	STATIC_REQUIRE(is_swappable<std::tuple<>[1]>::value);
	STATIC_REQUIRE(is_swappable<std::tuple<>[1][2]>::value);
	STATIC_REQUIRE(is_swappable<std::tuple<>[1][2][3]>::value);
	STATIC_REQUIRE(is_swappable<std::tuple<construct::Nontrivial>>::value);
	STATIC_REQUIRE(is_swappable<std::array<int, 1>>::value);
	STATIC_REQUIRE(is_swappable<std::array<int, 1>[1]>::value);
	STATIC_REQUIRE(is_swappable<std::array<int, 1>[1][2]>::value);
	STATIC_REQUIRE(is_swappable<std::array<int, 1>[1][2][3]>::value);
	STATIC_REQUIRE(is_swappable<std::array<construct::Nontrivial, 1>>::value);
	STATIC_REQUIRE(is_swappable<std::array<int, 0>>::value);
	STATIC_REQUIRE(is_swappable<std::array<construct::DelCopy, 0>>::value);
	STATIC_REQUIRE(is_swappable<std::queue<int>>::value);
	STATIC_REQUIRE(is_swappable<std::queue<int>[1]>::value);
	STATIC_REQUIRE(is_swappable<std::queue<int>[1][2]>::value);
	STATIC_REQUIRE(is_swappable<std::queue<int>[1][2][3]>::value);
	STATIC_REQUIRE(is_swappable<std::queue<construct::Nontrivial>>::value);
	STATIC_REQUIRE(is_swappable<std::priority_queue<int>>::value);
	STATIC_REQUIRE(is_swappable<std::priority_queue<int>[1]>::value);
	STATIC_REQUIRE(is_swappable<std::priority_queue<int>[1][2]>::value);
	STATIC_REQUIRE(is_swappable<std::priority_queue<int>[1][2][3]>::value);
	STATIC_REQUIRE(
		is_swappable<std::priority_queue<construct::Nontrivial>>::value);
	STATIC_REQUIRE(is_swappable<std::stack<int>>::value);
	STATIC_REQUIRE(is_swappable<std::stack<int>[1]>::value);
	STATIC_REQUIRE(is_swappable<std::stack<int>[1][2]>::value);
	STATIC_REQUIRE(is_swappable<std::stack<int>[1][2][3]>::value);
	STATIC_REQUIRE(is_swappable<std::stack<construct::Nontrivial>>::value);
	// Negative tests.
	STATIC_REQUIRE(!is_swappable<void>::value);
	STATIC_REQUIRE(!is_swappable<const void>::value);
	STATIC_REQUIRE(!is_swappable<void()>::value);
	STATIC_REQUIRE(!is_swappable<void() const>::value);
	STATIC_REQUIRE(!is_swappable<void() volatile>::value);
	STATIC_REQUIRE(!is_swappable<void() const volatile>::value);
	STATIC_REQUIRE(!is_swappable<const int>::value);
	STATIC_REQUIRE(!is_swappable<const bool>::value);
	STATIC_REQUIRE(!is_swappable<int[]>::value);
	STATIC_REQUIRE(!is_swappable<const int[]>::value);
	STATIC_REQUIRE(!is_swappable<int[][1]>::value);
	STATIC_REQUIRE(!is_swappable<const int[1]>::value);
	STATIC_REQUIRE(!is_swappable<const int[1][2]>::value);
	STATIC_REQUIRE(!is_swappable<const int[1][2][3]>::value);
	STATIC_REQUIRE(!is_swappable<construct::DelCopy>::value);
	STATIC_REQUIRE(!is_swappable<construct::Abstract>::value);
	STATIC_REQUIRE(!is_swappable<construct::NontrivialUnion>::value);
	STATIC_REQUIRE(!is_swappable<funny::F>::value);
	STATIC_REQUIRE(!is_swappable<funny::F[1]>::value);
	STATIC_REQUIRE(!is_swappable<funny::F[1][2]>::value);
	STATIC_REQUIRE(!is_swappable<funny::F[1][2][3]>::value);
	STATIC_REQUIRE(!is_swappable<funny::F[4]>::value);
	STATIC_REQUIRE(!is_swappable<construct::DelCopy>::value);
	STATIC_REQUIRE(!is_swappable<DeletedCopyAssignClass>::value);
	STATIC_REQUIRE(!is_swappable<DeletedMoveAssignClass>::value);
	STATIC_REQUIRE(!is_swappable<funny::F2>::value);
	STATIC_REQUIRE(!is_swappable<funny::F2[1]>::value);
	STATIC_REQUIRE(!is_swappable<funny::F2[1][2]>::value);
	STATIC_REQUIRE(!is_swappable<funny::F2[1][2][3]>::value);
	STATIC_REQUIRE(!is_swappable<funny::F2[4]>::value);
	STATIC_REQUIRE(!is_swappable<funny::F2[5]>::value);
}
