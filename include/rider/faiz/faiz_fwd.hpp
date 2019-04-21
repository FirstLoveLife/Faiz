/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2014-present
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
#ifndef FAIZ_FWD_HPP
#define FAIZ_FWD_HPP
#include "rider/faiz/range.hpp"
#include "rider/faiz/faiz_fwd_meta.hpp"
#include "rider/faiz/debug.hpp"

namespace Rider::Faiz::logic
{
	Tpl<Typ...> struct and_;


	Tpl<> struct and_<> : true_
	{};

	Tpl<Typ _b1> struct and_<_b1> : _b1
	{};

	Tpl<Typ _b1, Typ _b2, Typ... _bn> struct and_<_b1, _b2, _bn...>
		: meta::if_<_b1, and_<_b2, _bn...>, _b1>
	{};

	Tpl<Typ... bn> inline constexpr bool and_v = and_<bn...>::value;

	Tpl<Typ...> struct or_;

	Tpl<> struct or_<> : false_
	{};

	Tpl<Typ _b1> struct or_<_b1> : _b1
	{};

	Tpl<Typ _b1, class _b2, class... _bn> struct or_<_b1, _b2, _bn...>
		: meta::if_<_b1, _b1, or_<_b2, _bn...>>
	{};


	Tpl<Typ _b> struct not_ : bool_<!_b::value>
	{};


} // namespace Rider::Faiz::logic

namespace Rider::Faiz
{
	Tpl<class...> struct empty_struct_tpl
	{};
	using empty_struct = empty_struct_tpl<>;

	Tpl<bool _bCond> struct when;

	Tpl<Typ _type> struct always
	{
	private:
		Tpl<Typ...> struct impl : type_identity<_type>
		{};

	public:
		Tpl<Typ... _types> using apply = impl<_types...>;
	};


	Tpl<Typ _type, Typ...> using well_formed_t = _type;

	Tpl<Typ... _types> using when_valid = well_formed_t<when<true>, _types...>;

	Tpl<bool _bCond> using enable_when = enable_if_t<_bCond, when<true>>;

} // namespace Rider::Faiz

#endif
