//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14

// type_traits

// is_swappable

#include "../test_macros.h"
#include "rider/faiz/utility.hpp"
#include <type_traits>
#include <vector>

namespace MyNS
{

	// Make the test types non-copyable so that generic std::swap is not
	// valid.
	struct A
	{
		A(A const&) = delete;
		A&
		operator=(A const&)
			= delete;
	};

	struct B
	{
		B(B const&) = delete;
		B&
		operator=(B const&)
			= delete;
	};

	struct C
	{};
	struct D
	{};

	void
	swap(A&, A&)
	{}

	void
	swap(A&, B&)
	{}
	void
	swap(B&, A&)
	{}

	void
	swap(A&, C&)
	{} // missing swap(C, A)
	void
	swap(D&, C&)
	{}

	struct M
	{
		M(M const&) = delete;
		M&
		operator=(M const&)
			= delete;
	};

	void
	swap(M&&, M&&)
	{}

	struct DeletedSwap
	{
		friend void
		swap(DeletedSwap&, DeletedSwap&)
			= delete;
	};

} // namespace MyNS

namespace MyNS2
{

	struct AmbiguousSwap
	{};

	template<class T>
	void
	swap(T&, T&)
	{}

} // end namespace MyNS2

int
main()
{
	using namespace MyNS;
	{
		// 	// Test that is_swappable applies an lvalue reference to the type.
		// static_assert(Rider::Faiz::is_swappable<A>::value, "");
		// static_assert(Rider::Faiz::is_swappable<A&>::value, "");
		// static_assert(!Rider::Faiz::is_swappable<M>::value, "");
		// static_assert(!Rider::Faiz::is_swappable<M&&>::value, "");
	} // static_assert(!Rider::Faiz::is_swappable<B>::value, "");
	// static_assert(Rider::Faiz::is_swappable<C>::value, "");
	{
		// test non-referencable types
		static_assert(!Rider::Faiz::is_swappable<void>::value, "");
		static_assert(!Rider::Faiz::is_swappable<int() const>::value, "");
		static_assert(!Rider::Faiz::is_swappable<int>::value, "");
		static_assert(!Rider::Faiz::is_swappable<int()&>::value, "");
	}
	{
		// test that a deleted swap is correctly handled.
		static_assert(!Rider::Faiz::is_swappable<DeletedSwap>::value, "");
	}
	{
		// test that a swap with ambiguous overloads is handled correctly.
		static_assert(
			!Rider::Faiz::is_swappable<MyNS2::AmbiguousSwap>::value, "");
	} // __swappable

	namespace __detail
	{
		// ALL generic swap overloads MUST already have a declaration available
		// at this point.

		template<class _Tp,
			class _Up = _Tp,
			bool _NotVoid = !is_void<_Tp>::value && !is_void<_Up>::value>
		struct __swappable_with
		{
			template<class _LHS, class _RHS>
			static decltype(
				swap(_VSTD::declval<_LHS>(), _VSTD::declval<_RHS>()))
			__test_swap(int);
			template<class, class>
			static __nat
			__test_swap(long);

			// Extra parens are needed for the C++03 definition of decltype.
			typedef decltype((__test_swap<_Tp, _Up>(0))) __swap1;
			typedef decltype((__test_swap<_Up, _Tp>(0))) __swap2;

			static const bool value = !is_same<__swap1, __nat>::value
				&& !is_same<__swap2, __nat>::value;
		};

		template<class _Tp, class _Up>
		struct __swappable_with<_Tp, _Up, false> : false_type
		{};

		template<class _Tp,
			class _Up = _Tp,
			bool _Swappable = __swappable_with<_Tp, _Up>::value>
		struct __nothrow_swappable_with
		{
			static const bool value =
#ifndef _LIBCPP_HAS_NO_NOEXCEPT
				noexcept(swap(_VSTD::declval<_Tp>(), _VSTD::declval<_Up>()))
				&& noexcept(swap(_VSTD::declval<_Up>(), _VSTD::declval<_Tp>()));
#else
				false;
#endif
		};

		template<class _Tp, class _Up>
		struct __nothrow_swappable_with<_Tp, _Up, false> : false_type
		{};

	} // namespace __detail

	template<class _Tp>
	struct __is_swappable : public integral_constant<bool,
								__detail::__swappable_with<_Tp&>::value>
	{};

	template<class _Tp>
	struct __is_nothrow_swappable
		: public integral_constant<bool,
			  __detail::__nothrow_swappable_with<_Tp&>::value>
	{};

#if _LIBCPP_STD_VER > 14

	template<class _Tp, class _Up>
	struct _LIBCPP_TEMPLATE_VIS is_swappable_with
		: public integral_constant<bool,
			  __detail::__swappable_with<_Tp, _Up>::value>
	{};

	template<class _Tp>
	struct _LIBCPP_TEMPLATE_VIS is_swappable
		: public conditional<__is_referenceable<_Tp>::value,
			  is_swappable_with<typename add_lvalue_reference<_Tp>::type,
				  typename add_lvalue_reference<_Tp>::type>,
			  false_type>::type
	{};

	template<class _Tp, class _Up>
	struct _LIBCPP_TEMPLATE_VIS is_nothrow_swappable_with
		: public integral_constant<bool,
			  __detail::__nothrow_swappable_with<_Tp, _Up>::value>
	{};

	template<class _Tp>
	struct _LIBCPP_TEMPLATE_VIS is_nothrow_swappable
		: public conditional<__is_referenceable<_Tp>::value,
			  is_nothrow_swappable_with<typename add_lvalue_reference<_Tp>::type,
				  typename add_lvalue_reference<_Tp>::type>,
			  false_type>::type
	{};

	template<class _Tp, class _Up>
	_LIBCPP_INLINE_VAR constexpr bool is_swappable_with_v
		= is_swappable_with<_Tp, _Up>::value;

	template<class _Tp>
	_LIBCPP_INLINE_VAR constexpr bool is_swappable_v = is_swappable<_Tp>::value;

	template<class _Tp, class _Up>
	_LIBCPP_INLINE_VAR constexpr bool is_nothrow_swappable_with_v
		= is_nothrow_swappable_with<_Tp, _Up>::value;

	template<class _Tp>
	_LIBCPP_INLINE_VAR constexpr bool is_nothrow_swappable_v
		= is_nothrow_swappable<_Tp>::value;

#endif // _LIBCPP_STD_VER > 14

	{
		// test for presence of is_swappable_v
		static_assert(Rider::Faiz::is_swappable_v<int>, "");
		static_assert(!Rider::Faiz::is_swappable_v<M>, "");
	}
}
