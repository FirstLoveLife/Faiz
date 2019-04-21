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
// Acknowledgements: Thanks for Paul Fultz for the suggestions that
//                   concepts can be ordinary Boolean metafunctions.
//
// Project home: https://github.com/ericniebler/range-v3
//

#ifndef CONCEPTS
#define CONCEPTS

#include "rider/faiz/common_type.hpp"
#include "rider/faiz/cstddef.hpp"
#include "rider/faiz/macros.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"
#include "rider/faiz/utility/swap.hpp"


namespace Rider::Faiz::range
{
	namespace detail
	{

		cexp struct valid_expr_t
		{
			Tpl<Typ... T> void
			operator()(T&&...) const;
		} valid_expr{};

		cexp struct same_type_t
		{
			Tpl<Typ T, Typ U> auto
			operator()(T&&, U&&) const -> meta::if_<is_same<T, U>, int>;
		} same_type{};

		cexp struct is_true_t
		{
			Tpl<Typ Bool_> auto
			operator()(Bool_) const -> meta::if_c<Bool_::value, int>;
		} is_true{};

		cexp struct is_false_t
		{
			Tpl<Typ Bool_> auto
			operator()(Bool_) const -> meta::if_c<!Bool_::value, int>;
		} is_false{};

		Tpl<Typ Concept> struct base_concept : type_identity<Concept>
		{};

		Tpl<Typ Concept, Typ... Args> struct base_concept<Concept(Args...)>
			: type_identity<Concept>
		{};

		Tpl<Typ Concept> using base_concept_t = Typ base_concept<Concept>::type;

		Tpl<Typ Concept, Typ Enable = void> struct base_concepts_of
			: type_identity<meta::list<>>
		{};

		Tpl<Typ Concept> struct base_concepts_of<Concept,
			void_t<Typ Concept::base_concepts_t>>
			: type_identity<Typ Concept::base_concepts_t>
		{};

		Tpl<Typ Concept> using base_concepts_of_t
			= _t<base_concepts_of<Concept>>;

		Tpl<Typ... Ts> auto models_(range::detail::any) -> false_;


		Tpl<Typ... Ts,
			Typ Concept,
			Typ = decltype(&Concept::Tpl requires_<Ts...>)> auto
		models_(Concept*) -> meta::apply<meta::quote<meta::lazy::strict_and>,
			meta::transform<base_concepts_of_t<Concept>,
				meta::bind_back<meta::quote<range::concepts::models>, Ts...>>>;

		Tpl<Typ List> struct most_refined_
		{};

		Tpl<Typ Head,
			Typ... Tail> struct most_refined_<meta::list<Head, Tail...>>
			: type_identity<Head>
		{
			cexp operator Head*() const
			{
				return nullptr;
			}
			cexp Head*
			operator()() const
			{
				return nullptr;
			}
		};
	} // namespace detail

	namespace concepts
	{
		using detail::valid_expr;
		using detail::same_type;
		using detail::is_true;
		using detail::is_false;

		using _1 = integral_constant<int, 0>;
		using _2 = integral_constant<int, 1>;
		using _3 = integral_constant<int, 2>;
		using _4 = integral_constant<int, 3>;
		using _5 = integral_constant<int, 4>;
		using _6 = integral_constant<int, 5>;
		using _7 = integral_constant<int, 6>;
		using _8 = integral_constant<int, 7>;
		using _9 = integral_constant<int, 8>;

		Tpl<Typ Ret, Typ T> Ret
		returns_(T const&);

		Tpl<Typ T, Typ U> auto
		convertible_to(U&& u)
			-> decltype(concepts::returns_<int>(static_cast<T>((U &&) u)));

		Tpl<Typ T, Typ U> auto has_type(U &&) -> meta::if_<is_same<T, U>, int>;

		////////////////////////////////////////////////////////////////////////////////////////////
		// refines
		Tpl<Typ... Concepts> struct refines
			: virtual detail::base_concept_t<Concepts>...
		{
			// So that we don't create these by accident, since it's
			// surprisingly expensive to set up the vtable, given all the
			// virtual bases.
			refines() = delete;

			using base_concepts_t = meta::list<Concepts...>;

			Tpl<Typ... Ts> void
			requires_();
		};

