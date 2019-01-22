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
#include <catch2/catch.hpp>

namespace
{


	struct True
	{
		static constexpr bool value = true;
	};
	struct False
	{
		static constexpr bool value = false;
	};

} // namespace
TEST_CASE("disjunction.libcxx: ")
{
	STATIC_REQUIRE(!Rider::Faiz::disjunction<>::value);
	STATIC_REQUIRE(Rider::Faiz::disjunction<Rider::Faiz::true_type>::value);
	STATIC_REQUIRE(!Rider::Faiz::disjunction<Rider::Faiz::false_type>::value);

	STATIC_REQUIRE(!Rider::Faiz::disjunction_v<>);
	STATIC_REQUIRE(Rider::Faiz::disjunction_v<Rider::Faiz::true_type>);
	STATIC_REQUIRE(!Rider::Faiz::disjunction_v<Rider::Faiz::false_type>);

	STATIC_REQUIRE(Rider::Faiz::disjunction<Rider::Faiz::true_type,
		Rider::Faiz::true_type>::value);

	STATIC_REQUIRE(Rider::Faiz::disjunction<Rider::Faiz::true_type,
		Rider::Faiz::false_type>::value);

	STATIC_REQUIRE(Rider::Faiz::disjunction<Rider::Faiz::false_type,
		Rider::Faiz::true_type>::value);

	STATIC_REQUIRE(!Rider::Faiz::disjunction<Rider::Faiz::false_type,
				   Rider::Faiz::false_type>::value);


	STATIC_REQUIRE(Rider::Faiz::disjunction_v<Rider::Faiz::true_type,
		Rider::Faiz::true_type>);
	STATIC_REQUIRE(Rider::Faiz::disjunction_v<Rider::Faiz::true_type,
		Rider::Faiz::false_type>);
	STATIC_REQUIRE(Rider::Faiz::disjunction_v<Rider::Faiz::false_type,
		Rider::Faiz::true_type>);
	STATIC_REQUIRE(!Rider::Faiz::disjunction_v<Rider::Faiz::false_type,
				   Rider::Faiz::false_type>);

	STATIC_REQUIRE(Rider::Faiz::disjunction<Rider::Faiz::true_type,
		Rider::Faiz::true_type,
		Rider::Faiz::true_type>::value);
	STATIC_REQUIRE(Rider::Faiz::disjunction<Rider::Faiz::true_type,
		Rider::Faiz::false_type,
		Rider::Faiz::true_type>::value);
	STATIC_REQUIRE(Rider::Faiz::disjunction<Rider::Faiz::false_type,
		Rider::Faiz::true_type,
		Rider::Faiz::true_type>::value);
	STATIC_REQUIRE(Rider::Faiz::disjunction<Rider::Faiz::false_type,
		Rider::Faiz::false_type,
		Rider::Faiz::true_type>::value);
	STATIC_REQUIRE(Rider::Faiz::disjunction<Rider::Faiz::true_type,
		Rider::Faiz::true_type,
		Rider::Faiz::false_type>::value);
	STATIC_REQUIRE(Rider::Faiz::disjunction<Rider::Faiz::true_type,
		Rider::Faiz::false_type,
		Rider::Faiz::false_type>::value);
	STATIC_REQUIRE(Rider::Faiz::disjunction<Rider::Faiz::false_type,
		Rider::Faiz::true_type,
		Rider::Faiz::false_type>::value);
	STATIC_REQUIRE(!Rider::Faiz::disjunction<Rider::Faiz::false_type,
				   Rider::Faiz::false_type,
				   Rider::Faiz::false_type>::value);

	STATIC_REQUIRE(Rider::Faiz::disjunction_v<Rider::Faiz::true_type,
		Rider::Faiz::true_type,
		Rider::Faiz::true_type>);
	STATIC_REQUIRE(Rider::Faiz::disjunction_v<Rider::Faiz::true_type,
		Rider::Faiz::false_type,
		Rider::Faiz::true_type>);
	STATIC_REQUIRE(Rider::Faiz::disjunction_v<Rider::Faiz::false_type,
		Rider::Faiz::true_type,
		Rider::Faiz::true_type>);
	STATIC_REQUIRE(Rider::Faiz::disjunction_v<Rider::Faiz::false_type,
		Rider::Faiz::false_type,
		Rider::Faiz::true_type>);
	STATIC_REQUIRE(Rider::Faiz::disjunction_v<Rider::Faiz::true_type,
		Rider::Faiz::true_type,
		Rider::Faiz::false_type>);
	STATIC_REQUIRE(Rider::Faiz::disjunction_v<Rider::Faiz::true_type,
		Rider::Faiz::false_type,
		Rider::Faiz::false_type>);
	STATIC_REQUIRE(Rider::Faiz::disjunction_v<Rider::Faiz::false_type,
		Rider::Faiz::true_type,
		Rider::Faiz::false_type>);
	STATIC_REQUIRE(!Rider::Faiz::disjunction_v<Rider::Faiz::false_type,
				   Rider::Faiz::false_type,
				   Rider::Faiz::false_type>);

	STATIC_REQUIRE(Rider::Faiz::disjunction<True>::value);
	STATIC_REQUIRE(!Rider::Faiz::disjunction<False>::value);

	STATIC_REQUIRE(Rider::Faiz::disjunction_v<True>);
	STATIC_REQUIRE(!Rider::Faiz::disjunction_v<False>);
}
