
/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
#include "rider/faiz/macros.hpp"
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CRUFT_TIGHT_PAIR_H_
#define CRUFT_TIGHT_PAIR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "rider/faiz/math/RelationAlgebra.hpp" // for totally_ordered
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"
#include "rider/faiz/utility/swap.hpp"
#include <climits>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <tuple>

#ifndef CRUFT_TIGHT_PAIR_USE_UNSIGNED_128INT
#	if defined(__clang__)
#		define CRUFT_TIGHT_PAIR_USE_UNSIGNED_128INT 1
#	elif defined(__GNUC__)
#		if __GNUC__ >= 8
#			define CRUFT_TIGHT_PAIR_USE_UNSIGNED_128INT 1
#		else
#			define CRUFT_TIGHT_PAIR_USE_UNSIGNED_128INT 0
#		endif
#	else
#		define CRUFT_TIGHT_PAIR_USE_UNSIGNED_128INT 0
#	endif
#endif

namespace Rider::Faiz
{
	////////////////////////////////////////////////////////////
	// Forward declarations of main components

	Tpl<Typ T1, Typ T2> struct tight_pair;

	Tpl<size_t N, Typ T1, Typ T2> cfn
	get(tight_pair<T1, T2>&) noexcept
		->std::tuple_element_t<N, tight_pair<T1, T2>>&;

	Tpl<size_t N, Typ T1, Typ T2> cfn
	get(tight_pair<T1, T2> const&) noexcept
		->std::tuple_element_t<N, tight_pair<T1, T2>> const&;

	Tpl<size_t N, Typ T1, Typ T2> cfn
	get(tight_pair<T1, T2>&&) noexcept
		->std::tuple_element_t<N, tight_pair<T1, T2>>&&;

	Tpl<size_t N, Typ T1, Typ T2> cfn
	get(tight_pair<T1, T2> const&&) noexcept
		->std::tuple_element_t<N, tight_pair<T1, T2>> const&&;
} // namespace Rider::Faiz

namespace std
{
	////////////////////////////////////////////////////////////
	// Specializations needed for decomposition declarations

	Tpl<Typ T1, Typ T2> class tuple_size<Rider::Faiz::tight_pair<T1, T2>>
		: public Rider::Faiz::size_t_<2>
	{};

	Tpl<Typ T1, Typ T2> class tuple_element<0, Rider::Faiz::tight_pair<T1, T2>>
		: public Rider::type_identity<T1>
	{};

	Tpl<Typ T1, Typ T2> class tuple_element<1, Rider::Faiz::tight_pair<T1, T2>>
		: public Rider::type_identity<T2>
	{};

	Tpl<size_t N, Typ T1, Typ T2> class tuple_element<N,
		Rider::Faiz::tight_pair<T1, T2>>
	{
	public:
		static_assert(N < 2,
			"out of bounds index for tuple_element<N, "
			"Rider::Faiz::tight_pair<T1, "
			"T2>>");
	};
} // namespace std

namespace Rider::Faiz
{
	namespace detail
	{
		////////////////////////////////////////////////////////////
		// Whether a type can benefit from the empty base class
		// optimization

		Tpl<Typ T> struct is_ebco_eligible
			: logic::and_<is_empty<T>, not_final<T>>
		{};

		IS_NOT_ARE_ANY(ebco_eligible);

		////////////////////////////////////////////////////////////
		// Detect whether an integer has padding bits

		Tpl<Typ tUnsignedInteger> cfn
		has_padding_bits()->bool
		{
#ifdef __cpp_lib_has_unique_object_representations
			// A note in the standard mentions than unsigned integer types
			// are guaranteed to have unique object representations when
			// they don't have padding bits

			return not std::has_unique_object_representations_v<
				tUnsignedInteger>;
#else
			// Algorithm partly taken from WG14 N1899, also handles unsigned
			// integer types such as unsigned __int128 that don't have an
			// std::numeric_limits specialization

			size_t precision = 0;
			auto num = static_cast<tUnsignedInteger>(-1);
			while(num != 0)
			{
				if(num % 2 == 1)
				{
					++precision;
				}
				num >>= 1;
			}
			return precision != sizeof(tUnsignedInteger) * CHAR_BIT;
#endif
		}

		////////////////////////////////////////////////////////////
		// Find an unsigned integer type twice as big as the given
		// one, ensure all bits are used

