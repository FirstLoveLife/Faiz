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

#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/unique_ptr.hpp"
#include <catch2/catch.hpp>

#include <memory>

namespace
{
	struct A
	{
		A() : b(true)
		{}
		A(int) : b(false)
		{}
		bool b;
	};
} // namespace
TEST_CASE("array")
{
	{
		Rider::Faiz::unique_ptr<A[]> a = Rider::Faiz::make_unique<A[]>(3);
		// CHECK(a != nullptr);
		auto value = a[0].b && a[1].b && a[2].b;
		CHECK(value);
	}
}
