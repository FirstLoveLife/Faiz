//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// has_nothrow_move_constructor

#include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"
#include <catch2/catch.hpp>
using Rider::Faiz::is_nothrow_move_constructible;
using Rider::Faiz::is_nothrow_move_constructible_v;

namespace
{
	template<class T>
	void
	test_is_nothrow_move_constructible()
	{
		static_assert(is_nothrow_move_constructible<T>::value, "");
		static_assert(is_nothrow_move_constructible<const T>::value, "");
		static_assert(is_nothrow_move_constructible_v<T>, "");
		static_assert(is_nothrow_move_constructible_v<const T>, "");
	}

	template<class T>
	void
	test_has_not_nothrow_move_constructor()
	{
		static_assert(!is_nothrow_move_constructible<T>::value, "");
		static_assert(!is_nothrow_move_constructible<const T>::value, "");
		static_assert(!is_nothrow_move_constructible<volatile T>::value, "");
		static_assert(
			!is_nothrow_move_constructible<const volatile T>::value, "");
		static_assert(!is_nothrow_move_constructible_v<T>, "");
		static_assert(!is_nothrow_move_constructible_v<const T>, "");
		static_assert(!is_nothrow_move_constructible_v<volatile T>, "");
		static_assert(!is_nothrow_move_constructible_v<const volatile T>, "");
	}

	class Empty
	{};

	union Union
	{};

	struct bit_zero
	{
		int : 0;
	};

	struct A
	{
		A(const A&);
	};
} // namespace
TEST_CASE("test_has_not_nothrow_move_constructor ")
{
	test_has_not_nothrow_move_constructor<void>();
	test_has_not_nothrow_move_constructor<A>();

	test_is_nothrow_move_constructible<int&>();
	test_is_nothrow_move_constructible<Union>();
	test_is_nothrow_move_constructible<Empty>();
	test_is_nothrow_move_constructible<int>();
	test_is_nothrow_move_constructible<double>();
	test_is_nothrow_move_constructible<int*>();
	test_is_nothrow_move_constructible<const int*>();
	test_is_nothrow_move_constructible<bit_zero>();
}
