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
	typedef Rider::Faiz::integral_constant<int, 5> _5;
	static_assert(_5::value == 5, "");
	static_assert((Rider::Faiz::is_same<_5::value_type, int>::value), "");
	static_assert((Rider::Faiz::is_same<_5::type, _5>::value), "");
	static_assert((_5() == 5), "");
	assert(_5() == 5);


	static_assert(_5{}() == 5, "");
	static_assert(Rider::Faiz::true_type{}(), "");

	static_assert(Rider::Faiz::false_type::value == false, "");
	static_assert((Rider::Faiz::is_same<Rider::Faiz::false_type::value_type,
					  bool>::value),
		"");
	static_assert((Rider::Faiz::is_same<Rider::Faiz::false_type::type,
					  Rider::Faiz::false_type>::value),
		"");

	static_assert(Rider::Faiz::true_type::value == true, "");
	static_assert(
		(Rider::Faiz::is_same<Rider::Faiz::true_type::value_type, bool>::value),
		"");
	static_assert((Rider::Faiz::is_same<Rider::Faiz::true_type::type,
					  Rider::Faiz::true_type>::value),
		"");

	Rider::Faiz::false_type f1;
	Rider::Faiz::false_type f2 = f1;
	assert(!f2);

	Rider::Faiz::true_type t1;
	Rider::Faiz::true_type t2 = t1;
	assert(t2);
}
