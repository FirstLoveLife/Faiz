/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "archetypes.h"
#include "rider/faiz/compressed_pair.hpp"
#include "test_convertible.h"
#include <catch2/catch.hpp>
#include <type_traits>
using namespace ImplicitTypes; // Get implicitly archetypes

namespace
{
	template<typename T1,
		typename T1Arg,
		bool CanCopy = true,
		bool CanConvert = CanCopy>
	void
	test_sfinae()
	{
		using P1 = Rider::Faiz::tight_pair<T1, int>;
		using P2 = Rider::Faiz::tight_pair<int, T1>;
		using T2 = int const&;
		static_assert(std::is_constructible_v<P1, T1Arg, T2> == CanCopy);
		static_assert(test_convertible<P1, T1Arg, T2>() == CanConvert);
		static_assert(std::is_constructible_v<P2, T2, T1Arg> == CanCopy);
		static_assert(test_convertible<P2, T2, T1Arg>() == CanConvert);
	}

	struct ExplicitT
	{
		constexpr explicit ExplicitT(int x) : value(x)
		{}
		int value;
	};

	struct ImplicitT
	{
		constexpr ImplicitT(int x) : value(x)
		{}
		int value;
	};

	struct ExplicitNothrowT
	{
		explicit ExplicitNothrowT(int x) noexcept : value(x)
		{}
		int value;
	};

	struct ImplicitNothrowT
	{
		ImplicitNothrowT(int x) noexcept : value(x)
		{}
		int value;
	};
} // namespace

TEST_CASE("test U V")
{
	using Rider::Faiz::get;

	{
		using P = Rider::Faiz::tight_pair<std::unique_ptr<int>, short*>;
		P p(std::unique_ptr<int>(new int(3)), nullptr);
		CHECK(*get<0>(p) == 3);
		CHECK(get<1>(p) == nullptr);
	}
	{
		// Test non-const lvalue and rvalue types
		test_sfinae<AllCtors, AllCtors&>();
		test_sfinae<AllCtors, AllCtors&&>();
		test_sfinae<ExplicitTypes::AllCtors,
			ExplicitTypes::AllCtors&,
			true,
			false>();
		test_sfinae<ExplicitTypes::AllCtors,
			ExplicitTypes::AllCtors&&,
			true,
			false>();
		test_sfinae<CopyOnly, CopyOnly&>();
		test_sfinae<CopyOnly, CopyOnly&&>();
		test_sfinae<ExplicitTypes::CopyOnly,
			ExplicitTypes::CopyOnly&,
			true,
			false>();
		test_sfinae<ExplicitTypes::CopyOnly,
			ExplicitTypes::CopyOnly&&,
			true,
			false>();
		test_sfinae<MoveOnly, MoveOnly&, false>();
		test_sfinae<MoveOnly, MoveOnly&&>();
		test_sfinae<ExplicitTypes::MoveOnly, ExplicitTypes::MoveOnly&, false>();
		test_sfinae<ExplicitTypes::MoveOnly,
			ExplicitTypes::MoveOnly&&,
			true,
			false>();
		test_sfinae<NonCopyable, NonCopyable&, false>();
		test_sfinae<NonCopyable, NonCopyable&&, false>();
		test_sfinae<ExplicitTypes::NonCopyable,
			ExplicitTypes::NonCopyable&,
			false>();
		test_sfinae<ExplicitTypes::NonCopyable,
			ExplicitTypes::NonCopyable&&,
			false>();
	}
	{
		// Test converting types
		test_sfinae<ConvertingType, int&>();
		test_sfinae<ConvertingType, const int&>();
		test_sfinae<ConvertingType, int&&>();
		test_sfinae<ConvertingType, const int&&>();
		test_sfinae<ExplicitTypes::ConvertingType, int&, true, false>();
		test_sfinae<ExplicitTypes::ConvertingType, const int&, true, false>();
		test_sfinae<ExplicitTypes::ConvertingType, int&&, true, false>();
		test_sfinae<ExplicitTypes::ConvertingType, const int&&, true, false>();
	}
	{ // explicit constexpr test
		constexpr Rider::Faiz::tight_pair<ExplicitT, ExplicitT> p(42, 43);
		static_assert(get<0>(p).value == 42);
		static_assert(get<1>(p).value == 43);
	}
	{ // implicit constexpr test
		constexpr Rider::Faiz::tight_pair<ImplicitT, ImplicitT> p = {42, 43};
		static_assert(get<0>(p).value == 42);
		static_assert(get<1>(p).value == 43);
	}
}

TEST_CASE("test U V noexcept")
{
	{ // explicit noexcept test
		static_assert(not std::is_nothrow_constructible_v<
					  Rider::Faiz::tight_pair<ExplicitT, ExplicitT>,
					  int,
					  int>);
		static_assert(not std::is_nothrow_constructible_v<
					  Rider::Faiz::tight_pair<ExplicitNothrowT, ExplicitT>,
					  int,
					  int>);
		static_assert(not std::is_nothrow_constructible_v<
					  Rider::Faiz::tight_pair<ExplicitT, ExplicitNothrowT>,
					  int,
					  int>);
		static_assert(std::is_nothrow_constructible_v<
			Rider::Faiz::tight_pair<ExplicitNothrowT, ExplicitNothrowT>,
			int,
			int>);
	}
	{ // implicit noexcept test
		static_assert(not std::is_nothrow_constructible_v<
					  Rider::Faiz::tight_pair<ImplicitT, ImplicitT>,
					  int,
					  int>);
		static_assert(not std::is_nothrow_constructible_v<
					  Rider::Faiz::tight_pair<ImplicitNothrowT, ImplicitT>,
					  int,
					  int>);
		static_assert(not std::is_nothrow_constructible_v<
					  Rider::Faiz::tight_pair<ImplicitT, ImplicitNothrowT>,
					  int,
					  int>);
		static_assert(std::is_nothrow_constructible_v<
			Rider::Faiz::tight_pair<ImplicitNothrowT, ImplicitNothrowT>,
			int,
			int>);
	}
}
