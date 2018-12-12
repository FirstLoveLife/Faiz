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
#include <catch.hpp>
#include <complex>
#include <memory>
#include <utility>

TEST_CASE("pairs by type")
{
	using cf = std::complex<float>;
	{
		auto t1 = Rider::Faiz::tight_pair<int, cf>(42, {1, 2});
		CHECK(Rider::Faiz::get<int>(t1) == 42);
		CHECK(Rider::Faiz::get<cf>(t1).real() == 1);
		CHECK(Rider::Faiz::get<cf>(t1).imag() == 2);
	}

	{
		const Rider::Faiz::tight_pair<int, const int> p1{1, 2};
		const int& i1 = Rider::Faiz::get<int>(p1);
		const int& i2 = Rider::Faiz::get<const int>(p1);
		CHECK(i1 == 1);
		CHECK(i2 == 2);
	}

	{
		using upint = std::unique_ptr<int>;
		Rider::Faiz::tight_pair<upint, int> t(upint(new int(4)), 42);
		upint p = Rider::Faiz::get<upint>(std::move(t)); // get rvalue
		CHECK(*p == 4);
		CHECK(Rider::Faiz::get<upint>(t) == nullptr); // has been moved from
	}

	{
		using upint = std::unique_ptr<int>;
		const Rider::Faiz::tight_pair<upint, int> t(upint(new int(4)), 42);
		static_assert(std::is_same_v<const upint&&,
			decltype(Rider::Faiz::get<upint>(std::move(t)))>);
		static_assert(noexcept(Rider::Faiz::get<upint>(std::move(t))));
		static_assert(std::is_same_v<const int&&,
			decltype(Rider::Faiz::get<int>(std::move(t)))>);
		static_assert(noexcept(Rider::Faiz::get<int>(std::move(t))));
		auto&& p = Rider::Faiz::get<upint>(std::move(t)); // get const rvalue
		auto&& i = Rider::Faiz::get<int>(std::move(t)); // get const rvalue
		CHECK(*p == 4);
		CHECK(i == 42);
		CHECK(Rider::Faiz::get<upint>(t) != nullptr);
	}

	{
		int x = 42;
		int const y = 43;
		Rider::Faiz::tight_pair<int&, int const&> const p(x, y);
		static_assert(std::is_same_v<int&,
			decltype(Rider::Faiz::get<int&>(std::move(p)))>);
		static_assert(noexcept(Rider::Faiz::get<int&>(std::move(p))));
		static_assert(std::is_same_v<int const&,
			decltype(Rider::Faiz::get<int const&>(std::move(p)))>);
		static_assert(noexcept(Rider::Faiz::get<int const&>(std::move(p))));
	}

	{
		int x = 42;
		int const y = 43;
		Rider::Faiz::tight_pair<int&&, int const&&> const p(
			std::move(x), std::move(y));
		static_assert(std::is_same_v<int&&,
			decltype(Rider::Faiz::get<int&&>(std::move(p)))>);
		static_assert(noexcept(Rider::Faiz::get<int&&>(std::move(p))));
		static_assert(std::is_same_v<int const&&,
			decltype(Rider::Faiz::get<int const&&>(std::move(p)))>);
		static_assert(noexcept(Rider::Faiz::get<int const&&>(std::move(p))));
	}

	{
		constexpr const Rider::Faiz::tight_pair<int, const int> p{1, 2};
		static_assert(Rider::Faiz::get<int>(std::move(p)) == 1);
		static_assert(Rider::Faiz::get<const int>(std::move(p)) == 2);
	}
}
