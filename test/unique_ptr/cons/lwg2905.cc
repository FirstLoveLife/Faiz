// Copyright (C) 2017 Free Software Foundation, Inc.
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

// { dg-do compile { target c++11 } }

#include "rider/faiz/unique_ptr.hpp"
#include <catch2/catch.hpp>
#include <memory>

namespace
{
	template<typename T, typename D, typename P, typename E>
	constexpr bool
	check()
	{
		return std::is_constructible<Rider::Faiz::unique_ptr<T, D>, P, E>::value;
	}

	struct Del
	{
		void
		operator()(void*) const
		{}
	};
} // namespace

TEST_CASE("lwg2905 -- 1")
{
	static_assert(!check<int, Del&, int*, Del>(), "");
	static_assert(check<int, Del&, int*, Del&>(), "");
	static_assert(check<int, const Del&, int*, Del&>(), "");
	static_assert(!check<int, Del&, int*, const Del&>(), "");
	static_assert(check<int, Del, int*, const Del&>(), "");
	static_assert(check<int, Del, int*, Del>(), "");

	static_assert(!check<int[], Del&, int*, Del>(), "");
	static_assert(check<int[], Del&, int*, Del&>(), "");
	static_assert(check<int[], const Del&, int*, Del&>(), "");
	static_assert(!check<int[], Del&, int*, const Del&>(), "");
	static_assert(check<int[], Del, int*, const Del&>(), "");
	static_assert(check<int[], Del, int*, Del>(), "");
}

namespace
{
	struct DelNoCopy
	{
		DelNoCopy() = default;
		DelNoCopy(const DelNoCopy&) = delete;
		DelNoCopy(DelNoCopy&&) = default;
		void
		operator()(void*) const
		{}
	};

} // namespace

TEST_CASE("lwg2905 -- 2")
{
	static_assert(!check<int, DelNoCopy&, int*, DelNoCopy>(), "");
	static_assert(check<int, DelNoCopy&, int*, DelNoCopy&>(), "");
	static_assert(check<int, const DelNoCopy&, int*, DelNoCopy&>(), "");
	static_assert(!check<int, DelNoCopy&, int*, const DelNoCopy&>(), "");
	static_assert(!check<int, DelNoCopy, int*, const DelNoCopy&>(), "");
	static_assert(check<int, DelNoCopy, int*, DelNoCopy>(), "");

	static_assert(!check<int[], DelNoCopy&, int*, DelNoCopy>(), "");
	static_assert(check<int[], DelNoCopy&, int*, DelNoCopy&>(), "");
	static_assert(check<int[], const DelNoCopy&, int*, DelNoCopy&>(), "");
	static_assert(!check<int[], DelNoCopy&, int*, const DelNoCopy&>(), "");
	static_assert(!check<int[], DelNoCopy, int*, const DelNoCopy&>(), "");
	static_assert(check<int[], DelNoCopy, int*, DelNoCopy>(), "");
}


namespace
{
	struct Base
	{
		virtual ~Base()
		{}
	};
	struct Derived : Base
	{};

} // namespace

TEST_CASE("lwg2905 -- 3")
{
	static_assert(!check<Base[], Del&, Base*, Del>(), "");
	static_assert(check<Base[], Del&, Base*, Del&>(), "");
	static_assert(check<Base[], const Del&, Base*, Del&>(), "");
	static_assert(!check<Base[], Del&, Base*, const Del&>(), "");
	static_assert(check<Base[], Del, Base*, const Del&>(), "");
	static_assert(check<Base[], Del, Base*, Del>(), "");

	static_assert(!check<Base[], Del&, Derived*, Del>(), "");
	static_assert(!check<Base[], Del&, Derived*, Del&>(), "");
	static_assert(!check<Base[], const Del&, Derived*, Del&>(), "");
	static_assert(!check<Base[], Del&, Derived*, const Del&>(), "");
	static_assert(!check<Base[], Del, Derived*, const Del&>(), "");
	static_assert(!check<Base[], Del, Derived*, Del>(), "");
}
