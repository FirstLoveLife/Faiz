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
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"
#include "rider/faiz/utility/swap.hpp"


namespace rider::faiz::range
{
	namespace detail
	{

		constexpr struct valid_expr_t
		{
			template<typename... T>
			void
			operator()(T&&...) const;
		} valid_expr{};

		constexpr struct same_type_t
		{
			template<typename T, typename U>
			auto
			operator()(T&&, U&&) const -> meta::if_<is_same<T, U>, int>;
		} same_type{};

		constexpr struct is_true_t
		{
			template<typename Bool_>
			auto operator()(Bool_) const -> meta::if_c<Bool_::value, int>;
		} is_true{};

		constexpr struct is_false_t
		{
			template<typename Bool_>
			auto operator()(Bool_) const -> meta::if_c<!Bool_::value, int>;
		} is_false{};

		template<typename Concept>
		struct base_concept : type_identity<Concept>
		{};

		template<typename Concept, typename... Args>
		struct base_concept<Concept(Args...)> : type_identity<Concept>
		{};

		template<typename Concept>
		using base_concept_t = typename base_concept<Concept>::type;

		template<typename Concept, typename Enable = void>
		struct base_concepts_of : type_identity<meta::list<>>
		{};

		template<typename Concept>
		struct base_concepts_of<Concept,
			void_t<typename Concept::base_concepts_t>>
			: type_identity<typename Concept::base_concepts_t>
		{};

		template<typename Concept>
		using base_concepts_of_t = _t<base_concepts_of<Concept>>;

		template<typename... Ts>
		auto models_(range::detail::any) -> false_;


		template<typename... Ts,
			typename Concept,
			typename = decltype(&Concept::template requires_<Ts...>)>
		auto
		models_(Concept*) -> meta::apply<meta::quote<meta::lazy::strict_and>,
			meta::transform<base_concepts_of_t<Concept>,
				meta::bind_back<meta::quote<range::concepts::models>, Ts...>>>;

		template<typename List>
		struct most_refined_
		{};

		template<typename Head, typename... Tail>
		struct most_refined_<meta::list<Head, Tail...>> : type_identity<Head>
		{
			constexpr operator Head*() const
			{
				return nullptr;
			}
			constexpr Head*
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

		template<typename Ret, typename T>
		Ret
		returns_(T const&);

		template<typename T, typename U>
		auto
		convertible_to(U&& u)
			-> decltype(concepts::returns_<int>(static_cast<T>((U &&) u)));

		template<typename T, typename U>
		auto has_type(U &&) -> meta::if_<is_same<T, U>, int>;

		////////////////////////////////////////////////////////////////////////////////////////////
		// refines
		template<typename... Concepts>
		struct refines : virtual detail::base_concept_t<Concepts>...
		{
			// So that we don't create these by accident, since it's
			// surprisingly expensive to set up the vtable, given all the
			// virtual bases.
			refines() = delete;

			using base_concepts_t = meta::list<Concepts...>;

			template<typename... Ts>
			void
			requires_();
		};

		template<typename Concept, typename... Ts>
		struct models : bool_<_t<decltype(detail::models_<Ts...>(
							meta::detail::_nullptr_v<Concept>()))>::value>
		{};

		template<typename Concept, typename... Args, typename... Ts>
		struct models<Concept(Args...), Ts...>
			: models<Concept, meta::at<meta::list<Ts...>, Args>...>
		{};

		template<typename Concept, typename... Ts>
		auto
		model_of(Ts&&...)
			-> meta::if_c<concepts::models<Concept, Ts...>::value, int>;

		template<typename Concept, typename... Ts>
		auto
		model_of() -> meta::if_c<concepts::models<Concept, Ts...>::value, int>;

		// most_refined
		// Find the first concept in a list of concepts that is modeled by
		// the Args
		template<typename Concepts, typename... Ts>
		struct most_refined : detail::most_refined_<meta::find_if<Concepts,
								  meta::bind_back<meta::quote<models>, Ts...>>>
		{};

		template<typename Concepts, typename... Ts>
		using most_refined_t = _t<most_refined<Concepts, Ts...>>;

		////////////////////////////////////////////////////////////////////////////////////////////
		// Core language concepts
		////////////////////////////////////////////////////////////////////////////////////////////

		struct Satisfies
		{
			template<typename T, typename Trait, typename... Ts>
			auto
			requires_() -> decltype(concepts::valid_expr(
				concepts::is_true(meta::invoke<Trait, T, Ts...>{})));
		};

		struct Same
		{
			template<typename... Ts>
			struct same : true_
			{};
			template<typename T, typename... Us>
			struct same<T, Us...> : meta::and_c<is_same<T, Us>::value...>
			{};
			template<typename... Ts>
			using same_t = _t<same<Ts...>>;

			template<typename... Ts>
			auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::is_true(same_t<Ts...>{})));
		};

		struct ImplicitlyConvertibleTo
		{
			template<typename From, typename To>
			auto
			requires_() -> decltype(concepts::valid_expr(
				concepts::is_true(is_convertible<From, To>{})));
		};

