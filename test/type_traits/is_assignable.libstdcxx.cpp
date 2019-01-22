// { dg-do compile { target c++11 } }

// Copyright (C) 2011-2018 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include "rider/faiz/type_traits.hpp"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>
#include <initializer_list>
using namespace gnu_assign;
TEST_CASE("is_assignable.libstdcxx: ")
{
	STATIC_REQUIRE(Rider::Faiz::is_assignable<int&, const int>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<int&, int&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<int&, const int&>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int, int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int, const int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int, int&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int, const int&>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const int, int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const int, const int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const int, int&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const int, const int&>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const int&, int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const int&, const int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const int&, int&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const int&, const int&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<Empty&, Empty>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Empty&, const Empty>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Empty&, Empty&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Empty&, const Empty&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Empty, Empty>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Empty, const Empty>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Empty, Empty&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Empty, const Empty&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<B&, B>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<B&, const B>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<B&, B&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<B&, const B&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<B, B>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<B, const B>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<B, B&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<B, const B&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<bool&, bool>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<bool&, const bool>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<bool&, bool&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<bool&, const bool&>::value);

	// bool is a fundamental type that does not allow assignment to an rvalue:
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<bool, bool>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<bool, const bool>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<bool, bool&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<bool, const bool&>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<std::nullptr_t&, std::nullptr_t>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<std::nullptr_t&,
		const std::nullptr_t>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<std::nullptr_t&, std::nullptr_t&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<std::nullptr_t&,
		const std::nullptr_t&>::value);

	// std::nullptr_t is a fundamental type that does not allow
	// assignment to an rvalue:
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<std::nullptr_t, std::nullptr_t>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<std::nullptr_t,
				   const std::nullptr_t>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<std::nullptr_t, std::nullptr_t&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<std::nullptr_t,
				   const std::nullptr_t&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<E&, E>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<E&, const E>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<E&, E&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<E&, const E&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<int&, E>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<int&, const E>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<int&, E&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<int&, const E&>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<E&, int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<E&, const int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<E&, int&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<E&, const int&>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<E&, E2>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<E&, const E2>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<E&, E2&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<E&, const E2&>::value);

	// E is not a class type and thus does not allow assignment to an rvalue:
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<E, E>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<E, const E>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<E, E&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<E, const E&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<SE&, SE>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<SE&, const SE>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<SE&, SE&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<SE&, const SE&>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int&, SE>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int&, const SE>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int&, SE&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int&, const SE&>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<SE&, int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<SE&, const int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<SE&, int&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<SE&, const int&>::value);

	// SE is not a class type and thus does not allow assignment to an rvalue:
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<SE, SE>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<SE, const SE>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<SE, SE&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<SE, const SE&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, int>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<AnyAssign&, std::nullptr_t>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, E>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, SE>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, Empty>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, U>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, int&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<AnyAssign&, std::nullptr_t&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, E&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, SE&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, Empty&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, U&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, AnyAssign>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&,
		std::initializer_list<int>>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, int[1]>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<AnyAssign&, std::nullptr_t[1]>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, E[1]>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, SE[1]>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, int(&)[1]>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<AnyAssign&, std::nullptr_t(&)[1]>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, E(&)[1]>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AnyAssign&, SE(&)[1]>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<int&, E>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int&, SE>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<bool&, E>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<bool&, SE>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<bool&, void*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<bool&, int B::*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<bool&, void*>::value);
	// TODO:
	// https://stackoverflow.com/questions/54300586/is-stddeclvalbool-stddeclvalstdnullptr-t-well-formed
	//  STATIC_REQUIRE(!Rider::Faiz::is_assignable<bool&,
	//  std::nullptr_t>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<std::nullptr_t&, std::nullptr_t>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<void*&, std::nullptr_t>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<int*&, std::nullptr_t>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<int B::*&, std::nullptr_t>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<std::nullptr_t&, bool>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void*&, bool>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<E&, bool>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<SE&, bool>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<std::initializer_list<int>&,
		std::initializer_list<int>>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<std::initializer_list<int>&,
		std::initializer_list<int>&&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<std::initializer_list<int>&,
		const std::initializer_list<int>&&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<std::initializer_list<int>&,
		std::initializer_list<int>&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<std::initializer_list<int>&,
		const std::initializer_list<int>&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<const std::initializer_list<int>&,
			std::initializer_list<int>>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const AnyAssign&, int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<AnyAssign&, void>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void, int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const void, int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int, void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int, const void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const int, void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const int, const void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int&, void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int&, const void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const int&, void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const int&, const void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void, void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const void, void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const void, const void>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int[1], int[1]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int(&)[1], int[1]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int(&)[1], int(&)[1]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int[2], int[1]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int(&)[2], int[1]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int(&)[2], int(&)[1]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int[1], void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int(&)[1], void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void, int[1]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void, int(&)[1]>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int[], int[]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int(&)[], int[]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int(&)[], int(&)[]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int[1], int[]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int(&)[1], int[]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int(&)[1], int(&)[]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int[], int[1]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int(&)[], int[1]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int(&)[], int(&)[1]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int[], void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int(&)[], void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void, int[]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void, int(&)[]>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<DelCopyAssign&, DelCopyAssign>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelCopyAssign&,
				   const DelCopyAssign>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelCopyAssign&, DelCopyAssign&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelCopyAssign&,
				   const DelCopyAssign&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelCopyAssign&, void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelCopyAssign&, void()>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelCopyAssign&, void (&)()>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelCopyAssign&, int>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<DelAnyAssign&, DelAnyAssign&&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<DelAnyAssign&, const DelAnyAssign&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<DelAnyAssign, DelAnyAssign&&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<DelAnyAssign, const DelAnyAssign&>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const DelAnyAssign&,
				   DelAnyAssign&&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const DelAnyAssign&,
				   const DelAnyAssign&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<const DelAnyAssign, DelAnyAssign&&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const DelAnyAssign,
				   const DelAnyAssign&>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, int&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelAnyAssign&, const int&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, void()>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelAnyAssign&, void() const>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelAnyAssign&, void (&)()>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelAnyAssign&, void(&&)()>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelAnyAssign&, std::nullptr_t>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelAnyAssign&, std::nullptr_t&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&,
				   std::initializer_list<int>>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&,
				   std::initializer_list<int>&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, bool>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, bool&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, E>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, E&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, SE>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, SE&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, Empty>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, Empty&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, B>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, B&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, U>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, U&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, void*>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, int*>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, B*>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, D*>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, int B::*>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, int D::*>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, int[]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, int[1]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelAnyAssign&, int(&)[]>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelAnyAssign&, int(&)[1]>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void(), void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void, void()>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void(), void()>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void (&)(), void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void, void (&)()>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void (&)(), void (&)()>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void (&)(), void()>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void(), void (&)()>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<int&, ImplicitTo<int>>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int&, ExplicitTo<int>>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int, ImplicitTo<int>>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int, ExplicitTo<int>>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<const int, ImplicitTo<int>>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<const int, ExplicitTo<int>>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<const int&, ImplicitTo<int>>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<const int&, ExplicitTo<int>>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<DelImplicitTo<int>&,
		DelImplicitTo<int>>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<DelImplicitTo<int>,
		DelImplicitTo<int>>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<int&, DelImplicitTo<int>>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int, DelImplicitTo<int>>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<const int&, DelImplicitTo<int>>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<const int, DelImplicitTo<int>>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<int&, DelExplicitTo<int>>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int, DelExplicitTo<int>>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<const int&, DelExplicitTo<int>>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<const int, DelExplicitTo<int>>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<B&, B>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<B&, D>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<B&, B&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<B&, D&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const B&, B&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const B&, D&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<D&, B>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<D&, B&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<B*&, B*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<B*&, D*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<const B*&, D*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<const B*&, const D*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<B*&, B*&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<B*&, D*&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<const B*&, B*&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<const B*&, D*&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<B* const&, B*&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<B* const&, D*&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<D*&, B*>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<D*&, B*&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<MO&, MO>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<MO&, MO&&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<MO, MO>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<MO, MO&&>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const MO&, MO>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const MO&, MO&&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<MO&, const MO&&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<MO&, MO&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<MO&, const MO&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const MO, MO>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const MO, MO&&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<MO, const MO&&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<MO, MO&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<MO, const MO&>::value);

	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<NontrivialUnion&, NontrivialUnion>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<NontrivialUnion&,
				   NontrivialUnion&&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<NontrivialUnion&, NontrivialUnion&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<NontrivialUnion&,
				   const NontrivialUnion&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<NontrivialUnion&,
				   const NontrivialUnion&&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<Abstract&, Abstract>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Abstract&, Abstract&&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Abstract&, Abstract&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<Abstract&, const Abstract&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<Abstract&, const Abstract&&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Abstract&&, Abstract>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Abstract&&, Abstract&&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Abstract&&, Abstract&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<Abstract&&, const Abstract&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<Abstract&&, const Abstract&&>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<AbstractDelDtor&, AbstractDelDtor>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<AbstractDelDtor&, AbstractDelDtor&&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<AbstractDelDtor&, AbstractDelDtor&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AbstractDelDtor&,
		const AbstractDelDtor&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AbstractDelDtor&,
		const AbstractDelDtor&&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<AbstractDelDtor&&, AbstractDelDtor>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AbstractDelDtor&&,
		AbstractDelDtor&&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<AbstractDelDtor&&, AbstractDelDtor&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AbstractDelDtor&&,
		const AbstractDelDtor&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<AbstractDelDtor&&,
		const AbstractDelDtor&&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<DelDef&, DelDef>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<DelDef&, DelDef&&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<DelDef&, DelDef&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<DelDef&, const DelDef&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<DelDef&, const DelDef&&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<DelDef&&, DelDef>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<DelDef&&, DelDef&&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<DelDef&&, DelDef&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<DelDef&&, const DelDef&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<DelDef&&, const DelDef&&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, Ellipsis>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<Ellipsis&, const Ellipsis>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, Ellipsis&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<Ellipsis&, const Ellipsis&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis, Ellipsis>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis, const Ellipsis>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis, Ellipsis&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<Ellipsis, const Ellipsis&>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<Ellipsis&, void>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, int>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, const int>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, int&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, const int&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, Empty>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, const Empty>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, Empty&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, const Empty&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, E>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, const E>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, E&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, const E&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, SE>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, const SE>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, SE&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, const SE&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, bool>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, const bool>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, bool&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, const bool&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<Ellipsis&, std::nullptr_t>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<Ellipsis&, const std::nullptr_t>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<Ellipsis&, std::nullptr_t&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<Ellipsis&, const std::nullptr_t&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, void*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, const void*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, void*&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, const void*&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, void()>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<Ellipsis&, void (&)()>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<DelEllipsis&, DelEllipsis>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<DelEllipsis&, const DelEllipsis>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<DelEllipsis&, DelEllipsis&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<DelEllipsis&, const DelEllipsis&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<DelEllipsis, DelEllipsis>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<DelEllipsis, const DelEllipsis>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<DelEllipsis, DelEllipsis&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<DelEllipsis, const DelEllipsis&>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, const int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, int&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelEllipsis&, const int&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, Empty>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelEllipsis&, const Empty>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, Empty&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelEllipsis&, const Empty&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, E>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, const E>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, E&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, const E&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, SE>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, const SE>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, SE&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, const SE&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, bool>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelEllipsis&, const bool>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, bool&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelEllipsis&, const bool&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelEllipsis&, std::nullptr_t>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelEllipsis&, const std::nullptr_t>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelEllipsis&, std::nullptr_t&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&,
				   const std::nullptr_t&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, void*>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelEllipsis&, const void*>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, void*&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelEllipsis&, const void*&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelEllipsis&, void()>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelEllipsis&, void (&)()>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<FromArgs<int>&, int>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<FromArgs<int>&, const int>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<FromArgs<int>&, ImplicitTo<int>>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<FromArgs<int>&,
				   ImplicitTo<const int>>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<FromArgs<int>&, ExplicitTo<int>>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<FromArgs<int>&,
				   ExplicitTo<const int>>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<DelFromArgs<int>&, int>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<DelFromArgs<int>&, const int>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<void (*&)(), ImplicitTo<void (*)()>>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void (*&)(),
				   ExplicitTo<void (*)()>>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, UAssignAll>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<UAssignAll&, const UAssignAll>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, UAssignAll&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<UAssignAll&, const UAssignAll&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll, UAssignAll>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<UAssignAll, const UAssignAll>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll, UAssignAll&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<UAssignAll, const UAssignAll&>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UAssignAll&, void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const UAssignAll&, void>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<const UAssignAll&, UAssignAll>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const UAssignAll&,
				   const UAssignAll>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<const UAssignAll&, UAssignAll&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const UAssignAll&,
				   const UAssignAll&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<UAssignAll&, void() const>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UAssignAll&, void()&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UAssignAll&,
				   void() const volatile&&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, int>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, int&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, E>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, E&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, SE>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, SE&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, double>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, double&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, Empty>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, Empty&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, B>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, B&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, U>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, U&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<UAssignAll&, std::nullptr_t>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<UAssignAll&, std::nullptr_t&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, void()>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, void (&)()>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, void (*)()>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, void (*&)()>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, int*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, int*&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, void*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, void*&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, const int*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, const int*&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, const void*>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_assignable<UAssignAll&, const void*&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, int[1]>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, int(&)[1]>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, int[]>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<UAssignAll&, int(&)[]>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, int>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, int&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, E>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, E&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, SE>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, SE&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, double>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, double&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, Empty>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, Empty&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, B>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, B&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, U>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, U&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<UDelAssignAll&, std::nullptr_t>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<UDelAssignAll&, std::nullptr_t&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, void()>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<UDelAssignAll&, void (&)()>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<UDelAssignAll&, void() const>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<UDelAssignAll&, void (*)()>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<UDelAssignAll&, void (*&)()>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, int*>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, int*&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, void*>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, void*&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<UDelAssignAll&, const int*>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<UDelAssignAll&, const int*&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<UDelAssignAll&, const void*>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<UDelAssignAll&, const void*&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, int[1]>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<UDelAssignAll&, int(&)[1]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<UDelAssignAll&, int[]>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<UDelAssignAll&, int(&)[]>::value);

	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<void (&)(), std::nullptr_t>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_assignable<std::nullptr_t, void (&)()>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void (&)(), int[]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int[], void (&)()>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int[], std::nullptr_t>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<std::nullptr_t, int[]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<int[1], std::nullptr_t>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<std::nullptr_t, int[1]>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<void, std::nullptr_t>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<std::nullptr_t, void>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const D&, B&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_assignable<const B&, B&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_assignable<B&, const D&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<B&, const B&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<int&, const int&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_assignable<int&, const double&>::value);
}