		Tpl<Typ tUInt> constexpr decltype(auto)
		twice_as_big()
		{
			cIf(has_padding_bits<tUInt>())
			{
				return;
			}

			cElseIf(sizeof(unsigned short) == 2 * sizeof(tUInt)
				and not has_padding_bits<unsigned short>())
			{
				return static_cast<unsigned short>(0);
			}

			cElseIf(sizeof(unsigned int) == 2 * sizeof(tUInt)
				and not has_padding_bits<unsigned int>())
			{
				return static_cast<unsigned int>(0);
			}

			cElseIf(sizeof(unsigned long) == 2 * sizeof(tUInt)
				and not has_padding_bits<unsigned long>())
			{
				return static_cast<unsigned long>(0);
			}

			cElseIf(sizeof(unsigned long long) == 2 * sizeof(tUInt)
				and not has_padding_bits<unsigned long long>())
			{
				return static_cast<unsigned long long>(0);
			}

			cElseIf(sizeof(std::uintmax_t) == 2 * sizeof(tUInt)
				and not has_padding_bits<std::uintmax_t>())
			{
				return static_cast<std::uintmax_t>(0);
			}

#if CRUFT_TIGHT_PAIR_USE_UNSIGNED_128INT && defined(__SIZEOF_INT128__)
			// Only use unsigned __int128 with compilers
			// which are known to produce branchless code
			// for comparisons
			cElseIf(sizeof(unsigned __int128) == 2 * sizeof(tUInt)
				and not has_padding_bits<unsigned __int128>())
			{
				return static_cast<unsigned __int128>(0);
			}
#endif
		}

		Tpl<Typ T> struct has_twice_as_big
			: logic::not_<is_void<decltype(twice_as_big<T>())>>
		{};

		Tpl<Typ T> constexpr decltype(twice_as_big<T>())
		get_twice_as_big(tight_pair<T, T> const& value) noexcept
		{
			// If the two parts of the tight_pair are unsigned integers
			// suitably ordered (which depends on the byte order), this
			// whole function be optimized away as a no-op

			// We use CHAR_BIT instead of std::numeric_limits because the
			// latter might lack a few specializations for types such as
			// unsigned __int128 unless some specific constant is defined

			using Rider::Faiz::get;
			return static_cast<decltype(twice_as_big<T>())>(get<0>(value))
				<< sizeof(T) * CHAR_BIT
				| get<1>(value);
		}

		Tpl<Typ T> struct can_optimize_compare
			: bool_<(std::endian::native == std::endian::little
						or std::endian::native == std::endian::big)
				  and is_unsigned_v<T> and has_twice_as_big<T>::value>
		{};

		////////////////////////////////////////////////////////////
		// Bits from libc++ <__tuple> header and more

		struct nat
		{
			nat() = delete;
			nat(const nat&) = delete;
			nat&
			operator=(const nat&)
				= delete;
			~nat() = delete;
		};

		struct check_tuple_constructor_fail
		{
			Tpl<Typ...> static constexpr bool enable_default = false;

			Tpl<Typ...> static constexpr bool enable_explicit = false;

			Tpl<Typ...> static constexpr bool enable_implicit = false;

			Tpl<Typ...> static constexpr bool enable_assign = false;
		};

		////////////////////////////////////////////////////////////
		// Utilities to work with pair-like types: types for which
		// std::tuple_size_v<T> == 2 and get<0>/get<1> work

		namespace adl_hook
		{
			using std::get;

			Tpl<Typ T, Typ Enable> struct pair_like_impl : false_
			{};

			Tpl<Typ T> struct pair_like_impl<T,
				void_t<Typ std::tuple_size<decay_t<T>>::type,
					decltype(get<0>(declval<T&>())),
					decltype(get<1>(declval<T&>()))>>
				: bool_<std::tuple_size_v<decay_t<T>> == 2>
			{};
		} // namespace adl_hook

		Tpl<Typ T, Typ U = remove_cv_t<T>> struct pair_like
			: adl_hook::pair_like_impl<U, void>
		{};

		// Safeguard to avoid ambiguous conversions
		Tpl<Typ T1, Typ T2> struct pair_like<Rider::Faiz::tight_pair<T1, T2>>
			: false_
		{};

		// pair_assignable, pair_constructible, pair_convertible

		namespace adl_hook
		{
			// Import a Tpl get for ADL
			using std::get;

			Tpl<Typ tTo, Typ tFrom> struct pair_assignable
				: are_assignable<PAIR(decltype(get<0>(declval<tTo>())),
									 decltype(get<0>(declval<tFrom>()))),
					  PAIR(decltype(get<1>(declval<tTo>())),
						  decltype(get<1>(declval<tFrom>())))>
			{};

			Tpl<Typ tTo, Typ tFrom> struct pair_constructible
				: are_constructible<Pack<decltype(get<0>(declval<tTo>())),
										decltype(get<0>(declval<tFrom>()))>,
					  Pack<decltype(get<1>(declval<tTo>())),
						  decltype(get<1>(declval<tFrom>()))>>
			{};

			Tpl<Typ tFrom, Typ tTo> struct pair_convertible
				: logic::and_<is_convertible<decltype(get<0>(declval<tFrom>())),
								  decltype(get<0>(declval<tTo>()))>,
					  is_assignable<decltype(get<1>(declval<tFrom>())),
						  decltype(get<1>(declval<tTo>()))>>
			{};
		} // namespace adl_hook

		////////////////////////////////////////////////////////////
		// Type used for the storage of a single pair member: the
		// Tpl integer parameter is used to disambiguate the
		// types when both members the same underlying types

