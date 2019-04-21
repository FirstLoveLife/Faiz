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
		Tpl<Typ T> using tag_spec = meta::front<meta::as_list<T>>;

		Tpl<Typ T> using tag_elem = meta::back<meta::as_list<T>>;
	} // namespace detail

	namespace _tagged_
	{
		Tpl<Typ Base, std::size_t, Typ...> struct chain
		{
			using type = Base;
		};
		Tpl<Typ Base, std::size_t I, Typ First, Typ... Rest> struct chain<Base,
			I,
			First,
			Rest...>
		{
			using type = Typ First::Tpl
				getter<Base, I, _t<chain<Base, I + 1, Rest...>>>;
		};

#if RANGES_BROKEN_CPO_LOOKUP
		Tpl<Typ> struct adl_hook
		{};
#endif
	} // namespace _tagged_
	/// \endcond

	Tpl<Typ Base, Typ... Tags> class tagged
		: public _t<_tagged_::chain<Base, 0, Tags...>>
#if RANGES_BROKEN_CPO_LOOKUP
		,
		  private _tagged_::adl_hook<tagged<Base, Tags...>>
#endif
	{
		CONCEPT_ASSERT(range::Same<Base, remove_cvref_t<Base>>());
		using base_t = _t<_tagged_::chain<Base, 0, Tags...>>;

		Tpl<Typ Other> using can_convert
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
		Tpl<Typ Other, Typ = meta::if_<can_convert<Other>>> cexp
		tagged(tagged<Other, Tags...>&& that) noexcept(
			std::is_nothrow_constructible<Base, Other>::value)
			: base_t(static_cast<Other&&>(that))
		{}
		Tpl<Typ Other, Typ = meta::if_<can_convert<Other>>> cexp
		tagged(tagged<Other, Tags...> const& that) noexcept(
			std::is_nothrow_constructible<Base, Other const&>::value)
			: base_t(static_cast<Other const&>(that))
		{}
		Tpl<Typ Other, Typ = meta::if_<can_convert<Other>>> cexp tagged&
		operator=(tagged<Other, Tags...>&& that) noexcept(
			noexcept(std::declval<Base&>() = static_cast<Other&&>(that)))
		{
			static_cast<Base&>(*this) = static_cast<Other&&>(that);
			return *this;
		}
		Tpl<Typ Other, Typ = meta::if_<can_convert<Other>>> cexp tagged&
		operator=(tagged<Other, Tags...> const& that) noexcept(
			noexcept(std::declval<Base&>() = static_cast<Other const&>(that)))
		{
			static_cast<Base&>(*this) = static_cast<Other const&>(that);
			return *this;
		}
		Tpl<Typ U,
			Typ = meta::if_c<!std::is_same<tagged, decay_t<U>>::value>,
			Typ = decltype(std::declval<Base&>() = std::declval<U>())>
			cexp tagged&
			operator=(U&& u) noexcept(
				noexcept(std::declval<Base&>() = static_cast<U&&>(u)))
		{
			static_cast<Base&>(*this) = static_cast<U&&>(u);
			return *this;
		}
		Tpl<Typ B = Base> cexp meta::if_c<range::is_swappable<B>::value>
		swap(tagged& that) noexcept(range::is_nothrow_swappable<B>::value)
		{
			range::swap(static_cast<Base&>(*this), static_cast<Base&>(that));
		}
#if !RANGES_BROKEN_CPO_LOOKUP
		Tpl<Typ B = Base> friend cexp meta::if_c<range::is_swappable<B>::value>
		swap(tagged& x, tagged& y) noexcept(
			range::is_nothrow_swappable<B>::value)
		{
			x.swap(y);
		}
#endif
	};

	Tpl<std::size_t I, Typ Base, Typ... Tags> auto
	get(tagged<Base, Tags...>& t)
		DECLTYPE_AUTO_RETURN_NOEXCEPT(detail::adl_get<I>(static_cast<Base&>(t)))
			Tpl<std::size_t I, Typ Base, Typ... Tags> auto get(
				tagged<Base, Tags...> const& t)
				DECLTYPE_AUTO_RETURN_NOEXCEPT(
					detail::adl_get<I>(static_cast<Base const&>(t)))
					Tpl<std::size_t I, Typ Base, Typ... Tags> auto get(
						tagged<Base, Tags...>&& t)
						DECLTYPE_AUTO_RETURN_NOEXCEPT(
							detail::adl_get<I>(static_cast<Base&&>(t)))
							Tpl<std::size_t I, Typ Base, Typ... Tags> void get(
								tagged<Base, Tags...> const&&)
		= delete;

	Tpl<Typ T, Typ Base, Typ... Tags> auto
	get(tagged<Base, Tags...>& t)
		DECLTYPE_AUTO_RETURN_NOEXCEPT(detail::adl_get<T>(static_cast<Base&>(t)))
			Tpl<Typ T, Typ Base, Typ... Tags> auto get(
				tagged<Base, Tags...> const& t)
				DECLTYPE_AUTO_RETURN_NOEXCEPT(
					detail::adl_get<T>(static_cast<Base const&>(t)))
					Tpl<Typ T, Typ Base, Typ... Tags> auto get(
						tagged<Base, Tags...>&& t)
						DECLTYPE_AUTO_RETURN_NOEXCEPT(
							detail::adl_get<T>(static_cast<Base&&>(t)))
							Tpl<Typ T, Typ Base, Typ... Tags> void get(
								tagged<Base, Tags...> const&&)
		= delete;

	Tpl<Typ F, Typ S> using tagged_pair
		= tagged<std::pair<detail::tag_elem<F>, detail::tag_elem<S>>,
			detail::tag_spec<F>,
			detail::tag_spec<S>>;

	Tpl<Typ Tag1,
		Typ Tag2,
		Typ T1,
		Typ T2,
		Typ R = tagged_pair<Tag1(range::bind_element_t<T1>),
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
			Tpl<Typ Untagged, std::size_t I, Typ Next> class getter \
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
