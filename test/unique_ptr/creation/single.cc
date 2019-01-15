// { dg-do run { target c++14 } }

// Copyright (C) 2013-2018 Free Software Foundation, Inc.
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

// 20.9.1.4 unique_ptr creation [unique.ptr.create]

#include "../../testsuite_hooks.h"
#include "rider/faiz/unique_ptr.hpp"
#include <catch2/catch.hpp>
#include <memory>

namespace
{
	struct A
	{
		A() : b(false)
		{}
		A(int, double&, char&&, void*) : b(true)
		{}
		bool b;
	};
} // namespace
TEST_CASE("single")
{
	{
		int i = 0;
		double d = 0;
		char c = 0;
		Rider::Faiz::unique_ptr<A> a
			= Rider::Faiz::make_unique<A>(i, d, std::move(c), nullptr);
		CHECK(a != nullptr);
		CHECK(a->b);

		a = Rider::Faiz::make_unique<A>();
		CHECK(a != nullptr);
		CHECK(!a->b);
	}
}
