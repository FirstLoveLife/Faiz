// { dg-do run { target c++11 } }

// Copyright (C) 2010-2018 Free Software Foundation, Inc.
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

// 20.9.10.2.5 unique_ptr modifiers [unique.ptr.single.modifiers]

#include "../../testsuite_hooks.h"
#include "rider/faiz/unique_ptr.hpp"
#include <catch2/catch.hpp>
#include <memory>

namespace
{
	struct D
	{
		static int count;

		void
		operator()(int* p) const
		{
			++count;
			delete p;
		}
	};

	int D::count = 0;
} // namespace

TEST_CASE("43183")
{
	{
		Rider::Faiz::unique_ptr<int, D> up;
		up.reset();
		CHECK(D::count == 0);
		up.reset(new int);
		CHECK(D::count == 0);
		up.reset(up.get());
		CHECK(D::count == 1);
		up.release();
	}
}
