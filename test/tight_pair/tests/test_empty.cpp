#include "rider/faiz/compressed_pair.hpp"
#include <catch2/catch.hpp>

struct empty
{};
constexpr bool
operator==(empty e1, empty e2)
{
	return true;
}

TEST_CASE("test empty struct")
{
	using Rider::Faiz::get;

	Rider::Faiz::tight_pair<char*, empty> p(0);
	CHECK(get<0>(p) == nullptr);
	CHECK(get<1>(p) == empty{});
}
