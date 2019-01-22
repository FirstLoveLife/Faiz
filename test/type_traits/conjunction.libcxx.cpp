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
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

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
TEST_CASE("conjunction libcxx")
{
	STATIC_REQUIRE(Rider::Faiz::conjunction<>::value);
	STATIC_REQUIRE(Rider::Faiz::conjunction<Rider::Faiz::true_type>::value);
	STATIC_REQUIRE(!Rider::Faiz::conjunction<Rider::Faiz::false_type>::value);

	STATIC_REQUIRE(Rider::Faiz::conjunction_v<>);
	STATIC_REQUIRE(Rider::Faiz::conjunction_v<Rider::Faiz::true_type>);
	STATIC_REQUIRE(!Rider::Faiz::conjunction_v<Rider::Faiz::false_type>);

	STATIC_REQUIRE(Rider::Faiz::conjunction<Rider::Faiz::true_type,
		Rider::Faiz::true_type>::value);

	STATIC_REQUIRE(!Rider::Faiz::conjunction<Rider::Faiz::true_type,
				   Rider::Faiz::false_type>::value);

	STATIC_REQUIRE(!Rider::Faiz::conjunction<Rider::Faiz::false_type,
				   Rider::Faiz::true_type>::value);

	STATIC_REQUIRE(!Rider::Faiz::conjunction<Rider::Faiz::false_type,
				   Rider::Faiz::false_type>::value);


	STATIC_REQUIRE(Rider::Faiz::conjunction_v<Rider::Faiz::true_type,
		Rider::Faiz::true_type>);

	STATIC_REQUIRE(!Rider::Faiz::conjunction_v<Rider::Faiz::true_type,
				   Rider::Faiz::false_type>);

	STATIC_REQUIRE(!Rider::Faiz::conjunction_v<Rider::Faiz::false_type,
				   Rider::Faiz::true_type>);

	STATIC_REQUIRE(!Rider::Faiz::conjunction_v<Rider::Faiz::false_type,
				   Rider::Faiz::false_type>);


	STATIC_REQUIRE(Rider::Faiz::conjunction<Rider::Faiz::true_type,
		Rider::Faiz::true_type,
		Rider::Faiz::true_type>::value);

	STATIC_REQUIRE(!Rider::Faiz::conjunction<Rider::Faiz::true_type,
				   Rider::Faiz::false_type,
				   Rider::Faiz::true_type>::value);

	STATIC_REQUIRE(!Rider::Faiz::conjunction<Rider::Faiz::false_type,
				   Rider::Faiz::true_type,
				   Rider::Faiz::true_type>::value);

	STATIC_REQUIRE(!Rider::Faiz::conjunction<Rider::Faiz::false_type,
				   Rider::Faiz::false_type,
				   Rider::Faiz::true_type>::value);

	STATIC_REQUIRE(!Rider::Faiz::conjunction<Rider::Faiz::true_type,
				   Rider::Faiz::true_type,
				   Rider::Faiz::false_type>::value);

	STATIC_REQUIRE(!Rider::Faiz::conjunction<Rider::Faiz::true_type,
				   Rider::Faiz::false_type,
				   Rider::Faiz::false_type>::value);

	STATIC_REQUIRE(!Rider::Faiz::conjunction<Rider::Faiz::false_type,
				   Rider::Faiz::true_type,
				   Rider::Faiz::false_type>::value);

	STATIC_REQUIRE(!Rider::Faiz::conjunction<Rider::Faiz::false_type,
				   Rider::Faiz::false_type,
				   Rider::Faiz::false_type>::value);


	STATIC_REQUIRE(Rider::Faiz::conjunction_v<Rider::Faiz::true_type,
		Rider::Faiz::true_type,
		Rider::Faiz::true_type>);

	STATIC_REQUIRE(!Rider::Faiz::conjunction_v<Rider::Faiz::true_type,
				   Rider::Faiz::false_type,
				   Rider::Faiz::true_type>);

	STATIC_REQUIRE(!Rider::Faiz::conjunction_v<Rider::Faiz::false_type,
				   Rider::Faiz::true_type,
				   Rider::Faiz::true_type>);

	STATIC_REQUIRE(!Rider::Faiz::conjunction_v<Rider::Faiz::false_type,
				   Rider::Faiz::false_type,
				   Rider::Faiz::true_type>);

	STATIC_REQUIRE(!Rider::Faiz::conjunction_v<Rider::Faiz::true_type,
				   Rider::Faiz::true_type,
				   Rider::Faiz::false_type>);

	STATIC_REQUIRE(!Rider::Faiz::conjunction_v<Rider::Faiz::true_type,
				   Rider::Faiz::false_type,
				   Rider::Faiz::false_type>);

	STATIC_REQUIRE(!Rider::Faiz::conjunction_v<Rider::Faiz::false_type,
				   Rider::Faiz::true_type,
				   Rider::Faiz::false_type>);

	STATIC_REQUIRE(!Rider::Faiz::conjunction_v<Rider::Faiz::false_type,
				   Rider::Faiz::false_type,
				   Rider::Faiz::false_type>);


	STATIC_REQUIRE(Rider::Faiz::conjunction<True>::value);
	STATIC_REQUIRE(!Rider::Faiz::conjunction<False>::value);

	STATIC_REQUIRE(Rider::Faiz::conjunction_v<True>);
	STATIC_REQUIRE(!Rider::Faiz::conjunction_v<False>);
}
