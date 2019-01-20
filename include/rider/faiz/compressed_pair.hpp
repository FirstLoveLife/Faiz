// https://github.com/Morwenn/tight_pair
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

	tpl<typ T1, typ T2> struct tight_pair;

	tpl<size_t N, typ T1, typ T2> cfn
	get(tight_pair<T1, T2>&) noexcept
		->std::tuple_element_t<N, tight_pair<T1, T2>>&;

	tpl<size_t N, typ T1, typ T2> cfn
	get(tight_pair<T1, T2> const&) noexcept
		->std::tuple_element_t<N, tight_pair<T1, T2>> const&;

	tpl<size_t N, typ T1, typ T2> cfn
	get(tight_pair<T1, T2>&&) noexcept
		->std::tuple_element_t<N, tight_pair<T1, T2>>&&;

	tpl<size_t N, typ T1, typ T2> cfn
	get(tight_pair<T1, T2> const&&) noexcept
		->std::tuple_element_t<N, tight_pair<T1, T2>> const&&;
} // namespace Rider::Faiz

namespace std
{
	////////////////////////////////////////////////////////////
	// Specializations needed for decomposition declarations

	tpl<typ T1, typ T2> class tuple_size<Rider::Faiz::tight_pair<T1, T2>>
		: public Rider::Faiz::size_t_<2>
	{};

	tpl<typ T1, typ T2> class tuple_element<0, Rider::Faiz::tight_pair<T1, T2>>
		: public Rider::type_identity<T1>
	{};

	tpl<typ T1, typ T2> class tuple_element<1, Rider::Faiz::tight_pair<T1, T2>>
		: public Rider::type_identity<T2>
	{};

	tpl<size_t N, typ T1, typ T2> class tuple_element<N,
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

		tpl<typ T> struct is_ebco_eligible
			: logic::and_<is_empty<T>, logic::not_<is_final<T>>>
		{};

		IS_NOT_ARE_ANY(ebco_eligible);

		////////////////////////////////////////////////////////////
		// Detect whether an integer has padding bits

		tpl<typ UnsignedInteger> cfn
		has_padding_bits()
			->bool
		{
#ifdef __cpp_lib_has_unique_object_representations
			// A note in the standard mentions than unsigned integer types
			// are guaranteed to have unique object representations when
			// they don't have padding bits

			return std::has_unique_object_representations<
				UnsignedInteger>::value;
#else
			// Algorithm partly taken from WG14 N1899, also handles unsigned
			// integer types such as unsigned __int128 that don't have an
			// std::numeric_limits specialization

			size_t precision = 0;
			auto num = static_cast<UnsignedInteger>(-1);
			while(num != 0)
			{
				if(num % 2 == 1)
				{
					++precision;
				}
				num >>= 1;
			}
			return precision != sizeof(UnsignedInteger) * CHAR_BIT;
#endif
		}

		////////////////////////////////////////////////////////////
		// Find an unsigned integer type twice as big as the given
		// one, ensure all bits are used

		tpl<typ UInt> constexpr decltype(auto)
		twice_as_big()
		{
			if constexpr(has_padding_bits<UInt>())
			{
				return;
			}

			else if constexpr(sizeof(unsigned short) == 2 * sizeof(UInt)
				and not has_padding_bits<unsigned short>())
			{
				return static_cast<unsigned short>(0);
			}

			else if constexpr(sizeof(unsigned int) == 2 * sizeof(UInt)
				and not has_padding_bits<unsigned int>())
			{
				return static_cast<unsigned int>(0);
			}

			else if constexpr(sizeof(unsigned long) == 2 * sizeof(UInt)
				and not has_padding_bits<unsigned long>())
			{
				return static_cast<unsigned long>(0);
			}

			else if constexpr(sizeof(unsigned long long) == 2 * sizeof(UInt)
				and not has_padding_bits<unsigned long long>())
			{
				return static_cast<unsigned long long>(0);
			}

			else if constexpr(sizeof(std::uintmax_t) == 2 * sizeof(UInt)
				and not has_padding_bits<std::uintmax_t>())
			{
				return static_cast<std::uintmax_t>(0);
			}

#if CRUFT_TIGHT_PAIR_USE_UNSIGNED_128INT && defined(__SIZEOF_INT128__)
			// Only use unsigned __int128 with compilers
			// which are known to produce branchless code
			// for comparisons
			else if constexpr(sizeof(unsigned __int128) == 2 * sizeof(UInt)
				and not has_padding_bits<unsigned __int128>())
			{
				return static_cast<unsigned __int128>(0);
			}
#endif
		}

