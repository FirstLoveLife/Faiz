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

#include "rider/faiz/unique_ptr.hpp"
#include <catch2/catch.hpp>
#include <memory>

namespace
{
	struct A
	{
		virtual ~A() = default;
	};

	struct B : A
	{};

	// Construction from objects with different cv-qualification

	struct A_pointer
	{
		operator A*() const
		{
			return nullptr;
		}
	};

} // namespace

TEST_CASE("cons: cv_qual")
{
	{
		Rider::Faiz::unique_ptr<const A> cA(new A);
		Rider::Faiz::unique_ptr<volatile A> vA(new A);
		Rider::Faiz::unique_ptr<const volatile A> cvA(new A);
	}
	{
		Rider::Faiz::unique_ptr<const A> cB(new B);
		Rider::Faiz::unique_ptr<volatile A> vB(new B);
		Rider::Faiz::unique_ptr<const volatile A> cvB(new B);
	}
	{
		Rider::Faiz::unique_ptr<A> upA;

		Rider::Faiz::unique_ptr<const A> cA(std::move(upA));
		Rider::Faiz::unique_ptr<volatile A> vA(std::move(upA));
		Rider::Faiz::unique_ptr<const volatile A> cvA(std::move(upA));
	}
	{
		Rider::Faiz::unique_ptr<B> upB;

		Rider::Faiz::unique_ptr<const A> cA(std::move(upB));
		Rider::Faiz::unique_ptr<volatile A> vA(std::move(upB));
		Rider::Faiz::unique_ptr<const volatile A> cvA(std::move(upB));
	}
	{
		Rider::Faiz::unique_ptr<const A[]> cA(new A[1]);
		Rider::Faiz::unique_ptr<volatile A[]> vA(new A[1]);
		Rider::Faiz::unique_ptr<const volatile A[]> cvA(new A[1]);
	}
	{
		Rider::Faiz::unique_ptr<A[]> upA;

		Rider::Faiz::unique_ptr<const A[]> cA(std::move(upA));
		Rider::Faiz::unique_ptr<volatile A[]> vA(std::move(upA));
		Rider::Faiz::unique_ptr<const volatile A[]> cvA(std::move(upA));
	}
	{
		// Allow conversions from user-defined pointer-like types
		// for the single-object version
		A_pointer p;
		Rider::Faiz::unique_ptr<A> upA(p);
		Rider::Faiz::unique_ptr<const A> cA(p);
		Rider::Faiz::unique_ptr<volatile A> vA(p);
		Rider::Faiz::unique_ptr<const volatile A> cvA(p);
		// Allow conversions from user-defined pointer-like types
		// for the array version when the type is converted explicitly
		Rider::Faiz::unique_ptr<A[]> upA2((A*)p);
		Rider::Faiz::unique_ptr<const A[]> cA2((A*)p);
		Rider::Faiz::unique_ptr<volatile A[]> vA2((A*)p);
		Rider::Faiz::unique_ptr<const volatile A[]> cvA2((A*)p);
	}
}
