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

#ifndef RANGES_V3_UTILITY_TAGGED_PAIR_HPP
#define RANGES_V3_UTILITY_TAGGED_PAIR_HPP

#include "rider/faiz/adl_get.hpp"
#include "rider/faiz/faiz_fwd.hpp"
#include "rider/faiz/macros.hpp"
#include "rider/faiz/utility.hpp"
#include "rider/faiz/utility/concepts.hpp"
#include "rider/faiz/utility/swap.hpp"

namespace Rider::Faiz
{
	namespace detail
	{
		tpl<typ T> using tag_spec = meta::front<meta::as_list<T>>;

		tpl<typ T> using tag_elem = meta::back<meta::as_list<T>>;
	} // namespace detail

	namespace _tagged_
	{
		tpl<typ Base, std::size_t, typ...> struct chain
		{
			using type = Base;
		};
		tpl<typ Base, std::size_t I, typ First, typ... Rest> struct chain<Base,
			I,
			First,
			Rest...>
		{
			using type = typ First::tpl
				getter<Base, I, _t<chain<Base, I + 1, Rest...>>>;
		};

#if RANGES_BROKEN_CPO_LOOKUP
		tpl<typ> struct adl_hook
		{};
#endif
	} // namespace _tagged_
	/// \endcond

	tpl<typ Base, typ... Tags> class tagged
		: public _t<_tagged_::chain<Base, 0, Tags...>>
#if RANGES_BROKEN_CPO_LOOKUP
		,
		  private _tagged_::adl_hook<tagged<Base, Tags...>>
#endif
	{
		CONCEPT_ASSERT(range::Same<Base, remove_cvref_t<Base>>());
		using base_t = _t<_tagged_::chain<Base, 0, Tags...>>;

		tpl<typ Other> using can_convert
			= bool_ < !std::is_same<Other, Base>::value
			&& std::is_convertible<Other, Base>::value > ;

	public:
		tagged() = default;
		using base_t::base_t;
		CONCEPT_REQUIRES(range::MoveConstructible<Base>())
		cexp
		tagged(Base&& that) noexcept(
			std::is_nothrow_move_constructible<Base>::value)
			: base_t(move(that))
		{}
		CONCEPT_REQUIRES(range::CopyConstructible<Base>())
		cexp
		tagged(Base const& that) noexcept(
			std::is_nothrow_copy_constructible<Base>::value)
			: base_t(that)
		{}
		tpl<typ Other, typ = meta::if_<can_convert<Other>>> cexp
		tagged(tagged<Other, Tags...>&& that) noexcept(
			std::is_nothrow_constructible<Base, Other>::value)
			: base_t(static_cast<Other&&>(that))
		{}
		tpl<typ Other, typ = meta::if_<can_convert<Other>>> cexp
		tagged(tagged<Other, Tags...> const& that) noexcept(
			std::is_nothrow_constructible<Base, Other const&>::value)
			: base_t(static_cast<Other const&>(that))
		{}
		tpl<typ Other, typ = meta::if_<can_convert<Other>>> cexp tagged&
		operator=(tagged<Other, Tags...>&& that) noexcept(
			noexcept(std::declval<Base&>() = static_cast<Other&&>(that)))
		{
			static_cast<Base&>(*this) = static_cast<Other&&>(that);
			return *this;
		}
		tpl<typ Other, typ = meta::if_<can_convert<Other>>> cexp tagged&
		operator=(tagged<Other, Tags...> const& that) noexcept(
			noexcept(std::declval<Base&>() = static_cast<Other const&>(that)))
		{
			static_cast<Base&>(*this) = static_cast<Other const&>(that);
			return *this;
		}
		tpl<typ U,
			typ = meta::if_c<!std::is_same<tagged, decay_t<U>>::value>,
			typ = decltype(std::declval<Base&>() = std::declval<U>())>
			cexp tagged&
			operator=(U&& u) noexcept(
				noexcept(std::declval<Base&>() = static_cast<U&&>(u)))
		{
			static_cast<Base&>(*this) = static_cast<U&&>(u);
			return *this;
		}
		tpl<typ B = Base> cexp meta::if_c<range::is_swappable<B>::value>
		swap(tagged& that) noexcept(range::is_nothrow_swappable<B>::value)
		{
			range::swap(static_cast<Base&>(*this), static_cast<Base&>(that));
		}
#if !RANGES_BROKEN_CPO_LOOKUP
		tpl<typ B = Base> friend cexp meta::if_c<range::is_swappable<B>::value>
		swap(tagged& x, tagged& y) noexcept(
			range::is_nothrow_swappable<B>::value)
		{
			x.swap(y);
		}
#endif
	};

