//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// add_pointer
// If T names a referenceable type or a (possibly cv-qualified) void type then
//    the member typedef type shall name the same type as
//    remove_reference_t<T>*; otherwise, type shall name T.

// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{
	template<class T, class U>
	void
	test_add_pointer()
	{
		STATIC_REQUIRE(
			Rider::Faiz::is_same<typename Rider::Faiz::add_pointer<T>::type,
				U>::value);

		STATIC_REQUIRE(
			Rider::Faiz::is_same<Rider::Faiz::add_pointer_t<T>, U>::value);
	}

	template<class F>
	void
	test_function0()
	{
		STATIC_REQUIRE(
			Rider::Faiz::is_same<typename Rider::Faiz::add_pointer<F>::type,
				F*>::value);
		STATIC_REQUIRE(
			Rider::Faiz::is_same<Rider::Faiz::add_pointer_t<F>, F*>::value);
	}

	template<class F>
	void
	test_function1()
	{
		STATIC_REQUIRE(
			Rider::Faiz::is_same<typename Rider::Faiz::add_pointer<F>::type,
				F>::value);

		STATIC_REQUIRE(
			Rider::Faiz::is_same<Rider::Faiz::add_pointer_t<F>, F>::value);
	}

	struct Foo
	{};

} // namespace
TEST_CASE("add_pointer.libcxx: <void, void*>")
{
	test_add_pointer<void, void*>();

	//  LWG 2101 specifically talks about add_pointer and functions.
	//  The term of art is "a referenceable type", which a cv- or ref-qualified
	//  function is not.

	//  But a cv- or ref-qualified member function *is* "a referenceable type"
}

TEST_CASE("add_pointer.libcxx: member function: <void (Foo::*)()>")
{
	test_function0<void (Foo::*)()>();
}
TEST_CASE("add_pointer.libcxx: member function: <void (Foo::*)() const>")
{
	test_function0<void (Foo::*)() const>();
}
TEST_CASE("add_pointer.libcxx: member function: <void (Foo::*)()&>")
{
	test_function0<void (Foo::*)()&>();
}
TEST_CASE("add_pointer.libcxx: member function: <void (Foo::*)() &&>")
{
	test_function0<void (Foo::*)() &&>();
}
TEST_CASE("add_pointer.libcxx: member function: <void (Foo::*)() const&&>")
{
	test_function0<void (Foo::*)() const&&>();
}
TEST_CASE("add_pointer.libcxx: member function: <void (Foo::*)() const&>")
{
	test_function0<void (Foo::*)() const&>();
}

TEST_CASE("add_pointer.libcxx: <int, int*>")
{
	test_add_pointer<int, int*>();
}
TEST_CASE("add_pointer.libcxx: <int[3], int(*)[3]>")
{
	test_add_pointer<int[3], int(*)[3]>();
}
TEST_CASE("add_pointer.libcxx: <int&, int*>")
{
	test_add_pointer<int&, int*>();
}
TEST_CASE("add_pointer.libcxx: <const int&, const int*>")
{
	test_add_pointer<const int&, const int*>();
}
TEST_CASE("add_pointer.libcxx: <int*, int**>")
{
	test_add_pointer<int*, int**>();
}
TEST_CASE("add_pointer.libcxx: <const int*, const int**>")
{
	test_add_pointer<const int*, const int**>();
}
TEST_CASE("add_pointer.libcxx: <Foo, Foo*>")
{
	test_add_pointer<Foo, Foo*>();
}


TEST_CASE("add_pointer.libcxx: free function: <void()>")
{
	test_function0<void()>();
}

TEST_CASE("add_pointer.libcxx: free function: <void() const>")
{
	test_function1<void() const>();
}
TEST_CASE("add_pointer.libcxx: free function: <void()&>")
{
	test_function1<void()&>();
}
TEST_CASE("add_pointer.libcxx: free function: <void() &&>")
{
	test_function1<void() &&>();
}
TEST_CASE("add_pointer.libcxx: free function: <void() const&>")
{
	test_function1<void() const&>();
}
TEST_CASE("add_pointer.libcxx: free function: <void() const&&>")
{
	test_function1<void() const&&>();
}
