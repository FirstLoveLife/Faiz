/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2013-present
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
// The implementation of swap (see below) has been adapted from libc++
// (http://libcxx.llvm.org).

#ifndef SWAP_HPP
#define SWAP_HPP
#include "rider/faiz/associated_types.hpp"
#include "rider/faiz/faiz_fwd.hpp"
#include "rider/faiz/macros.hpp"
#include "rider/faiz/move.hpp"
#include <tuple>
namespace Rider::Faiz::range
{
	/// \cond
	namespace detail
	{
		tpl<typ T> struct is_movable_
			: bool_<is_object_v<T> && std::is_move_constructible<T>::value
				  && std::is_move_assignable<T>::value>
		{};
	} // namespace detail
	/// \endcond

	tpl<typ T> struct is_swappable;
	tpl<typ T> inline cexp bool is_swappable_v = is_swappable<T>::value;

	ARE(swappable);

	tpl<typ T> struct is_nothrow_swappable;

	IS_NOT_ARE_ANY(nothrow_swappable);

	tpl<typ T, typ U> struct is_swappable_with;

	tpl<typ T, typ U> struct is_nothrow_swappable_with;

	tpl<typ T, typ U = T> struct is_indirectly_swappable;

	tpl<typ T, typ U = T> struct is_nothrow_indirectly_swappable;

	tpl<typ T, typ U = T> cexp meta::if_c<std::is_move_constructible<T>::value
			&& is_assignable<T&, U>::value,
		T>
	exchange(T& t, U&& u) noexcept(
		std::is_nothrow_move_constructible<T>::value&&
			is_nothrow_assignable<T&, U>::value)
	{
		T tmp((T &&) t);
		t = (U &&) u;
		return tmp;
	}

	/// \cond
	namespace adl_swap_detail
	{
		// Intentionally create an ambiguity with std::swap, which is
		// (possibly) unconstrained.
		tpl<typ T> void
		swap(T&, T&)
			= delete;

		tpl<typ T, Faiz::size_t N> void swap(T (&)[N], T (&)[N]) = delete;

		tpl<typ T, typ U, typ = decltype(swap(declval<T>(), declval<U>()))>
			true_
			try_adl_swap_(int);

		tpl<typ T, typ U> false_
		try_adl_swap_(long);

		tpl<typ T, typ U = T> struct is_adl_swappable_
			: meta::id_t<decltype(adl_swap_detail::try_adl_swap_<T, U>(42))>
		{};

		struct swap_fn
		{
			// Dispatch to user-defined swap found via ADL:
			tpl<typ T, typ U> cexp meta::if_c<is_adl_swappable_<T, U>::value>
			operator()(T&& t, U&& u) const
				AUTO_RETURN_NOEXCEPT((void)swap((T &&) t, (U &&) u))

				// For instrinsicly swappable (i.e., movable) types for
				// which a swap overload cannot be found via ADL, swap by
				// moving.
				tpl<typ T> cexp meta::if_c<!is_adl_swappable_<T&>::value
					&& detail::is_movable_<T>::value>
				operator()(T& a, T& b) const
				AUTO_RETURN_NOEXCEPT((void)(b = range::exchange(a, (T &&) b)))

				// For arrays of instrinsicly swappable (i.e., movable)
				// types for which a swap overload cannot be found via ADL,
				// swap array elements by moving.
				tpl<typ T, typ U, Faiz::size_t N> cexp
				meta::if_c<!is_adl_swappable_<T (&)[N], U (&)[N]>::value
					&& is_swappable_with<T&, U&>::value>
				operator()(T (&t)[N], U (&u)[N]) const
				noexcept(is_nothrow_swappable_with<T&, U&>::value)
			{
				for(Faiz::size_t i = 0; i < N; ++i)
					(*this)(t[i], u[i]);
			}

