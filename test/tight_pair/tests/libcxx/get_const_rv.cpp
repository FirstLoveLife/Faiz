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
#include "rider/faiz/compressed_pair.hpp"
#include <catch2/catch.hpp>
#include <memory>
#include <type_traits>

TEST_CASE("get const rv")
{
	{
		using P = Rider::Faiz::tight_pair<std::unique_ptr<int>, short>;
		const P p(std::unique_ptr<int>(new int(3)), static_cast<short>(4));
		static_assert(std::is_same_v<const std::unique_ptr<int>&&,
			decltype(Rider::Faiz::get<0>(std::move(p)))>);
		static_assert(noexcept(Rider::Faiz::get<0>(std::move(p))));
		const std::unique_ptr<int>&& ptr = Rider::Faiz::get<0>(std::move(p));
		CHECK(*ptr == 3);
	}

	{
		int x = 42;
		int const y = 43;
		Rider::Faiz::tight_pair<int&, int const&> const p(x, y);
		static_assert(
			std::is_same_v<int&, decltype(Rider::Faiz::get<0>(std::move(p)))>);
		static_assert(noexcept(Rider::Faiz::get<0>(std::move(p))));
		static_assert(std::is_same_v<int const&,
			decltype(Rider::Faiz::get<1>(std::move(p)))>);
		static_assert(noexcept(Rider::Faiz::get<1>(std::move(p))));
	}

	{
		int x = 42;
		int const y = 43;
		Rider::Faiz::tight_pair<int&&, int const&&> const p(
			std::move(x), std::move(y));
		static_assert(
			std::is_same_v<int&&, decltype(Rider::Faiz::get<0>(std::move(p)))>);
		static_assert(noexcept(Rider::Faiz::get<0>(std::move(p))));
		static_assert(std::is_same_v<int const&&,
			decltype(Rider::Faiz::get<1>(std::move(p)))>);
		static_assert(noexcept(Rider::Faiz::get<1>(std::move(p))));
	}

	{
		using P = Rider::Faiz::tight_pair<int, short>;
		constexpr const P p1(3, static_cast<short>(4));
		static_assert(Rider::Faiz::get<0>(std::move(p1)) == 3);
		static_assert(Rider::Faiz::get<1>(std::move(p1)) == 4);
	}
}
