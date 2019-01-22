//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_base_of

#include "rider/faiz/type_traits.hpp"
#include <catch2/catch.hpp>

namespace
{


	template<class T, class U>
	void
	test_is_base_of()
	{
		STATIC_REQUIRE(Rider::Faiz::is_base_of<T, U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_base_of<const T, U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_base_of<T, const U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_base_of<const T, const U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_base_of_v<T, U>);
		STATIC_REQUIRE(Rider::Faiz::is_base_of_v<const T, U>);
		STATIC_REQUIRE(Rider::Faiz::is_base_of_v<T, const U>);
		STATIC_REQUIRE(Rider::Faiz::is_base_of_v<const T, const U>);
	}

	template<class T, class U>
	void
	test_is_not_base_of()
	{
		STATIC_REQUIRE(!Rider::Faiz::is_base_of<T, U>::value);
	}

	struct B
	{};
	struct B1 : B
	{};
	struct B2 : B
	{};
	struct D : private B1, private B2
	{};

} // namespace
TEST_CASE("is_base_of.libcxx: ")
{
	test_is_base_of<B, D>();
	test_is_base_of<B1, D>();
	test_is_base_of<B2, D>();
	test_is_base_of<B, B1>();
	test_is_base_of<B, B2>();
	test_is_base_of<B, B>();

	test_is_not_base_of<D, B>();
	test_is_not_base_of<B&, D&>();
	test_is_not_base_of<B[3], D[3]>();
	test_is_not_base_of<int, int>();
}