		Tpl<size_t N,
			Typ T,
			bool = is_ebco_eligible_v<T>,
			bool = is_reference_v<T>> struct tight_pair_element;

		Tpl<size_t N, Typ T> struct tight_pair_element<N, T, false, false>
		{
		private:
			T value;

		public:
			////////////////////////////////////////////////////////////
			// Construction

			tight_pair_element(tight_pair_element const&) = default;
			tight_pair_element(tight_pair_element&&) = default;

			cCTOR
			tight_pair_element() noexcept(is_nothrow_default_constructible_v<T>)
				: value()
			{
				static_assert(not_reference_v<T>,
					"attempted to default construct a reference element");
			}

			// clang-format on
			Tpl<Typ U, Typ = enable_if_t<is_constructible_v<T, U>>> ceCTOR
			tight_pair_element(U&& other) noexcept(
				is_nothrow_constructible_v<T, U>)
				: value(Faiz::forward<U>(other))
			{}

			Tpl<Typ Tuple> cCTOR
			tight_pair_element(std::piecewise_construct_t, Tuple&& args)
				: value(std::make_from_tuple<T>(Faiz::forward<Tuple>(args)))
			{}

			Tpl<Typ U> cfn
			operator=(U&& other) noexcept(is_nothrow_assignable_v<T&, U>)
				->tight_pair_element&
			{
				value = Faiz::forward<U>(other);
				return *this;
			}

			tight_pair_element&
			operator=(tight_pair_element const&)
				= default;
			tight_pair_element&
			operator=(tight_pair_element&&)
				= default;

			////////////////////////////////////////////////////////////
			// Element access

			cfn
			do_get() & noexcept->T&
			{
				return static_cast<T&>(value);
			}

			cfn
			do_get() const & noexcept->T const&
			{
				return static_cast<T const&>(value);
			}

			cfn
			do_get() && noexcept->T&&
			{
				return static_cast<T&&>(value);
			}

			cfn
			do_get() const && noexcept->T const&&
			{
				return static_cast<T const&&>(value);
			}
		};

		Tpl<size_t N, Typ T> struct tight_pair_element<N, T, false, true>
		{
		private:
			T value;

		public:
			////////////////////////////////////////////////////////////
			// Construction

			tight_pair_element(tight_pair_element const&) = default;
			tight_pair_element(tight_pair_element&&) = default;

			cCTOR
			tight_pair_element() noexcept(is_nothrow_default_constructible_v<T>)
				: value()
			{
				static_assert(not_reference_v<T>,
					"attempted to default construct a reference element");
			}

			Tpl<Typ U, Typ = enable_if_t<is_constructible_v<T, U>>> ceCTOR
			tight_pair_element(U&& other) noexcept(
				is_nothrow_constructible_v<T, U>)
				: value(Faiz::forward<U>(other))
			{}

			Tpl<Typ Tuple> cCTOR
			tight_pair_element(std::piecewise_construct_t, Tuple&& args)
				: value(std::make_from_tuple<T>(Faiz::forward<Tuple>(args)))
			{}

			cfn
			operator=(tight_pair_element const& other)->tight_pair_element&
			{
				value = other.value;
				return *this;
			}

			cfn
			operator=(tight_pair_element&& other)->tight_pair_element&
			{
				value = move(other.value);
				return *this;
			}

			Tpl<Typ U> cfn
			operator=(U&& other) noexcept(is_nothrow_assignable_v<T&, U>)
				->tight_pair_element&
			{
				value = Faiz::forward<U>(other);
				return *this;
			}

			////////////////////////////////////////////////////////////
			// Element access

			cfn
			do_get() & noexcept->T&
			{
				return static_cast<T&>(value);
			}

			cfn
			do_get() const & noexcept->T const&
			{
				return static_cast<T const&>(value);
			}

			cfn
			do_get() && noexcept->T&&
			{
				return static_cast<T&&>(value);
			}

			cfn
			do_get() const && noexcept->T const&&
			{
				return static_cast<T const&&>(value);
			}
		};

		Tpl<size_t N, Typ T> struct tight_pair_element<N, T, true, false>
			: private T
		{
			////////////////////////////////////////////////////////////
			// Construction

			tight_pair_element(tight_pair_element const&) = default;
			tight_pair_element(tight_pair_element&&) = default;

			cCTOR
			tight_pair_element() noexcept(is_nothrow_default_constructible_v<T>)
			{}

			Tpl<Typ U, Typ = enable_if_t<is_constructible_v<T, U>>> ceCTOR
			tight_pair_element(U&& other) noexcept(
				is_nothrow_constructible_v<T, U>)
				: T(Faiz::forward<U>(other))
			{}

			Tpl<Typ Tuple> cCTOR
			tight_pair_element(std::piecewise_construct_t, Tuple&& args)
				: T(std::make_from_tuple<T>(Faiz::forward<Tuple>(args)))
			{}

			tight_pair_element&
			operator=(tight_pair_element const&)
				= default;
			tight_pair_element&
			operator=(tight_pair_element&&)
				= default;

			////////////////////////////////////////////////////////////
			// Element access

			cfn
			do_get() & noexcept->T&
			{
				return static_cast<T&>(*this);
			}

			cfn
			do_get() const & noexcept->T const&
			{
				return static_cast<T const&>(*this);
			}

			cfn
			do_get() && noexcept->T&&
			{
				return static_cast<T&&>(*this);
			}

			cfn
			do_get() const && noexcept->T const&&
			{
				return static_cast<T const&&>(*this);
			}
		};

