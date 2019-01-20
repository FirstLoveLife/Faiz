// The Art of C++ / Operators
// Copyright (c) 2013-2018 Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/operators/

#ifndef TAO_OPERATORS_HPP
#define TAO_OPERATORS_HPP

#include "rider/faiz/macros.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"

#define TAO_OPERATORS_BASIC_OP(name, op) \
	tpl<typ T, typ U = T> class name \
	{ \
		friend T operator op(const T& lhs, const U& rhs) noexcept(noexcept( \
			T(lhs), Faiz::declval<T&>() op## = rhs, T(Faiz::declval<T&>()))) \
		{ \
			T nrv(lhs); \
			nrv op## = rhs; \
			return nrv; \
		} \
\
		friend T operator op(const T& lhs, U&& rhs) noexcept(noexcept(T(lhs), \
			Faiz::declval<T&>() op## = Faiz::move(rhs), \
			T(Faiz::declval<T&>()))) \
		{ \
			T nrv(lhs); \
			nrv op## = Faiz::move(rhs); \
			return nrv; \
		} \
\
		friend T operator op(T&& lhs, const U& rhs) noexcept( \
			noexcept(T(Faiz::move(lhs)), \
				Faiz::declval<T&>() op## = rhs, \
				T(Faiz::declval<T&>()))) \
		{ \
			T nrv(Faiz::move(lhs)); \
			nrv op## = rhs; \
			return nrv; \
		} \
\
		friend T \
		operator op(T&& lhs, U&& rhs) noexcept(noexcept(T(Faiz::move(lhs)), \
			Faiz::declval<T&>() op## = Faiz::move(rhs), \
			T(Faiz::declval<T&>()))) \
		{ \
			T nrv(Faiz::move(lhs)); \
			nrv op## = Faiz::move(rhs); \
			return nrv; \
		} \
	}

#define TAO_OPERATORS_BASIC_OP_LEFT(name, op) \
	tpl<typ T, typ U> class name##_left \
	{ \
		friend T operator op(const U& lhs, const T& rhs) noexcept(noexcept( \
			T(lhs), Faiz::declval<T&>() op## = rhs, T(Faiz::declval<T&>()))) \
		{ \
			T nrv(lhs); \
			nrv op## = rhs; \
			return nrv; \
		} \
\
		friend T operator op(const U& lhs, T&& rhs) noexcept(noexcept(T(lhs), \
			Faiz::declval<T&>() op## = Faiz::move(rhs), \
			T(Faiz::declval<T&>()))) \
		{ \
			T nrv(lhs); \
			nrv op## = Faiz::move(rhs); \
			return nrv; \
		} \
\
		friend T operator op(U&& lhs, const T& rhs) noexcept( \
			noexcept(T(Faiz::move(lhs)), \
				Faiz::declval<T&>() op## = rhs, \
				T(Faiz::declval<T&>()))) \
		{ \
			T nrv(Faiz::move(lhs)); \
			nrv op## = rhs; \
			return nrv; \
		} \
\
		friend T \
		operator op(U&& lhs, T&& rhs) noexcept(noexcept(T(Faiz::move(lhs)), \
			Faiz::declval<T&>() op## = Faiz::move(rhs), \
			T(Faiz::declval<T&>()))) \
		{ \
			T nrv(Faiz::move(lhs)); \
			nrv op## = Faiz::move(rhs); \
			return nrv; \
		} \
	}


#define TAO_OPERATORS_BASIC_OP_COMMUTATIVE(name, op) \
	tpl<typ T, typ U = T> class commutative_##name \
	{ \
		friend T operator op(const T& lhs, const U& rhs) noexcept(noexcept( \
			T(lhs), Faiz::declval<T&>() op## = rhs, T(Faiz::declval<T&>()))) \
		{ \
			T nrv(lhs); \
			nrv op## = rhs; \
			return nrv; \
		} \
\
		friend T operator op(const T& lhs, U&& rhs) noexcept(noexcept(T(lhs), \
			Faiz::declval<T&>() op## = Faiz::move(rhs), \
			T(Faiz::declval<T&>()))) \
		{ \
			T nrv(lhs); \
			nrv op## = Faiz::move(rhs); \
			return nrv; \
		} \
\
		friend T operator op(T&& lhs, const U& rhs) noexcept( \
			noexcept(T(Faiz::move(lhs)), \
				Faiz::declval<T&>() op## = rhs, \
				T(Faiz::declval<T&>()))) \
		{ \
			T nrv(Faiz::move(lhs)); \
			nrv op## = rhs; \
			return nrv; \
		} \
\
		friend T \
		operator op(T&& lhs, U&& rhs) noexcept(noexcept(T(Faiz::move(lhs)), \
			Faiz::declval<T&>() op## = Faiz::move(rhs), \
			T(Faiz::declval<T&>()))) \
		{ \
			T nrv(Faiz::move(lhs)); \
			nrv op## = Faiz::move(rhs); \
			return nrv; \
		} \
\
		friend T operator op(const U& lhs, const T& rhs) noexcept(noexcept( \
			T(rhs), Faiz::declval<T&>() op## = lhs, T(Faiz::declval<T&>()))) \
		{ \
			T nrv(rhs); \
			nrv op## = lhs; \
			return nrv; \
		} \
\
		friend T operator op(const U& lhs, T&& rhs) noexcept( \
			noexcept(T(Faiz::move(rhs)), \
				Faiz::declval<T&>() op## = lhs, \
				T(Faiz::declval<T&>()))) \
		{ \
			T nrv(Faiz::move(rhs)); \
			nrv op## = lhs; \
			return nrv; \
		} \
\
		friend T operator op(U&& lhs, const T& rhs) noexcept( \
			noexcept(T(rhs), Faiz::declval<T&>() op## = Faiz::move(lhs))) \
		{ \
			T nrv(rhs); \
			nrv op## = Faiz::move(lhs); \
			return nrv; \
		} \
\
		friend T operator op(U&& lhs, T&& rhs) noexcept(noexcept( \
			T(Faiz::move(rhs)), Faiz::declval<T&>() op## = Faiz::move(lhs))) \
		{ \
			T nrv(Faiz::move(rhs)); \
			nrv op## = Faiz::move(lhs); \
			return nrv; \
		} \
	}; \
\
	tpl<typ T> class commutative_##name<T> \
	{ \
		friend T operator op(const T& lhs, const T& rhs) noexcept(noexcept( \
			T(lhs), Faiz::declval<T&>() op## = rhs, T(Faiz::declval<T&>()))) \
		{ \
			T nrv(lhs); \
			nrv op## = rhs; \
			return nrv; \
		} \
\
		friend T operator op(const T& lhs, T&& rhs) noexcept(noexcept(T(lhs), \
			Faiz::declval<T&>() op## = Faiz::move(rhs), \
			T(Faiz::declval<T&>()))) \
		{ \
			T nrv(lhs); \
			nrv op## = Faiz::move(rhs); \
			return nrv; \
		} \
\
		friend T operator op(T&& lhs, const T& rhs) noexcept( \
			noexcept(T(Faiz::move(lhs)), \
				Faiz::declval<T&>() op## = rhs, \
				T(Faiz::declval<T&>()))) \
		{ \
			T nrv(Faiz::move(lhs)); \
			nrv op## = rhs; \
			return nrv; \
		} \
\
		friend T \
		operator op(T&& lhs, T&& rhs) noexcept(noexcept(T(Faiz::move(lhs)), \
			Faiz::declval<T&>() op## = Faiz::move(rhs), \
			T(Faiz::declval<T&>()))) \
		{ \
			T nrv(Faiz::move(lhs)); \
			nrv op## = Faiz::move(rhs); \
			return nrv; \
		} \
	}
// https://en.wikipedia.org/wiki/Category:Binary_operations
namespace Rider::Math::AbstractAlgebra::RelationAlgebra
{
	tpl<typ T, typ U = T> class equality_comparable
	{
		friend cfn
		operator!=(const T& lhs, const U& rhs)
			AUTO_RETURN_NOEXCEPT(!static_cast<bool>(lhs == rhs));

		friend cfn
		operator==(const U& lhs, const T& rhs)
			AUTO_RETURN_NOEXCEPT(static_cast<bool>(rhs == lhs));

		friend cfn
		operator!=(const U& lhs, const T& rhs)
			AUTO_RETURN_NOEXCEPT(static_cast<bool>(rhs != lhs));
	};

	tpl<typ T> class equality_comparable<T>
	{
		friend cfn
		operator!=(const T& lhs, const T& rhs)
			AUTO_RETURN_NOEXCEPT(!static_cast<bool>(lhs == rhs));
	};

	tpl<typ T, typ U = T> class less_than_comparable
	{
		friend cfn
		operator<=(const T& lhs, const U& rhs)
			AUTO_RETURN_NOEXCEPT(!static_cast<bool>(lhs > rhs));

		friend cfn
		operator>=(const T& lhs, const U& rhs)
			AUTO_RETURN_NOEXCEPT(!static_cast<bool>(lhs < rhs));

		friend cfn
		operator<(const U& lhs, const T& rhs)
			AUTO_RETURN_NOEXCEPT(static_cast<bool>(rhs > lhs));

		friend cfn
		operator>(const U& lhs, const T& rhs)
			AUTO_RETURN_NOEXCEPT(static_cast<bool>(rhs < lhs));

		friend cfn
		operator<=(const U& lhs, const T& rhs)
			AUTO_RETURN_NOEXCEPT(static_cast<bool>(rhs >= lhs));

		friend cfn
		operator>=(const U& lhs, const T& rhs)
			AUTO_RETURN_NOEXCEPT(static_cast<bool>(rhs <= lhs));
	};

	tpl<typ T> class less_than_comparable<T>
	{
		friend cfn
		operator>(const T& lhs, const T& rhs)
			AUTO_RETURN_NOEXCEPT(static_cast<bool>(rhs < lhs));

		friend cfn
		operator<=(const T& lhs, const T& rhs)
			AUTO_RETURN_NOEXCEPT(!static_cast<bool>(rhs < lhs));

		friend cfn
		operator>=(const T& lhs, const T& rhs)
			AUTO_RETURN_NOEXCEPT(!static_cast<bool>(lhs < rhs));
	};

	tpl<typ T, typ U = T> class totally_ordered : less_than_comparable<T, U>,
												  equality_comparable<T, U>
	{};

	tpl<typ T, typ U = T> class equivalent
	{
		friend cfn
		operator==(const T& lhs, const U& rhs) noexcept(noexcept(
			static_cast<bool>(lhs < rhs), static_cast<bool>(lhs > rhs)))
		{
			return !static_cast<bool>(lhs < rhs)
				&& !static_cast<bool>(lhs > rhs);
		}
	};

	tpl<typ T> class equivalent<T>
	{
		friend cfn
		operator==(const T& lhs, const T& rhs) noexcept(
			noexcept(static_cast<bool>(lhs < rhs)))
		{
			return !static_cast<bool>(lhs < rhs)
				&& !static_cast<bool>(rhs < lhs);
		}
	};

	tpl<typ T, typ U = T> class partially_ordered
	{
		friend cfn
		operator<=(const T& lhs, const U& rhs) noexcept(noexcept(
			static_cast<bool>(lhs < rhs), static_cast<bool>(lhs == rhs)))
		{
			return static_cast<bool>(lhs < rhs)
				|| static_cast<bool>(lhs == rhs);
		}

		friend cfn
		operator>=(const T& lhs, const U& rhs) noexcept(noexcept(
			static_cast<bool>(lhs > rhs), static_cast<bool>(lhs == rhs)))
		{
			return static_cast<bool>(lhs > rhs)
				|| static_cast<bool>(lhs == rhs);
		}

		friend cfn
		operator<(const U& lhs, const T& rhs)
			AUTO_RETURN_NOEXCEPT(static_cast<bool>(rhs > lhs));

		friend cfn
		operator>(const U& lhs, const T& rhs)
			AUTO_RETURN_NOEXCEPT(static_cast<bool>(rhs < lhs));

		friend cfn
		operator<=(const U& lhs, const T& rhs)
			AUTO_RETURN_NOEXCEPT(static_cast<bool>(rhs >= lhs));

		friend cfn
		operator>=(const U& lhs, const T& rhs)
			AUTO_RETURN_NOEXCEPT(static_cast<bool>(rhs <= lhs));
	};

	tpl<typ T> class partially_ordered<T>
	{
		friend cfn
		operator>(const T& lhs, const T& rhs)
			AUTO_RETURN_NOEXCEPT(static_cast<bool>(rhs < lhs));

		friend cfn
		operator<=(const T& lhs, const T& rhs) noexcept(noexcept(
			static_cast<bool>(lhs < rhs), static_cast<bool>(lhs == rhs)))
		{
			return static_cast<bool>(lhs < rhs)
				|| static_cast<bool>(lhs == rhs);
		}

		friend cfn
		operator>=(const T& lhs, const T& rhs) noexcept(noexcept(
			static_cast<bool>(rhs < lhs), static_cast<bool>(lhs == rhs)))
		{
			return static_cast<bool>(rhs < lhs)
				|| static_cast<bool>(lhs == rhs);
		}
	};

	TAO_OPERATORS_BASIC_OP(addable, +);
	TAO_OPERATORS_BASIC_OP_LEFT(addable, +);
	TAO_OPERATORS_BASIC_OP_COMMUTATIVE(addable, +);

	TAO_OPERATORS_BASIC_OP(subtractable, -);
	TAO_OPERATORS_BASIC_OP_LEFT(subtractable, -);

	TAO_OPERATORS_BASIC_OP(multipliable, *);
	TAO_OPERATORS_BASIC_OP_LEFT(multipliable, *);
	TAO_OPERATORS_BASIC_OP_COMMUTATIVE(multipliable, *);

	TAO_OPERATORS_BASIC_OP(dividable, /);
	TAO_OPERATORS_BASIC_OP_LEFT(dividable, /);

	TAO_OPERATORS_BASIC_OP(modable, %);
	TAO_OPERATORS_BASIC_OP_LEFT(modable, %);

	tpl<typ T, typ U = T> class ring : commutative_addable<T, U>,
									   subtractable<T, U>,
									   subtractable_left<T, U>,
									   multipliable<T, U>
	{};

	tpl<typ T> class ring<T>
		: commutative_addable<T>, subtractable<T>, multipliable<T>
	{};

	tpl<typ T, typ U = T> class ordered_ring : ring<T, U>, totally_ordered<T, U>
	{};

	tpl<typ T, typ U = T> class commutative_ring
		: commutative_addable<T, U>,
		  subtractable<T, U>,
		  subtractable_left<T, U>,
		  commutative_multipliable<T, U>
	{};

	tpl<typ T> class commutative_ring<T>
		: commutative_addable<T>, subtractable<T>, commutative_multipliable<T>
	{};

	tpl<typ T, typ U = T> class ordered_commutative_ring
		: commutative_ring<T, U>,
		  totally_ordered<T, U>
	{};

	tpl<typ T, typ U = T> class field : commutative_ring<T, U>,
										dividable<T, U>,
										dividable_left<T, U>
	{};

	tpl<typ T> class field<T> : commutative_ring<T>, dividable<T>
	{};

	tpl<typ T, typ U = T> class ordered_field : field<T, U>,
												totally_ordered<T, U>
	{};

	TAO_OPERATORS_BASIC_OP(andable, &);
	TAO_OPERATORS_BASIC_OP_LEFT(andable, &);
	TAO_OPERATORS_BASIC_OP_COMMUTATIVE(andable, &);

	TAO_OPERATORS_BASIC_OP(orable, |);
	TAO_OPERATORS_BASIC_OP_LEFT(orable, |);
	TAO_OPERATORS_BASIC_OP_COMMUTATIVE(orable, |);

	TAO_OPERATORS_BASIC_OP(xorable, ^);
	TAO_OPERATORS_BASIC_OP_LEFT(xorable, ^);
	TAO_OPERATORS_BASIC_OP_COMMUTATIVE(xorable, ^);

	tpl<typ T, typ U = T> class bitwise : andable<T, U>,
										  orable<T, U>,
										  xorable<T, U>
	{};

	tpl<typ T, typ U> class bitwise_left : andable_left<T, U>,
										   orable_left<T, U>,
										   xorable_left<T, U>
	{};

	tpl<typ T, typ U = T> class commutative_bitwise : commutative_andable<T, U>,
													  commutative_orable<T, U>,
													  commutative_xorable<T, U>
	{};

	TAO_OPERATORS_BASIC_OP(left_shiftable, <<);
	TAO_OPERATORS_BASIC_OP(right_shiftable, >>);

	tpl<typ T, typ U = T> class shiftable : left_shiftable<T, U>,
											right_shiftable<T, U>
	{};

	tpl<typ T> class incrementable
	{
		friend T
		operator++(T& arg, int /*unused*/) noexcept(
			noexcept(T(arg), ++arg, T(Faiz::declval<T>()))) // NOLINT
		{
			const T nrv(arg);
			++arg;
			return nrv;
		}
	};

	tpl<typ T> class decrementable
	{
		friend T
		operator--(T& arg, int /*unused*/) noexcept(
			noexcept(T(arg), --arg, T(Faiz::declval<T>()))) // NOLINT
		{
			const T nrv(arg);
			--arg;
			return nrv;
		}
	};

	tpl<typ T> class unit_steppable : incrementable<T>, decrementable<T>
	{};


} // namespace Rider::Math::AbstractAlgebra::RelationAlgebra

#undef TAO_OPERATORS_BASIC_OP
#undef TAO_OPERATORS_BASIC_OP_LEFT
#undef TAO_OPERATORS_BASIC_OP_COMMUTATIVE

#endif
