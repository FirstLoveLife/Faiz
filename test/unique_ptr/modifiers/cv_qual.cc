// { dg-do compile { target c++11 } }

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

// 20.7.1 Class template unique_ptr [unique.ptr]

#include <catch2/catch.hpp>
#include <memory>

#include "rider/faiz/unique_ptr.hpp"

namespace
{
	struct A
	{
		virtual ~A() = default;
	};

	struct B : A
	{};
} // namespace

TEST_CASE("cv_qual, Construction from objects with different cv-qualification")
{
	{
		Rider::Faiz::unique_ptr<const A> cA;
		cA.reset(new A);
		Rider::Faiz::unique_ptr<volatile A> vA;
		vA.reset(new A);
		Rider::Faiz::unique_ptr<const volatile A> cvA;
		cvA.reset(new A);
	}
	{
		Rider::Faiz::unique_ptr<const A> cB;
		cB.reset(new B);
		Rider::Faiz::unique_ptr<volatile A> vB;
		vB.reset(new B);
		Rider::Faiz::unique_ptr<const volatile A> cvB;
		cvB.reset(new B);
	}
	{
		Rider::Faiz::unique_ptr<const A[]> cA;
		cA.reset(new A[1]);
		Rider::Faiz::unique_ptr<volatile A[]> vA;
		vA.reset(new A[1]);
		Rider::Faiz::unique_ptr<const volatile A[]> cvA;
		cvA.reset(new A[1]);
	}
}

namespace
{
	struct A_pointer
	{
		operator A*() const
		{
			return nullptr;
		}
	};
} // namespace
TEST_CASE("modifiers: cv_qual")
{
	{
		A_pointer p;
		// Allow conversions from user-defined pointer-like types
		// for the single-object version
		Rider::Faiz::unique_ptr<A> upA;
		upA.reset(p);
		Rider::Faiz::unique_ptr<const A> cA;
		cA.reset(p);
		Rider::Faiz::unique_ptr<volatile A> vA;
		vA.reset(p);
		Rider::Faiz::unique_ptr<const volatile A> cvA;
		cvA.reset(p);
		// Allow conversions from user-defined pointer-like types
		// for the array version when the type is converted explicitly
		Rider::Faiz::unique_ptr<A[]> upA2;
		upA2.reset((A*)p);
		Rider::Faiz::unique_ptr<const A[]> cA2;
		cA2.reset((A*)p);
		Rider::Faiz::unique_ptr<volatile A[]> vA2;
		vA2.reset((A*)p);
		Rider::Faiz::unique_ptr<const volatile A[]> cvA2;
		cvA2.reset((A*)p);
	}
}
