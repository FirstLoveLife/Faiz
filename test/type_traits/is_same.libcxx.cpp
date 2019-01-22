//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_same

// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{

	template<class T, class U>
	void
	test_is_same()
	{
		STATIC_REQUIRE(Rider::Faiz::is_same<T, U>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_same<const T, U>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_same<T, const U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_same<const T, const U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_same_v<T, U>);
		STATIC_REQUIRE(!Rider::Faiz::is_same_v<const T, U>);
		STATIC_REQUIRE(!Rider::Faiz::is_same_v<T, const U>);
		STATIC_REQUIRE(Rider::Faiz::is_same_v<const T, const U>);
	}

	template<class T, class U>
	void
	test_is_same_ref()
	{
		STATIC_REQUIRE(Rider::Faiz::is_same<T, U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_same<const T, U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_same<T, const U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_same<const T, const U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_same_v<T, U>);
		STATIC_REQUIRE(Rider::Faiz::is_same_v<const T, U>);
		STATIC_REQUIRE(Rider::Faiz::is_same_v<T, const U>);
		STATIC_REQUIRE(Rider::Faiz::is_same_v<const T, const U>);
	}

	template<class T, class U>
	void
	test_is_not_same()
	{
		STATIC_REQUIRE(!Rider::Faiz::is_same<T, U>::value);
	}

	class Class
	{
	public:
		~Class();
	};

} // namespace
TEST_CASE("is_same.libcxx: ")
{
	test_is_same<int, int>();
	test_is_same<void, void>();
	test_is_same<Class, Class>();
	test_is_same<int*, int*>();
	test_is_same_ref<int&, int&>();

	test_is_not_same<int, void>();
	test_is_not_same<void, Class>();
	test_is_not_same<Class, int*>();
	test_is_not_same<int*, int&>();
	test_is_not_same<int&, int>();
}
