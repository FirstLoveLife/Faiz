//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// integral_constant

#include "rider/faiz/type_traits.hpp"
#include "test_macros.h"
#include <cassert>

int
main()
{
	typedef rider::faiz::integral_constant<int, 5> _5;
	static_assert(_5::value == 5, "");
	static_assert((rider::faiz::is_same<_5::value_type, int>::value), "");
	static_assert((rider::faiz::is_same<_5::type, _5>::value), "");
	static_assert((_5() == 5), "");
	assert(_5() == 5);


	static_assert(_5{}() == 5, "");
	static_assert(rider::faiz::true_type{}(), "");

	static_assert(rider::faiz::false_type::value == false, "");
	static_assert((rider::faiz::is_same<rider::faiz::false_type::value_type,
					  bool>::value),
		"");
	static_assert((rider::faiz::is_same<rider::faiz::false_type::type,
					  rider::faiz::false_type>::value),
		"");

	static_assert(rider::faiz::true_type::value == true, "");
	static_assert(
		(rider::faiz::is_same<rider::faiz::true_type::value_type, bool>::value),
		"");
	static_assert((rider::faiz::is_same<rider::faiz::true_type::type,
					  rider::faiz::true_type>::value),
		"");

	rider::faiz::false_type f1;
	rider::faiz::false_type f2 = f1;
	assert(!f2);

	rider::faiz::true_type t1;
	rider::faiz::true_type t2 = t1;
	assert(t2);
}