	tpl<std::size_t I, typ Base, typ... Tags> auto
	get(tagged<Base, Tags...>& t)
		DECLTYPE_AUTO_RETURN_NOEXCEPT(detail::adl_get<I>(static_cast<Base&>(t)))
			tpl<std::size_t I, typ Base, typ... Tags> auto get(
				tagged<Base, Tags...> const& t)
				DECLTYPE_AUTO_RETURN_NOEXCEPT(
					detail::adl_get<I>(static_cast<Base const&>(t)))
					tpl<std::size_t I, typ Base, typ... Tags> auto get(
						tagged<Base, Tags...>&& t)
						DECLTYPE_AUTO_RETURN_NOEXCEPT(
							detail::adl_get<I>(static_cast<Base&&>(t)))
							tpl<std::size_t I, typ Base, typ... Tags> void get(
								tagged<Base, Tags...> const&&)
		= delete;

	tpl<typ T, typ Base, typ... Tags> auto
	get(tagged<Base, Tags...>& t)
		DECLTYPE_AUTO_RETURN_NOEXCEPT(detail::adl_get<T>(static_cast<Base&>(t)))
			tpl<typ T, typ Base, typ... Tags> auto get(
				tagged<Base, Tags...> const& t)
				DECLTYPE_AUTO_RETURN_NOEXCEPT(
					detail::adl_get<T>(static_cast<Base const&>(t)))
					tpl<typ T, typ Base, typ... Tags> auto get(
						tagged<Base, Tags...>&& t)
						DECLTYPE_AUTO_RETURN_NOEXCEPT(
							detail::adl_get<T>(static_cast<Base&&>(t)))
							tpl<typ T, typ Base, typ... Tags> void get(
								tagged<Base, Tags...> const&&)
		= delete;

	tpl<typ F, typ S> using tagged_pair
		= tagged<std::pair<detail::tag_elem<F>, detail::tag_elem<S>>,
			detail::tag_spec<F>,
			detail::tag_spec<S>>;

	tpl<typ Tag1,
		typ Tag2,
		typ T1,
		typ T2,
		typ R = tagged_pair<Tag1(range::bind_element_t<T1>),
			Tag2(range::bind_element_t<T2>)>>
		cexp R
		make_tagged_pair(T1&& t1, T2&& t2) noexcept(
			std::is_nothrow_constructible<R, T1, T2>::value)
	{
		return {static_cast<T1&&>(t1), static_cast<T2&&>(t2)};
	}
} // namespace Rider::Faiz

#define RANGES_DEFINE_TAG_SPECIFIER(NAME) \
	namespace tag \
	{ \
		struct NAME \
		{ \
			tpl<typ Untagged, std::size_t I, typ Next> class getter \
				: public Next \
			{ \
			protected: \
				~getter() = default; \
\
			public: \
				getter() = default; \
				getter(getter&&) = default; \
				getter(getter const&) = default; \
				using Next::Next; \
				CONCEPT_REQUIRES(MoveConstructible<Untagged>()) \
				cexp \
				getter(Untagged&& that) noexcept( \
					std::is_nothrow_move_constructible<Untagged>::value) \
					: Next(detail::move(that)) \
				{} \
				CONCEPT_REQUIRES(CopyConstructible<Untagged>()) \
				cexp \
				getter(Untagged const& that) noexcept( \
					std::is_nothrow_copy_constructible<Untagged>::value) \
					: Next(that) \
				{} \
				getter& \
				operator=(getter&&) \
					= default; \
				getter& \
				operator=(getter const&) \
					= default; \
				RANGES_CXX14_CEXP \
				meta::_t<std::tuple_element<I, Untagged>>& \
					NAME() \
					& noexcept(noexcept( \
						  detail::adl_get<I>(std::declval<Untagged&>()))) \
				{ \
					return detail::adl_get<I>(static_cast<Untagged&>(*this)); \
				} \
				RANGES_CXX14_CEXP \
				meta::_t<std::tuple_element<I, Untagged>>&& \
					NAME() \
					&& noexcept(noexcept( \
						   detail::adl_get<I>(std::declval<Untagged>()))) \
				{ \
					return detail::adl_get<I>(static_cast<Untagged&&>(*this)); \
				} \
				cexp meta::_t<std::tuple_element<I, Untagged>> const& \
				NAME() const& noexcept(noexcept( \
					detail::adl_get<I>(std::declval<Untagged const&>()))) \
				{ \
					return detail::adl_get<I>( \
						static_cast<Untagged const&>(*this)); \
				} \
			}; \
		}; \
	} \
	/**/


#endif
