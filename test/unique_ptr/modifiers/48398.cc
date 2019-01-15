// { dg-do compile { target c++11 } }

// Copyright (C) 2011-2018 Free Software Foundation, Inc.
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

// 20.7.1 Class template unique_ptr [unique.ptr]

#include "rider/faiz/unique_ptr.hpp"
#include <catch2/catch.hpp>
#include <memory>

// PR libstdc++/48398

namespace
{
	struct my_deleter
	{
		typedef int* pointer;

		void
		operator()(pointer p)
		{
			delete p;
		}
	};
} // namespace
TEST_CASE("48398")
{
	{
		Rider::Faiz::unique_ptr<void, my_deleter> p(new int());

		p.get();
		p.reset();
	}
}
