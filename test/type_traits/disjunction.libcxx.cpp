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

// template<class... B> struct disjunction;                           // C++17
// template<class... B>
//   constexpr bool disjunction_v = disjunction<B...>::value;         // C++17

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
	static_assert(!Rider::Faiz::disjunction<>::value, "");
	static_assert(Rider::Faiz::disjunction<Rider::Faiz::true_type>::value, "");
	static_assert(
		!Rider::Faiz::disjunction<Rider::Faiz::false_type>::value, "");

	static_assert(!Rider::Faiz::disjunction_v<>, "");
	static_assert(Rider::Faiz::disjunction_v<Rider::Faiz::true_type>, "");
	static_assert(!Rider::Faiz::disjunction_v<Rider::Faiz::false_type>, "");

	static_assert(Rider::Faiz::disjunction<Rider::Faiz::true_type,
					  Rider::Faiz::true_type>::value,
		"");
	static_assert(Rider::Faiz::disjunction<Rider::Faiz::true_type,
					  Rider::Faiz::false_type>::value,
		"");
	static_assert(Rider::Faiz::disjunction<Rider::Faiz::false_type,
					  Rider::Faiz::true_type>::value,
		"");
	static_assert(!Rider::Faiz::disjunction<Rider::Faiz::false_type,
					  Rider::Faiz::false_type>::value,
		"");

	static_assert(Rider::Faiz::disjunction_v<Rider::Faiz::true_type,
					  Rider::Faiz::true_type>,
		"");
	static_assert(Rider::Faiz::disjunction_v<Rider::Faiz::true_type,
					  Rider::Faiz::false_type>,
		"");
	static_assert(Rider::Faiz::disjunction_v<Rider::Faiz::false_type,
					  Rider::Faiz::true_type>,
		"");
	static_assert(!Rider::Faiz::disjunction_v<Rider::Faiz::false_type,
					  Rider::Faiz::false_type>,
		"");

	static_assert(Rider::Faiz::disjunction<Rider::Faiz::true_type,
					  Rider::Faiz::true_type,
					  Rider::Faiz::true_type>::value,
		"");
	static_assert(Rider::Faiz::disjunction<Rider::Faiz::true_type,
					  Rider::Faiz::false_type,
					  Rider::Faiz::true_type>::value,
		"");
	static_assert(Rider::Faiz::disjunction<Rider::Faiz::false_type,
					  Rider::Faiz::true_type,
					  Rider::Faiz::true_type>::value,
		"");
	static_assert(Rider::Faiz::disjunction<Rider::Faiz::false_type,
					  Rider::Faiz::false_type,
					  Rider::Faiz::true_type>::value,
		"");
	static_assert(Rider::Faiz::disjunction<Rider::Faiz::true_type,
					  Rider::Faiz::true_type,
					  Rider::Faiz::false_type>::value,
		"");
	static_assert(Rider::Faiz::disjunction<Rider::Faiz::true_type,
					  Rider::Faiz::false_type,
					  Rider::Faiz::false_type>::value,
		"");
	static_assert(Rider::Faiz::disjunction<Rider::Faiz::false_type,
					  Rider::Faiz::true_type,
					  Rider::Faiz::false_type>::value,
		"");
	static_assert(!Rider::Faiz::disjunction<Rider::Faiz::false_type,
					  Rider::Faiz::false_type,
					  Rider::Faiz::false_type>::value,
		"");

	static_assert(Rider::Faiz::disjunction_v<Rider::Faiz::true_type,
					  Rider::Faiz::true_type,
					  Rider::Faiz::true_type>,
		"");
	static_assert(Rider::Faiz::disjunction_v<Rider::Faiz::true_type,
					  Rider::Faiz::false_type,
					  Rider::Faiz::true_type>,
		"");
	static_assert(Rider::Faiz::disjunction_v<Rider::Faiz::false_type,
					  Rider::Faiz::true_type,
					  Rider::Faiz::true_type>,
		"");
	static_assert(Rider::Faiz::disjunction_v<Rider::Faiz::false_type,
					  Rider::Faiz::false_type,
					  Rider::Faiz::true_type>,
		"");
	static_assert(Rider::Faiz::disjunction_v<Rider::Faiz::true_type,
					  Rider::Faiz::true_type,
					  Rider::Faiz::false_type>,
		"");
	static_assert(Rider::Faiz::disjunction_v<Rider::Faiz::true_type,
					  Rider::Faiz::false_type,
					  Rider::Faiz::false_type>,
		"");
	static_assert(Rider::Faiz::disjunction_v<Rider::Faiz::false_type,
					  Rider::Faiz::true_type,
					  Rider::Faiz::false_type>,
		"");
	static_assert(!Rider::Faiz::disjunction_v<Rider::Faiz::false_type,
					  Rider::Faiz::false_type,
					  Rider::Faiz::false_type>,
		"");

	static_assert(Rider::Faiz::disjunction<True>::value, "");
	static_assert(!Rider::Faiz::disjunction<False>::value, "");

	static_assert(Rider::Faiz::disjunction_v<True>, "");
	static_assert(!Rider::Faiz::disjunction_v<False>, "");
}