		////////////////////////////////////////////////////////////
		// Whether the array elements need to be swapped

		// Additional notes: in the case of little endian architectures
		// the fields of the array will be reversed so that the array
		// can be reinterpreted as an integer twice as big when storing
		// integer data

		Tpl<Typ T, Typ U = remove_cv_t<T>> struct needs_reordering
			: bool_<std::endian::native == std::endian::little
				  and can_optimize_compare<T>::value>
		{};

		////////////////////////////////////////////////////////////
		// Type used for the storage of the pair member: the
		// Tpl integer parameter is used to disambiguate the
		// types when both have the same underlying types

		Tpl<Typ T1,
			Typ T2,
			bool RegularStorage = is_ebco_eligible_v<T1> or is_reference_v<T1>,
			bool NeedsReodering
			= needs_reordering<T1>::value> struct tight_pair_storage
			: tight_pair_element<0, T1>,
			  tight_pair_element<1, T2>
		{
			////////////////////////////////////////////////////////////
			// Construction

			tight_pair_storage(tight_pair_storage const&) = default;
			tight_pair_storage(tight_pair_storage&&) = default;

			cCTOR
			tight_pair_storage()
				: tight_pair_element<0, T1>(), tight_pair_element<1, T2>()
			{}

			Tpl<Typ U1, Typ U2> cCTOR
			tight_pair_storage(U1&& first, U2&& second)
				: tight_pair_element<0, T1>(Faiz::forward<U1>(first)),
				  tight_pair_element<1, T2>(Faiz::forward<U2>(second))
			{}

			Tpl<Typ Tuple1, Typ Tuple2> cCTOR
			tight_pair_storage(std::piecewise_construct_t pc,
				Tuple1&& first_args,
				Tuple2&& second_args)
				: tight_pair_element<0, T1>(
					pc, Faiz::forward<Tuple1>(first_args)),
				  tight_pair_element<1, T2>(
					  pc, Faiz::forward<Tuple2>(second_args))
			{}

			tight_pair_storage&
			operator=(tight_pair_storage const&)
				= default;
			tight_pair_storage&
			operator=(tight_pair_storage&&)
				= default;

			////////////////////////////////////////////////////////////
			// Element access

			Tpl<size_t N> cfn
			do_get() &->decltype(auto)
			{
				cIf(N == 0)
				{
					return static_cast<tight_pair_element<0, T1>&>(*this)
						.do_get();
				}
				cElseIf(N == 1)
				{
					return static_cast<tight_pair_element<1, T2>&>(*this)
						.do_get();
				}
			}

			Tpl<size_t N> cfn
			do_get() const&->decltype(auto)
			{
				cIf(N == 0)
				{
					return static_cast<tight_pair_element<0, T1> const&>(*this)
						.do_get();
				}
				cElseIf(N == 1)
				{
					return static_cast<tight_pair_element<1, T2> const&>(*this)
						.do_get();
				}
			}

			Tpl<size_t N> cfn
			do_get() &&->decltype(auto)
			{
				cIf(N == 0)
				{
					return static_cast<tight_pair_element<0, T1>&&>(*this)
						.do_get();
				}
				cElseIf(N == 1)
				{
					return static_cast<tight_pair_element<1, T2>&&>(*this)
						.do_get();
				}
			}

			Tpl<size_t N> cfn
			do_get() const&&->decltype(auto)
			{
				cIf(N == 0)
				{
					return static_cast<tight_pair_element<0, T1> const&&>(*this)
						.do_get();
				}
				cElseIf(N == 1)
				{
					return static_cast<tight_pair_element<1, T2> const&&>(*this)
						.do_get();
				}
			}
		};

