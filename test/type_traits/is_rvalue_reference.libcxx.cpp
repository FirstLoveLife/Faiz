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

// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{

	template<class T>
	void
	test_rvalue_ref()
	{
		STATIC_REQUIRE(!Rider::Faiz::is_void<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_null_pointer<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_integral<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_floating_point<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_array<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_pointer<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_lvalue_reference<T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_rvalue_reference<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_member_object_pointer<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_member_function_pointer<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_enum<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_union<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_class<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_function<T>::value);
	}

	struct incomplete_type;

} // namespace
TEST_CASE("is_rvalue_reference.libcxx: ")
{
	test_rvalue_ref<int&&>();
	test_rvalue_ref<const int&&>();

	//  LWG#2582
	STATIC_REQUIRE(!Rider::Faiz::is_rvalue_reference<incomplete_type>::value);
}
