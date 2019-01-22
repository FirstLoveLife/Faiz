// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{
	template<class T, class U>
	void
	test_add_lvalue_reference()
	{
		STATIC_REQUIRE(Rider::Faiz::is_same<
			typename Rider::Faiz::add_lvalue_reference<T>::type,
			U>::value);
		STATIC_REQUIRE(
			Rider::Faiz::is_same<Rider::Faiz::add_lvalue_reference_t<T>,
				U>::value);
	}

	template<class F>
	void
	test_function0()
	{
		STATIC_REQUIRE(Rider::Faiz::is_same<
			typename Rider::Faiz::add_lvalue_reference<F>::type,
			F&>::value);
		STATIC_REQUIRE(
			Rider::Faiz::is_same<Rider::Faiz::add_lvalue_reference_t<F>,
				F&>::value);
	}

	template<class F>
	void
	test_function1()
	{
		STATIC_REQUIRE(Rider::Faiz::is_same<
			typename Rider::Faiz::add_lvalue_reference<F>::type,
			F>::value);
		STATIC_REQUIRE(
			Rider::Faiz::is_same<Rider::Faiz::add_lvalue_reference_t<F>,
				F>::value);
	}

	struct Foo
	{};


} // namespace
TEST_CASE("add_lvalue_reference: void (Foo::*)() const&")
{
	test_function0<void (Foo::*)() const&>();
}
TEST_CASE("add_lvalue_reference: void (Foo::*)() &&")
{
	test_function0<void (Foo::*)() &&>();
}
TEST_CASE("add_lvalue_reference: void (Foo::*)()&")
{
	test_function0<void (Foo::*)()&>();
}
TEST_CASE("add_lvalue_reference: void (Foo::*)() const")
{
	test_function0<void (Foo::*)() const>();
}
TEST_CASE("add_lvalue_reference: void (Foo::*)()")
{
	test_function0<void (Foo::*)()>();
}
TEST_CASE("add_lvalue_reference: void() const&&")
{
	test_function1<void() const&&>();
}
TEST_CASE("add_lvalue_reference: void() const&")
{
	test_function1<void() const&>();
}
TEST_CASE("add_lvalue_reference: void() &&")
{
	test_function1<void() &&>();
}
TEST_CASE("add_lvalue_reference: void()&")
{
	test_function1<void()&>();
}
TEST_CASE("add_lvalue_reference: void() const")
{
	test_function1<void() const>();
}
TEST_CASE("add_lvalue_reference: void()")
{
	test_function0<void()>();
}

//  But a cv- or ref-qualified member function *is* "a referenceable type"

TEST_CASE("add_lvalue_reference: <Foo, Foo&>")
{
	test_add_lvalue_reference<Foo, Foo&>();
}
TEST_CASE("add_lvalue_reference: <const int*, const int*&>")
{
	test_add_lvalue_reference<const int*, const int*&>();
}
TEST_CASE("add_lvalue_reference: <int*, int*&>")
{
	test_add_lvalue_reference<int*, int*&>();
}
TEST_CASE("add_lvalue_reference: <const int&, const int&>")
{
	test_add_lvalue_reference<const int&, const int&>();
}
TEST_CASE("add_lvalue_reference: <int&, int&>")
{
	test_add_lvalue_reference<int&, int&>();
}
TEST_CASE("add_lvalue_reference: <int[3], int(&)[3]>")
{
	test_add_lvalue_reference<int[3], int(&)[3]>();
}
TEST_CASE("add_lvalue_reference: <int, int&>")
{
	test_add_lvalue_reference<int, int&>();
}
TEST_CASE("add_lvalue_reference: <void, void>")
{
	test_add_lvalue_reference<void, void>();
}
TEST_CASE("add_lvalue_reference: void (Foo::*)() const&&")
{

	test_function0<void (Foo::*)() const&&>();
}
//  LWG 2101 specifically talks about add_lvalue_reference and functions.
//  The term of art is "a referenceable type", which a cv- or ref-qualified
//  function is not.
