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
#include <catch.hpp>
#include <memory>
#include <type_traits>
#include <utility>

using namespace ImplicitTypes; // Get implicitly archetypes

namespace
{
	template<typename T1,
		typename U1,
		bool CanCopy = true,
		bool CanConvert = CanCopy>
	void
	test_pair_rv()
	{
		using P1 = Rider::Faiz::tight_pair<T1, int>;
		using P2 = Rider::Faiz::tight_pair<int, T1>;
		using UP1 = Rider::Faiz::tight_pair<U1, int>&&;
		using UP2 = Rider::Faiz::tight_pair<int, U1>&&;
		static_assert(std::is_constructible_v<P1, UP1> == CanCopy);
		static_assert(test_convertible<P1, UP1>() == CanConvert);
		static_assert(std::is_constructible_v<P2, UP2> == CanCopy);
		static_assert(test_convertible<P2, UP2>() == CanConvert);
	}

	struct Base
	{
		virtual ~Base()
		{}
	};

	struct Derived : Base
	{};


	template<typename T, typename U>
	struct DPair : public Rider::Faiz::tight_pair<T, U>
	{
		using Base = Rider::Faiz::tight_pair<T, U>;
		using Base::Base;
	};

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

TEST_CASE("rv pair U V")
{
	using Rider::Faiz::get;

	{
		using P1 = Rider::Faiz::tight_pair<std::unique_ptr<Derived>, short>;
		using P2 = Rider::Faiz::tight_pair<std::unique_ptr<Base>, long>;
		P1 p1(std::unique_ptr<Derived>(), 4);
		P2 p2 = std::move(p1);
		CHECK(get<0>(p2) == nullptr);
		CHECK(get<1>(p2) == 4);
	}
	{
		// We allow derived types to use this constructor
		using P1 = DPair<long, long>;
		using P2 = Rider::Faiz::tight_pair<int, int>;
		P1 p1(42, 101);
		P2 p2(std::move(p1));
		CHECK(get<0>(p2) == 42);
		CHECK(get<1>(p2) == 101);
	}
	{
		test_pair_rv<AllCtors, AllCtors>();
		test_pair_rv<AllCtors, AllCtors&>();
		test_pair_rv<AllCtors, AllCtors&&>();
		test_pair_rv<AllCtors, const AllCtors&>();
		test_pair_rv<AllCtors, const AllCtors&&>();

		test_pair_rv<ExplicitTypes::AllCtors, ExplicitTypes::AllCtors>();
		test_pair_rv<ExplicitTypes::AllCtors,
			ExplicitTypes::AllCtors&,
			true,
			false>();
		test_pair_rv<ExplicitTypes::AllCtors,
			ExplicitTypes::AllCtors&&,
			true,
			false>();
		test_pair_rv<ExplicitTypes::AllCtors,
			const ExplicitTypes::AllCtors&,
			true,
			false>();
		test_pair_rv<ExplicitTypes::AllCtors,
			const ExplicitTypes::AllCtors&&,
			true,
			false>();

		test_pair_rv<MoveOnly, MoveOnly>();
		test_pair_rv<MoveOnly, MoveOnly&, false>();
		test_pair_rv<MoveOnly, MoveOnly&&>();

		test_pair_rv<ExplicitTypes::MoveOnly,
			ExplicitTypes::MoveOnly>(); // copy construction
		test_pair_rv<ExplicitTypes::MoveOnly, ExplicitTypes::MoveOnly&, false>();
		test_pair_rv<ExplicitTypes::MoveOnly,
			ExplicitTypes::MoveOnly&&,
			true,
			false>();

		test_pair_rv<CopyOnly, CopyOnly>();
		test_pair_rv<CopyOnly, CopyOnly&>();
		test_pair_rv<CopyOnly, CopyOnly&&>();

		test_pair_rv<ExplicitTypes::CopyOnly, ExplicitTypes::CopyOnly>();
		test_pair_rv<ExplicitTypes::CopyOnly,
			ExplicitTypes::CopyOnly&,
			true,
			false>();
		test_pair_rv<ExplicitTypes::CopyOnly,
			ExplicitTypes::CopyOnly&&,
			true,
			false>();

		test_pair_rv<NonCopyable, NonCopyable, false>();
		test_pair_rv<NonCopyable, NonCopyable&, false>();
		test_pair_rv<NonCopyable, NonCopyable&&, false>();
		test_pair_rv<NonCopyable, const NonCopyable&, false>();
		test_pair_rv<NonCopyable, const NonCopyable&&, false>();
	}
	{ // Test construction of references
		test_pair_rv<NonCopyable&, NonCopyable&>();
		test_pair_rv<NonCopyable&, NonCopyable&&>();
		test_pair_rv<NonCopyable&, NonCopyable const&, false>();
		test_pair_rv<NonCopyable const&, NonCopyable&&>();
		test_pair_rv<NonCopyable&&, NonCopyable&&>();

		test_pair_rv<ConvertingType&, int, false>();
		test_pair_rv<ExplicitTypes::ConvertingType&, int, false>();
		// Unfortunately the below conversions are allowed and create dangling
		// references.
		// test_pair_rv<ConvertingType&&, int>();
		// test_pair_rv<ConvertingType const&, int>();
		// test_pair_rv<ConvertingType const&&, int>();
		// But these are not because the converting constructor is explicit.
		test_pair_rv<ExplicitTypes::ConvertingType&&, int, false>();
		test_pair_rv<ExplicitTypes::ConvertingType const&, int, false>();
		test_pair_rv<ExplicitTypes::ConvertingType const&&, int, false>();
	}
	{
		test_pair_rv<AllCtors, int, false>();
		test_pair_rv<ExplicitTypes::AllCtors, int, false>();
		test_pair_rv<ConvertingType, int>();
		test_pair_rv<ExplicitTypes::ConvertingType, int, true, false>();

		test_pair_rv<ConvertingType, int>();
		test_pair_rv<ConvertingType, ConvertingType>();
		test_pair_rv<ConvertingType, ConvertingType const&>();
		test_pair_rv<ConvertingType, ConvertingType&>();
		test_pair_rv<ConvertingType, ConvertingType&&>();

		test_pair_rv<ExplicitTypes::ConvertingType, int, true, false>();
		test_pair_rv<ExplicitTypes::ConvertingType, int&, true, false>();
		test_pair_rv<ExplicitTypes::ConvertingType, const int&, true, false>();
		test_pair_rv<ExplicitTypes::ConvertingType, int&&, true, false>();
		test_pair_rv<ExplicitTypes::ConvertingType, const int&&, true, false>();

		test_pair_rv<ExplicitTypes::ConvertingType,
			ExplicitTypes::ConvertingType>();
		test_pair_rv<ExplicitTypes::ConvertingType,
			ExplicitTypes::ConvertingType const&,
			true,
			false>();
		test_pair_rv<ExplicitTypes::ConvertingType,
			ExplicitTypes::ConvertingType&,
			true,
			false>();
		test_pair_rv<ExplicitTypes::ConvertingType,
			ExplicitTypes::ConvertingType&&,
			true,
			false>();
	}
	{ // explicit constexpr test
		constexpr Rider::Faiz::tight_pair<int, int> p1(42, 43);
		constexpr Rider::Faiz::tight_pair<ExplicitT, ExplicitT> p2(
			std::move(p1));
		static_assert(get<0>(p2).value == 42);
		static_assert(get<1>(p2).value == 43);
	}
	{ // implicit constexpr test
		constexpr Rider::Faiz::tight_pair<int, int> p1(42, 43);
		constexpr Rider::Faiz::tight_pair<ImplicitT, ImplicitT> p2
			= std::move(p1);
		static_assert(get<0>(p2).value == 42);
		static_assert(get<1>(p2).value == 43);
	}
}

TEST_CASE("rv pair U V noexcept")
{
	{ // explicit noexcept test
		static_assert(not std::is_nothrow_constructible_v<
					  Rider::Faiz::tight_pair<ExplicitT, ExplicitT>,
					  Rider::Faiz::tight_pair<int, int>&&>);
		static_assert(not std::is_nothrow_constructible_v<
					  Rider::Faiz::tight_pair<ExplicitNothrowT, ExplicitT>,
					  Rider::Faiz::tight_pair<int, int>&&>);
		static_assert(not std::is_nothrow_constructible_v<
					  Rider::Faiz::tight_pair<ExplicitT, ExplicitNothrowT>,
					  Rider::Faiz::tight_pair<int, int>&&>);
		static_assert(std::is_nothrow_constructible_v<
			Rider::Faiz::tight_pair<ExplicitNothrowT, ExplicitNothrowT>,
			Rider::Faiz::tight_pair<int, int>&&>);
	}
	{ // implicit noexcept test
		static_assert(not std::is_nothrow_constructible_v<
					  Rider::Faiz::tight_pair<ImplicitT, ImplicitT>,
					  Rider::Faiz::tight_pair<int, int>&&>);
		static_assert(not std::is_nothrow_constructible_v<
					  Rider::Faiz::tight_pair<ImplicitNothrowT, ImplicitT>,
					  Rider::Faiz::tight_pair<int, int>&&>);
		static_assert(not std::is_nothrow_constructible_v<
					  Rider::Faiz::tight_pair<ImplicitT, ImplicitNothrowT>,
					  Rider::Faiz::tight_pair<int, int>&&>);
		static_assert(std::is_nothrow_constructible_v<
			Rider::Faiz::tight_pair<ImplicitNothrowT, ImplicitNothrowT>,
			Rider::Faiz::tight_pair<int, int>&&>);
	}
}