		Tpl<Typ Concept, Typ... Ts> struct models
			: bool_<_t<decltype(detail::models_<Ts...>(
				  meta::detail::_nullptr_v<Concept>()))>::value>
		{};

		Tpl<Typ Concept, Typ... Args, Typ... Ts> struct models<Concept(Args...),
			Ts...> : models<Concept, meta::at<meta::list<Ts...>, Args>...>
		{};

		Tpl<Typ Concept, Typ... Ts> auto
		model_of(Ts&&...)
			-> meta::if_c<concepts::models<Concept, Ts...>::value, int>;

		Tpl<Typ Concept, Typ... Ts> auto
		model_of() -> meta::if_c<concepts::models<Concept, Ts...>::value, int>;

		// most_refined
		// Find the first concept in a list of concepts that is modeled by
		// the Args
		Tpl<Typ Concepts, Typ... Ts> struct most_refined
			: detail::most_refined_<meta::find_if<Concepts,
				  meta::bind_back<meta::quote<models>, Ts...>>>
		{};

		Tpl<Typ Concepts, Typ... Ts> using most_refined_t
			= _t<most_refined<Concepts, Ts...>>;

		////////////////////////////////////////////////////////////////////////////////////////////
		// Core language concepts
		////////////////////////////////////////////////////////////////////////////////////////////

		struct Satisfies
		{
			Tpl<Typ T, Typ Trait, Typ... Ts> auto
			requires_() -> decltype(concepts::valid_expr(
				concepts::is_true(meta::invoke<Trait, T, Ts...>{})));
		};

		struct Same
		{
			Tpl<Typ... Ts> struct same : true_
			{};
			Tpl<Typ T, Typ... Us> struct same<T, Us...>
				: meta::and_c<is_same<T, Us>::value...>
			{};
			Tpl<Typ... Ts> using same_t = _t<same<Ts...>>;

