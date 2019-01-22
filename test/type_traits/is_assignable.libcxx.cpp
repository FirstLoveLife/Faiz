//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_assignable

// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{

	struct A
	{};

	struct B
	{
		void operator=(A);
	};

	template<class T, class U>
	void
	test_is_assignable()
	{
		STATIC_REQUIRE(Rider::Faiz::is_assignable<T, U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_assignable_v<T, U>);
	}

	template<class T, class U>
	void
	test_is_not_assignable()
	{
		STATIC_REQUIRE(!Rider::Faiz::is_assignable<T, U>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_assignable_v<T, U>);
	}

	struct D;

	struct C
	{
		template <class U>
    D operator,(U&&);
	};

	struct E
	{
		C
		operator=(int);
	};

	template<typename T>
	struct XX
	{
		T t;
	};

} // namespace
TEST_CASE("is_assignable.libcxx: ")
{
	test_is_assignable<int&, int&>();
	test_is_assignable<int&, int>();
	test_is_assignable<int&, double>();
	test_is_assignable<B, A>();
	test_is_assignable<void*&, void*>();

	test_is_assignable<E, int>();

	test_is_not_assignable<int, int&>();
	test_is_not_assignable<int, int>();
	test_is_not_assignable<A, B>();
	test_is_not_assignable<void, const void>();
	test_is_not_assignable<const void, const void>();
	test_is_not_assignable<int(), int>();

	//  pointer to incomplete template type
	test_is_assignable<XX<D>*&, XX<D>*>();
}
