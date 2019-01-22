//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_move_assignable

// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{

	template<class T>
	void
	test_is_move_assignable()
	{
		STATIC_REQUIRE(Rider::Faiz::is_move_assignable<T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_move_assignable_v<T>);
	}

	template<class T>
	void
	test_is_not_move_assignable()
	{
		STATIC_REQUIRE(!Rider::Faiz::is_move_assignable<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_move_assignable_v<T>);
	}

	class Empty
	{};

	class NotEmpty
	{
	public:
		virtual ~NotEmpty();
	};

	union Union
	{};

	struct bit_zero
	{
		int : 0;
	};

	struct A
	{
		A();
	};

} // namespace
TEST_CASE("is_move_assignable.libcxx: ")
{
	test_is_move_assignable<int>();
	test_is_move_assignable<A>();
	test_is_move_assignable<bit_zero>();
	test_is_move_assignable<Union>();
	test_is_move_assignable<NotEmpty>();
	test_is_move_assignable<Empty>();

	test_is_not_move_assignable<const int>();
	test_is_not_move_assignable<int[]>();
	test_is_not_move_assignable<int[3]>();
	test_is_not_move_assignable<void>();
}
