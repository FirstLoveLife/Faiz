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
#include <type_traits>

namespace
{
	struct NoDefault
	{
		NoDefault() = delete;
	};
} // namespace

TEST_CASE("test default constructor")
{
	using Rider::Faiz::get;

	{
		using P = Rider::Faiz::tight_pair<float, short*>;
		P p;
		CHECK(get<0>(p) == 0.0f);
		CHECK(get<1>(p) == nullptr);
	}

	{
		using P = Rider::Faiz::tight_pair<float, short*>;
		;
		constexpr P p;
		static_assert(get<0>(p) == 0.0f);
		static_assert(get<1>(p) == nullptr);
	}

	{
		using P = Rider::Faiz::tight_pair<int, NoDefault>;
		static_assert(not std::is_default_constructible_v<P>);
		using P2 = Rider::Faiz::tight_pair<NoDefault, int>;
		static_assert(not std::is_default_constructible_v<P2>);
	}
}
