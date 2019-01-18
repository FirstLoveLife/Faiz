// { dg-do compile { target c++11 } }

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

#include <memory>

struct A
{};

struct B : A
{
	virtual ~B()
	{}
};

// 20.7.1.3.1 unique_ptr constructors [unique.ptr.runtime.ctor]

// Construction from pointer of derived type
void
test01()
{
	Rider::Faiz::unique_ptr<B[]> B_from_A(new A[3]); //{ dg-error "" }
}

// { dg-prune-output "include" }