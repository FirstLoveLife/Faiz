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
#ifndef UTILITY_FUNCTIONAL_HPP
#define UTILITY_FUNCTIONAL_HPP

#include "rider/faiz/faiz_fwd.hpp"
#include "rider/faiz/macros.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"
#include "rider/faiz/utility/box.hpp"
#include "rider/faiz/utility/concepts.hpp"
#include "rider/faiz/utility/invoke.hpp"
#include <functional>

namespace Rider::Faiz::range
{
	struct equal_to
	{
		tpl<typ T, typ U, CONCEPT_REQUIRES_(EqualityComparable<T, U>())>
			cexp bool
			operator()(T&& t, U&& u) const
		{
			return (T &&) t == (U &&) u;
		}
		using is_transparent = void;
	};

	struct not_equal_to
	{
		tpl<typ T, typ U, CONCEPT_REQUIRES_(EqualityComparable<T, U>())>
			cexp bool
			operator()(T&& t, U&& u) const
		{
			return (T &&) t != (U &&) u;
		}
		using is_transparent = void;
	};

	struct less
	{
		tpl<typ T, typ U, CONCEPT_REQUIRES_(WeaklyOrdered<T, U>())> cexp bool
		operator()(T&& t, U&& u) const
		{
			return (T &&) t < (U &&) u;
		}
		using is_transparent = void;
	};

	struct ordered_less
	{
		tpl<typ T, typ U, CONCEPT_REQUIRES_(TotallyOrdered<T, U>())> cexp bool
		operator()(T&& t, U&& u) const
		{
			return (T &&) t < (U &&) u;
		}
		using is_transparent = void;
	};

	struct ident
	{
		tpl<typ T> cexp T&&
		operator()(T&& t) const noexcept
		{
			return (T &&) t;
		}
		using is_transparent = void;
	};

	struct plus
	{
		tpl<typ T, typ U> cexp auto
		operator()(T&& t, U&& u) const -> decltype((T &&) t + (U &&) u)
		{
			return (T &&) t + (U &&) u;
		}
		using is_transparent = void;
	};

	struct minus
	{
		tpl<typ T, typ U> cexp auto
		operator()(T&& t, U&& u) const -> decltype((T &&) t - (U &&) u)
		{
			return (T &&) t - (U &&) u;
		}
		using is_transparent = void;
	};

	struct multiplies
	{
		tpl<typ T, typ U> cexp auto
		operator()(T&& t, U&& u) const -> decltype((T &&) t * (U &&) u)
		{
			return (T &&) t * (U &&) u;
		}
		using is_transparent = void;
	};

	struct bitwise_or
	{
		tpl<typ T, typ U> cexp auto
		operator()(T&& t, U&& u) const -> decltype((T &&) t | (U &&) u)
		{
			return (T &&) t | (U &&) u;
		}
		using is_transparent = void;
	};

	tpl<typ T> struct convert_to
	{
		tpl<typ U> cexp auto
		operator()(U&& u) const
			DECLTYPE_AUTO_RETURN_NOEXCEPT(static_cast<T>((U &&) u))
	};

	tpl<typ T> struct coerce
	{
		cexp T&
		operator()(T& t) const
		{
			return t;
		}
		/// \overload
		cexp T const&
		operator()(T const& t) const
		{
			return t;
		}
		/// \overload
		cexp T
		operator()(T&& t) const
		{
			return (T &&) t;
		}
		T
		operator()(T const&&) const = delete;
	};

	tpl<typ T> struct coerce<T const> : coerce<T>
	{};

	tpl<typ T> struct coerce<T&> : coerce<T>
	{};

	tpl<typ T> struct coerce<T&&> : coerce<T>
	{};

	struct dereference_fn
	{
		tpl<typ I> cexp auto
		operator()(I& i) const DECLTYPE_AUTO_RETURN_NOEXCEPT(*i)
	};
	INLINE_VARIABLE(dereference_fn, dereference)

	/// \addtogroup group-concepts
	/// @{
	namespace concepts
	{
		struct Invocable
		{
			tpl<typ Fun, typ... Args> using result_t
				= invoke_result_t<Fun, Args...>;

			tpl<typ Fun, typ... Args> auto
			requires_() -> void_t<invoke_result_t<Fun, Args...>>;
		};

