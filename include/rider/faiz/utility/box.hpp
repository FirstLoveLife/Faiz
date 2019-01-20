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

#ifndef RANGES_V3_UTILITY_BOX_HPP
#define RANGES_V3_UTILITY_BOX_HPP

#include "rider/faiz/faiz_fwd.hpp"
#include "rider/faiz/macros.hpp"
#include "rider/faiz/utility/concepts.hpp"
#include <utility>

namespace Rider::Faiz
{
	tpl<typ T> struct mutable_
	{
		mutable T value;
		CONCEPT_REQUIRES(std::is_default_constructible<T>::value)
		cexp
		mutable_()
			: value{}
		{}
		cexp explicit mutable_(T const& t) : value(t)
		{}
		cexp explicit mutable_(T&& t) : value(move(t))
		{}
		mutable_ const&
		operator=(T const& t) const
		{
			value = t;
			return *this;
		}
		mutable_ const&
		operator=(T&& t) const
		{
			value = move(t);
			return *this;
		}
		cexp operator T&() const&
		{
			return value;
		}
	};

	tpl<typ T, T v> struct constant
	{
		constant() = default;
		cexp explicit constant(T const&)
		{}
		constant&
		operator=(T const&)
		{
			return *this;
		}
		constant const&
		operator=(T const&) const
		{
			return *this;
		}
		cexp operator T() const
		{
			return v;
		}
		cexp T
		exchange(T const&) const
		{
			return v;
		}
	};

	static_assert(std::is_trivial<constant<int, 0>>::value,
		"Expected constant to be trivial");

	/// \cond
	namespace detail
	{
		// "box" has three different implementations that store a T
		// differently:
		enum class box_compress
		{
			none, // Nothing special: get() returns a reference to a T
				  // member subobject
			ebo, // Apply Empty Base Optimization: get() returns a reference
				 // to a T base subobject
			coalesce // Coalesce all Ts into one T: get() returns a
					 // reference to a static T singleton
		};

		// Per N4582, lambda closures are *not*:
		// - aggregates             ([expr.prim.lambda]/4)
		// - default constructible  ([expr.prim.lambda]/p21)
		// - copy assignable        ([expr.prim.lambda]/p21)
		tpl<typ Fn> using could_be_lambda
			= bool_ < !std::is_default_constructible<Fn>::value
			&& !std::is_copy_assignable<Fn>::value > ;

		tpl<typ> cexp box_compress
		box_compression_(...)
		{
			return box_compress::none;
		}
		tpl<typ T,
			typ = meta::if_<meta::strict_and<std::is_empty<T>,
				meta::not_<is_final<T>>
#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ == 6 \
	&& __GNUC_MINOR__ < 2
				// GCC 6.0 & 6.1 find empty lambdas' implicit conversion to
				// function pointer when doing overload resolution for
				// function calls. That causes hard errors.
				// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=71117
				,
				meta::not_<could_be_lambda<T>>
#endif
				>>>
			cexp box_compress
			box_compression_(long)
		{
			return box_compress::ebo;
		}
#ifndef RANGES_WORKAROUND_MSVC_249830
		// MSVC pukes passing non-constant-expression objects to cexp
		// functions, so do not coalesce.
		tpl<typ T,
			typ = meta::if_<meta::strict_and<std::is_empty<T>,
				std::is_trivial<T>,
				std::is_default_constructible<T>>>>
			cexp box_compress
			box_compression_(int)
		{
			return box_compress::coalesce;
		}
#endif
		tpl<typ T> cexp box_compress
		box_compression()
		{
			return box_compression_<T>(0);
		}
	} // namespace detail
	/// \endcond

