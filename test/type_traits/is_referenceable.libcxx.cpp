//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//

// __is_referenceable<Tp>
//
// [defns.referenceable] defines "a referenceable type" as:
// An object type, a function type that does not have cv-qualifiers
//    or a ref-qualifier, or a reference type.
//

// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"
#include <cassert>

namespace
{

	struct Foo
	{};


} // namespace
TEST_CASE("is_referenceable.libcxx: ")
{
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<int>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<int[3]>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<int[]>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<int&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<const int&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<int*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<const int*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<Foo>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<const Foo>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<Foo&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<const Foo&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<Foo&&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<const Foo&&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<int
		__attribute__((__vector_size__(8)))>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<const int
		__attribute__((__vector_size__(8)))>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<float
		__attribute__((__vector_size__(16)))>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<const float
		__attribute__((__vector_size__(16)))>::value);


	// Functions without cv-qualifiers are referenceable
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void()>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void() const>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void()&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void() const&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void() &&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void() const&&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void(int)>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(int) const>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(int)&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(int) const&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(int) &&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(int) const&&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void(int, float)>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, float) const>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(int, float)&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, float) const&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(int, float) &&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, float) const&&>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void(int, float, Foo&)>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, float, Foo&) const>::value);

	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, float, Foo&)&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, float, Foo&) const&>::value);

	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, float, Foo&) &&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, float, Foo&) const&&>::value);


	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void(...)>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(...) const>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(...)&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(...) const&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(...) &&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(...) const&&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void(int, ...)>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(int, ...) const>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(int, ...)&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, ...) const&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(int, ...) &&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, ...) const&&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void(int, float, ...)>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, float, ...) const>::value);

	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, float, ...)&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, float, ...) const&>::value);

	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, float, ...) &&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, float, ...) const&&>::value);


	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void(int, float, Foo&, ...)>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(int, float, Foo&, ...)
					   const>::value);

	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, float, Foo&, ...)&>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(int, float, Foo&, ...)
					   const&>::value);

	STATIC_REQUIRE(
		!Rider::Faiz::is_referenceable<void(int, float, Foo&, ...) &&>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_referenceable<void(int, float, Foo&, ...)
					   const&&>::value);


	// member functions with or without cv-qualifiers are referenceable
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)()>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)() const>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)()&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)() const&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)() &&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)() const&&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(int)>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int) const>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(int)&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int) const&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(int) &&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int) const&&>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int, float)>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int, float) const>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int, float)&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int, float) const&>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int, float) &&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(int, float)
			const&&>::value);


	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int, float, Foo&)>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(int, float, Foo&)
			const>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int, float, Foo&)&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(int, float, Foo&)
			const&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(int, float, Foo&)
		&&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(int, float, Foo&)
			const&&>::value);


	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(...)>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(...) const>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(...)&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(...) const&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(...) &&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(...) const&&>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int, ...)>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int, ...) const>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int, ...)&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int, ...) const&>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int, ...) &&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int, ...) const&&>::value);


	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int, float, ...)>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(int, float, ...)
			const>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int, float, ...)&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(int, float, ...)
			const&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(int, float, ...)
		&&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(int, float, ...)
			const&&>::value);


	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(
			int, float, Foo&, ...)>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(
			int, float, Foo&, ...) const>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(
			int, float, Foo&, ...)&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(
			int, float, Foo&, ...) const&>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_referenceable<void (Foo::*)(int, float, Foo&, ...)
			&&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_referenceable<void (Foo::*)(
			int, float, Foo&, ...) const&&>::value);
}
