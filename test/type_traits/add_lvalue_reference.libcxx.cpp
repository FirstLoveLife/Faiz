#include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"
template<class T, class U>
void
test_add_lvalue_reference()
{
	static_assert((Rider::Faiz::is_same<
					  typename Rider::Faiz::add_lvalue_reference<T>::type,
					  U>::value),
		"");
	static_assert((Rider::Faiz::is_same<Rider::Faiz::add_lvalue_reference_t<T>,
					  U>::value),
		"");
}

template<class F>
void
test_function0()
{
	static_assert((Rider::Faiz::is_same<
					  typename Rider::Faiz::add_lvalue_reference<F>::type,
					  F&>::value),
		"");
	static_assert((Rider::Faiz::is_same<Rider::Faiz::add_lvalue_reference_t<F>,
					  F&>::value),
		"");
}

template<class F>
void
test_function1()
{
	static_assert((Rider::Faiz::is_same<
					  typename Rider::Faiz::add_lvalue_reference<F>::type,
					  F>::value),
		"");
	static_assert((Rider::Faiz::is_same<Rider::Faiz::add_lvalue_reference_t<F>,
					  F>::value),
		"");
}

struct Foo
{};

int
main()
{
	test_add_lvalue_reference<void, void>();
	test_add_lvalue_reference<int, int&>();
	test_add_lvalue_reference<int[3], int(&)[3]>();
	test_add_lvalue_reference<int&, int&>();
	test_add_lvalue_reference<const int&, const int&>();
	test_add_lvalue_reference<int*, int*&>();
	test_add_lvalue_reference<const int*, const int*&>();
	test_add_lvalue_reference<Foo, Foo&>();

	//  LWG 2101 specifically talks about add_lvalue_reference and functions.
	//  The term of art is "a referenceable type", which a cv- or ref-qualified
	//  function is not.
	test_function0<void()>();
	test_function1<void() const>();
	test_function1<void()&>();
	test_function1<void() &&>();
	test_function1<void() const&>();
	test_function1<void() const&&>();

	//  But a cv- or ref-qualified member function *is* "a referenceable type"
	test_function0<void (Foo::*)()>();
	test_function0<void (Foo::*)() const>();
	test_function0<void (Foo::*)()&>();
	test_function0<void (Foo::*)() &&>();
	test_function0<void (Foo::*)() const&>();
	test_function0<void (Foo::*)() const&&>();
}
