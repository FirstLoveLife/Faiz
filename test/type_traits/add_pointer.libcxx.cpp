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

#include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"
template<class T, class U>
void
test_add_pointer()
{
	static_assert(
		(Rider::Faiz::is_same<typename Rider::Faiz::add_pointer<T>::type,
			U>::value),
		"");
	static_assert(
		(Rider::Faiz::is_same<Rider::Faiz::add_pointer_t<T>, U>::value), "");
}

template<class F>
void
test_function0()
{
	static_assert(
		(Rider::Faiz::is_same<typename Rider::Faiz::add_pointer<F>::type,
			F*>::value),
		"");
	static_assert(
		(Rider::Faiz::is_same<Rider::Faiz::add_pointer_t<F>, F*>::value), "");
}

template<class F>
void
test_function1()
{
	static_assert(
		(Rider::Faiz::is_same<typename Rider::Faiz::add_pointer<F>::type,
			F>::value),
		"");
	static_assert(
		(Rider::Faiz::is_same<Rider::Faiz::add_pointer_t<F>, F>::value), "");
}

struct Foo
{};

int
main()
{
	test_add_pointer<void, void*>();
	test_add_pointer<int, int*>();
	test_add_pointer<int[3], int(*)[3]>();
	test_add_pointer<int&, int*>();
	test_add_pointer<const int&, const int*>();
	test_add_pointer<int*, int**>();
	test_add_pointer<const int*, const int**>();
	test_add_pointer<Foo, Foo*>();

	//  LWG 2101 specifically talks about add_pointer and functions.
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
