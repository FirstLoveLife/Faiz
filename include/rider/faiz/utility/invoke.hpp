/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2013-present
//  Copyright Casey Carter 2016
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
#ifndef RANGES_V3_UTILITY_INVOKE_HPP
#define RANGES_V3_UTILITY_INVOKE_HPP

#include "rider/faiz/faiz_fwd.hpp"
#include "rider/faiz/macros.hpp"
#include "rider/faiz/utility/concepts.hpp"
#include "rider/faiz/utility/static_const.hpp"

namespace Rider::Faiz
{
	tpl<typ T> struct is_reference_wrapper
		: meta::if_<is_same<remove_cvref_t<T>, T>,
			  false_,
			  is_reference_wrapper<remove_cvref_t<T>>>
	{};

	tpl<typ T> struct is_reference_wrapper<reference_wrapper<T>> : true_
	{};

	tpl<typ T> struct is_reference_wrapper<std::reference_wrapper<T>> : true_
	{};

	tpl<typ T> using is_reference_wrapper_t = _t<is_reference_wrapper<T>>;

	struct invoke_fn
	{
	private:
		tpl<typ MemberFunctionPtr, typ First, typ... Rest> static cexp auto
		invoke_member_fn(true_,
			range::detail::any,
			MemberFunctionPtr fn,
			First&& first,
			Rest&&... rest)
			DECLTYPE_AUTO_RETURN_NOEXCEPT(
				(static_cast<First&&>(first).*fn)(static_cast<Rest&&>(rest)...))
				tpl<typ MemberFunctionPtr, typ First, typ... Rest> static cexp
			auto invoke_member_fn(false_,
				true_,
				MemberFunctionPtr fn,
				First&& first,
				Rest&&... rest)
				DECLTYPE_AUTO_RETURN_NOEXCEPT(
					(static_cast<First&&>(first).get().*fn)(static_cast<Rest&&>(
						rest)...)) tpl<typ MemberFunctionPtr,
					typ First,
					typ... Rest> static cexp
			auto invoke_member_fn(false_,
				false_,
				MemberFunctionPtr fn,
				First&& first,
				Rest&&... rest)
				DECLTYPE_AUTO_RETURN_NOEXCEPT(((*static_cast<First&&>(first))
					.*fn)(static_cast<Rest&&>(rest)...))

					tpl<typ MemberDataPtr, typ First> static cexp
			auto invoke_member_data(
				true_, range::detail::any, MemberDataPtr ptr, First&& first)
				DECLTYPE_AUTO_RETURN_NOEXCEPT(static_cast<First&&>(first)
					.*ptr) tpl<typ MemberDataPtr, typ First> static cexp
			auto invoke_member_data(
				false_, true_, MemberDataPtr ptr, First&& first)
				DECLTYPE_AUTO_RETURN_NOEXCEPT(static_cast<First&&>(first).get()
					.*ptr) tpl<typ MemberDataPtr, typ First> static cexp
			auto invoke_member_data(
				false_, false_, MemberDataPtr ptr, First&& first)
				DECLTYPE_AUTO_RETURN_NOEXCEPT(
					(*static_cast<First&&>(first)).*ptr) public
			: tpl<typ F,
				  typ Obj,
				  typ First,
				  typ... Rest,
				  meta::if_c<is_function<F>::value, int> = 0> cexp auto
			  operator()(F Obj::*ptr, First&& first, Rest&&... rest) const
			  DECLTYPE_AUTO_RETURN_NOEXCEPT(
				  invoke_member_fn(is_base_of<Obj, decay_t<First>>{},
					  is_reference_wrapper_t<decay_t<First>>{},
					  ptr,
					  static_cast<First&&>(first),
					  static_cast<Rest&&>(rest)...)) tpl<typ Data,
				  typ Obj,
				  typ First,
				  meta::if_c<!is_function<Data>::value, int> = 0> cexp auto
			  operator()(Data Obj::*ptr, First&& first) const
			  DECLTYPE_AUTO_RETURN_NOEXCEPT(
				  invoke_member_data(is_base_of<Obj, decay_t<First>>{},
					  is_reference_wrapper_t<decay_t<First>>{},
					  ptr,
					  static_cast<First&&>(first))) tpl<typ F,
				  typ... Args,
				  meta::if_c<!std::is_member_pointer<remove_cvref_t<F>>::value,
					  int> = 0> cexp auto
			  operator()(F&& fn, Args&&... args) const
			  DECLTYPE_AUTO_RETURN_NOEXCEPT(
				  static_cast<F&&>(fn)(static_cast<Args&&>(args)...))
	};
	INLINE_VARIABLE(invoke_fn, invoke)

	/// \cond
	namespace detail
	{
		tpl<typ T> struct reference_wrapper_
		{
			T* t_ = nullptr;
			cexp
			reference_wrapper_()
				= default;
			cexp
			reference_wrapper_(T& t) noexcept
				: t_(std::addressof(t))
			{}
			cexp
			reference_wrapper_(T&&)
				= delete;
			cexp T&
			get() const noexcept
			{
				return *t_;
			}
		};
		tpl<typ T> struct reference_wrapper_<T&> : reference_wrapper_<T>
		{
			using reference_wrapper_<T>::reference_wrapper_;
		};
		tpl<typ T> struct reference_wrapper_<T&&>
		{
			T* t_ = nullptr;
			cexp
			reference_wrapper_()
				= default;
			cexp
			reference_wrapper_(T&& t) noexcept
				: t_(std::addressof(t))
			{}
			cexp T&&
			get() const noexcept
			{
				return static_cast<T&&>(*t_);
			}
		};
	} // namespace detail

	// Can be used to store rvalue references in addition to lvalue
	// references. Also, see: https://wg21.link/lwg2993
	tpl<typ T> struct reference_wrapper : private detail::reference_wrapper_<T>
	{
	private:
		using base_ = detail::reference_wrapper_<T>;

	public:
		using type = _t<remove_reference<T>>;
		using reference = meta::if_<is_reference<T>, T, T&>;

		cexp
		reference_wrapper()
			= default;
		tpl<typ U,
			CONCEPT_REQUIRES_(range::Constructible<base_, U>()
				&& !range::Same<remove_cvref_t<U>, reference_wrapper>())>
			cexp
			reference_wrapper(U&& u) noexcept(
				std::is_nothrow_constructible<base_, U>::value)
			: detail::reference_wrapper_<T>{static_cast<U&&>(u)}
		{}
		cexp reference
		get() const noexcept
		{
			return this->base_::get();
		}
		cexp operator reference() const noexcept
		{
			return get();
		}
		CONCEPT_REQUIRES(!is_rvalue_reference<T>::value)
		operator std::reference_wrapper<type>() const noexcept
		{
			return {get()};
		}
		tpl<typ... Args> cexp auto
		operator()(Args&&... args) const
			DECLTYPE_NOEXCEPT(invoke(declval<reference>(), declval<Args>()...))
		{
			return invoke(get(), static_cast<Args&&>(args)...);
		}
	};

	tpl<typ Fun, typ... Args> using invoke_result_t
		= decltype(invoke(declval<Fun>(), declval<Args>()...));

	tpl<typ Fun, typ... Args> struct invoke_result
		: meta::defer<invoke_result_t, Fun, Args...>
	{};

} // namespace Rider::Faiz

#endif
