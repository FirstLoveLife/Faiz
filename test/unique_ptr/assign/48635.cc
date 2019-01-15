// { dg-do run { target c++11 } }

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

#include "../../testsuite_hooks.h"
#include "rider/faiz/unique_ptr.hpp"
#include <catch2/catch.hpp>
#include <memory>

namespace
{
	struct Deleter
	{
		Deleter() = default;
		Deleter(const Deleter&) = default;
		Deleter(Deleter&&) = default;

		Deleter&
		operator=(const Deleter&)
		{
			CHECK(true);
			return *this;
		}

		Deleter&
		operator=(Deleter&&)
		{
			CHECK(false);
			return *this;
		}

		template<class T>
		void
		operator()(T*) const
		{}
	};

	struct DDeleter : Deleter
	{};

} // namespace

TEST_CASE("48635")
{
	Deleter d;

	Rider::Faiz::unique_ptr<int, Deleter&> p1(nullptr, d), p2(nullptr, d);
	p2 = std::move(p1);

	DDeleter dd;

	Rider::Faiz::unique_ptr<int[], Deleter&> p1a(nullptr, d), p2a(nullptr, d);
	p2a = std::move(p1a);
}
