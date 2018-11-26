#include "../testsuite_tr1.h"
#include "rider/faiz/type_traits.hpp"

void
test01()
{
	using rider::faiz::add_rvalue_reference;
	using std::is_same;
	using namespace __gnu_test;

	static_assert(is_same<add_rvalue_reference<int>::type, int&&>::value, "");
	static_assert(is_same<add_rvalue_reference<int&&>::type, int&&>::value, "");
	static_assert(is_same<add_rvalue_reference<int&>::type, int&>::value, "");
	static_assert(
		is_same<add_rvalue_reference<const int>::type, const int&&>::value, "");
	static_assert(is_same<add_rvalue_reference<int*>::type, int*&&>::value, "");
	static_assert(
		is_same<add_rvalue_reference<ClassType&&>::type, ClassType&&>::value,
		"");
	static_assert(
		is_same<add_rvalue_reference<ClassType>::type, ClassType&&>::value, "");
	static_assert(
		is_same<add_rvalue_reference<int(int)>::type, int(&&)(int)>::value, "");
	static_assert(is_same<add_rvalue_reference<void>::type, void>::value, "");
	static_assert(
		is_same<add_rvalue_reference<const void>::type, const void>::value, "");
	static_assert(is_same<add_rvalue_reference<bool(int) const>::type,
					  bool(int) const>::value,
		"");
	static_assert(
		is_same<add_rvalue_reference<bool(int)&>::type, bool(int)&>::value, "");
	static_assert(is_same<add_rvalue_reference<bool(int) const&&>::type,
					  bool(int) const&&>::value,
		"");
	static_assert(
		is_same<add_rvalue_reference<bool(int)>::type, bool(&&)(int)>::value,
		"");
}
int
main()
{
	test01();
}