		Tpl<Typ T> struct tight_pair_storage<T, T, false, false>
		{
			// Store elements contiguously, avoid padding between elements
			T elements[2];

			////////////////////////////////////////////////////////////
			// Construction

			tight_pair_storage(tight_pair_storage const&) = default;
			tight_pair_storage(tight_pair_storage&&) = default;

			constexpr tight_pair_storage() : elements()
			{}

			Tpl<Typ U1, Typ U2> constexpr tight_pair_storage(
				U1&& first, U2&& second)
				: elements{
					T(Faiz::forward<U1>(first)), T(Faiz::forward<U2>(second))}
			{}

			Tpl<Typ Tuple1, Typ Tuple2> constexpr tight_pair_storage(
				std::piecewise_construct_t,
				Tuple1&& first_args,
				Tuple2&& second_args)
				: elements{
					std::make_from_tuple<T>(Faiz::forward<Tuple1>(first_args)),
					std::make_from_tuple<T>(Faiz::forward<Tuple2>(second_args))}
			{}

			tight_pair_storage&
			operator=(tight_pair_storage const&)
				= default;
			tight_pair_storage&
			operator=(tight_pair_storage&&)
				= default;

			////////////////////////////////////////////////////////////
			// Element access

			Tpl<size_t N> cfn
			do_get() &->decltype(auto)
			{
				return static_cast<T&>(elements[N]);
			}

			Tpl<size_t N> cfn
			do_get() const&->decltype(auto)
			{
				return static_cast<T const&>(elements[N]);
			}

			Tpl<size_t N> cfn
			do_get() &&->decltype(auto)
			{
				return static_cast<T&&>(elements[N]);
			}

			Tpl<size_t N> cfn
			do_get() const&&->decltype(auto)
			{
				return static_cast<T const&&>(elements[N]);
			}
		};

		Tpl<Typ T> struct tight_pair_storage<T, T, false, true>
		{
			// Store elements contiguously, avoid padding between elements,
			// reorder for more efficient bit tricks
			T elements[2];

			////////////////////////////////////////////////////////////
			// Construction

			tight_pair_storage(tight_pair_storage const&) = default;
			tight_pair_storage(tight_pair_storage&&) = default;

			constexpr tight_pair_storage() : elements()
			{}

			Tpl<Typ U1, Typ U2> constexpr tight_pair_storage(
				U1&& second, U2&& first)
				: elements{
					T(Faiz::forward<U2>(first)), T(Faiz::forward<U1>(second))}
			{}

			Tpl<Typ Tuple1, Typ Tuple2> constexpr tight_pair_storage(
				std::piecewise_construct_t,
				Tuple2&& second_args,
				Tuple1&& first_args)
				: elements{
					std::make_from_tuple<T>(Faiz::forward<Tuple1>(first_args)),
					std::make_from_tuple<T>(Faiz::forward<Tuple2>(second_args))}
			{}

			tight_pair_storage&
			operator=(tight_pair_storage const&)
				= default;
			tight_pair_storage&
			operator=(tight_pair_storage&&)
				= default;

			////////////////////////////////////////////////////////////
			// Element access

			Tpl<size_t N> cfn
			do_get() &->decltype(auto)
			{
				cIf(N == 0)
				{
					return static_cast<T&>(elements[1]);
				}
				cElseIf(N == 1)
				{
					return static_cast<T&>(elements[0]);
				}
			}

			Tpl<size_t N> cfn
			do_get() const&->decltype(auto)
			{
				cIf(N == 0)
				{
					return static_cast<T const&>(elements[1]);
				}
				cElseIf(N == 1)
				{
					return static_cast<T const&>(elements[0]);
				}
			}

			Tpl<size_t N> cfn
			do_get() &&->decltype(auto)
			{
				cIf(N == 0)
				{
					return static_cast<T&&>(elements[1]);
				}
				cElseIf(N == 1)
				{
					return static_cast<T&&>(elements[0]);
				}
			}

			Tpl<size_t N> cfn
			do_get() const&&->decltype(auto)
			{
				cIf(N == 0)
				{
					return static_cast<T const&&>(elements[1]);
				}
				cElseIf(N == 1)
				{
					return static_cast<T const&&>(elements[0]);
				}
			}
		};
	} // namespace detail

	////////////////////////////////////////////////////////////
	// Main class

