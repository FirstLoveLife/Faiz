//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// extent

#include "rider/faiz/type_traits.hpp"
#include <catch2/catch.hpp>

namespace
{


	template<class T, unsigned A>
	void
	test_extent()
	{
		STATIC_REQUIRE(Rider::Faiz::extent<T>::value == A);
		STATIC_REQUIRE(Rider::Faiz::extent<const T>::value == A);
		STATIC_REQUIRE(Rider::Faiz::extent<volatile T>::value == A);
		STATIC_REQUIRE(Rider::Faiz::extent<const volatile T>::value == A);
		STATIC_REQUIRE(Rider::Faiz::extent_v<T> == A);
		STATIC_REQUIRE(Rider::Faiz::extent_v<const T> == A);
		STATIC_REQUIRE(Rider::Faiz::extent_v<volatile T> == A);
		STATIC_REQUIRE(Rider::Faiz::extent_v<const volatile T> == A);
	}

	template<class T, unsigned A>
	void
	test_extent1()
	{
		STATIC_REQUIRE(Rider::Faiz::extent<T, 1>::value == A);
		STATIC_REQUIRE(Rider::Faiz::extent<const T, 1>::value == A);
		STATIC_REQUIRE(Rider::Faiz::extent<volatile T, 1>::value == A);
		STATIC_REQUIRE(Rider::Faiz::extent<const volatile T, 1>::value == A);
		STATIC_REQUIRE(Rider::Faiz::extent_v<T, 1> == A);
		STATIC_REQUIRE(Rider::Faiz::extent_v<const T, 1> == A);
		STATIC_REQUIRE(Rider::Faiz::extent_v<volatile T, 1> == A);
		STATIC_REQUIRE(Rider::Faiz::extent_v<const volatile T, 1> == A);
	}

	class Class
	{
	public:
		~Class();
	};

} // namespace
TEST_CASE("extent.libcxx: <int, 0>")
{
	test_extent<int, 0>();
}
TEST_CASE("extent.libcxx: <bool, 0>")
{
	test_extent<bool, 0>();
}
TEST_CASE("extent.libcxx: <double, 0>")
{
	test_extent<double, 0>();
}
TEST_CASE("extent.libcxx: 1  <int, 0>")
{
	test_extent1<int, 0>();
}
TEST_CASE("extent.libcxx: 1 <int[][4], 4>")
{
	test_extent1<int[][4], 4>();
}
TEST_CASE("extent.libcxx: 1 <int[2], 0>")
{
	test_extent1<int[2], 0>();
}
TEST_CASE("extent.libcxx: <unsigned, 0>")
{
	test_extent<unsigned, 0>();
}
TEST_CASE("extent.libcxx: <int[][4], 0>")
{
	test_extent<int[][4], 0>();
}
TEST_CASE("extent.libcxx: 1 <int[2][4], 4>")
{
	test_extent1<int[2][4], 4>();
}
TEST_CASE("extent.libcxx: <int[2][4], 2>")
{
	test_extent<int[2][4], 2>();
}
TEST_CASE("extent.libcxx: <int[2], 2>")
{
	test_extent<int[2], 2>();
}
TEST_CASE("extent.libcxx: <const int*, 0>")
{
	test_extent<const int*, 0>();
}
TEST_CASE("extent.libcxx: <Class, 0>")
{
	test_extent<Class, 0>();
}
TEST_CASE("extent.libcxx: <int&, 0>")
{
	test_extent<int&, 0>();
}
TEST_CASE("extent.libcxx: <void, 0>")
{
	test_extent<void, 0>();
}

TEST_CASE("extent.libcxx: <int*, 0>")
{
	test_extent<int*, 0>();
}