		tpl<typ T> struct has_twice_as_big
			: logic::not_<is_void<decltype(twice_as_big<T>())>>
		{};

		tpl<typ T> constexpr decltype(twice_as_big<T>())
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

		tpl<typ T> struct can_optimize_compare
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
			tpl<typ...> static constexpr bool enable_default = false;

			tpl<typ...> static constexpr bool enable_explicit = false;

			tpl<typ...> static constexpr bool enable_implicit = false;

			tpl<typ...> static constexpr bool enable_assign = false;
		};

		////////////////////////////////////////////////////////////
		// Utilities to work with pair-like types: types for which
		// std::tuple_size_v<T> == 2 and get<0>/get<1> work

		namespace adl_hook
		{
			using std::get;

			tpl<typ T, typ Enable> struct pair_like_impl : false_
			{};

			tpl<typ T> struct pair_like_impl<T,
				void_t<typ std::tuple_size<decay_t<T>>::type,
					decltype(get<0>(declval<T&>())),
					decltype(get<1>(declval<T&>()))>>
				: bool_<std::tuple_size_v<decay_t<T>> == 2>
			{};
		} // namespace adl_hook

		tpl<typ T, typ U = remove_cv_t<T>> struct pair_like
			: adl_hook::pair_like_impl<U, void>
		{};

		// Safeguard to avoid ambiguous conversions
		tpl<typ T1, typ T2> struct pair_like<Rider::Faiz::tight_pair<T1, T2>>
			: false_
		{};

		// pair_assignable, pair_constructible, pair_convertible

		namespace adl_hook
		{
			// Import a tpl get for ADL
			using std::get;

			tpl<typ To, typ From> struct pair_assignable
				: logic::and_<is_assignable<decltype(get<0>(declval<To>())),
								  decltype(get<0>(declval<From>()))>,
					  is_assignable<decltype(get<1>(declval<To>())),
						  decltype(get<1>(declval<From>()))>>
			{};

			tpl<typ To, typ From> struct pair_constructible
				: logic::and_<is_constructible<decltype(get<0>(declval<To>())),
								  decltype(get<0>(declval<From>()))>,
					  is_constructible<decltype(get<1>(declval<To>())),
						  decltype(get<1>(declval<From>()))>>
			{};

			tpl<typ From, typ To> struct pair_convertible
				: logic::and_<is_convertible<decltype(get<0>(declval<From>())),
								  decltype(get<0>(declval<To>()))>,
					  is_assignable<decltype(get<1>(declval<From>())),
						  decltype(get<1>(declval<To>()))>>
			{};
		} // namespace adl_hook

		////////////////////////////////////////////////////////////
		// Type used for the storage of a single pair member: the
		// tpl integer parameter is used to disambiguate the
		// types when both members the same underlying types

		tpl<size_t N,
			typ T,
			bool = is_ebco_eligible_v<T>,
			bool = is_reference_v<T>> struct tight_pair_element;

		tpl<size_t N, typ T> struct tight_pair_element<N, T, false, false>
		{
		private:
			T value;

		public:
			////////////////////////////////////////////////////////////
			// Construction

			tight_pair_element(tight_pair_element const&) = default;
			tight_pair_element(tight_pair_element&&) = default;

			constexpr tight_pair_element() noexcept(
				is_nothrow_default_constructible_v<T>)
				: value()
			{
				static_assert(not_reference_v<T>,
					"attempted to default construct a reference element");
			}

			tpl<typ U,
				typ = enable_if_t<is_constructible_v<T,
					U>>> constexpr explicit tight_pair_element(U&&
					other) noexcept(is_nothrow_constructible_v<T, U>)
				: value(Faiz::forward<U>(other))
			{}

			tpl<typ Tuple> constexpr tight_pair_element(
				std::piecewise_construct_t, Tuple&& args)
				: value(std::make_from_tuple<T>(Faiz::forward<Tuple>(args)))
			{}

			tpl<typ U> cfn
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
				do_get()
				& noexcept->T&
			{
				return static_cast<T&>(value);
			}

			cfn
				do_get() const
				& noexcept->T const&
			{
				return static_cast<T const&>(value);
			}

			cfn
				do_get()
				&& noexcept->T&&
			{
				return static_cast<T&&>(value);
			}

			cfn
				do_get() const
				&& noexcept->T const&&
			{
				return static_cast<T const&&>(value);
			}
		};

		tpl<size_t N, typ T> struct tight_pair_element<N, T, false, true>
		{
		private:
			T value;

		public:
			////////////////////////////////////////////////////////////
			// Construction

			tight_pair_element(tight_pair_element const&) = default;
			tight_pair_element(tight_pair_element&&) = default;

			constexpr tight_pair_element() noexcept(
				is_nothrow_default_constructible_v<T>)
				: value()
			{
				static_assert(not_reference_v<T>,
					"attempted to default construct a reference element");
			}

			tpl<typ U,
				typ = enable_if_t<is_constructible_v<T,
					U>>> constexpr explicit tight_pair_element(U&&
					other) noexcept(is_nothrow_constructible_v<T, U>)
				: value(Faiz::forward<U>(other))
			{}

			tpl<typ Tuple> constexpr tight_pair_element(
				std::piecewise_construct_t, Tuple&& args)
				: value(std::make_from_tuple<T>(Faiz::forward<Tuple>(args)))
			{}

			auto
			operator=(tight_pair_element const& other) -> tight_pair_element&
			{
				value = other.value;
				return *this;
			}

			auto
			operator=(tight_pair_element&& other) -> tight_pair_element&
			{
				value = move(other.value);
				return *this;
			}

			tpl<typ U> cfn
			operator=(U&& other) noexcept(is_nothrow_assignable_v<T&, U>)
				->tight_pair_element&
			{
				value = Faiz::forward<U>(other);
				return *this;
			}

			////////////////////////////////////////////////////////////
			// Element access

			cfn
				do_get()
				& noexcept->T&
			{
				return static_cast<T&>(value);
			}

			cfn
				do_get() const
				& noexcept->T const&
			{
				return static_cast<T const&>(value);
			}

			cfn
				do_get()
				&& noexcept->T&&
			{
				return static_cast<T&&>(value);
			}

			cfn
				do_get() const
				&& noexcept->T const&&
			{
				return static_cast<T const&&>(value);
			}
		};

		tpl<size_t N, typ T> struct tight_pair_element<N, T, true, false>
			: private T
		{
			////////////////////////////////////////////////////////////
			// Construction

			tight_pair_element(tight_pair_element const&) = default;
			tight_pair_element(tight_pair_element&&) = default;

			constexpr tight_pair_element() noexcept(
				is_nothrow_default_constructible_v<T>)
			{}

			tpl<typ U,
				typ = enable_if_t<is_constructible_v<T,
					U>>> constexpr explicit tight_pair_element(U&&
					other) noexcept(is_nothrow_constructible_v<T, U>)
				: T(Faiz::forward<U>(other))
			{}

			tpl<typ Tuple> constexpr tight_pair_element(
				std::piecewise_construct_t, Tuple&& args)
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
				do_get()
				& noexcept->T&
			{
				return static_cast<T&>(*this);
			}

			cfn
				do_get() const
				& noexcept->T const&
			{
				return static_cast<T const&>(*this);
			}

			cfn
				do_get()
				&& noexcept->T&&
			{
				return static_cast<T&&>(*this);
			}

			cfn
				do_get() const
				&& noexcept->T const&&
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

		tpl<typ T, typ U = remove_cv_t<T>> struct needs_reordering
			: bool_<std::endian::native == std::endian::little
				  and logic::and_<is_unsigned<U>, has_twice_as_big<U>>::value>
		{};

		////////////////////////////////////////////////////////////
		// Type used for the storage of the pair member: the
		// tpl integer parameter is used to disambiguate the
		// types when both have the same underlying types

		tpl<typ T1,
			typ T2,
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

			constexpr tight_pair_storage()
				: tight_pair_element<0, T1>(), tight_pair_element<1, T2>()
			{}

			tpl<typ U1, typ U2> constexpr tight_pair_storage(
				U1&& first, U2&& second)
				: tight_pair_element<0, T1>(Faiz::forward<U1>(first)),
				  tight_pair_element<1, T2>(Faiz::forward<U2>(second))
			{}

			tpl<typ Tuple1, typ Tuple2> constexpr tight_pair_storage(
				std::piecewise_construct_t pc,
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

			tpl<size_t N> cfn
			do_get() &->decltype(auto)
			{
				if constexpr(N == 0)
				{
					return static_cast<tight_pair_element<0, T1>&>(*this)
						.do_get();
				}
				else if constexpr(N == 1)
				{
					return static_cast<tight_pair_element<1, T2>&>(*this)
						.do_get();
				}
			}

			tpl<size_t N> cfn
			do_get() const&->decltype(auto)
			{
				if constexpr(N == 0)
				{
					return static_cast<tight_pair_element<0, T1> const&>(*this)
						.do_get();
				}
				else if constexpr(N == 1)
				{
					return static_cast<tight_pair_element<1, T2> const&>(*this)
						.do_get();
				}
			}

			tpl<size_t N> cfn
			do_get() &&->decltype(auto)
			{
				if constexpr(N == 0)
				{
					return static_cast<tight_pair_element<0, T1>&&>(*this)
						.do_get();
				}
				else if constexpr(N == 1)
				{
					return static_cast<tight_pair_element<1, T2>&&>(*this)
						.do_get();
				}
			}

			tpl<size_t N> cfn
			do_get() const&&->decltype(auto)
			{
				if constexpr(N == 0)
				{
					return static_cast<tight_pair_element<0, T1> const&&>(*this)
						.do_get();
				}
				else if constexpr(N == 1)
				{
					return static_cast<tight_pair_element<1, T2> const&&>(*this)
						.do_get();
				}
			}
		};

		tpl<typ T> struct tight_pair_storage<T, T, false, false>
		{
			// Store elements contiguously, avoid padding between elements
			T elements[2];

			////////////////////////////////////////////////////////////
			// Construction

			tight_pair_storage(tight_pair_storage const&) = default;
			tight_pair_storage(tight_pair_storage&&) = default;

			constexpr tight_pair_storage() : elements()
			{}

			tpl<typ U1, typ U2> constexpr tight_pair_storage(
				U1&& first, U2&& second)
				: elements{
					  T(Faiz::forward<U1>(first)), T(Faiz::forward<U2>(second))}
			{}

			tpl<typ Tuple1, typ Tuple2> constexpr tight_pair_storage(
				std::piecewise_construct_t,
				Tuple1&& first_args,
				Tuple2&& second_args)
				: elements{std::make_from_tuple<T>(
							   Faiz::forward<Tuple1>(first_args)),
					  std::make_from_tuple<T>(
						  Faiz::forward<Tuple2>(second_args))}
			{}

			tight_pair_storage&
			operator=(tight_pair_storage const&)
				= default;
			tight_pair_storage&
			operator=(tight_pair_storage&&)
				= default;

			////////////////////////////////////////////////////////////
			// Element access

			tpl<size_t N> cfn
			do_get() &->decltype(auto)
			{
				return static_cast<T&>(elements[N]);
			}

			tpl<size_t N> cfn
			do_get() const&->decltype(auto)
			{
				return static_cast<T const&>(elements[N]);
			}

			tpl<size_t N> cfn
			do_get() &&->decltype(auto)
			{
				return static_cast<T&&>(elements[N]);
			}

			tpl<size_t N> cfn
			do_get() const&&->decltype(auto)
			{
				return static_cast<T const&&>(elements[N]);
			}
		};

		tpl<typ T> struct tight_pair_storage<T, T, false, true>
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

			tpl<typ U1, typ U2> constexpr tight_pair_storage(
				U1&& second, U2&& first)
				: elements{
					  T(Faiz::forward<U2>(first)), T(Faiz::forward<U1>(second))}
			{}

			tpl<typ Tuple1, typ Tuple2> constexpr tight_pair_storage(
				std::piecewise_construct_t,
				Tuple2&& second_args,
				Tuple1&& first_args)
				: elements{std::make_from_tuple<T>(
							   Faiz::forward<Tuple1>(first_args)),
					  std::make_from_tuple<T>(
						  Faiz::forward<Tuple2>(second_args))}
			{}

			tight_pair_storage&
			operator=(tight_pair_storage const&)
				= default;
			tight_pair_storage&
			operator=(tight_pair_storage&&)
				= default;

			////////////////////////////////////////////////////////////
			// Element access

			tpl<size_t N> cfn
			do_get() &->decltype(auto)
			{
				if constexpr(N == 0)
				{
					return static_cast<T&>(elements[1]);
				}
				else if constexpr(N == 1)
				{
					return static_cast<T&>(elements[0]);
				}
			}

			tpl<size_t N> cfn
			do_get() const&->decltype(auto)
			{
				if constexpr(N == 0)
				{
					return static_cast<T const&>(elements[1]);
				}
				else if constexpr(N == 1)
				{
					return static_cast<T const&>(elements[0]);
				}
			}

			tpl<size_t N> cfn
			do_get() &&->decltype(auto)
			{
				if constexpr(N == 0)
				{
					return static_cast<T&&>(elements[1]);
				}
				else if constexpr(N == 1)
				{
					return static_cast<T&&>(elements[0]);
				}
			}

			tpl<size_t N> cfn
			do_get() const&&->decltype(auto)
			{
				if constexpr(N == 0)
				{
					return static_cast<T const&&>(elements[1]);
				}
				else if constexpr(N == 1)
				{
					return static_cast<T const&&>(elements[0]);
				}
			}
		};
	} // namespace detail

	////////////////////////////////////////////////////////////
	// Main class

	tpl<typ T1, typ T2> struct tight_pair
		: private detail::tight_pair_storage<T1, T2>,
		  private Math::AbstractAlgebra::RelationAlgebra::totally_ordered<
			  tight_pair<T1, T2>>
	{
	private:
		struct check_args
		{
			tpl<typ U1, typ U2> static constexpr bool
			enable_default()
			{
				return are_default_constructible_v<U1, U2>;
			}

			tpl<typ U1, typ U2> static constexpr bool
			enable_explicit()
			{
				return logic::and_<is_constructible<T1, U1>,
					is_constructible<T2, U2>,
					logic::or_<logic::not_<is_convertible<U1, T1>>,
						logic::not_<is_convertible<U2, T2>>>>::value;
			}

			tpl<typ U1, typ U2> static constexpr bool
			enable_implicit()
			{
				return logic::and_<is_constructible<T1, U1>,
					is_constructible<T2, U2>,
					are_convertible<PAIR(U1, T1), PAIR(U2, T2)>>::value;
			}
		};

		struct check_tuple_like_constructor
		{
			tpl<typ Tuple> static constexpr bool
			enable_implicit()
			{
				return detail::adl_hook::pair_convertible<Tuple,
					tight_pair>::value;
			}

			tpl<typ Tuple> static constexpr bool
			enable_explicit()
			{
				return detail::adl_hook::pair_constructible<tight_pair&,
						   Tuple>::value
					and not detail::adl_hook::pair_convertible<Tuple,
							tight_pair>::value;
			}

			tpl<typ Tuple> static constexpr bool
			enable_assign()
			{
				return detail::adl_hook::pair_assignable<tight_pair&,
					Tuple>::value;
			}
		};

		tpl<typ T> using check_pair_like
			= meta::if_c<detail::pair_like<remove_reference_t<T>>::value,
				check_tuple_like_constructor,
				detail::check_tuple_constructor_fail>;

	public:
		////////////////////////////////////////////////////////////
		// Construction

		tight_pair(tight_pair const&) = default;
		tight_pair(tight_pair&&) = default;

		tpl<typ U1 = T1,
			typ U2 = T2,
			enable_if_t<check_args::tpl enable_default<U1, U2>(),
				bool> = false> constexpr tight_pair() noexcept(are_nothrow_default_constructible_v<T1,
			T2>)
			: detail::tight_pair_storage<T1, T2>()
		{}
		// clang-format on
		tpl<typ U1 = T1,
			typ U2 = T2,
			enable_if_t<check_args::tpl enable_explicit<U1 const&, U2 const&>(),
				bool> = false> constexpr explicit tight_pair(T1 const& first,
			T2 const& second
			= {}) noexcept(are_nothrow_copy_constructible_v<T1, T2>)
			: detail::tight_pair_storage<T1, T2>(first, second)
		{}

		tpl<typ U1 = T1,
			typ U2 = T2,
			enable_if_t<check_args::tpl enable_implicit<U1 const&, U2 const&>(),
				bool> = false> constexpr tight_pair(T1 const& first,
			T2 const& second
			= {}) noexcept(are_nothrow_copy_constructible_v<T1, T2>)
			: detail::tight_pair_storage<T1, T2>(first, second)
		{}

		tpl<typ U1 = T1,
			typ U2 = T2,
			enable_if_t<check_args::tpl enable_explicit<U1, U2>(),
				bool> = false> constexpr explicit tight_pair(U1&& first,
			U2&& second) noexcept(is_nothrow_constructible_v<T1, U1>and
				is_nothrow_constructible_v<T2, U2>)
			: detail::tight_pair_storage<T1, T2>(
				  Faiz::forward<U1>(first), Faiz::forward<U2>(second))
		{}

		tpl<typ U1 = T1,
			typ U2 = T2,
			enable_if_t<check_args::tpl enable_implicit<U1, U2>(),
				bool> = false> constexpr tight_pair(U1&& first,
			U2&& second) noexcept(is_nothrow_constructible_v<T1, U1>and
				is_nothrow_constructible_v<T2, U2>)
			: detail::tight_pair_storage<T1, T2>(
				  Faiz::forward<U1>(first), Faiz::forward<U2>(second))
		{}

		tpl<typ U1 = T1,
			typ U2 = T2,
			enable_if_t<check_args::tpl enable_explicit<U1 const&, U2 const&>(),
				bool> = false> constexpr explicit tight_pair(tight_pair<U1,
			U2> const& pair) noexcept(is_nothrow_constructible_v<T1,
			U1 const&>and is_nothrow_constructible_v<T2, U2 const&>)
			: detail::tight_pair_storage<T1, T2>(
				  pair.tpl get<0>(), pair.tpl get<1>())
		{}

		tpl<typ U1 = T1,
			typ U2 = T2,
			enable_if_t<check_args::tpl enable_implicit<U1 const&, U2 const&>(),
				bool> = false> constexpr tight_pair(tight_pair<U1, U2> const&
				pair) noexcept(is_nothrow_constructible_v<T1, U1 const&>and
				is_nothrow_constructible_v<T2, U2 const&>)
			: detail::tight_pair_storage<T1, T2>(
				  pair.tpl get<0>(), pair.tpl get<1>())
		{}

		tpl<typ U1 = T1,
			typ U2 = T2,
			enable_if_t<check_args::tpl enable_explicit<U1, U2>(),
				bool> = false> constexpr explicit tight_pair(tight_pair<U1,
			U2>&& pair) noexcept(is_nothrow_constructible_v<T1, U1&&>and
				is_nothrow_constructible_v<T2, U2&&>)
			: detail::tight_pair_storage<T1, T2>(
				  Faiz::forward<U1>(pair.tpl get<0>()),
				  Faiz::forward<U2>(pair.tpl get<1>()))
		{}

		tpl<typ U1 = T1,
			typ U2 = T2,
			enable_if_t<check_args::tpl enable_implicit<U1, U2>(),
				bool> = false> constexpr tight_pair(tight_pair<U1, U2>&&
				pair) noexcept(is_nothrow_constructible_v<T1, U1&&>and
				is_nothrow_constructible_v<T2, U2&&>)
			: detail::tight_pair_storage<T1, T2>(
				  Faiz::forward<U1>(pair.tpl get<0>()),
				  Faiz::forward<U2>(pair.tpl get<1>()))
		{}

		tpl<typ Tuple,
			enable_if_t<check_pair_like<Tuple>::tpl enable_explicit<
				Tuple>()>> constexpr explicit tight_pair(Tuple&& tuple)
			: detail::tight_pair_storage<T1, T2>(
				  Faiz::forward<T1>(get<0>(Faiz::forward<Tuple>(tuple))),
				  Faiz::forward<T2>(get<1>(Faiz::forward<Tuple>(tuple))))
		{}

		tpl<typ Tuple,
			enable_if_t<check_pair_like<Tuple>::tpl enable_implicit<
				Tuple>()>> constexpr tight_pair(Tuple&& tuple)
			: detail::tight_pair_storage<T1, T2>(
				  Faiz::forward<T1>(get<0>(Faiz::forward<Tuple>(tuple))),
				  Faiz::forward<T2>(get<1>(Faiz::forward<Tuple>(tuple))))
		{}

		tpl<typ Tuple1, typ Tuple2> constexpr tight_pair(
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

		tpl<typ Tuple,
			typ
			= enable_if_t<check_pair_like<Tuple>::tpl enable_assign<Tuple>()>>
			cfn
			operator=(Tuple&& other)
				->tight_pair&
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

		tpl<size_t N> cfn
			get()
			& noexcept->std::tuple_element_t<N, tight_pair>&
		{
			using storage_t = detail::tight_pair_storage<T1, T2>;
			return static_cast<std::tuple_element_t<N, tight_pair<T1, T2>>&>(
				static_cast<storage_t&>(*this).tpl do_get<N>());
		}

		tpl<size_t N> cfn
			get() const
			& noexcept->std::tuple_element_t<N, tight_pair> const&
		{
			using storage_t = detail::tight_pair_storage<T1, T2>;
			return static_cast<
				std::tuple_element_t<N, tight_pair<T1, T2>> const&>(
				static_cast<storage_t const&>(*this).tpl do_get<N>());
		}

		tpl<size_t N> cfn
			get()
			&& noexcept->std::tuple_element_t<N, tight_pair>&&
		{
			using storage_t = detail::tight_pair_storage<T1, T2>;
			return static_cast<std::tuple_element_t<N, tight_pair<T1, T2>>&&>(
				static_cast<storage_t&&>(*this).tpl do_get<N>());
		}

		tpl<size_t N> cfn
			get() const
			&& noexcept->std::tuple_element_t<N, tight_pair> const&&
		{
			using storage_t = detail::tight_pair_storage<T1, T2>;
			return static_cast<
				std::tuple_element_t<N, tight_pair<T1, T2>> const&&>(
				static_cast<storage_t const&&>(*this).tpl do_get<N>());
		}
	};

	tpl<typ T, typ U> cfn
	operator<(tight_pair<T, U> const& lhs, tight_pair<T, U> const& rhs)
		->bool
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
	tpl<typ T, typ U> cfn
	operator==(tight_pair<T, U> const& lhs, tight_pair<T, U> const& rhs)
		->bool
	{
		return Rider::Faiz::get<0>(lhs) == Rider::Faiz::get<0>(rhs)
			and Rider::Faiz::get<1>(lhs) == Rider::Faiz::get<1>(rhs);
	}
	tpl<typ T> auto
	operator<(tight_pair<T, T> const& lhs, tight_pair<T, T> const& rhs)
		-> enable_if_t<detail::can_optimize_compare<T>::value, bool>
	{
		auto big_lhs = detail::get_twice_as_big(lhs);
		auto big_rhs = detail::get_twice_as_big(rhs);
		return big_lhs < big_rhs;
	}

	////////////////////////////////////////////////////////////
	// Deduction guide

	tpl<typ T1, typ T2> tight_pair(T1, T2)->tight_pair<T1, T2>;

	tpl<typ T1, typ T2>
		tight_pair(std::reference_wrapper<T1>, T2)->tight_pair<T1&, T2>;

	tpl<typ T1, typ T2>
		tight_pair(T1, std::reference_wrapper<T2>)->tight_pair<T1, T2&>;

	tpl<typ T1, typ T2>
		tight_pair(std::reference_wrapper<T1>, std::reference_wrapper<T2>)
			->tight_pair<T1&, T2&>;

	////////////////////////////////////////////////////////////
	// Free swap function

	tpl<typ T1, typ T2, typ = enable_if_t<range::are_swappable_v<T1, T2>>> cfn
	swap(tight_pair<T1, T2>& lhs, tight_pair<T1, T2>& rhs) noexcept(
		noexcept(lhs.swap(rhs)))
		->void
	{
		lhs.swap(rhs);
	}

	////////////////////////////////////////////////////////////
	// Index-based get functions for element access and
	// structured bindings

	tpl<size_t N, typ T1, typ T2> cfn
	get(tight_pair<T1, T2>& pair) noexcept
		->std::tuple_element_t<N, tight_pair<T1, T2>>&
	{
		return static_cast<std::tuple_element_t<N, tight_pair<T1, T2>>&>(
			pair.tpl get<N>());
	}

	tpl<size_t N, typ T1, typ T2> cfn
	get(tight_pair<T1, T2> const& pair) noexcept
		->std::tuple_element_t<N, tight_pair<T1, T2>> const&
	{
		return static_cast<std::tuple_element_t<N, tight_pair<T1, T2>> const&>(
			pair.tpl get<N>());
	}

	tpl<size_t N, typ T1, typ T2> cfn
	get(tight_pair<T1, T2>&& pair) noexcept
		->std::tuple_element_t<N, tight_pair<T1, T2>>&&
	{
		return static_cast<std::tuple_element_t<N, tight_pair<T1, T2>>&&>(
			pair.tpl get<N>());
	}

	tpl<size_t N, typ T1, typ T2> cfn
	get(tight_pair<T1, T2> const&& pair) noexcept
		->std::tuple_element_t<N, tight_pair<T1, T2>> const&&
	{
		return static_cast<std::tuple_element_t<N, tight_pair<T1, T2>> const&&>(
			pair.tpl get<N>());
	}

	////////////////////////////////////////////////////////////
	// Type-based get functions for element access

	tpl<typ T1, typ T2> cfn
	get(tight_pair<T1, T2>& pair) noexcept->T1&
	{
		return get<0>(pair);
	}

	tpl<typ T1, typ T2> cfn
	get(tight_pair<T1, T2> const& pair) noexcept->T1 const&
	{
		return get<0>(pair);
	}

	tpl<typ T1, typ T2> cfn
	get(tight_pair<T1, T2>&& pair) noexcept->T1&&
	{
		return get<0>(Faiz::move(pair));
	}

	tpl<typ T1, typ T2> cfn
	get(tight_pair<T1, T2> const&& pair) noexcept->T1 const&&
	{
		return get<0>(Faiz::move(pair));
	}

	tpl<typ T1, typ T2> cfn
	get(tight_pair<T2, T1>& pair) noexcept->T1&
	{
		return get<1>(pair);
	}

	tpl<typ T1, typ T2> cfn
	get(tight_pair<T2, T1> const& pair) noexcept->T1 const&
	{
		return get<1>(pair);
	}

	tpl<typ T1, typ T2> cfn
	get(tight_pair<T2, T1>&& pair) noexcept->T1&&
	{
		return get<1>(Faiz::move(pair));
	}

	tpl<typ T1, typ T2> cfn
	get(tight_pair<T2, T1> const&& pair) noexcept->T1 const&&
	{
		return get<1>(Faiz::move(pair));
	}
} // namespace Rider::Faiz

#endif // CRUFT_TIGHT_PAIR_H_