	Tpl<Typ T1, Typ T2> struct tight_pair
		: private detail::tight_pair_storage<T1, T2>,
		  private Math::AbstractAlgebra::RelationAlgebra::totally_ordered<
			  tight_pair<T1, T2>>
	{
	private:
		struct check_args
		{
			Tpl<Typ U1, Typ U2> static cfn
			enable_default()->bool
			{
				return are_default_constructible_v<U1, U2>;
			}

			Tpl<Typ U1, Typ U2> static cfn
			enable_explicit()->bool
			{
				// clang-format off
				return
					are_constructible_v<Pack<T1, U1>,
										Pack<T2, U2>>
					and
					not_all_convertible_v<PAIR(U1, T1),
										  PAIR(U2, T2)>;
			}

			Tpl<Typ U1, Typ U2> static cfn
			enable_implicit()->bool
			{
				return
					are_constructible_v<Pack<T1, U1>,
										Pack<T2, U2>>
					and
					are_convertible_v<PAIR(U1, T1),
									  PAIR(U2, T2)>;
				// clang-format on
			}
		};

		struct check_tuple_like_constructor
		{
			Tpl<Typ Tuple> static cfn
			enable_implicit()->bool
			{
				return detail::adl_hook::pair_convertible<Tuple,
					tight_pair>::value;
			}

			Tpl<Typ Tuple> static cfn
			enable_explicit()->bool
			{
				return detail::adl_hook::pair_constructible<tight_pair&,
						   Tuple>::value
					and not detail::adl_hook::pair_convertible<Tuple,
						tight_pair>::value;
			}

			Tpl<Typ Tuple> static cfn
			enable_assign()->bool
			{
				return detail::adl_hook::pair_assignable<tight_pair&,
					Tuple>::value;
			}
		};

		Tpl<Typ T> using check_pair_like
			= meta::if_c<detail::pair_like<remove_reference_t<T>>::value,
				check_tuple_like_constructor,
				detail::check_tuple_constructor_fail>;

	public:
		////////////////////////////////////////////////////////////
		// Construction

		tight_pair(tight_pair const&) = default;
		tight_pair(tight_pair&&) = default;

		Tpl<Typ U1 = T1,
			Typ U2 = T2,
			enable_if_t<check_args::Tpl enable_default<U1, U2>(), bool> = false>
			cCTOR
			tight_pair() noexcept(are_nothrow_default_constructible_v<T1, T2>)
			: detail::tight_pair_storage<T1, T2>()
		{}

		Tpl<Typ U1 = T1,
			Typ U2 = T2,
			enable_if_t<check_args::Tpl enable_explicit<U1 const&, U2 const&>(),
				bool> = false>
			ceCTOR
			tight_pair(T1 const& first, T2 const& second = {}) noexcept(
				are_nothrow_copy_constructible_v<T1, T2>)
			: detail::tight_pair_storage<T1, T2>(first, second)
		{}

		Tpl<Typ U1 = T1,
			Typ U2 = T2,
			enable_if_t<check_args::Tpl enable_implicit<U1 const&, U2 const&>(),
				bool> = false>
			cCTOR
			tight_pair(T1 const& first, T2 const& second = {}) noexcept(
				are_nothrow_copy_constructible_v<T1, T2>)
			: detail::tight_pair_storage<T1, T2>(first, second)
		{}

		Tpl<Typ U1 = T1,
			Typ U2 = T2,
			enable_if_t<check_args::Tpl enable_explicit<U1, U2>(), bool> = false>
			ceCTOR
			tight_pair(U1&& first, U2&& second) noexcept(
				are_nothrow_constructible_v<Pack<T1, U1>, Pack<T2, U2>>)
			: detail::tight_pair_storage<T1, T2>(
				Faiz::forward<U1>(first), Faiz::forward<U2>(second))
		{}

		Tpl<Typ U1 = T1,
			Typ U2 = T2,
			enable_if_t<check_args::Tpl enable_implicit<U1, U2>(),
				bool> = false> constexpr tight_pair(U1&& first,
			U2&& second) noexcept(are_nothrow_constructible_v<Pack<T1, U1>,
			Pack<T2, U2>>)
			: detail::tight_pair_storage<T1, T2>(
				Faiz::forward<U1>(first), Faiz::forward<U2>(second))
		{}

		Tpl<Typ U1 = T1,
			Typ U2 = T2,
			enable_if_t<check_args::Tpl enable_explicit<U1 const&, U2 const&>(),
				bool> = false>
			ceCTOR
			tight_pair(tight_pair<U1, U2> const& pair) noexcept(
				are_nothrow_constructible_v<Pack<T1, U1 const&>,
					Pack<T2, U2 const&>>)
			: detail::tight_pair_storage<T1, T2>(
				pair.Tpl get<0>(), pair.Tpl get<1>())
		{}

		Tpl<Typ U1 = T1,
			Typ U2 = T2,
			enable_if_t<check_args::Tpl enable_implicit<U1 const&, U2 const&>(),
				bool> = false>
			cCTOR
			tight_pair(tight_pair<U1, U2> const& pair) noexcept(
				are_nothrow_constructible_v<Pack<T1, U1 const&>,
					Pack<T2, U2 const&>>)
			: detail::tight_pair_storage<T1, T2>(
				pair.Tpl get<0>(), pair.Tpl get<1>())
		{}

		Tpl<Typ U1 = T1,
			Typ U2 = T2,
			enable_if_t<check_args::Tpl enable_explicit<U1, U2>(), bool> = false>
			ceCTOR
			tight_pair(tight_pair<U1, U2>&& pair) noexcept(
				are_nothrow_constructible_v<Pack<T1, U1&&>, Pack<T2, U2&&>>)
			: detail::tight_pair_storage<T1, T2>(
				Faiz::forward<U1>(pair.Tpl get<0>()),
				Faiz::forward<U2>(pair.Tpl get<1>()))
		{}

		Tpl<Typ U1 = T1,
			Typ U2 = T2,
			enable_if_t<check_args::Tpl enable_implicit<U1, U2>(), bool> = false>
			cCTOR
			tight_pair(tight_pair<U1, U2>&& pair) noexcept(
				are_nothrow_constructible_v<Pack<T1, U1&&>, Pack<T2, U2&&>>)
			: detail::tight_pair_storage<T1, T2>(
				Faiz::forward<U1>(pair.Tpl get<0>()),
				Faiz::forward<U2>(pair.Tpl get<1>()))
		{}

		Tpl<Typ Tuple,
			enable_if_t<check_pair_like<Tuple>::Tpl enable_explicit<
				Tuple>()>> constexpr explicit tight_pair(Tuple&& tuple)
			: detail::tight_pair_storage<T1, T2>(
				Faiz::forward<T1>(get<0>(Faiz::forward<Tuple>(tuple))),
				Faiz::forward<T2>(get<1>(Faiz::forward<Tuple>(tuple))))
		{}

		Tpl<Typ Tuple,
			enable_if_t<check_pair_like<Tuple>::Tpl enable_implicit<
				Tuple>()>> constexpr tight_pair(Tuple&& tuple)
			: detail::tight_pair_storage<T1, T2>(
				Faiz::forward<T1>(get<0>(Faiz::forward<Tuple>(tuple))),
				Faiz::forward<T2>(get<1>(Faiz::forward<Tuple>(tuple))))
		{}

		Tpl<Typ Tuple1, Typ Tuple2> constexpr tight_pair(
			std::piecewise_construct_t pc,
			Tuple1&& first_args,
			Tuple2&& second_args)
			: detail::tight_pair_storage<T1, T2>(pc,
				Faiz::forward<Tuple1>(first_args),
				Faiz::forward<Tuple2>(second_args))
		{}

		////////////////////////////////////////////////////////////
		// Assignment operator

		cfn
		operator=(meta::if_<are_copy_assignable<T1, T2>,
			tight_pair,
			detail::nat> const&
				other) noexcept(noexcept(are_nothrow_copy_assignable_v<T1, T2>))
			->tight_pair&
		{
			static_cast<detail::tight_pair_storage<T1, T2>&>(*this)
				= static_cast<detail::tight_pair_storage<T1, T2> const&>(other);
			return *this;
		}

		cfn
		operator=(
			meta::if_<are_move_assignable<T1, T2>, tight_pair, detail::nat>&&
				other) noexcept(noexcept(are_nothrow_move_assignable_v<T1, T2>))
			->tight_pair&
		{
			static_cast<detail::tight_pair_storage<T1, T2>&>(*this)
				= static_cast<detail::tight_pair_storage<T1, T2>&&>(other);
			return *this;
		}

		Tpl<Typ Tuple,
			Typ
			= enable_if_t<check_pair_like<Tuple>::Tpl enable_assign<Tuple>()>>
			cfn
		operator=(Tuple&& other)->tight_pair&
		{
			using std::get;
			get<0>(*this) = get<0>(Faiz::forward<Tuple>(other));
			get<1>(*this) = get<1>(Faiz::forward<Tuple>(other));
			return *this;
		}

		////////////////////////////////////////////////////////////
		// Swap

		cfn
		swap(tight_pair& other) noexcept(range::are_nothrow_swappable_v<T1, T2>)
			->void
		{
			std::swap(static_cast<detail::tight_pair_storage<T1, T2>&>(*this),
				static_cast<detail::tight_pair_storage<T1, T2>&>(other));
		}

		////////////////////////////////////////////////////////////
		// Element access
		//
		// This function is necessary to avoid structured bindings
		// picking a potential inaccessible get from an empty base
		// class

		Tpl<size_t N> cfn
		get() & noexcept->std::tuple_element_t<N, tight_pair>&
		{
			using storage_t = detail::tight_pair_storage<T1, T2>;
			return static_cast<std::tuple_element_t<N, tight_pair<T1, T2>>&>(
				static_cast<storage_t&>(*this).Tpl do_get<N>());
		}

		Tpl<size_t N> cfn
		get() const & noexcept->std::tuple_element_t<N, tight_pair> const&
		{
			using storage_t = detail::tight_pair_storage<T1, T2>;
			return static_cast<
				std::tuple_element_t<N, tight_pair<T1, T2>> const&>(
				static_cast<storage_t const&>(*this).Tpl do_get<N>());
		}

		Tpl<size_t N> cfn
		get() && noexcept->std::tuple_element_t<N, tight_pair>&&
		{
			using storage_t = detail::tight_pair_storage<T1, T2>;
			return static_cast<std::tuple_element_t<N, tight_pair<T1, T2>>&&>(
				static_cast<storage_t&&>(*this).Tpl do_get<N>());
		}

		Tpl<size_t N> cfn
		get() const && noexcept->std::tuple_element_t<N, tight_pair> const&&
		{
			using storage_t = detail::tight_pair_storage<T1, T2>;
			return static_cast<
				std::tuple_element_t<N, tight_pair<T1, T2>> const&&>(
				static_cast<storage_t const&&>(*this).Tpl do_get<N>());
		}
	};

