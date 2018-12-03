//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// conditional

#include "../test_macros.h"
#include "rider/faiz/type_traits.hpp"

int
main()
{
	static_assert(
		(rider::faiz::is_same<rider::faiz::conditional<true, char, int>::type,
			char>::value),
		"");
	static_assert(
		(rider::faiz::is_same<rider::faiz::conditional<false, char, int>::type,
			int>::value),
		"");
	static_assert(
		(rider::faiz::is_same<rider::faiz::conditional_t<true, char, int>,
			char>::value),
		"");
	static_assert(
		(rider::faiz::is_same<rider::faiz::conditional_t<false, char, int>,
			int>::value),
		"");
}
