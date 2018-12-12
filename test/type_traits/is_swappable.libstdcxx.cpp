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

#include "../testsuite_tr1.h"
#include "rider/faiz/type_traits.hpp"
#include <array>
#include <queue>
#include <stack>
#include <tuple>
#include <utility>
using Rider::Faiz::is_swappable;

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
void
test01()
{
	using namespace __gnu_test;
	// Positive tests.
	static_assert(is_swappable<int>::value, "");
	static_assert(is_swappable<bool>::value, "");
	static_assert(is_swappable<decltype(nullptr)>::value, "");
	static_assert(is_swappable<int&>::value, "");
	static_assert(is_swappable<int&&>::value, "");
	static_assert(is_swappable<int[1]>::value, "");
	static_assert(is_swappable<int[1][2]>::value, "");
	static_assert(is_swappable<int[1][2][3]>::value, "");
	static_assert(is_swappable<int(&)[1]>::value, "");
	static_assert(is_swappable<funny::F[5]>::value, "");
	static_assert(is_swappable<funny::F3>::value, "");
	static_assert(is_swappable<funny::F3[1]>::value, "");
	static_assert(is_swappable<funny::F3[1][2]>::value, "");
	static_assert(is_swappable<funny::F3[1][2][3]>::value, "");
	static_assert(is_swappable<ThrowCopyConsClass>::value, "");
	static_assert(is_swappable<EnumType>::value, "");
	static_assert(is_swappable<PODType>::value, "");
	static_assert(is_swappable<UnionType>::value, "");
	static_assert(is_swappable<construct::SE>::value, "");
	static_assert(is_swappable<construct::Empty>::value, "");
	static_assert(is_swappable<void*>::value, "");
	static_assert(is_swappable<int const*>::value, "");
	static_assert(is_swappable<ClassType*>::value, "");
	static_assert(is_swappable<int ClassType::*>::value, "");
	static_assert(is_swappable<void (ClassType::*)()>::value, "");
	static_assert(is_swappable<construct::Nontrivial>::value, "");
	static_assert(is_swappable<construct::Any>::value, "");
	static_assert(is_swappable<construct::nAny>::value, "");
	static_assert(is_swappable<std::pair<int, int>>::value, "");
	static_assert(is_swappable<std::pair<int, int>[1]>::value, "");
	static_assert(is_swappable<std::pair<int, int>[1][2]>::value, "");
	static_assert(is_swappable<std::pair<int, int>[1][2][3]>::value, "");
	static_assert(
		is_swappable<std::pair<construct::Nontrivial, construct::Nontrivial>>(
			true),
		"");
	static_assert(is_swappable<std::tuple<int>>::value, "");
	static_assert(is_swappable<std::tuple<int>[1]>::value, "");
	static_assert(is_swappable<std::tuple<int>[1][2]>::value, "");
	static_assert(is_swappable<std::tuple<int>[1][2][3]>::value, "");
	static_assert(is_swappable<std::tuple<>>::value, "");
	static_assert(is_swappable<std::tuple<>[1]>::value, "");
	static_assert(is_swappable<std::tuple<>[1][2]>::value, "");
	static_assert(is_swappable<std::tuple<>[1][2][3]>::value, "");
	static_assert(is_swappable<std::tuple<construct::Nontrivial>>::value, "");
	static_assert(is_swappable<std::array<int, 1>>::value, "");
	static_assert(is_swappable<std::array<int, 1>[1]>::value, "");
	static_assert(is_swappable<std::array<int, 1>[1][2]>::value, "");
	static_assert(is_swappable<std::array<int, 1>[1][2][3]>::value, "");
	static_assert(
		is_swappable<std::array<construct::Nontrivial, 1>>::value, "");
	static_assert(is_swappable<std::array<int, 0>>::value, "");
	static_assert(is_swappable<std::array<construct::DelCopy, 0>>::value, "");
	static_assert(is_swappable<std::queue<int>>::value, "");
	static_assert(is_swappable<std::queue<int>[1]>::value, "");
	static_assert(is_swappable<std::queue<int>[1][2]>::value, "");
	static_assert(is_swappable<std::queue<int>[1][2][3]>::value, "");
	static_assert(is_swappable<std::queue<construct::Nontrivial>>::value, "");
	static_assert(is_swappable<std::priority_queue<int>>::value, "");
	static_assert(is_swappable<std::priority_queue<int>[1]>::value, "");
	static_assert(is_swappable<std::priority_queue<int>[1][2]>::value, "");
	static_assert(is_swappable<std::priority_queue<int>[1][2][3]>::value, "");
	static_assert(
		is_swappable<std::priority_queue<construct::Nontrivial>>::value, "");
	static_assert(is_swappable<std::stack<int>>::value, "");
	static_assert(is_swappable<std::stack<int>[1]>::value, "");
	static_assert(is_swappable<std::stack<int>[1][2]>::value, "");
	static_assert(is_swappable<std::stack<int>[1][2][3]>::value, "");
	static_assert(is_swappable<std::stack<construct::Nontrivial>>::value, "");
	// Negative tests.
	static_assert(!is_swappable<void>::value, "");
	static_assert(!is_swappable<const void>::value, "");
	static_assert(!is_swappable<void()>::value, "");
	static_assert(!is_swappable<void() const>::value, "");
	static_assert(!is_swappable<void() volatile>::value, "");
	static_assert(!is_swappable<void() const volatile>::value, "");
	static_assert(!is_swappable<const int>::value, "");
	static_assert(!is_swappable<const bool>::value, "");
	static_assert(!is_swappable<int[]>::value, "");
	static_assert(!is_swappable<const int[]>::value, "");
	static_assert(!is_swappable<int[][1]>::value, "");
	static_assert(!is_swappable<const int[1]>::value, "");
	static_assert(!is_swappable<const int[1][2]>::value, "");
	static_assert(!is_swappable<const int[1][2][3]>::value, "");
	static_assert(!is_swappable<construct::DelCopy>::value, "");
	static_assert(!is_swappable<construct::Abstract>::value, "");
	static_assert(!is_swappable<construct::NontrivialUnion>::value, "");
	static_assert(!is_swappable<funny::F>::value, "");
	static_assert(!is_swappable<funny::F[1]>::value, "");
	static_assert(!is_swappable<funny::F[1][2]>::value, "");
	static_assert(!is_swappable<funny::F[1][2][3]>::value, "");
	static_assert(!is_swappable<funny::F[4]>::value, "");
	static_assert(!is_swappable<construct::DelCopy>::value, "");
	static_assert(!is_swappable<DeletedCopyAssignClass>::value, "");
	static_assert(!is_swappable<DeletedMoveAssignClass>::value, "");
	static_assert(!is_swappable<funny::F2>::value, "");
	static_assert(!is_swappable<funny::F2[1]>::value, "");
	static_assert(!is_swappable<funny::F2[1][2]>::value, "");
	static_assert(!is_swappable<funny::F2[1][2][3]>::value, "");
	static_assert(!is_swappable<funny::F2[4]>::value, "");
	static_assert(!is_swappable<funny::F2[5]>::value, "");
}
