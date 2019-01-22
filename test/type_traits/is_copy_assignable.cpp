//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_copy_assignable

// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{

	template<class T>
	void
	test_is_copy_assignable()
	{
		STATIC_REQUIRE(Rider::Faiz::is_copy_assignable<T>::value);
#if TEST_STD_VER > 14
		STATIC_REQUIRE(Rider::Faiz::is_copy_assignable_v<T>);
#endif
	}

	template<class T>
	void
	test_is_not_copy_assignable()
	{
		STATIC_REQUIRE(!Rider::Faiz::is_copy_assignable<T>::value);
#if TEST_STD_VER > 14
		STATIC_REQUIRE(!Rider::Faiz::is_copy_assignable_v<T>);
#endif
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

	class B
	{
		B&
		operator=(const B&);
	};

	struct C
	{
		void
		operator=(C&); // not const
	};

} // namespace
TEST_CASE("is_copy_assignable: ")
{
	test_is_copy_assignable<int>();
	test_is_copy_assignable<int&>();
	test_is_copy_assignable<A>();
	test_is_copy_assignable<bit_zero>();
	test_is_copy_assignable<Union>();
	test_is_copy_assignable<NotEmpty>();
	test_is_copy_assignable<Empty>();

#if TEST_STD_VER >= 11
	test_is_not_copy_assignable<const int>();
	test_is_not_copy_assignable<int[]>();
	test_is_not_copy_assignable<int[3]>();
	test_is_not_copy_assignable<B>();
#endif
	test_is_not_copy_assignable<void>();
	test_is_not_copy_assignable<C>();
}
