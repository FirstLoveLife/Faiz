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
	tpl<typ...> struct and_;


	tpl<> struct and_<> : true_
	{};

	tpl<typ _b1> struct and_<_b1> : _b1
	{};

	tpl<typ _b1, typ _b2, typ... _bn> struct and_<_b1, _b2, _bn...>
		: meta::if_<_b1, and_<_b2, _bn...>, _b1>
	{};

	tpl<typ... bn> inline constexpr bool and_v = and_<bn...>::value;

	tpl<typ...> struct or_;

	tpl<> struct or_<> : false_
	{};

	tpl<typ _b1> struct or_<_b1> : _b1
	{};

	tpl<typ _b1, class _b2, class... _bn> struct or_<_b1, _b2, _bn...>
		: meta::if_<_b1, _b1, or_<_b2, _bn...>>
	{};


	tpl<typ _b> struct not_ : bool_<!_b::value>
	{};


} // namespace Rider::Faiz::logic

namespace Rider::Faiz
{
	tpl<class...> struct empty_struct_tpl
	{};
	using empty_struct = empty_struct_tpl<>;

	tpl<bool _bCond> struct when;

	tpl<typ _type> struct always
	{
	private:
		tpl<typ...> struct impl : type_identity<_type>
		{};

	public:
		tpl<typ... _types> using apply = impl<_types...>;
	};


	tpl<typ _type, typ...> using well_formed_t = _type;

	tpl<typ... _types> using when_valid = well_formed_t<when<true>, _types...>;

	tpl<bool _bCond> using enable_when = enable_if_t<_bCond, when<true>>;

} // namespace Rider::Faiz

#endif
