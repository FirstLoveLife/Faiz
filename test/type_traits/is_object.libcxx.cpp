// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{

	template<class T>
	void
	test_is_object()
	{
		STATIC_REQUIRE(Rider::Faiz::is_object<T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_object<const T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_object<volatile T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_object<const volatile T>::value);
		STATIC_REQUIRE(Rider::Faiz::is_object_v<T>);
		STATIC_REQUIRE(Rider::Faiz::is_object_v<const T>);
		STATIC_REQUIRE(Rider::Faiz::is_object_v<volatile T>);
		STATIC_REQUIRE(Rider::Faiz::is_object_v<const volatile T>);
	}

	template<class T>
	void
	test_is_not_object()
	{
		STATIC_REQUIRE(!Rider::Faiz::is_object<T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_object<const T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_object<volatile T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_object<const volatile T>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_object_v<T>);
		STATIC_REQUIRE(!Rider::Faiz::is_object_v<const T>);
		STATIC_REQUIRE(!Rider::Faiz::is_object_v<volatile T>);
		STATIC_REQUIRE(!Rider::Faiz::is_object_v<const volatile T>);
	}

	class incomplete_type;

	class Empty
	{};

	class NotEmpty
	{
		virtual ~NotEmpty();
	};

	union Union
	{};

	struct bit_zero
	{
		int : 0;
	};

	class Abstract
	{
		virtual ~Abstract() = 0;
	};

	enum Enum
	{
		zero,
		one
	};

	typedef void (*FunctionPtr)();


} // namespace
TEST_CASE("is_object.libcxx: ")
{
	// An object type is a (possibly cv-qualified) type that is not a function
	// type, not a reference type, and not a void type.

	test_is_object<Rider::Faiz::nullptr_t>();
	test_is_object<void*>();
	test_is_object<char[3]>();
	test_is_object<char[]>();
	test_is_object<int>();
	test_is_object<int*>();
	test_is_object<Union>();
	test_is_object<int*>();
	test_is_object<const int*>();
	test_is_object<Enum>();
	test_is_object<incomplete_type>();
	test_is_object<bit_zero>();
	test_is_object<NotEmpty>();
	test_is_object<Abstract>();
	test_is_object<FunctionPtr>();
	test_is_object<int Empty::*>();
	test_is_object<void (Empty::*)(int)>();

	test_is_not_object<void>();
	test_is_not_object<int&>();
	test_is_not_object<int&&>();
	test_is_not_object<int(int)>();
}
