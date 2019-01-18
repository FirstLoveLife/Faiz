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
#include <memory>

namespace
{
	struct A
	{
		virtual ~A()
		{}
	};

	struct B : A
	{};


} // namespace

TEST_CASE("comparisons,	20.6.11.5 unqiue_ptr specialized algorithms "
		  "[unique.ptr.special] ")
{
	{
		Rider::Faiz::unique_ptr<A> p1;
		Rider::Faiz::unique_ptr<A> p2(new A);

		CHECK(p1 != p2);
		CHECK(!(p1 == p2));
		auto value1 = (p1 < p2) || (p1 > p2);
		CHECK(value1);
		auto value2 = ((p1 <= p2) and (p1 != p2)) || ((p1 >= p2) && (p1 != p2));
		CHECK(value2);
	}
	{
		Rider::Faiz::unique_ptr<A> p1;
		Rider::Faiz::unique_ptr<A> p2;

		CHECK(p1 == p2);
		CHECK(!(p1 != p2));
		auto value1 = !(p1 < p2) and !(p1 > p2);
		CHECK(value1);
	}
}