			// For rvalue pairs and tuples of swappable types, swap the
			// members. This permits code like:
			//   ranges::swap(std::tie(a,b,c), std::tie(d,e,f));
			tpl<typ F0, typ S0, typ F1, typ S1>
				cexp meta::if_c<is_swappable_with<F0, F1>::value
					&& is_swappable_with<S0, S1>::value>
				operator()(
					std::pair<F0, S0>&& left, std::pair<F1, S1>&& right) const
				noexcept(is_nothrow_swappable_with<F0, F1>::value&&
						is_nothrow_swappable_with<S0, S1>::value)
			{
				swap_fn{}(move(left).first, move(right).first);
				swap_fn{}(move(left).second, move(right).second);
			}

			tpl<typ... Ts, typ... Us> cexp meta::if_c<
				meta::and_c<is_swappable_with<Ts, Us>::value...>::value>
			operator()(
				std::tuple<Ts...>&& left, std::tuple<Us...>&& right) const
				noexcept(meta::and_c<
					is_nothrow_swappable_with<Ts, Us>::value...>::value)
			{
				swap_fn::impl(move(left),
					move(right),
					make_index_sequence<sizeof...(Ts)>{});
			}

		private:
			tpl<typ T, typ U, Faiz::size_t... Is> cexp static void
			impl(T&& left, U&& right, index_sequence<Is...>)
			{
				(void)ignore_unused((swap_fn{}(std::get<Is>(move(left)),
										 std::get<Is>(move(right))),
					42)...);
			}
		};

		tpl<typ T, typ U, typ = void> struct is_swappable_with_ : false_
		{};

		tpl<typ T, typ U> struct is_swappable_with_<T,
			U,
			void_t<decltype(swap_fn{}(declval<T>(), declval<U>())),
				decltype(swap_fn{}(declval<U>(), declval<T>()))>> : true_
		{};

		tpl<typ T, typ U> struct is_nothrow_swappable_with_
			: bool_<noexcept(swap_fn{}(declval<T>(), declval<U>()))
				  && noexcept(swap_fn{}(declval<U>(), declval<T>()))>
		{};

		// Q: Should std::reference_wrapper be considered a proxy wrt
		// swapping rvalues? A: No. Its operator= is currently defined to
		// reseat the references, so
		//    std::swap(ra, rb) already means something when ra and rb are
		//    (lvalue) reference_wrappers. That reseats the reference
		//    wrappers but leaves the referents unmodified. Treating rvalue
		//    reference_wrappers differently would be confusing.

		// Q: Then why is it OK to "re"-define swap for pairs and tuples of
		// references? A: Because as defined above, swapping an rvalue tuple
		// of references has the same
		//    semantics as swapping an lvalue tuple of references. Rather
		//    than reseat the references, assignment happens *through* the
		//    references.

		// Q: But I have an iterator whose operator* returns an rvalue
		//    std::reference_wrapper<T>. How do I make it model
		//    IndirectlySwappable?
		// A: With an overload of iter_swap.

		// Intentionally create an ambiguity with std::iter_swap, which is
		// (possibly) unconstrained.
		tpl<typ T> void iter_swap(T, T) = delete;

#ifdef RANGES_WORKAROUND_MSVC_620035
		void
		iter_swap();
#endif

		tpl<typ T, typ U, typ = decltype(iter_swap(declval<T>(), declval<U>()))>
			true_
			try_adl_iter_swap_(int);

		tpl<typ T, typ U> false_
		try_adl_iter_swap_(long);

		// Test whether an overload of iter_swap for a T and a U can be
		// found via ADL with the iter_swap overload above participating in
		// the overload set. This depends on user-defined iter_swap
		// overloads being a better match than the overload in namespace
		// std.
		tpl<typ T, typ U> struct is_adl_indirectly_swappable_
			: meta::id_t<decltype(
				  adl_swap_detail::try_adl_iter_swap_<T, U>(42))>
		{};

		struct iter_swap_fn
		{
			// *If* a user-defined iter_swap is found via ADL, call that:
			tpl<typ T, typ U>
				cexp meta::if_c<is_adl_indirectly_swappable_<T, U>::value>
				operator()(T&& t, U&& u) const
				AUTO_RETURN_NOEXCEPT((void)iter_swap((T &&) t, (U &&) u))

