// { dg-do run { target c++11 } }

// Copyright (C) 2012-2018 Free Software Foundation, Inc.
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

// 20.7.1 Template class unique_ptr [unique.ptr]

#include "../testsuite_hooks.h"
#include "rider/faiz/unique_ptr.hpp"
#include <catch2/catch.hpp>
#include <memory>

namespace
{
	struct A;

	struct B
	{
		// Rider::Faiz::unique_ptr<A> a;
		Rider::Faiz::unique_ptr<A> a;
		std::unique_ptr<A> aa;
	};

	struct A
	{
		B* b;
		~A()
		{
			CHECK(b->a != nullptr);
            CHECK(b->aa != nullptr);
		}
	};


	struct C;

	struct D
	{
		// Rider::Faiz::unique_ptr<C[]> c;
		Rider::Faiz::unique_ptr<C[]> c;
	};

	struct C
	{
		D* d;
		~C()
		{
			CHECK(d->c != nullptr);
		}
	};

} // namespace


TEST_CASE("54351")
{
	{
		B b;
		b.a.reset(new A);
		b.a->b = &b;
	}

	{
		D d;
		d.c.reset(new C[1]);
		d.c[0].d = &d;
	}
}
