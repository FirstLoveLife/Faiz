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
	static_assert(!rider::faiz::disjunction<>::value, "");
	static_assert(rider::faiz::disjunction<rider::faiz::true_type>::value, "");
	static_assert(
		!rider::faiz::disjunction<rider::faiz::false_type>::value, "");

	static_assert(!rider::faiz::disjunction_v<>, "");
	static_assert(rider::faiz::disjunction_v<rider::faiz::true_type>, "");
	static_assert(!rider::faiz::disjunction_v<rider::faiz::false_type>, "");

	static_assert(rider::faiz::disjunction<rider::faiz::true_type,
					  rider::faiz::true_type>::value,
		"");
	static_assert(rider::faiz::disjunction<rider::faiz::true_type,
					  rider::faiz::false_type>::value,
		"");
	static_assert(rider::faiz::disjunction<rider::faiz::false_type,
					  rider::faiz::true_type>::value,
		"");
	static_assert(!rider::faiz::disjunction<rider::faiz::false_type,
					  rider::faiz::false_type>::value,
		"");

	static_assert(rider::faiz::disjunction_v<rider::faiz::true_type,
					  rider::faiz::true_type>,
		"");
	static_assert(rider::faiz::disjunction_v<rider::faiz::true_type,
					  rider::faiz::false_type>,
		"");
	static_assert(rider::faiz::disjunction_v<rider::faiz::false_type,
					  rider::faiz::true_type>,
		"");
	static_assert(!rider::faiz::disjunction_v<rider::faiz::false_type,
					  rider::faiz::false_type>,
		"");

	static_assert(rider::faiz::disjunction<rider::faiz::true_type,
					  rider::faiz::true_type,
					  rider::faiz::true_type>::value,
		"");
	static_assert(rider::faiz::disjunction<rider::faiz::true_type,
					  rider::faiz::false_type,
					  rider::faiz::true_type>::value,
		"");
	static_assert(rider::faiz::disjunction<rider::faiz::false_type,
					  rider::faiz::true_type,
					  rider::faiz::true_type>::value,
		"");
	static_assert(rider::faiz::disjunction<rider::faiz::false_type,
					  rider::faiz::false_type,
					  rider::faiz::true_type>::value,
		"");
	static_assert(rider::faiz::disjunction<rider::faiz::true_type,
					  rider::faiz::true_type,
					  rider::faiz::false_type>::value,
		"");
	static_assert(rider::faiz::disjunction<rider::faiz::true_type,
					  rider::faiz::false_type,
					  rider::faiz::false_type>::value,
		"");
	static_assert(rider::faiz::disjunction<rider::faiz::false_type,
					  rider::faiz::true_type,
					  rider::faiz::false_type>::value,
		"");
	static_assert(!rider::faiz::disjunction<rider::faiz::false_type,
					  rider::faiz::false_type,
					  rider::faiz::false_type>::value,
		"");

	static_assert(rider::faiz::disjunction_v<rider::faiz::true_type,
					  rider::faiz::true_type,
					  rider::faiz::true_type>,
		"");
	static_assert(rider::faiz::disjunction_v<rider::faiz::true_type,
					  rider::faiz::false_type,
					  rider::faiz::true_type>,
		"");
	static_assert(rider::faiz::disjunction_v<rider::faiz::false_type,
					  rider::faiz::true_type,
					  rider::faiz::true_type>,
		"");
	static_assert(rider::faiz::disjunction_v<rider::faiz::false_type,
					  rider::faiz::false_type,
					  rider::faiz::true_type>,
		"");
	static_assert(rider::faiz::disjunction_v<rider::faiz::true_type,
					  rider::faiz::true_type,
					  rider::faiz::false_type>,
		"");
	static_assert(rider::faiz::disjunction_v<rider::faiz::true_type,
					  rider::faiz::false_type,
					  rider::faiz::false_type>,
		"");
	static_assert(rider::faiz::disjunction_v<rider::faiz::false_type,
					  rider::faiz::true_type,
					  rider::faiz::false_type>,
		"");
	static_assert(!rider::faiz::disjunction_v<rider::faiz::false_type,
					  rider::faiz::false_type,
					  rider::faiz::false_type>,
		"");

	static_assert(rider::faiz::disjunction<True>::value, "");
	static_assert(!rider::faiz::disjunction<False>::value, "");

	static_assert(rider::faiz::disjunction_v<True>, "");
	static_assert(!rider::faiz::disjunction_v<False>, "");
}
