#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{

	template<class T, class... U>
	void
	test_are_same()
	{
		STATIC_REQUIRE(Rider::Faiz::are_same<T, U...>::value);
		STATIC_REQUIRE(!Rider::Faiz::are_same<const T, U...>::value);
		STATIC_REQUIRE(!Rider::Faiz::are_same<T, const U...>::value);
		STATIC_REQUIRE(Rider::Faiz::are_same<const T, const U...>::value);
		STATIC_REQUIRE(Rider::Faiz::are_same_v<T, U...>);
		STATIC_REQUIRE(!Rider::Faiz::are_same_v<const T, U...>);
		STATIC_REQUIRE(!Rider::Faiz::are_same_v<T, const U...>);
		STATIC_REQUIRE(Rider::Faiz::are_same_v<const T, const U...>);
	}

	template<class T, class... U>
	void
	test_are_same_ref()
	{
		STATIC_REQUIRE(Rider::Faiz::are_same<T, U...>::value);
		STATIC_REQUIRE(Rider::Faiz::are_same<const T, U...>::value);
		STATIC_REQUIRE(Rider::Faiz::are_same<T, const U...>::value);
		STATIC_REQUIRE(Rider::Faiz::are_same<const T, const U...>::value);
		STATIC_REQUIRE(Rider::Faiz::are_same_v<T, U...>);
		STATIC_REQUIRE(Rider::Faiz::are_same_v<const T, U...>);
		STATIC_REQUIRE(Rider::Faiz::are_same_v<T, const U...>);
		STATIC_REQUIRE(Rider::Faiz::are_same_v<const T, const U...>);
	}

	template<class T, class... U>
	void
	test_are_not_same()
	{
		STATIC_REQUIRE(!Rider::Faiz::are_same<T, U...>::value);
	}

	class Class
	{
	public:
		~Class();
	};

} // namespace
TEST_CASE("are_same.libcxx: ")
{
	test_are_same<int, int, int, int>();
	test_are_same<int, int, int>();
	test_are_same<void, void, void, void>();
	test_are_same<Class, Class, Class, Class>();
	test_are_same<int*, int*, int*, int*>();
	test_are_same_ref<int&, int&, int&, int&>();

	// test_are_same<int, void, void, int>();
	test_are_not_same<int, void, void, int>();
	test_are_not_same<void, Class, Class>();
	test_are_not_same<Class, int*, float>();
	test_are_not_same<int*, int&, int*>();
	test_are_not_same<int&, int>();
}
