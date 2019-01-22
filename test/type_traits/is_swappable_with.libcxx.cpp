//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14

// type_traits

// is_swappable_with

// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/utility.hpp"
#include <vector>

namespace
{

	namespace MyNS
	{

		struct A
		{
			A(A const&) = delete;
			A&
			operator=(A const&)
				= delete;
		};

		struct B
		{
			B(B const&) = delete;
			B&
			operator=(B const&)
				= delete;
		};

		struct C
		{};
		struct D
		{};

		void
		swap(A&, A&)
		{}

		void
		swap(A&, B&)
		{}
		void
		swap(B&, A&)
		{}

		void
		swap(A&, C&)
		{} // missing swap(C, A)
		void
		swap(D&, C&)
		{}

		struct M
		{};

		void
		swap(M&&, M&&)
		{}

	} // namespace MyNS

} // namespace
TEST_CASE("is_swappable_with.libcxx: ")
{}
TEST_CASE("is_swappable_with.libcxx: ")
{
	using namespace MyNS;
	{
		// Test that is_swappable_with doesn't apply an lvalue reference
		// to the type. Instead it is up to the user.
		STATIC_REQUIRE(!Rider::Faiz::is_swappable_with<int, int>::value);
		STATIC_REQUIRE(Rider::Faiz::is_swappable_with<int&, int&>::value);
		STATIC_REQUIRE(Rider::Faiz::is_swappable_with<M, M>::value);
		STATIC_REQUIRE(Rider::Faiz::is_swappable_with<A&, A&>::value);
	}
	{
		// test that heterogeneous swap is allowed only if both 'swap(A, B)' and
		// 'swap(B, A)' are valid.
		STATIC_REQUIRE(Rider::Faiz::is_swappable_with<A&, B&>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_swappable_with<A&, C&>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_swappable_with<D&, C&>::value);
	}
	{
		// test that cv void is guarded against as required.
		STATIC_REQUIRE(!Rider::Faiz::is_swappable_with_v<void, int>);
		STATIC_REQUIRE(!Rider::Faiz::is_swappable_with_v<int, void>);
		STATIC_REQUIRE(
			!Rider::Faiz::is_swappable_with_v<const void, const volatile void>,
	}
	{
		// test for presence of is_swappable_with_v
		STATIC_REQUIRE(Rider::Faiz::is_swappable_with_v<int&, int&>);
		STATIC_REQUIRE(!Rider::Faiz::is_swappable_with_v<D&, C&>);
	}
}