	Tpl<Typ T, Typ U> cfn
	operator<(tight_pair<T, U> const& lhs, tight_pair<T, U> const& rhs)->bool
	{
		if(Rider::Faiz::get<0>(lhs) < Rider::Faiz::get<0>(rhs))
		{
			return true;
		}
		if(Rider::Faiz::get<0>(rhs) < Rider::Faiz::get<0>(lhs))
		{
			return false;
		}
		return Rider::Faiz::get<1>(lhs) < Rider::Faiz::get<1>(rhs);
	}
	Tpl<Typ T, Typ U> cfn
	operator==(tight_pair<T, U> const& lhs, tight_pair<T, U> const& rhs)->bool
	{
		return Rider::Faiz::get<0>(lhs) == Rider::Faiz::get<0>(rhs)
			and Rider::Faiz::get<1>(lhs) == Rider::Faiz::get<1>(rhs);
	}
	Tpl<Typ T> cfn
	operator<(tight_pair<T, T> const& lhs, tight_pair<T, T> const& rhs)
		->enable_if_t<detail::can_optimize_compare<T>::value, bool>
	{
		auto big_lhs = detail::get_twice_as_big(lhs);
		auto big_rhs = detail::get_twice_as_big(rhs);
		return big_lhs < big_rhs;
	}

