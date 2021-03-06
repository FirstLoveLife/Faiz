// #include "../test_macros.h"
#include "test-utilities.hpp"
#include <catch2/catch.hpp>

#include "rider/faiz/type_traits.hpp"

namespace
{

	template<class T, class U>
	void
	test_is_convertible()
	{
		STATIC_REQUIRE(Rider::Faiz::is_convertible<T, U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_convertible<const T, U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_convertible<T, const U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_convertible<const T, const U>::value);
		STATIC_REQUIRE(Rider::Faiz::is_convertible_v<T, U>);
		STATIC_REQUIRE(Rider::Faiz::is_convertible_v<const T, U>);
		STATIC_REQUIRE(Rider::Faiz::is_convertible_v<T, const U>);
		STATIC_REQUIRE(Rider::Faiz::is_convertible_v<const T, const U>);
	}

	template<class T, class U>
	void
	test_is_not_convertible()
	{
		STATIC_REQUIRE(!Rider::Faiz::is_convertible<T, U>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_convertible<const T, U>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_convertible<T, const U>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_convertible<const T, const U>::value);
		STATIC_REQUIRE(!Rider::Faiz::is_convertible_v<T, U>);
		STATIC_REQUIRE(!Rider::Faiz::is_convertible_v<const T, U>);
		STATIC_REQUIRE(!Rider::Faiz::is_convertible_v<T, const U>);
		STATIC_REQUIRE(!Rider::Faiz::is_convertible_v<const T, const U>);
	}

	typedef void
	Function();
	typedef void
	ConstFunction() const;
	typedef char Array[1];

	struct StringType
	{
		StringType(const char*)
		{}
	};

	class NonCopyable
	{
		NonCopyable(NonCopyable&);
	};

