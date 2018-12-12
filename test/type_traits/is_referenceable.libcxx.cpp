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

#include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"
#include <cassert>

struct Foo
{};


int
main()
{
	static_assert((!Rider::Faiz::is_referenceable<void>::value), "");
	static_assert((Rider::Faiz::is_referenceable<int>::value), "");
	static_assert((Rider::Faiz::is_referenceable<int[3]>::value), "");
	static_assert((Rider::Faiz::is_referenceable<int[]>::value), "");
	static_assert((Rider::Faiz::is_referenceable<int&>::value), "");
	static_assert((Rider::Faiz::is_referenceable<const int&>::value), "");
	static_assert((Rider::Faiz::is_referenceable<int*>::value), "");
	static_assert((Rider::Faiz::is_referenceable<const int*>::value), "");
	static_assert((Rider::Faiz::is_referenceable<Foo>::value), "");
	static_assert((Rider::Faiz::is_referenceable<const Foo>::value), "");
	static_assert((Rider::Faiz::is_referenceable<Foo&>::value), "");
	static_assert((Rider::Faiz::is_referenceable<const Foo&>::value), "");
	static_assert((Rider::Faiz::is_referenceable<Foo&&>::value), "");
	static_assert((Rider::Faiz::is_referenceable<const Foo&&>::value), "");

	static_assert((Rider::Faiz::is_referenceable<int
					  __attribute__((__vector_size__(8)))>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<const int
					  __attribute__((__vector_size__(8)))>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<float
					  __attribute__((__vector_size__(16)))>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<const float
					  __attribute__((__vector_size__(16)))>::value),
		"");

	// Functions without cv-qualifiers are referenceable
	static_assert((Rider::Faiz::is_referenceable<void()>::value), "");
	static_assert((!Rider::Faiz::is_referenceable<void() const>::value), "");
	static_assert((!Rider::Faiz::is_referenceable<void()&>::value), "");
	static_assert((!Rider::Faiz::is_referenceable<void() const&>::value), "");
	static_assert((!Rider::Faiz::is_referenceable<void() &&>::value), "");
	static_assert((!Rider::Faiz::is_referenceable<void() const&&>::value), "");

	static_assert((Rider::Faiz::is_referenceable<void(int)>::value), "");
	static_assert((!Rider::Faiz::is_referenceable<void(int) const>::value), "");
	static_assert((!Rider::Faiz::is_referenceable<void(int)&>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int) const&>::value), "");
	static_assert((!Rider::Faiz::is_referenceable<void(int) &&>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int) const&&>::value), "");

	static_assert((Rider::Faiz::is_referenceable<void(int, float)>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float) const>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float)&>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float) const&>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float) &&>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float) const&&>::value), "");

	static_assert(
		(Rider::Faiz::is_referenceable<void(int, float, Foo&)>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float, Foo&) const>::value),
		"");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float, Foo&)&>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float, Foo&) const&>::value),
		"");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float, Foo&) &&>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float, Foo&) const&&>::value),
		"");

	static_assert((Rider::Faiz::is_referenceable<void(...)>::value), "");
	static_assert((!Rider::Faiz::is_referenceable<void(...) const>::value), "");
	static_assert((!Rider::Faiz::is_referenceable<void(...)&>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(...) const&>::value), "");
	static_assert((!Rider::Faiz::is_referenceable<void(...) &&>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(...) const&&>::value), "");

	static_assert((Rider::Faiz::is_referenceable<void(int, ...)>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, ...) const>::value), "");
	static_assert((!Rider::Faiz::is_referenceable<void(int, ...)&>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, ...) const&>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, ...) &&>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, ...) const&&>::value), "");

	static_assert(
		(Rider::Faiz::is_referenceable<void(int, float, ...)>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float, ...) const>::value),
		"");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float, ...)&>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float, ...) const&>::value),
		"");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float, ...) &&>::value), "");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float, ...) const&&>::value),
		"");

	static_assert(
		(Rider::Faiz::is_referenceable<void(int, float, Foo&, ...)>::value),
		"");
	static_assert((!Rider::Faiz::is_referenceable<void(int, float, Foo&, ...)
						  const>::value),
		"");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float, Foo&, ...)&>::value),
		"");
	static_assert((!Rider::Faiz::is_referenceable<void(int, float, Foo&, ...)
						  const&>::value),
		"");
	static_assert(
		(!Rider::Faiz::is_referenceable<void(int, float, Foo&, ...) &&>::value),
		"");
	static_assert((!Rider::Faiz::is_referenceable<void(int, float, Foo&, ...)
						  const&&>::value),
		"");

	// member functions with or without cv-qualifiers are referenceable
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)()>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)() const>::value), "");
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)()&>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)() const&>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)() &&>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)() const&&>::value), "");

	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int)>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int) const>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int)&>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int) const&>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int) &&>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int) const&&>::value), "");

	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int, float)>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int, float) const>::value),
		"");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int, float)&>::value), "");
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)(int, float)
						  const&>::value),
		"");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int, float) &&>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)(int, float)
						  const&&>::value),
		"");

	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int, float, Foo&)>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)(int, float, Foo&)
						  const>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)(
						  int, float, Foo&)&>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)(int, float, Foo&)
						  const&>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)(int, float, Foo&)
					  &&>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)(int, float, Foo&)
						  const&&>::value),
		"");

	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(...)>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(...) const>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(...)&>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(...) const&>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(...) &&>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(...) const&&>::value), "");

	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int, ...)>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int, ...) const>::value),
		"");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int, ...)&>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int, ...) const&>::value),
		"");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int, ...) &&>::value), "");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int, ...) const&&>::value),
		"");

	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int, float, ...)>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)(int, float, ...)
						  const>::value),
		"");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int, float, ...)&>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)(int, float, ...)
						  const&>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)(int, float, ...)
					  &&>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)(int, float, ...)
						  const&&>::value),
		"");

	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)(
						  int, float, Foo&, ...)>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)(
						  int, float, Foo&, ...) const>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)(
						  int, float, Foo&, ...)&>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)(
						  int, float, Foo&, ...) const&>::value),
		"");
	static_assert(
		(Rider::Faiz::is_referenceable<void (Foo::*)(int, float, Foo&, ...)
			&&>::value),
		"");
	static_assert((Rider::Faiz::is_referenceable<void (Foo::*)(
						  int, float, Foo&, ...) const&&>::value),
		"");
}
