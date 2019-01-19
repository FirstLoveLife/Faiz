/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Morwenn
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
#include <catch2/catch.hpp>

#include <catch2/catch.hpp>
#include <tight_pair.h>
#include <type_traits>

TEST_CASE("default noexcept")
{
	using NonThrowingDefault = NonThrowingTypes::DefaultOnly;
#ifdef __clang__
	using ThrowingDefault = NonTrivialTypes::DefaultOnly;
	// NOTE: the behaviour for these test cases depends on whether the compiler
	// implements
	//       the resolution of P0003R5 which removes a noexcept special case
	//       with regard to constant expressions, which apparently was a
	//       serendipitous change so we can't rely rely of a guaranteed
	//       behaviour right now
	static_assert(not std::is_nothrow_default_constructible_v<
				  Rider::Faiz::tight_pair<ThrowingDefault, ThrowingDefault>>);
	static_assert(
		not std::is_nothrow_default_constructible_v<
			Rider::Faiz::tight_pair<NonThrowingDefault, ThrowingDefault>>);
	static_assert(
		not std::is_nothrow_default_constructible_v<
			Rider::Faiz::tight_pair<ThrowingDefault, NonThrowingDefault>>);
#endif
	static_assert(std::is_nothrow_default_constructible_v<
		Rider::Faiz::tight_pair<NonThrowingDefault, NonThrowingDefault>>);
}