	template<typename T>
	class CannotInstantiate
	{
		enum
		{
			X = T::ThisExpressionWillBlowUp
		};
	};

} // namespace
TEST_CASE("is_convertible.libcxx: ")
{
	// void
	test_is_convertible<void, void>();
	test_is_not_convertible<void, Function>();
	test_is_not_convertible<void, Function&>();
	test_is_not_convertible<void, Function*>();
	test_is_not_convertible<void, Array>();
	test_is_not_convertible<void, Array&>();
	test_is_not_convertible<void, char>();
	test_is_not_convertible<void, char&>();
	test_is_not_convertible<void, char*>();
	test_is_not_convertible<char, void>();

	// Function
	test_is_not_convertible<Function, void>();
	test_is_not_convertible<Function, Function>();
	test_is_convertible<Function, Function&>();
	test_is_convertible<Function, Function*>();
	test_is_convertible<Function, Function* const>();

	STATIC_REQUIRE(Rider::Faiz::is_convertible<Function, Function&&>::value);

	test_is_not_convertible<Function, Array>();
	test_is_not_convertible<Function, Array&>();
	test_is_not_convertible<Function, char>();
	test_is_not_convertible<Function, char&>();
	test_is_not_convertible<Function, char*>();

	// Function&
	test_is_not_convertible<Function&, void>();
	test_is_not_convertible<Function&, Function>();
	test_is_convertible<Function&, Function&>();

	test_is_convertible<Function&, Function*>();
	test_is_not_convertible<Function&, Array>();
	test_is_not_convertible<Function&, Array&>();
	test_is_not_convertible<Function&, char>();
	test_is_not_convertible<Function&, char&>();
	test_is_not_convertible<Function&, char*>();

	// Function*
	test_is_not_convertible<Function*, void>();
	test_is_not_convertible<Function*, Function>();
	test_is_not_convertible<Function*, Function&>();
	test_is_convertible<Function*, Function*>();

	test_is_not_convertible<Function*, Array>();
	test_is_not_convertible<Function*, Array&>();
	test_is_not_convertible<Function*, char>();
	test_is_not_convertible<Function*, char&>();
	test_is_not_convertible<Function*, char*>();

	// Non-referencable function type
	STATIC_REQUIRE(
		!Rider::Faiz::is_convertible<ConstFunction, Function>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_convertible<ConstFunction, Function*>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_convertible<ConstFunction, Function&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_convertible<ConstFunction, Function&&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_convertible<Function*, ConstFunction>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_convertible<Function&, ConstFunction>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_convertible<ConstFunction, ConstFunction>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_convertible<ConstFunction, void>::value);

	// Array
	test_is_not_convertible<Array, void>();
	test_is_not_convertible<Array, Function>();
	test_is_not_convertible<Array, Function&>();
	test_is_not_convertible<Array, Function*>();
	test_is_not_convertible<Array, Array>();

	STATIC_REQUIRE(!Rider::Faiz::is_convertible<Array, Array&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_convertible<Array, const Array&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_convertible<Array, const volatile Array&>::value);


	STATIC_REQUIRE(!Rider::Faiz::is_convertible<const Array, Array&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_convertible<const Array, const Array&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_convertible<Array, volatile Array&>::value);
	STATIC_REQUIRE(
		!Rider::Faiz::is_convertible<Array, const volatile Array&>::value);


	STATIC_REQUIRE(Rider::Faiz::is_convertible<Array, Array&&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_convertible<Array, const Array&&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_convertible<Array, volatile Array&&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_convertible<Array, const volatile Array&&>::value);

	STATIC_REQUIRE(
		Rider::Faiz::is_convertible<const Array, const Array&&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_convertible<Array&, Array&&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_convertible<Array&&, Array&>::value);

	test_is_not_convertible<Array, char>();
	test_is_not_convertible<Array, char&>();

	STATIC_REQUIRE(Rider::Faiz::is_convertible<Array, char*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_convertible<Array, const char*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_convertible<Array, char* const>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_convertible<Array, char* const volatile>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_convertible<const Array, char*>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_convertible<const Array, const char*>::value);

	STATIC_REQUIRE(!Rider::Faiz::is_convertible<char[42][42], char*>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_convertible<char[][1], char*>::value);

	// Array&
	test_is_not_convertible<Array&, void>();
	test_is_not_convertible<Array&, Function>();
	test_is_not_convertible<Array&, Function&>();
	test_is_not_convertible<Array&, Function*>();
	test_is_not_convertible<Array&, Array>();

	STATIC_REQUIRE(Rider::Faiz::is_convertible<Array&, Array&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_convertible<Array&, const Array&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_convertible<const Array&, Array&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_convertible<const Array&, const Array&>::value);

	test_is_not_convertible<Array&, char>();
	test_is_not_convertible<Array&, char&>();

	STATIC_REQUIRE(Rider::Faiz::is_convertible<Array&, char*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_convertible<Array&, const char*>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_convertible<const Array&, char*>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_convertible<const Array&, const char*>::value);

	STATIC_REQUIRE(Rider::Faiz::is_convertible<Array, StringType>::value);
	STATIC_REQUIRE(Rider::Faiz::is_convertible<char(&)[], StringType>::value);

	// char
	test_is_not_convertible<char, void>();
	test_is_not_convertible<char, Function>();
	test_is_not_convertible<char, Function&>();
	test_is_not_convertible<char, Function*>();
	test_is_not_convertible<char, Array>();
	test_is_not_convertible<char, Array&>();

	test_is_convertible<char, char>();

	STATIC_REQUIRE(!Rider::Faiz::is_convertible<char, char&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_convertible<char, const char&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_convertible<const char, char&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_convertible<const char, const char&>::value);

	test_is_not_convertible<char, char*>();

	// char&
	test_is_not_convertible<char&, void>();
	test_is_not_convertible<char&, Function>();
	test_is_not_convertible<char&, Function&>();
	test_is_not_convertible<char&, Function*>();
	test_is_not_convertible<char&, Array>();
	test_is_not_convertible<char&, Array&>();

	test_is_convertible<char&, char>();

	STATIC_REQUIRE(Rider::Faiz::is_convertible<char&, char&>::value);
	STATIC_REQUIRE(Rider::Faiz::is_convertible<char&, const char&>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_convertible<const char&, char&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_convertible<const char&, const char&>::value);

	test_is_not_convertible<char&, char*>();

	// char*
	test_is_not_convertible<char*, void>();
	test_is_not_convertible<char*, Function>();
	test_is_not_convertible<char*, Function&>();
	test_is_not_convertible<char*, Function*>();
	test_is_not_convertible<char*, Array>();
	test_is_not_convertible<char*, Array&>();

	test_is_not_convertible<char*, char>();
	test_is_not_convertible<char*, char&>();

	STATIC_REQUIRE(Rider::Faiz::is_convertible<char*, char*>::value);
	STATIC_REQUIRE(Rider::Faiz::is_convertible<char*, const char*>::value);
	STATIC_REQUIRE(!Rider::Faiz::is_convertible<const char*, char*>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_convertible<const char*, const char*>::value);

	// NonCopyable
	STATIC_REQUIRE(
		Rider::Faiz::is_convertible<NonCopyable&, NonCopyable&>::value);
	STATIC_REQUIRE(
		Rider::Faiz::is_convertible<NonCopyable&, const NonCopyable&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_convertible<NonCopyable&,
		const volatile NonCopyable&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_convertible<NonCopyable&,
		volatile NonCopyable&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_convertible<const NonCopyable&,
		const NonCopyable&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_convertible<const NonCopyable&,
		const volatile NonCopyable&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_convertible<volatile NonCopyable&,
		const volatile NonCopyable&>::value);

	STATIC_REQUIRE(Rider::Faiz::is_convertible<const volatile NonCopyable&,
		const volatile NonCopyable&>::value);

	STATIC_REQUIRE(
		!Rider::Faiz::is_convertible<const NonCopyable&, NonCopyable&>::value);

	// This test requires Access control SFINAE which we only have in C++11 or
	// when we are using the compiler builtin for is_convertible.
	test_is_not_convertible<NonCopyable&, NonCopyable>();

	// Ensure that CannotInstantiate is not instantiated by is_convertible when
	// it is not needed. For example CannotInstantiate is instatiated as a part
	// of ADL lookup for arguments of type CannotInstantiate*.
	// STATIC_REQUIRE(Rider::Faiz::is_convertible<CannotInstantiate<int>*,
	// 				  CannotInstantiate<int>*>::value);
	//
}
