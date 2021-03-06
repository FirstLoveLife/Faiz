// { dg-do compile { target c++11 } }

// Copyright (C) 2015-2018 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include "rider/faiz/unique_ptr.hpp"
#include <catch2/catch.hpp>

template<typename X>
void tfoo(X)
{}

template<>
void
tfoo(int)
{}

template<>
void
tfoo(char)
{}

int
foo()
{
	return 5;
}

void
bar()
{
	tfoo('c');
}

namespace
{
	struct do_nothing
	{
		template<class T>
		void
		operator()(T*)
		{}
	};
} // namespace
TEST_CASE("dr2228")
{
	{
		int i = 0;
		Rider::Faiz::unique_ptr<int, do_nothing> p1(&i);
		Rider::Faiz::unique_ptr<int> p2;
		static_assert(
			!std::is_assignable<decltype(p2), decltype(p1)>::value, "");
	}
}
