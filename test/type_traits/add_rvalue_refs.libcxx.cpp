#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{
	template<class T, class U>
	void
	test_add_rvalue_reference()
	{
		STATIC_REQUIRE(Rider::Faiz::is_same<
			typename Rider::Faiz::add_rvalue_reference<T>::type,
			U>::value);

		STATIC_REQUIRE(
			Rider::Faiz::is_same<Rider::Faiz::add_rvalue_reference_t<T>,
				U>::value);
	}

	template<class F>
	void
	test_function0()
	{
		STATIC_REQUIRE(Rider::Faiz::is_same<
			typename Rider::Faiz::add_rvalue_reference<F>::type,
			F&&>::value);

		STATIC_REQUIRE(
			Rider::Faiz::is_same<Rider::Faiz::add_rvalue_reference_t<F>,
				F&&>::value);
	}

	template<class F>
	void
	test_function1()
	{
		STATIC_REQUIRE(Rider::Faiz::is_same<
			typename Rider::Faiz::add_rvalue_reference<F>::type,
			F>::value);

		STATIC_REQUIRE(
			Rider::Faiz::is_same<Rider::Faiz::add_rvalue_reference_t<F>,
				F>::value);
	}

	struct Foo
	{};


} // namespace
TEST_CASE("add_rvalue_reference libcxx: <void (Foo::*)()>")
{
	test_function0<void (Foo::*)()>();
}
TEST_CASE("add_rvalue_reference libcxx: <void (Foo::*)() const>")
{
	test_function0<void (Foo::*)() const>();
}
TEST_CASE("add_rvalue_reference libcxx: <void (Foo::*)()&>")
{
	test_function0<void (Foo::*)()&>();
}
TEST_CASE("add_rvalue_reference libcxx: <void (Foo::*)() &&>")
{
	test_function0<void (Foo::*)() &&>();
}
TEST_CASE("add_rvalue_reference libcxx: <void (Foo::*)() const&>")
{
	test_function0<void (Foo::*)() const&>();
}
TEST_CASE("add_rvalue_reference libcxx: <void (Foo::*)() const&&>")
{
	test_function0<void (Foo::*)() const&&>();
}


TEST_CASE("add_rvalue_reference libcxx: free function: <void()>")
{
	test_function0<void()>();
}
TEST_CASE("add_rvalue_reference libcxx: free function: <void() const>")
{
	test_function1<void() const>();
}
TEST_CASE("add_rvalue_reference libcxx: free function: <void()&>")
{
	test_function1<void()&>();
}
TEST_CASE("add_rvalue_reference libcxx: free function: <void() &&>")
{
	test_function1<void() &&>();
}
TEST_CASE("add_rvalue_reference libcxx: free function: <void() const&>")
{
	test_function1<void() const&>();
}
TEST_CASE("add_rvalue_reference libcxx: free function: <void() const&&>")
{
	test_function1<void() const&&>();
}


TEST_CASE("add_rvalue_reference libcxx: <void, void>")
{
	test_add_rvalue_reference<void, void>();
}
TEST_CASE("add_rvalue_reference libcxx: <const int*, const int*&&>")
{
	test_add_rvalue_reference<const int*, const int*&&>();
}
TEST_CASE("add_rvalue_reference libcxx: <int&, int&>")
{
	test_add_rvalue_reference<int&, int&>();
}
TEST_CASE("add_rvalue_reference libcxx: <const int&, const int&>")
{
	test_add_rvalue_reference<const int&, const int&>();
}
TEST_CASE("add_rvalue_reference libcxx: <int, int&&>")
{
	test_add_rvalue_reference<int, int&&>();
}
TEST_CASE("add_rvalue_reference libcxx: <int[3], int(&&)[3]>")
{
	test_add_rvalue_reference<int[3], int(&&)[3]>();
}
TEST_CASE("add_rvalue_reference libcxx: <int*, int*&&>")
{
	test_add_rvalue_reference<int*, int*&&>();
}
TEST_CASE("add_rvalue_reference libcxx: <Foo, Foo&&>")
{
	test_add_rvalue_reference<Foo, Foo&&>();
}
