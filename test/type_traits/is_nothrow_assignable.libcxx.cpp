//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_nothrow_assignable

// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{

	template<class T, class U>
	void
	test_is_nothrow_assignable()
	{
		STATIC_REQUIRE(Rider::Faiz::is_nothrow_assignable<T, U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_nothrow_assignable_v<T, U>);
	}

	template<class T, class U>
	void
	test_is_not_nothrow_assignable()
	{
		STATIC_REQUIRE(!Rider::Faiz::is_nothrow_assignable<T, U>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_nothrow_assignable_v<T, U>);
	}

	struct A
	{};

	struct B
	{
		void operator=(A);
	};

	struct C
	{
		void
		operator=(C&); // not const
	};

} // namespace
TEST_CASE("is_nothrow_assignable.libcxx: ")
{
	test_is_nothrow_assignable<int&, int&>();
	test_is_nothrow_assignable<int&, int>();
	test_is_nothrow_assignable<int&, double>();

	test_is_not_nothrow_assignable<int, int&>();
	test_is_not_nothrow_assignable<int, int>();
	test_is_not_nothrow_assignable<B, A>();
	test_is_not_nothrow_assignable<A, B>();
	test_is_not_nothrow_assignable<C, C&>();
}