	tpl<typ Element,
		typ Tag = void,
		detail::box_compress = detail::box_compression<Element>()> class box
	{
		Element value;

	public:
		CONCEPT_REQUIRES(std::is_default_constructible<Element>::value)
		cexp
		box() noexcept(std::is_nothrow_default_constructible<Element>::value)
			: value{}
		{}
		tpl<typ E,
			CONCEPT_REQUIRES_(std::is_constructible<Element, E>::value&&
					std::is_convertible<E, Element>::value)>
			cexp
			box(E&& e) noexcept(
				std::is_nothrow_constructible<Element, E>::value)
			: value(static_cast<E&&>(e))
		{}
		tpl<typ E,
			CONCEPT_REQUIRES_(std::is_constructible<Element, E>::value
				&& !std::is_convertible<E, Element>::value)>
			cexp explicit box(E&& e) noexcept(
				std::is_nothrow_constructible<Element, E>::value)
			: value(static_cast<E&&>(e))
		{}

		cexp Element&
			get()
			& noexcept
		{
			return value;
		}
		cexp Element const&
		get() const& noexcept
		{
			return value;
		}
		cexp Element&&
			get()
			&& noexcept
		{
			return move(value);
		}
	};

	tpl<typ Element, typ Tag> class box<Element, Tag, detail::box_compress::ebo>
		: Element
	{
	public:
		CONCEPT_REQUIRES(std::is_default_constructible<Element>::value)
		cexp
		box() noexcept(std::is_nothrow_default_constructible<Element>::value)
			: Element{}
		{}
		tpl<typ E,
			CONCEPT_REQUIRES_(std::is_constructible<Element, E>::value&&
					std::is_convertible<E, Element>::value)>
			cexp
			box(E&& e) noexcept(
				std::is_nothrow_constructible<Element, E>::value)
			: Element(static_cast<E&&>(e))
		{}
		tpl<typ E,
			CONCEPT_REQUIRES_(std::is_constructible<Element, E>::value
				&& !std::is_convertible<E, Element>::value)>
			cexp explicit box(E&& e) noexcept(
				std::is_nothrow_constructible<Element, E>::value)
			: Element(static_cast<E&&>(e))
		{}

		cexp Element&
			get()
			& noexcept
		{
			return *this;
		}
		cexp Element const&
		get() const& noexcept
		{
			return *this;
		}
		cexp Element&&
			get()
			&& noexcept
		{
			return move(*this);
		}
	};

	tpl<typ Element,
		typ Tag> class box<Element, Tag, detail::box_compress::coalesce>
	{
		static Element value;

	public:
		cexp
		box() noexcept
		{}
		tpl<typ E,
			CONCEPT_REQUIRES_(std::is_constructible<Element, E>::value&&
					std::is_convertible<E, Element>::value)>
			cexp
			box(E&&) noexcept
		{}
		tpl<typ E,
			CONCEPT_REQUIRES_(std::is_constructible<Element, E>::value
				&& !std::is_convertible<E, Element>::value)>
			cexp explicit box(E&&) noexcept
		{}

		cexp Element&
			get()
			& noexcept
		{
			return value;
		}
		cexp Element const&
		get() const& noexcept
		{
			return value;
		}
		cexp Element&&
			get()
			&& noexcept
		{
			return move(value);
		}
	};

	tpl<typ Element, typ Tag> Element
		box<Element, Tag, detail::box_compress::coalesce>::value;

	// Get by tag type
	tpl<typ Tag, typ Element, detail::box_compress BC> cexp Element&
	get(box<Element, Tag, BC>& b) noexcept
	{
		return b.get();
	}

	tpl<typ Tag, typ Element, detail::box_compress BC> cexp Element const&
	get(box<Element, Tag, BC> const& b) noexcept
	{
		return b.get();
	}

	tpl<typ Tag, typ Element, detail::box_compress BC> cexp Element&&
	get(box<Element, Tag, BC>&& b) noexcept
	{
		return move(b).get();
	}

	// Get by index
	tpl<std::size_t I, typ Element, detail::box_compress BC> cexp Element&
	get(box<Element, size_t_<I>, BC>& b) noexcept
	{
		return b.get();
	}

	tpl<std::size_t I, typ Element, detail::box_compress BC> cexp Element const&
	get(box<Element, size_t_<I>, BC> const& b) noexcept
	{
		return b.get();
	}

	tpl<std::size_t I, typ Element, detail::box_compress BC> cexp Element&&
	get(box<Element, size_t_<I>, BC>&& b) noexcept
	{
		return move(b).get();
	}
	/// @}
} // namespace Rider::Faiz

#endif
