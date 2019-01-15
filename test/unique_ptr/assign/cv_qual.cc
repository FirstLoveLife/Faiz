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
#include "rider/faiz/utility.hpp"
#include <catch2/catch.hpp>

namespace
{
	struct A
	{
		virtual ~A() = default;
	};

	struct B : A
	{};

	// Assignment from objects with different cv-qualification


} // namespace

TEST_CASE("cv_qual")
{
	{
		Rider::Faiz::unique_ptr<B> upB;

		Rider::Faiz::unique_ptr<const A> cA;
		cA = Rider::Faiz::move(upB);
		Rider::Faiz::unique_ptr<volatile A> vA;
		vA = Rider::Faiz::move(upB);
		Rider::Faiz::unique_ptr<const volatile A> cvA;
		cvA = Rider::Faiz::move(upB);
	}

	{
		Rider::Faiz::unique_ptr<A[]> upA;

		Rider::Faiz::unique_ptr<const A[]> cA;
		cA = Rider::Faiz::move(upA);
		Rider::Faiz::unique_ptr<volatile A[]> vA;
		vA = Rider::Faiz::move(upA);
		Rider::Faiz::unique_ptr<const volatile A[]> cvA;
		cvA = Rider::Faiz::move(upA);
	}

	{
		Rider::Faiz::unique_ptr<A> upA;

		Rider::Faiz::unique_ptr<const A> cA;
		cA = Rider::Faiz::move(upA);
		Rider::Faiz::unique_ptr<volatile A> vA;
		vA = Rider::Faiz::move(upA);
		Rider::Faiz::unique_ptr<const volatile A> cvA;
		cvA = Rider::Faiz::move(upA);
	}
}
