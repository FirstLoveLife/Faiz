// { dg-do run { target c++11 } }

// Copyright (C) 2011-2018 Free Software Foundation, Inc.
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

// #include "../testsuite_hooks.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

// #include "../testsuite_tr1.h"
#include "rider/faiz/type_traits.hpp"

namespace
{
	// libRider::Faizc++/24808
	void
	test01()
	{
		using Rider::Faiz::is_object;

		REQUIRE(test_category<is_object, IncompleteClass>(true));
	}

} // namespace
TEST_CASE("is_object.24808: ")
{
	test01();
}