		struct ExplicitlyConvertibleTo
		{
			template<typename From, typename To>
			auto requires_(From (&from)()) -> decltype(
				concepts::valid_expr(((void)static_cast<To>(from()), 42)));
		};

		struct ConvertibleTo
			: refines<ImplicitlyConvertibleTo, ExplicitlyConvertibleTo>
		{};

		struct DerivedFrom
		{
			template<typename T, typename U>
			auto
			requires_() -> decltype(concepts::valid_expr(
				concepts::is_true(is_base_of<U, T>{}),
				concepts::is_true(
					is_convertible<_t<remove_cv<T>>*, _t<remove_cv<U>>*>{})));
		};

		struct CommonReference
		{
			template<typename T, typename U, typename... Rest>
			using reference_t = common_reference_t<T, U, Rest...>;

			template<typename T, typename U>
			auto
			requires_() -> decltype(concepts::valid_expr(
				concepts::model_of<Same, reference_t<T, U>, reference_t<U, T>>(),
				concepts::model_of<ConvertibleTo, T, reference_t<T, U>>(),
				concepts::model_of<ConvertibleTo, U, reference_t<T, U>>()));

			template<typename T,
				typename U,
				typename... Rest,
				typename CommonReference_ = CommonReference>
			auto
			requires_() -> decltype(concepts::valid_expr(
				concepts::model_of<CommonReference_, T, U>(),
				concepts::
					model_of<CommonReference_, reference_t<T, U>, Rest...>()));
		};

		struct Common
		{
			template<typename T, typename U, typename... Rest>
			using value_t = common_type_t<T, U, Rest...>;

			template<typename T, typename U>
			auto
			requires_() -> decltype(concepts::valid_expr(concepts::is_true(
				is_same<remove_cvref_t<T>, remove_cvref_t<U>>{})));

			template<typename T, typename U>
			auto
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

