// { dg-do run { target c++11 } }

// Copyright (C) 2008-2018 Free Software Foundation, Inc.
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

// 20.6.11 Template class unique_ptr [unique.ptr]

#include "rider/faiz/unique_ptr.hpp"
#include <catch2/catch.hpp>

namespace
{
	struct B
	{
		virtual ~B()
		{}
	};
	struct D : public B
	{};
} // namespace

TEST_CASE("move")
{
	D* d = new D;
	Rider::Faiz::unique_ptr<D> p1(d);
	Rider::Faiz::unique_ptr<D> p2(new D);
	p2 = std::move(p1);

	CHECK(p1.get() == 0);
	CHECK(p2.get() == d);

	Rider::Faiz::unique_ptr<B> p3(new B);
	p3 = std::move(p2);

	CHECK(p2.get() == 0);
	CHECK(p3.get() == d);
}
