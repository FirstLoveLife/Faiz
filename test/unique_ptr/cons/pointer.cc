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

#include "../../testsuite_hooks.h"
#include "rider/faiz/unique_ptr.hpp"
#include <catch2/catch.hpp>
#include <memory>

namespace
{
	struct A
	{
		A()
		{
			++ctor_count;
		}
		virtual ~A()
		{
			++dtor_count;
		}
		static long ctor_count;
		static long dtor_count;
	};
	long A::ctor_count = 0;
	long A::dtor_count = 0;

	struct B : A
	{
		B()
		{
			++ctor_count;
		}
		virtual ~B()
		{
			++dtor_count;
		}
		static long ctor_count;
		static long dtor_count;
	};
	long B::ctor_count = 0;
	long B::dtor_count = 0;


	struct reset_count_struct
	{
		~reset_count_struct()
		{
			A::ctor_count = 0;
			A::dtor_count = 0;
			B::ctor_count = 0;
			B::dtor_count = 0;
		}
	};
} // namespace

//

//
TEST_CASE("pointer 20.6.11.2.1 unique_ptr constructors "
		  "[unique.ptr.single.ctor] Construction from pointer")
{
	{
		reset_count_struct __attribute__((unused)) reset;

		Rider::Faiz::unique_ptr<A> A_default;
		CHECK(A_default.get() == 0);
		CHECK(A::ctor_count == 0);
		CHECK(A::dtor_count == 0);
		CHECK(B::ctor_count == 0);
		CHECK(B::dtor_count == 0);

		Rider::Faiz::unique_ptr<A> A_from_A(new A);
		CHECK(A_from_A.get() != 0);
		CHECK(A::ctor_count == 1);
		CHECK(A::dtor_count == 0);
		CHECK(B::ctor_count == 0);
		CHECK(B::dtor_count == 0);

		Rider::Faiz::unique_ptr<A> A_from_B(new B);
		CHECK(A_from_B.get() != 0);
		CHECK(A::ctor_count == 2);
		CHECK(A::dtor_count == 0);
		CHECK(B::ctor_count == 1);
		CHECK(B::dtor_count == 0);
	}
	{
		reset_count_struct __attribute__((unused)) reset;

		A* const A_default = 0;
		Rider::Faiz::unique_ptr<A> p1(A_default);
		CHECK(p1.get() == 0);
		CHECK(A::ctor_count == 0);
		CHECK(A::dtor_count == 0);
		CHECK(B::ctor_count == 0);
		CHECK(B::dtor_count == 0);

		A* const A_from_A = new A;
		Rider::Faiz::unique_ptr<A> p2(A_from_A);
		CHECK(p2.get() == A_from_A);
		CHECK(A::ctor_count == 1);
		CHECK(A::dtor_count == 0);
		CHECK(B::ctor_count == 0);
		CHECK(B::dtor_count == 0);
	}
}