			template<typename T,
				typename U,
				typename... Rest,
				typename Common_ = Common>
			auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::model_of<Common_, T, U>(),
					concepts::model_of<Common_, value_t<T, U>, Rest...>()));
		};

		struct Integral
		{
			template<typename T>
			auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::is_true(is_integral<T>{})));
		};

		struct SignedIntegral : refines<Integral>
		{
			template<typename T>
			auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::is_true(is_signed<T>{})));
		};

		struct UnsignedIntegral : refines<Integral>
		{
			template<typename T>
			auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::is_false(is_signed<T>{})));
		};

		struct Assignable
		{
			template<typename T, typename U>
			auto
			requires_(T&& t, U&& u) -> decltype(concepts::valid_expr(
				concepts::is_true(is_lvalue_reference<T>{}),
				concepts::model_of<CommonReference,
					range::detail::as_cref_t<T>,
					range::detail::as_cref_t<U>>(),
				concepts::has_type<T>((T &&) t = (U &&) u)));
		};

		struct Swappable
		{
			template<typename T>
			auto
			requires_(T&& t) -> decltype(concepts::valid_expr(
				((void)faiz::swap((T &&) t, (T &&) t), 42)));

			template<typename T, typename U>
			auto
			requires_(T&& t, U&& u) -> decltype(
				concepts::valid_expr(concepts::model_of<Swappable, T>(),
					concepts::model_of<Swappable, U>(),
					concepts::model_of<CommonReference,
						range::detail::as_cref_t<T>,
						range::detail::as_cref_t<U>>(),
					((void)faiz::swap((T &&) t, (U &&) u), 42),
					((void)faiz::swap((U &&) u, (T &&) t), 42)));
		};

		////////////////////////////////////////////////////////////////////////////////////////////
		// Comparison concepts
		////////////////////////////////////////////////////////////////////////////////////////////
		struct WeaklyEqualityComparable
		{
			template<typename T, typename U>
			auto
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
			template<typename T>
			auto
			requires_(range::detail::as_cref_t<T> t) -> decltype(
				concepts::valid_expr(concepts::convertible_to<bool>(t == t),
					concepts::convertible_to<bool>(t != t)));

			template<typename T, typename U>
			auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::is_true(is_same<T, U>{}),
					concepts::model_of<EqualityComparable, T>()));

			// Cross-type equality comparison from N3351:
			// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3351.pdf
			template<typename T,
				typename U,
				typename C = common_reference_t<range::detail::as_cref_t<T>,
					range::detail::as_cref_t<U>>>
			auto
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
			template<typename T>
			auto
			requires_(T&& t) -> decltype(
				concepts::valid_expr(concepts::convertible_to<bool>(t < t),
					concepts::convertible_to<bool>(t > t),
					concepts::convertible_to<bool>(t <= t),
					concepts::convertible_to<bool>(t >= t)));

			template<typename T,
				typename U,
				typename C = common_reference_t<range::detail::as_cref_t<T>,
					range::detail::as_cref_t<U>>>
			auto
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
			template<typename T>
			void
			requires_();

			template<typename T, typename U>
			auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::model_of<TotallyOrdered, T>(),
					concepts::model_of<TotallyOrdered, U>()));
		};

		// TODO: implement is_nothrow_destructible
		struct Destructible
		{
			template<typename T>
			auto
			requires_() -> decltype(concepts::valid_expr(
				concepts::is_true(std::is_nothrow_destructible<T>())));
		};

		struct Constructible : refines<Destructible(_1)>
		{
			template<typename T, typename... Args>
			auto
			requires_() -> decltype(concepts::valid_expr(
				concepts::is_true(is_constructible<T, Args...>{})));
		};

		struct DefaultConstructible : refines<Constructible>
		{
			template<typename T>
			void
			requires_();
		};

		struct MoveConstructible
		{
			template<typename T>
			auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::model_of<Constructible, T, T>(),
					concepts::model_of<ConvertibleTo, T, T>()));
		};

		struct CopyConstructible : refines<MoveConstructible>
		{
			template<typename T>
			auto
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
			template<typename T>
			auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::is_true(is_object<T>{}),
					concepts::model_of<Assignable, T&, T>(),
					concepts::model_of<Swappable, T&>()));
		};

		struct Copyable : refines<Movable, CopyConstructible>
		{
			template<typename T>
			auto
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

	template<typename T, template<typename...> class Trait, typename... Ts>
	using Satisfies
		= concepts::models<concepts::Satisfies, T, meta::quote<Trait>, Ts...>;

	template<typename... Ts>
	using Same = concepts::Same::same_t<Ts...>; // This handles void better than
												// using the Same concept

	template<typename T, typename U>
	using ImplicitlyConvertibleTo
		= concepts::models<concepts::ImplicitlyConvertibleTo, T, U>;

	template<typename T, typename U>
	using ExplicitlyConvertibleTo
		= concepts::models<concepts::ExplicitlyConvertibleTo, T, U>;

	template<typename T, typename U>
	using ConvertibleTo = concepts::models<concepts::ConvertibleTo, T, U>;

	template<typename T, typename U>
	using DerivedFrom = concepts::models<concepts::DerivedFrom, T, U>;

	template<typename T, typename U, typename... Rest>
	using CommonReference
		= concepts::models<concepts::CommonReference, T, U, Rest...>;

	template<typename T, typename U, typename... Rest>
	using Common = concepts::models<concepts::Common, T, U, Rest...>;

	template<typename T>
	using Integral = concepts::models<concepts::Integral, T>;

	template<typename T>
	using SignedIntegral = concepts::models<concepts::SignedIntegral, T>;

	template<typename T>
	using UnsignedIntegral = concepts::models<concepts::UnsignedIntegral, T>;

	template<typename T>
	using Destructible = concepts::models<concepts::Destructible, T>;

	template<typename T, typename... Args>
	using Constructible = concepts::models<concepts::Constructible, T, Args...>;

	template<typename T>
	using DefaultConstructible
		= concepts::models<concepts::DefaultConstructible, T>;

	template<typename T>
	using MoveConstructible = concepts::models<concepts::MoveConstructible, T>;

	template<typename T>
	using CopyConstructible = concepts::models<concepts::CopyConstructible, T>;

	template<typename T, typename U>
	using Assignable = concepts::models<concepts::Assignable, T, U>;

	template<typename T>
	using Movable = concepts::models<concepts::Movable, T>;

	template<typename T>
	using Copyable = concepts::models<concepts::Copyable, T>;

	template<typename T, typename U>
	using WeaklyEqualityComparable
		= concepts::models<concepts::WeaklyEqualityComparable, T, U>;

	template<typename T, typename U = T>
	using EqualityComparable
		= concepts::models<concepts::EqualityComparable, T, U>;

	template<typename T, typename U = T>
	using WeaklyOrdered = concepts::models<concepts::WeaklyOrdered, T, U>;

	template<typename T, typename U = T>
	using TotallyOrdered = concepts::models<concepts::TotallyOrdered, T, U>;

	template<typename T>
	using SemiRegular = concepts::models<concepts::SemiRegular, T>;

	template<typename T>
	using Regular = concepts::models<concepts::Regular, T>;

	template<typename T, typename U = T>
	using Swappable = concepts::models<concepts::Swappable, T, U>;
} // namespace rider::faiz::range

#define CONCEPT_PP_CAT_(X, Y) X##Y
#define CONCEPT_PP_CAT(X, Y) CONCEPT_PP_CAT_(X, Y)

#define CONCEPT_REQUIRES_(...) \
	bool CONCEPT_PP_CAT(_concept_requires_, __LINE__) \
		= false, \
		typename enable_if < CONCEPT_PP_CAT(_concept_requires_, __LINE__) \
		|| (__VA_ARGS__) > ::type * = nullptr /**/

#define CONCEPT_REQUIRES(...) template<CONCEPT_REQUIRES_(__VA_ARGS__)> /**/

#define CONCEPT_ASSERT(...) \
	static_assert((__VA_ARGS__), "Concept check failed: " #__VA_ARGS__)

#define CONCEPT_ASSERT_MSG static_assert

#endif // RANGES_V3_UTILITY_CONCEPTS_HPP