	////////////////////////////////////////////////////////////
	// Deduction guide

	Tpl<Typ T1, Typ T2> tight_pair(T1, T2)->tight_pair<T1, T2>;

	Tpl<Typ T1, Typ T2>
		tight_pair(std::reference_wrapper<T1>, T2)->tight_pair<T1&, T2>;

	Tpl<Typ T1, Typ T2>
		tight_pair(T1, std::reference_wrapper<T2>)->tight_pair<T1, T2&>;

	Tpl<Typ T1, Typ T2>
		tight_pair(std::reference_wrapper<T1>, std::reference_wrapper<T2>)
			->tight_pair<T1&, T2&>;

	////////////////////////////////////////////////////////////
	// Free swap function

	Tpl<Typ T1, Typ T2, Typ = enable_if_t<range::are_swappable_v<T1, T2>>> cfn
	swap(tight_pair<T1, T2>& lhs, tight_pair<T1, T2>& rhs) noexcept(
		noexcept(lhs.swap(rhs)))
		->void
	{
		lhs.swap(rhs);
	}

	////////////////////////////////////////////////////////////
	// Index-based get functions for element access and
	// structured bindings

	Tpl<size_t N, Typ T1, Typ T2> cfn
	get(tight_pair<T1, T2>& pair) noexcept
		->std::tuple_element_t<N, tight_pair<T1, T2>>&
	{
		return static_cast<std::tuple_element_t<N, tight_pair<T1, T2>>&>(
			pair.Tpl get<N>());
	}

	Tpl<size_t N, Typ T1, Typ T2> cfn
	get(tight_pair<T1, T2> const& pair) noexcept
		->std::tuple_element_t<N, tight_pair<T1, T2>> const&
	{
		return static_cast<std::tuple_element_t<N, tight_pair<T1, T2>> const&>(
			pair.Tpl get<N>());
	}

	Tpl<size_t N, Typ T1, Typ T2> cfn
	get(tight_pair<T1, T2>&& pair) noexcept
		->std::tuple_element_t<N, tight_pair<T1, T2>>&&
	{
		return static_cast<std::tuple_element_t<N, tight_pair<T1, T2>>&&>(
			pair.Tpl get<N>());
	}

	Tpl<size_t N, Typ T1, Typ T2> cfn
	get(tight_pair<T1, T2> const&& pair) noexcept
		->std::tuple_element_t<N, tight_pair<T1, T2>> const&&
	{
		return static_cast<std::tuple_element_t<N, tight_pair<T1, T2>> const&&>(
			pair.Tpl get<N>());
	}

	////////////////////////////////////////////////////////////
	// Type-based get functions for element access

	Tpl<Typ T1, Typ T2> cfn
	get(tight_pair<T1, T2>& pair) noexcept->T1&
	{
		return get<0>(pair);
	}

	Tpl<Typ T1, Typ T2> cfn
	get(tight_pair<T1, T2> const& pair) noexcept->T1 const&
	{
		return get<0>(pair);
	}

	Tpl<Typ T1, Typ T2> cfn
	get(tight_pair<T1, T2>&& pair) noexcept->T1&&
	{
		return get<0>(Faiz::move(pair));
	}

	Tpl<Typ T1, Typ T2> cfn
	get(tight_pair<T1, T2> const&& pair) noexcept->T1 const&&
	{
		return get<0>(Faiz::move(pair));
	}

	Tpl<Typ T1, Typ T2> cfn
	get(tight_pair<T2, T1>& pair) noexcept->T1&
	{
		return get<1>(pair);
	}

	Tpl<Typ T1, Typ T2> cfn
	get(tight_pair<T2, T1> const& pair) noexcept->T1 const&
	{
		return get<1>(pair);
	}

	Tpl<Typ T1, Typ T2> cfn
	get(tight_pair<T2, T1>&& pair) noexcept->T1&&
	{
		return get<1>(Faiz::move(pair));
	}

	Tpl<Typ T1, Typ T2> cfn
	get(tight_pair<T2, T1> const&& pair) noexcept->T1 const&&
	{
		return get<1>(Faiz::move(pair));
	}
} // namespace Rider::Faiz

#endif // CRUFT_TIGHT_PAIR_H_