		struct RegularInvocable : refines<Invocable>
		{
			// Axiom: equality_preserving(invoke(f, args...))
		};

		struct Predicate : refines<RegularInvocable>
		{
			tpl<typ Fun, typ... Args> auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::model_of<ConvertibleTo,
					Invocable::result_t<Fun, Args...>,
					bool>()));
		};

		struct Relation
		{
			tpl<typ Fun, typ T> auto
			requires_() -> decltype(concepts::valid_expr(
				concepts::model_of<Predicate, Fun, T, T>()));

			tpl<typ Fun, typ T, typ U> auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::is_true(is_same<T, U>{}),
					concepts::model_of<Predicate, Fun, T, U>()));

			tpl<typ Fun,
				typ T,
				typ U,
				typ C = common_reference_t<detail::as_cref_t<T>,
					detail::as_cref_t<U>>> auto
			requires_() -> decltype(
				concepts::valid_expr(concepts::is_false(is_same<T, U>{}),
					concepts::model_of<Relation, Fun, T, T>(),
					concepts::model_of<Relation, Fun, U, U>(),
					concepts::model_of<CommonReference,
						detail::as_cref_t<T>,
						detail::as_cref_t<U>>(),
					concepts::model_of<Relation, Fun, C, C>(),
					concepts::model_of<Predicate, Fun, T, U>(),
					concepts::model_of<Predicate, Fun, U, T>()));
		};
	} // namespace concepts

	tpl<typ Fun, typ... Args> using Invocable
		= concepts::models<concepts::Invocable, Fun, Args...>;

	tpl<typ Fun, typ... Args> using RegularInvocable
		= concepts::models<concepts::RegularInvocable, Fun, Args...>;

	tpl<typ Fun, typ... Args> using Predicate
		= concepts::models<concepts::Predicate, Fun, Args...>;

	tpl<typ Fun, typ T, typ U = T> using Relation
		= concepts::models<concepts::Relation, Fun, T, U>;
	/// @}

	tpl<typ FD> struct logical_negate_
	{
	private:
		CONCEPT_ASSERT(Same<FD, decay_t<FD>>() && MoveConstructible<FD>());
		FD pred_;

	public:
		CONCEPT_REQUIRES(DefaultConstructible<FD>())
		cexp
		logical_negate_() noexcept(
			std::is_nothrow_default_constructible<FD>::value)
		{}
		tpl<typ T,
			typ U = meta::if_c<!Same<decay_t<T>, logical_negate_>(), T>,
			CONCEPT_REQUIRES_(Constructible<FD, U>())> explicit cexp
		logical_negate_(T&& pred)
			: pred_(static_cast<T&&>(pred))
		{}


		tpl<typ... Args, CONCEPT_REQUIRES_(Predicate<FD&, Args...>())> cexp auto
			operator()(Args&&... args)
			& DECLTYPE_NOEXCEPT(
				  !invoke(declval<FD&>(), static_cast<Args&&>(args)...))
		{
			return !invoke(pred_, static_cast<Args&&>(args)...);
		}
		/// \overload
		tpl<typ... Args, CONCEPT_REQUIRES_(Predicate<FD const&, Args...>())>
			cexp auto
			operator()(Args&&... args) const& DECLTYPE_NOEXCEPT(
				!invoke(declval<FD const&>(), static_cast<Args&&>(args)...))
		{
			return !invoke(pred_, static_cast<Args&&>(args)...);
		}
		/// \overload
		tpl<typ... Args, CONCEPT_REQUIRES_(Predicate<FD, Args...>())> cexp auto
			operator()(Args&&... args)
			&& DECLTYPE_NOEXCEPT(
				   !invoke(declval<FD>(), static_cast<Args&&>(args)...))
		{
			return !invoke(
				static_cast<FD&&>(pred_), static_cast<Args&&>(args)...);
		}
		/// \overload
		tpl<typ... Args, CONCEPT_REQUIRES_(Predicate<FD const, Args...>())>
			cexp auto
			operator()(Args&&... args) const&& DECLTYPE_NOEXCEPT(
				!invoke(declval<FD const>(), static_cast<Args&&>(args)...))
		{
			return !invoke(
				static_cast<FD const&&>(pred_), static_cast<Args&&>(args)...);
		}
	};

	tpl<typ Pred> using logical_negate = logical_negate_<decay_t<Pred>>;

	struct not_fn_fn
	{
		tpl<typ Pred,
			typ FD = decay_t<Pred>,
			CONCEPT_REQUIRES_(
				MoveConstructible<FD>() && Constructible<FD, Pred>())>
			cexp logical_negate_<FD>
			operator()(Pred&& pred) const
		{
			return logical_negate_<FD>{(Pred &&) pred};
		}
	};

	/// \ingroup group-utility
	/// \sa `not_fn_fn`
	INLINE_VARIABLE(not_fn_fn, not_fn)

	/// \cond
	inline namespace
	{
		DEPRECATED("\"not_\" now uses the C++17 name \"not_fn\".")
		cexp const auto& not_ = not_fn;
	} // namespace
	/// \endcond

	tpl<typ Second, typ First> struct composed
		: private compressed_pair<First, Second>
	{
	private:
		using composed::compressed_pair::first;
		using composed::compressed_pair::second;
		tpl<typ A, typ B, typ... Ts> static auto
		do_(A& a, B& b, false_, Ts&&... ts)
			DECLTYPE_AUTO_RETURN_NOEXCEPT(invoke(b, invoke(a, (Ts &&) ts...)))
				tpl<typ A, typ B, typ... Ts> static auto do_(
					A& a, B& b, true_, Ts&&... ts)
					DECLTYPE_AUTO_RETURN_NOEXCEPT((invoke(a, (Ts &&) ts...),
						invoke(b))) public : composed()
											 = default;
		composed(Second second, First first)
			: composed::compressed_pair{move(first), move(second)}
		{}
		tpl<typ... Ts, typ FirstResultT = invoke_result_t<First&, Ts...>> auto
		operator()(Ts&&... ts)
			DECLTYPE_NOEXCEPT(composed::do_(declval<First&>(),
				declval<Second&>(),
				is_void<FirstResultT>{},
				(Ts &&) ts...))
		{
			return composed::do_(
				first(), second(), is_void<FirstResultT>{}, (Ts &&) ts...);
		}
		tpl<typ... Ts,
			typ FirstResultT = invoke_result_t<First const&, Ts...>> auto
		operator()(Ts&&... ts) const
			DECLTYPE_NOEXCEPT(composed::do_(declval<First const&>(),
				declval<Second const&>(),
				is_void<FirstResultT>{},
				(Ts &&) ts...))
		{
			return composed::do_(
				first(), second(), is_void<FirstResultT>{}, (Ts &&) ts...);
		}
	};

	struct compose_fn
	{
		tpl<typ Second, typ First> composed<Second, First>
		operator()(Second second, First first) const
		{
			return {move(second), move(first)};
		}
	};

	/// \ingroup group-utility
	/// \sa `compose_fn`
	INLINE_VARIABLE(compose_fn, compose)

	tpl<> struct overloaded<>
	{};

	tpl<typ First, typ... Rest> struct overloaded<First, Rest...>
		: private compressed_pair<First, overloaded<Rest...>>
	{
	private:
		using base_t = compressed_pair<First, overloaded<Rest...>>;
		using base_t::first;
		using base_t::second;

	public:
		overloaded() = default;
		cexp
		overloaded(First first, Rest... rest)
			: overloaded::compressed_pair{detail::move(first),
				  overloaded<Rest...>{detail::move(rest)...}}
		{}
		tpl<typ... Args> auto
		operator()(Args&&... args) DECLTYPE_NOEXCEPT(
			invoke(declval<First&>(), static_cast<Args&&>(args)...))
		{
			return invoke(first(), static_cast<Args&&>(args)...);
		}
		tpl<typ... Args> auto
		operator()(Args&&... args) const DECLTYPE_NOEXCEPT(
			invoke(declval<First const&>(), static_cast<Args&&>(args)...))
		{
			return invoke(first(), static_cast<Args&&>(args)...);
		}
		tpl<typ... Args> auto
		operator()(Args&&... args) DECLTYPE_NOEXCEPT(
			declval<overloaded<Rest...>&>()(static_cast<Args&&>(args)...))
		{
			return second()(static_cast<Args&&>(args)...);
		}
		tpl<typ... Args> auto
		operator()(Args&&... args) const DECLTYPE_NOEXCEPT(
			declval<overloaded<Rest...> const&>()(static_cast<Args&&>(args)...))
		{
			return second()(static_cast<Args&&>(args)...);
		}
	};

	struct overload_fn
	{
		tpl<typ Fn> cexp Fn
		operator()(Fn fn) const
		{
			return fn;
		}

		tpl<typ... Fns> cexp overloaded<Fns...>
		operator()(Fns... fns) const
		{
			return overloaded<Fns...>{detail::move(fns)...};
		}
	};

	/// \ingroup group-utility
	/// \sa `overload_fn`
	INLINE_VARIABLE(overload_fn, overload)

	tpl<typ Fn> struct indirected : private box<Fn, indirected<Fn>>
	{
	private:
		using box<Fn, indirected<Fn>>::get;

	public:
		indirected() = default;
		indirected(Fn fn) : indirected::box(move(fn))
		{}
		// value_type (needs no impl)
		tpl<typ... Its>[[noreturn]] auto
		operator()(copy_tag, Its...) const
			-> invoke_result_t<Fn&, reference_t<Its>...>
		{
			Expects(false);
		}

		// Reference
		tpl<typ... Its> auto
		operator()(Its... its)
			DECLTYPE_NOEXCEPT(invoke(declval<Fn&>(), *its...))
		{
			return invoke(get(), *its...);
		}
		tpl<typ... Its> auto
		operator()(Its... its) const
			DECLTYPE_NOEXCEPT(invoke(declval<Fn const&>(), *its...))
		{
			return invoke(get(), *its...);
		}

		// Rvalue reference
		tpl<typ... Its> auto
		operator()(move_tag, Its... its) noexcept(
			noexcept(aux::move(invoke(declval<Fn&>(), *its...))))
			-> aux::move_t<decltype(invoke(declval<Fn&>(), *its...))>
		{
			return aux::move(invoke(get(), *its...));
		}
		tpl<typ... Its> auto
		operator()(move_tag, Its... its) const
			noexcept(noexcept(aux::move(invoke(declval<Fn const&>(), *its...))))
				-> aux::move_t<decltype(invoke(declval<Fn const&>(), *its...))>
		{
			return aux::move(invoke(get(), *its...));
		}
	};

	struct indirect_fn
	{
		tpl<typ Fn> cexp indirected<Fn>
		operator()(Fn fn) const
		{
			return indirected<Fn>{detail::move(fn)};
		}
	};

	/// \ingroup group-utility
	/// \sa `indirect_fn`
	INLINE_VARIABLE(indirect_fn, indirect)

	tpl<typ Fn1, typ Fn2> struct transformed : private compressed_pair<Fn1, Fn2>
	{
	private:
		using transformed::compressed_pair::first;
		using transformed::compressed_pair::second;

	public:
		transformed() = default;
		cexp
		transformed(Fn1 fn1, Fn2 fn2)
			: transformed::compressed_pair{detail::move(fn1), detail::move(fn2)}
		{}
		tpl<typ... Args> auto
		operator()(Args&&... args) DECLTYPE_NOEXCEPT(invoke(declval<Fn1&>(),
			invoke(declval<Fn2&>(), static_cast<Args&&>(args))...))
		{
			return invoke(
				first(), invoke(second(), static_cast<Args&&>(args)...));
		}
		tpl<typ... Args> auto
		operator()(Args&&... args) const
			DECLTYPE_NOEXCEPT(invoke(declval<Fn1 const&>(),
				invoke(declval<Fn2 const&>(), static_cast<Args&&>(args))...))
		{
			return invoke(
				first(), invoke(second(), static_cast<Args&&>(args)...));
		}
	};

	struct on_fn
	{
		tpl<typ Fn1, typ Fn2> cexp transformed<Fn1, Fn2>
		operator()(Fn1 fn1, Fn2 fn2) const
		{
			return transformed<Fn1, Fn2>{detail::move(fn1), detail::move(fn2)};
		}
	};

	/// \ingroup group-utility
	/// \sa `on_fn`
	INLINE_VARIABLE(on_fn, on)

	/// \cond
	namespace detail
	{
		tpl<typ Bind> struct pipeable_binder : Bind,
											   pipeable<pipeable_binder<Bind>>
		{
			pipeable_binder(Bind bind) : Bind(move(bind))
			{}
		};

		tpl<typ Pipe0, typ Pipe1> struct composed_pipe
		{
			Pipe0 pipe0_;
			Pipe1 pipe1_;
			tpl<typ Arg> auto
			operator()(Arg&& arg) const DECLTYPE_AUTO_RETURN_NOEXCEPT(
				static_cast<Arg&&>(arg) | pipe0_ | pipe1_)
		};
	} // namespace detail
	/// \endcond

	struct make_pipeable_fn
	{
		tpl<typ Fun> detail::pipeable_binder<Fun>
		operator()(Fun fun) const
		{
			return {move(fun)};
		}
	};

	/// \ingroup group-utility
	/// \sa `make_pipeable_fn`
	INLINE_VARIABLE(make_pipeable_fn, make_pipeable)

	tpl<typ T,
		typ U = meta::if_<is_lvalue_reference<T>,
			std::reference_wrapper<_t<remove_reference<T>>>,
			T&&>>
		U
		bind_forward(_t<remove_reference<T>>& t) noexcept
	{
		return static_cast<U>(t);
	}

	tpl<typ T> T&&
	bind_forward(_t<remove_reference<T>>&& t) noexcept
	{
		// This is to catch way sketchy stuff like: forward<int const &>(42)
		static_assert(
			!is_lvalue_reference<T>::value, "You didn't just do that!");
		return static_cast<T&&>(t);
	}

	struct pipeable_base
	{};

	tpl<typ T> struct is_pipeable : is_base_of<pipeable_base, T>
	{};

	tpl<typ T> struct is_pipeable<T&> : is_pipeable<T>
	{};

	struct pipeable_access
	{
		tpl<typ Pipeable> struct impl : Pipeable
		{
			using Pipeable::pipe;
		};

		tpl<typ Pipeable> struct impl<Pipeable&> : impl<Pipeable>
		{};
	};

	tpl<typ Derived> struct pipeable : pipeable_base
	{
	private:
		friend pipeable_access;
		// Default Pipe behavior just passes the argument to the pipe's function
		// call operator
		tpl<typ Arg, typ Pipe> static auto
		pipe(Arg&& arg, Pipe pipe)
			DECLTYPE_AUTO_RETURN(pipe(static_cast<Arg&&>(arg)))
	};

	// Evaluate the pipe with an argument
	tpl<typ Arg,
		typ Pipe,
		CONCEPT_REQUIRES_(!is_pipeable<Arg>() && is_pipeable<Pipe>())> auto
	operator|(Arg&& arg, Pipe pipe) DECLTYPE_AUTO_RETURN(
		pipeable_access::impl<Pipe>::pipe(static_cast<Arg&&>(arg), pipe))

		// Compose two pipes
		tpl<typ Pipe0,
			typ Pipe1,
			CONCEPT_REQUIRES_(
				is_pipeable<Pipe0>() && is_pipeable<Pipe1>())> auto
		operator|(Pipe0 pipe0, Pipe1 pipe1) DECLTYPE_AUTO_RETURN(
			make_pipeable(detail::composed_pipe<Pipe0, Pipe1>{pipe0, pipe1}))

			tpl<typ T> struct bind_element : meta::if_<is_same<decay_t<T>, T>,
												 meta::id<T>,
												 bind_element<decay_t<T>>>
	{};

	tpl<typ T> struct bind_element<std::reference_wrapper<T>>
	{
		using type = T&;
	};

	tpl<typ T> struct bind_element<reference_wrapper<T>>
	{
		using type = typ reference_wrapper<T>::reference;
	};

	tpl<typ T> using bind_element_t = _t<bind_element<T>>;

	struct ref_fn : pipeable<ref_fn>
	{
		tpl<typ T, CONCEPT_REQUIRES_(!is_reference_wrapper_t<T>())>
			reference_wrapper<T>
			operator()(T& t) const
		{
			return {t};
		}
		/// \overload
		tpl<typ T> reference_wrapper<T>
		operator()(reference_wrapper<T> t) const
		{
			return t;
		}
		/// \overload
		tpl<typ T> reference_wrapper<T>
		operator()(std::reference_wrapper<T> t) const
		{
			return {t.get()};
		}
	};

	/// \ingroup group-utility
	/// \sa `ref_fn`
	INLINE_VARIABLE(ref_fn, ref)

	tpl<typ T> using ref_t = decltype(ref(declval<T>()));

	struct unwrap_reference_fn : pipeable<unwrap_reference_fn>
	{
		tpl<typ T, CONCEPT_REQUIRES_(!is_reference_wrapper<T>())> T&&
		operator()(T&& t) const noexcept
		{
			return static_cast<T&&>(t);
		}
		/// \overload
		tpl<typ T> typ reference_wrapper<T>::reference
		operator()(reference_wrapper<T> t) const noexcept
		{
			return t.get();
		}
		/// \overload
		tpl<typ T> T&
		operator()(std::reference_wrapper<T> t) const noexcept
		{
			return t.get();
		}
	};

	/// \ingroup group-utility
	/// \sa `unwrap_reference_fn`
	INLINE_VARIABLE(unwrap_reference_fn, unwrap_reference)

	tpl<typ T> using unwrap_reference_t
		= decltype(unwrap_reference(declval<T>()));

	/// \cond
	namespace detail
	{
		tpl<typ Bind> struct protect
		{
		private:
			Bind bind_;

		public:
			protect() = default;
			protect(Bind b) : bind_(move(b))
			{}
			tpl<typ... Ts> auto
			operator()(Ts&&... ts)
				DECLTYPE_AUTO_RETURN(bind_(static_cast<Ts&&>(ts)...))
				/// \overload
				tpl<typ... Ts> auto
				operator()(Ts&&... ts) const
				DECLTYPE_AUTO_RETURN(bind_(static_cast<Ts&&>(ts)...))
		};
	} // namespace detail
	/// \endcond

	struct protect_fn
	{
		tpl<typ F,
			CONCEPT_REQUIRES_(std::is_bind_expression<remove_cvref_t<F>>())>
			detail::protect<remove_cvref_t<F>>
			operator()(F&& f) const
		{
			return {static_cast<F&&>(f)};
		}
		/// \overload
		tpl<typ F,
			CONCEPT_REQUIRES_(!std::is_bind_expression<remove_cvref_t<F>>())>
			F
			operator()(F&& f) const
		{
			return static_cast<F&&>(f);
		}
	};

	/// Protect a callable so that it can be safely used in a bind expression
	/// without accidentally becoming a "nested" bind. \ingroup group-utility
	/// \sa `protect_fn`
	INLINE_VARIABLE(protect_fn, protect)

	// Accepts initializer_lists as either the first or second parameter, or
	// both, and forwards on to an implementation.
	tpl<typ ImplFn> struct with_braced_init_args : ImplFn
	{
	private:
		cexp ImplFn const&
		base() const
		{
			return *this;
		}

	public:
		using ImplFn::operator();

		tpl<typ V0, typ... Args> cexp auto
		operator()(initializer_list<V0>&& rng0, Args&&... args) const
			-> decltype(
				declval<ImplFn const&>()(move(rng0), declval<Args>()...))
		{
			return base()(move(rng0), static_cast<Args&&>(args)...);
		}
		/// \overload
		tpl<typ Rng0, typ V1, typ... Args> cexp auto
		operator()(
			Rng0&& rng0, initializer_list<V1>&& rng1, Args&&... args) const
			-> decltype(declval<ImplFn const&>()(
				declval<Rng0>(), move(rng1), declval<Args>()...))
		{
			return base()(static_cast<Rng0&&>(rng0),
				move(rng1),
				static_cast<Args&&>(args)...);
		}
		/// \overload
		tpl<typ V0, typ V1, typ... Args> cexp auto
		operator()(initializer_list<V0> rng0,
			initializer_list<V1>&& rng1,
			Args&&... args) const
			-> decltype(declval<ImplFn const&>()(
				move(rng0), move(rng1), declval<Args>()...))
		{
			return base()(move(rng0), move(rng1), static_cast<Args&&>(args)...);
		}
	};
	/// @}
} // namespace Rider::Faiz::range

#endif