			Tpl<Typ... Ts> auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::is_true(same_t<Ts...>{})));
		};

		struct ImplicitlyConvertibleTo
		{
			Tpl<Typ From, Typ To> auto
			requires_() -> decltype(concepts::valid_expr(
				concepts::is_true(is_convertible<From, To>{})));
		};

		struct ExplicitlyConvertibleTo
		{
			Tpl<Typ From, Typ To> auto requires_(From (&from)()) -> decltype(
				concepts::valid_expr(((void)static_cast<To>(from()), 42)));
		};

		struct ConvertibleTo
			: refines<ImplicitlyConvertibleTo, ExplicitlyConvertibleTo>
		{};

		struct DerivedFrom
		{
			Tpl<Typ T, Typ U> auto
			requires_() -> decltype(concepts::valid_expr(
				concepts::is_true(is_base_of<U, T>{}),
				concepts::is_true(
					is_convertible<_t<remove_cv<T>>*, _t<remove_cv<U>>*>{})));
		};

		struct CommonReference
		{
			Tpl<Typ T, Typ U, Typ... Rest> using reference_t
				= common_reference_t<T, U, Rest...>;

			Tpl<Typ T, Typ U> auto
			requires_() -> decltype(concepts::valid_expr(
				concepts::model_of<Same, reference_t<T, U>, reference_t<U, T>>(),
				concepts::model_of<ConvertibleTo, T, reference_t<T, U>>(),
				concepts::model_of<ConvertibleTo, U, reference_t<T, U>>()));

			Tpl<Typ T,
				Typ U,
				Typ... Rest,
				Typ CommonReference_ = CommonReference> auto
			requires_() -> decltype(concepts::valid_expr(
				concepts::model_of<CommonReference_, T, U>(),
				concepts::
					model_of<CommonReference_, reference_t<T, U>, Rest...>()));
		};

		struct Common
		{
			Tpl<Typ T, Typ U, Typ... Rest> using value_t
				= common_type_t<T, U, Rest...>;

			Tpl<Typ T, Typ U> auto
			requires_() -> decltype(concepts::valid_expr(concepts::is_true(
				is_same<remove_cvref_t<T>, remove_cvref_t<U>>{})));

			Tpl<Typ T, Typ U> auto
			requires_() -> decltype(concepts::valid_expr(
				concepts::is_false(
					is_same<remove_cvref_t<T>, remove_cvref_t<U>>{}),
				concepts::model_of<Same, value_t<T, U>, value_t<U, T>>(),
				concepts::model_of<ConvertibleTo, T, value_t<T, U>>(),
				concepts::model_of<ConvertibleTo, U, value_t<T, U>>(),
				concepts::model_of<CommonReference,
					range::detail::as_cref_t<T>,
					range::detail::as_cref_t<U>>(),
				concepts::model_of<CommonReference,
					value_t<T, U>&,
					common_reference_t<range::detail::as_cref_t<T>,
						range::detail::as_cref_t<U>>>()));

			Tpl<Typ T, Typ U, Typ... Rest, Typ Common_ = Common> auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::model_of<Common_, T, U>(),
					concepts::model_of<Common_, value_t<T, U>, Rest...>()));
		};

		struct Integral
		{
			Tpl<Typ T> auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::is_true(is_integral<T>{})));
		};

		struct SignedIntegral : refines<Integral>
		{
			Tpl<Typ T> auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::is_true(is_signed<T>{})));
		};

		struct UnsignedIntegral : refines<Integral>
		{
			Tpl<Typ T> auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::is_false(is_signed<T>{})));
		};

		struct Assignable
		{
			Tpl<Typ T, Typ U> auto
			requires_(T&& t, U&& u) -> decltype(concepts::valid_expr(
				concepts::is_true(is_lvalue_reference<T>{}),
				concepts::model_of<CommonReference,
					range::detail::as_cref_t<T>,
					range::detail::as_cref_t<U>>(),
				concepts::has_type<T>((T &&) t = (U &&) u)));
		};

		struct Swappable
		{
			Tpl<Typ T> auto
			requires_(T&& t) -> decltype(concepts::valid_expr(
				((void)range::swap((T &&) t, (T &&) t), 42)));

			Tpl<Typ T, Typ U> auto
			requires_(T&& t, U&& u) -> decltype(
				concepts::valid_expr(concepts::model_of<Swappable, T>(),
					concepts::model_of<Swappable, U>(),
					concepts::model_of<CommonReference,
						range::detail::as_cref_t<T>,
						range::detail::as_cref_t<U>>(),
					((void)range::swap((T &&) t, (U &&) u), 42),
					((void)range::swap((U &&) u, (T &&) t), 42)));
		};

		////////////////////////////////////////////////////////////////////////////////////////////
		// Comparison concepts
		////////////////////////////////////////////////////////////////////////////////////////////
		struct WeaklyEqualityComparable
		{
			Tpl<Typ T, Typ U> auto
			requires_(
				range::detail::as_cref_t<T> t, range::detail::as_cref_t<U> u)
				-> decltype(
					// Not to spec: doesn't compare to a Boolean trait
					concepts::valid_expr(concepts::convertible_to<bool>(t == u),
						concepts::convertible_to<bool>(t != u),
						concepts::convertible_to<bool>(u == t),
						concepts::convertible_to<bool>(u != t)));
		};

		struct EqualityComparable
		{
			Tpl<Typ T> auto
			requires_(range::detail::as_cref_t<T> t) -> decltype(
				concepts::valid_expr(concepts::convertible_to<bool>(t == t),
					concepts::convertible_to<bool>(t != t)));

			Tpl<Typ T, Typ U> auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::is_true(is_same<T, U>{}),
					concepts::model_of<EqualityComparable, T>()));

			// Cross-type equality comparison from N3351:
			// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3351.pdf
			Tpl<Typ T,
				Typ U,
				Typ C = common_reference_t<range::detail::as_cref_t<T>,
					range::detail::as_cref_t<U>>> auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::is_false(is_same<T, U>{}),
					concepts::model_of<EqualityComparable, T>(),
					concepts::model_of<EqualityComparable, U>(),
					concepts::model_of<WeaklyEqualityComparable, T, U>(),
					concepts::model_of<CommonReference,
						range::detail::as_cref_t<T>,
						range::detail::as_cref_t<U>>(),
					concepts::model_of<EqualityComparable, C>()));
		};

		struct WeaklyOrdered
		{
			Tpl<Typ T> auto
			requires_(T&& t) -> decltype(
				concepts::valid_expr(concepts::convertible_to<bool>(t < t),
					concepts::convertible_to<bool>(t > t),
					concepts::convertible_to<bool>(t <= t),
					concepts::convertible_to<bool>(t >= t)));

			Tpl<Typ T,
				Typ U,
				Typ C = common_reference_t<range::detail::as_cref_t<T>,
					range::detail::as_cref_t<U>>> auto
			requires_(
				range::detail::as_cref_t<T> t, range::detail::as_cref_t<U> u)
				-> decltype(
					concepts::valid_expr(concepts::model_of<WeaklyOrdered, T>(),
						concepts::model_of<WeaklyOrdered, U>(),
						concepts::model_of<CommonReference,
							range::detail::as_cref_t<T>,
							range::detail::as_cref_t<U>>(),
						concepts::model_of<WeaklyOrdered, C>(),
						concepts::convertible_to<bool>(t < u),
						concepts::convertible_to<bool>(u < t),
						concepts::convertible_to<bool>(t > u),
						concepts::convertible_to<bool>(u > t),
						concepts::convertible_to<bool>(t <= u),
						concepts::convertible_to<bool>(u <= t),
						concepts::convertible_to<bool>(t >= u),
						concepts::convertible_to<bool>(u >= t)));
		};

		struct TotallyOrdered : refines<EqualityComparable, WeaklyOrdered>
		{
			Tpl<Typ T> void
			requires_();

			Tpl<Typ T, Typ U> auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::model_of<TotallyOrdered, T>(),
					concepts::model_of<TotallyOrdered, U>()));
		};

		// TODO: implement is_nothrow_destructible
		struct Destructible
		{
			Tpl<Typ T> auto
			requires_() -> decltype(concepts::valid_expr(
				concepts::is_true(std::is_nothrow_destructible<T>())));
		};

		struct Constructible : refines<Destructible(_1)>
		{
			Tpl<Typ T, Typ... Args> auto
			requires_() -> decltype(concepts::valid_expr(
				concepts::is_true(is_constructible<T, Args...>{})));
		};

		struct DefaultConstructible : refines<Constructible>
		{
			Tpl<Typ T> void
			requires_();
		};

		struct MoveConstructible
		{
			Tpl<Typ T> auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::model_of<Constructible, T, T>(),
					concepts::model_of<ConvertibleTo, T, T>()));
		};

		struct CopyConstructible : refines<MoveConstructible>
		{
			Tpl<Typ T> auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::model_of<Constructible, T, T&>(),
					concepts::model_of<Constructible, T, T const&>(),
					concepts::model_of<Constructible, T, T const>(),
					concepts::model_of<ConvertibleTo, T&, T>(),
					concepts::model_of<ConvertibleTo, T const&, T>(),
					concepts::model_of<ConvertibleTo, T const, T>()));
		};

		struct Movable : refines<MoveConstructible>
		{
			Tpl<Typ T> auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::is_true(is_object<T>{}),
					concepts::model_of<Assignable, T&, T>(),
					concepts::model_of<Swappable, T&>()));
		};

		struct Copyable : refines<Movable, CopyConstructible>
		{
			Tpl<Typ T> auto
			requires_() -> decltype(concepts::valid_expr(
				// Spec requires this to be validated
				concepts::model_of<Assignable, T&, T const&>(),
				// Spec does not require these to be validated
				concepts::model_of<Assignable, T&, T&>(),
				concepts::model_of<Assignable, T&, T const>()));
		};

		struct SemiRegular : refines<Copyable, DefaultConstructible>
		{
			// Axiom: copies are independent. See Fundamentals of Generic
			// Programming http://www.stepanovpapers.com/DeSt98.pdf
		};

		struct Regular : refines<SemiRegular, EqualityComparable>
		{};
	} // namespace concepts

	Tpl<Typ T, Tpl<Typ...> class Trait, Typ... Ts> using Satisfies
		= concepts::models<concepts::Satisfies, T, meta::quote<Trait>, Ts...>;

	Tpl<Typ... Ts> using Same
		= concepts::Same::same_t<Ts...>; // This handles void better than
										 // using the Same concept

	Tpl<Typ T, Typ U> using ImplicitlyConvertibleTo
		= concepts::models<concepts::ImplicitlyConvertibleTo, T, U>;

	Tpl<Typ T, Typ U> using ExplicitlyConvertibleTo
		= concepts::models<concepts::ExplicitlyConvertibleTo, T, U>;

	Tpl<Typ T, Typ U> using ConvertibleTo
		= concepts::models<concepts::ConvertibleTo, T, U>;

	Tpl<Typ T, Typ U> using DerivedFrom
		= concepts::models<concepts::DerivedFrom, T, U>;

	Tpl<Typ T, Typ U, Typ... Rest> using CommonReference
		= concepts::models<concepts::CommonReference, T, U, Rest...>;

	Tpl<Typ T, Typ U, Typ... Rest> using Common
		= concepts::models<concepts::Common, T, U, Rest...>;

	Tpl<Typ T> using Integral = concepts::models<concepts::Integral, T>;

	Tpl<Typ T> using SignedIntegral
		= concepts::models<concepts::SignedIntegral, T>;

	Tpl<Typ T> using UnsignedIntegral
		= concepts::models<concepts::UnsignedIntegral, T>;

	Tpl<Typ T> using Destructible = concepts::models<concepts::Destructible, T>;

	Tpl<Typ T, Typ... Args> using Constructible
		= concepts::models<concepts::Constructible, T, Args...>;

	Tpl<Typ T> using DefaultConstructible
		= concepts::models<concepts::DefaultConstructible, T>;

	Tpl<Typ T> using MoveConstructible
		= concepts::models<concepts::MoveConstructible, T>;

	Tpl<Typ T> using CopyConstructible
		= concepts::models<concepts::CopyConstructible, T>;

	Tpl<Typ T, Typ U> using Assignable
		= concepts::models<concepts::Assignable, T, U>;

	Tpl<Typ T> using Movable = concepts::models<concepts::Movable, T>;

	Tpl<Typ T> using Copyable = concepts::models<concepts::Copyable, T>;

	Tpl<Typ T, Typ U> using WeaklyEqualityComparable
		= concepts::models<concepts::WeaklyEqualityComparable, T, U>;

	Tpl<Typ T, Typ U = T> using EqualityComparable
		= concepts::models<concepts::EqualityComparable, T, U>;

	Tpl<Typ T, Typ U = T> using WeaklyOrdered
		= concepts::models<concepts::WeaklyOrdered, T, U>;

	Tpl<Typ T, Typ U = T> using TotallyOrdered
		= concepts::models<concepts::TotallyOrdered, T, U>;

	Tpl<Typ T> using SemiRegular = concepts::models<concepts::SemiRegular, T>;

	Tpl<Typ T> using Regular = concepts::models<concepts::Regular, T>;

	Tpl<Typ T, Typ U = T> using Swappable
		= concepts::models<concepts::Swappable, T, U>;
} // namespace Rider::Faiz::range

#define CONCEPT_PP_CAT_(X, Y) X##Y
#define CONCEPT_PP_CAT(X, Y) CONCEPT_PP_CAT_(X, Y)

#define CONCEPT_REQUIRES_(...) \
	bool CONCEPT_PP_CAT(_concept_requires_, __LINE__) \
		= false, \
		Typ enable_if < CONCEPT_PP_CAT(_concept_requires_, __LINE__) \
		|| (__VA_ARGS__) > ::type * = nullptr /**/

#define CONCEPT_REQUIRES(...) Tpl<CONCEPT_REQUIRES_(__VA_ARGS__)> /**/

#define CONCEPT_ASSERT(...) \
	static_assert((__VA_ARGS__), "Concept check failed: " #__VA_ARGS__)

#define CONCEPT_ASSERT_MSG static_assert

#endif // RANGES_V3_UTILITY_CONCEPTS_HPP
