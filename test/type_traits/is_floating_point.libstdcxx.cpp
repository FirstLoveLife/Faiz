#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{
	using namespace Rider::Faiz;

	void
	test01()
	{
		STATIC_REQUIRE(!is_floating_point<char>::value);
		STATIC_REQUIRE(!is_floating_point<signed char>::value);
		STATIC_REQUIRE(!is_floating_point<unsigned char>::value);
		STATIC_REQUIRE(!is_floating_point<wchar_t>::value);
		STATIC_REQUIRE(!is_floating_point<short>::value);
		STATIC_REQUIRE(!is_floating_point<unsigned short>::value);
		STATIC_REQUIRE(!is_floating_point<int>::value);
		STATIC_REQUIRE(!is_floating_point<unsigned int>::value);
		STATIC_REQUIRE(!is_floating_point<long>::value);
		STATIC_REQUIRE(!is_floating_point<unsigned long>::value);
		STATIC_REQUIRE(!is_floating_point<long long>::value);
		STATIC_REQUIRE(!is_floating_point<unsigned long long>::value);

		STATIC_REQUIRE(is_floating_point<float>::value);
		STATIC_REQUIRE(is_floating_point<double>::value);
		STATIC_REQUIRE(is_floating_point<long double>::value);
		STATIC_REQUIRE(is_floating_point<__float128>::value);
		STATIC_REQUIRE(!is_floating_point<__int128>::value);
		STATIC_REQUIRE(!is_floating_point<unsigned __int128>::value);
		STATIC_REQUIRE(!is_floating_point<ClassType>::value);
	}

} // namespace
TEST_CASE("is_floating_point.libstdcxx: ")
{
	test01();
}
