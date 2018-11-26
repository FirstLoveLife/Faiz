//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// type_traits

// rvalue_ref

#include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"

template<class T>
void
test_rvalue_ref()
{
	static_assert(!rider::faiz::is_void<T>::value, "");
	static_assert(!rider::faiz::is_null_pointer<T>::value, "");
	static_assert(!rider::faiz::is_integral<T>::value, "");
	static_assert(!rider::faiz::is_floating_point<T>::value, "");
	static_assert(!rider::faiz::is_array<T>::value, "");
	static_assert(!rider::faiz::is_pointer<T>::value, "");
	static_assert(!rider::faiz::is_lvalue_reference<T>::value, "");
	static_assert(rider::faiz::is_rvalue_reference<T>::value, "");
	static_assert(!rider::faiz::is_member_object_pointer<T>::value, "");
	static_assert(!rider::faiz::is_member_function_pointer<T>::value, "");
	static_assert(!rider::faiz::is_enum<T>::value, "");
	static_assert(!rider::faiz::is_union<T>::value, "");
	static_assert(!rider::faiz::is_class<T>::value, "");
	static_assert(!rider::faiz::is_function<T>::value, "");
}

struct incomplete_type;

int
main()
{
	test_rvalue_ref<int&&>();
	test_rvalue_ref<const int&&>();

	//  LWG#2582
	static_assert(
		!rider::faiz::is_rvalue_reference<incomplete_type>::value, "");
}
