//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// decay


#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{

	template<class T, class U>
	void
	test_decay()
	{
		STATIC_REQUIRE(
			Rider::Faiz::is_same<typename std::decay<T>::type, U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_same<Rider::Faiz::decay_t<T>, U>::value);
	}


} // namespace
TEST_CASE("decay.libcxx: <void, void>")
{
	test_decay<void, void>();
}
TEST_CASE("decay.libcxx: <int, int>")
{
	test_decay<int, int>();
}
TEST_CASE("decay.libcxx: <const int[3], const int*>")
{
	test_decay<const int[3], const int*>();
}
TEST_CASE("decay.libcxx: <void(), void (*)()>")
{
	test_decay<void(), void (*)()>();
}
TEST_CASE("decay.libcxx: <int[3], int*>")
{
	test_decay<int[3], int*>();
}
TEST_CASE("decay.libcxx: <int*, int*>")
{
	test_decay<int*, int*>();
}
TEST_CASE("decay.libcxx: <const volatile int, int>")
{
	test_decay<const volatile int, int>();
}
TEST_CASE("decay.libcxx: <int(int) const, int(int) const>")
{
	test_decay<int(int) const, int(int) const>();
}
TEST_CASE("decay.libcxx: <int(int) volatile, int(int) volatile>")
{
	test_decay<int(int) volatile, int(int) volatile>();
}
TEST_CASE("decay.libcxx: <int(int)&, int(int)&>")
{
	test_decay<int(int)&, int(int)&>();
}

TEST_CASE("decay.libcxx: <int(int)&&, int(int) &&>")
{
	test_decay<int(int)&&, int(int) &&>();
}
