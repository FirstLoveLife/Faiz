//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14
// type_traits

// template<class... B> struct conjunction;                           // C++17
// template<class... B>
//   constexpr bool conjunction_v = conjunction<B...>::value;         // C++17
#include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"

struct True
{
	static constexpr bool value = true;
};
struct False
{
	static constexpr bool value = false;
};

int
main()
{
	static_assert(Rider::Faiz::conjunction<>::value, "");
	static_assert(Rider::Faiz::conjunction<Rider::Faiz::true_type>::value, "");
	static_assert(
		!Rider::Faiz::conjunction<Rider::Faiz::false_type>::value, "");

	static_assert(Rider::Faiz::conjunction_v<>, "");
	static_assert(Rider::Faiz::conjunction_v<Rider::Faiz::true_type>, "");
	static_assert(!Rider::Faiz::conjunction_v<Rider::Faiz::false_type>, "");

	static_assert(Rider::Faiz::conjunction<Rider::Faiz::true_type,
					  Rider::Faiz::true_type>::value,
		"");
	static_assert(!Rider::Faiz::conjunction<Rider::Faiz::true_type,
					  Rider::Faiz::false_type>::value,
		"");
	static_assert(!Rider::Faiz::conjunction<Rider::Faiz::false_type,
					  Rider::Faiz::true_type>::value,
		"");
	static_assert(!Rider::Faiz::conjunction<Rider::Faiz::false_type,
					  Rider::Faiz::false_type>::value,
		"");

	static_assert(Rider::Faiz::conjunction_v<Rider::Faiz::true_type,
					  Rider::Faiz::true_type>,
		"");
	static_assert(!Rider::Faiz::conjunction_v<Rider::Faiz::true_type,
					  Rider::Faiz::false_type>,
		"");
	static_assert(!Rider::Faiz::conjunction_v<Rider::Faiz::false_type,
					  Rider::Faiz::true_type>,
		"");
	static_assert(!Rider::Faiz::conjunction_v<Rider::Faiz::false_type,
					  Rider::Faiz::false_type>,
		"");

	static_assert(Rider::Faiz::conjunction<Rider::Faiz::true_type,
					  Rider::Faiz::true_type,
					  Rider::Faiz::true_type>::value,
		"");
	static_assert(!Rider::Faiz::conjunction<Rider::Faiz::true_type,
					  Rider::Faiz::false_type,
					  Rider::Faiz::true_type>::value,
		"");
	static_assert(!Rider::Faiz::conjunction<Rider::Faiz::false_type,
					  Rider::Faiz::true_type,
					  Rider::Faiz::true_type>::value,
		"");
	static_assert(!Rider::Faiz::conjunction<Rider::Faiz::false_type,
					  Rider::Faiz::false_type,
					  Rider::Faiz::true_type>::value,
		"");
	static_assert(!Rider::Faiz::conjunction<Rider::Faiz::true_type,
					  Rider::Faiz::true_type,
					  Rider::Faiz::false_type>::value,
		"");
	static_assert(!Rider::Faiz::conjunction<Rider::Faiz::true_type,
					  Rider::Faiz::false_type,
					  Rider::Faiz::false_type>::value,
		"");
	static_assert(!Rider::Faiz::conjunction<Rider::Faiz::false_type,
					  Rider::Faiz::true_type,
					  Rider::Faiz::false_type>::value,
		"");
	static_assert(!Rider::Faiz::conjunction<Rider::Faiz::false_type,
					  Rider::Faiz::false_type,
					  Rider::Faiz::false_type>::value,
		"");

	static_assert(Rider::Faiz::conjunction_v<Rider::Faiz::true_type,
					  Rider::Faiz::true_type,
					  Rider::Faiz::true_type>,
		"");
	static_assert(!Rider::Faiz::conjunction_v<Rider::Faiz::true_type,
					  Rider::Faiz::false_type,
					  Rider::Faiz::true_type>,
		"");
	static_assert(!Rider::Faiz::conjunction_v<Rider::Faiz::false_type,
					  Rider::Faiz::true_type,
					  Rider::Faiz::true_type>,
		"");
	static_assert(!Rider::Faiz::conjunction_v<Rider::Faiz::false_type,
					  Rider::Faiz::false_type,
					  Rider::Faiz::true_type>,
		"");
	static_assert(!Rider::Faiz::conjunction_v<Rider::Faiz::true_type,
					  Rider::Faiz::true_type,
					  Rider::Faiz::false_type>,
		"");
	static_assert(!Rider::Faiz::conjunction_v<Rider::Faiz::true_type,
					  Rider::Faiz::false_type,
					  Rider::Faiz::false_type>,
		"");
	static_assert(!Rider::Faiz::conjunction_v<Rider::Faiz::false_type,
					  Rider::Faiz::true_type,
					  Rider::Faiz::false_type>,
		"");
	static_assert(!Rider::Faiz::conjunction_v<Rider::Faiz::false_type,
					  Rider::Faiz::false_type,
					  Rider::Faiz::false_type>,
		"");

	static_assert(Rider::Faiz::conjunction<True>::value, "");
	static_assert(!Rider::Faiz::conjunction<False>::value, "");

	static_assert(Rider::Faiz::conjunction_v<True>, "");
	static_assert(!Rider::Faiz::conjunction_v<False>, "");
}