				// *Otherwise*, for Readable types with swappable reference
				// types, call ranges::swap(*a, *b)
				tpl<typ I0, typ I1> cexp
				meta::if_c<!is_adl_indirectly_swappable_<I0, I1>::value
					&& is_swappable_with<reference_t<I0>,
						   reference_t<I1>>::value>
				operator()(I0&& a, I1&& b) const
				AUTO_RETURN_NOEXCEPT(swap_fn{}(*a, *b))

				// *Otherwise*, for Readable types that are mutually
				// IndirectlyMovableStorable, implement as:
				//      value_type_t<T0> tmp = iter_move(a);
				//      *a = iter_move(b);
				//      *b = std::move(tmp);
				tpl<typ I0, typ I1> cexp
				meta::if_c<!is_adl_indirectly_swappable_<I0, I1>::value
					&& !is_swappable_with<reference_t<I0>,
						   reference_t<I1>>::value
					&& is_indirectly_movable<I0, I1>::value
					&& is_indirectly_movable<I1, I0>::value>
				operator()(I0&& a, I1&& b) const
				noexcept(is_nothrow_indirectly_movable<I0, I1>::value&&
						is_nothrow_indirectly_movable<I1, I0>::value)
			{
				_t<value_type<I0>> v0 = iter_move(a);
				*a = iter_move(b);
				*b = move(v0);
			}
		};

		tpl<typ T, typ U, typ Enable = void> struct is_indirectly_swappable_
			: false_
		{};

		tpl<typ T, typ U> struct is_indirectly_swappable_<T,
			U,
			void_t<decltype(iter_swap_fn{}(declval<T>(), declval<U>()))>>
			: true_
		{};

		tpl<typ T, typ U> struct is_nothrow_indirectly_swappable_
			: bool_<noexcept(iter_swap_fn{}(declval<T>(), declval<U>()))>
		{};
	} // namespace adl_swap_detail
	/// \endcond

	/// \ingroup group-utility
	tpl<typ T, typ U> struct is_swappable_with
		: adl_swap_detail::is_swappable_with_<T, U>
	{};

	/// \ingroup group-utility
	tpl<typ T, typ U> struct is_nothrow_swappable_with
		: meta::and_<is_swappable_with<T, U>,
			  adl_swap_detail::is_nothrow_swappable_with_<T, U>>
	{};

	/// \ingroup group-utility
	tpl<typ T> struct is_swappable : is_swappable_with<T&, T&>
	{};

	/// \ingroup group-utility
	tpl<typ T> struct is_nothrow_swappable : is_nothrow_swappable_with<T&, T&>
	{};

	/// \ingroup group-utility
	tpl<typ T, typ U> struct is_indirectly_swappable
		: adl_swap_detail::is_indirectly_swappable_<T, U>
	{};

	/// \ingroup group-utility
	tpl<typ T, typ U> struct is_nothrow_indirectly_swappable
		: meta::and_<is_indirectly_swappable<T, U>,
			  adl_swap_detail::is_nothrow_indirectly_swappable_<T, U>>
	{};

	inline namespace CPOs
	{
		inline cexp adl_swap_detail::swap_fn swap{};

		inline cexp adl_swap_detail::iter_swap_fn iter_swap{};
	} // namespace CPOs


	/// \cond
	struct indirect_swap_fn
	{
		tpl<typ I0, typ I1>
			DEPRECATED("Please replace uses of ranges::indirect_swap "
					   "with ranges::iter_swap.") void
			operator()(I0&& i0, I1&& i1) const
			AUTO_RETURN_NOEXCEPT(iter_swap((I0 &&) i0, (I1 &&) i1))
	};

	inline cexp indirect_swap_fn indirect_swap{};
	/// \endcond
} // namespace Rider::Faiz::range

#endif
