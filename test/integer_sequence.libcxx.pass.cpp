//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11
// <utility>

// template<class T, T... I>
// struct integer_sequence
// {
//     typedef T type;
//
//     static constexpr size_t size() noexcept;
// };

#include "rider/faiz/cstddef.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"
int
main()
{
	//  Make a few of sequences
	using int3 = Rider::Faiz::integer_sequence<int, 3, 2, 1>;
	using size1 = Rider::Faiz::integer_sequence<Rider::Faiz::size_t, 7>;
	using ushort2 = Rider::Faiz::integer_sequence<unsigned short, 4, 6>;
	using bool0 = Rider::Faiz::integer_sequence<bool>;

	//  Make sure they're what we expect
	static_assert(
		Rider::Faiz::is_same<int3::value_type, int>::value, "int3 type wrong");
	static_assert(int3::size() == 3, "int3 size wrong");

	static_assert(
		Rider::Faiz::is_same<size1::value_type, Rider::Faiz::size_t>::value,
		"size1 type wrong");
	static_assert(size1::size() == 1, "size1 size wrong");

	static_assert(
		Rider::Faiz::is_same<ushort2::value_type, unsigned short>::value,
		"ushort2 type wrong");
	static_assert(ushort2::size() == 2, "ushort2 size wrong");

	static_assert(Rider::Faiz::is_same<bool0::value_type, bool>::value,
		"bool0 type wrong");
	static_assert(bool0::size() == 0, "bool0 size wrong");
}
