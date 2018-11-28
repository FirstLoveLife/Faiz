#ifndef FAIZ
#define FAIZ
#include "rider/faiz/cstddef.hpp"
#include "rider/faiz/faiz_fwd.hpp"
#include "rider/faiz/initializer_list.hpp"
#include "rider/faiz/integer_sequence.hpp"
#include <iostream>

namespace rider::faiz
{
	template<typename...>
	struct empty_base
	{};

	template<class... Args>
	void
	print(Args... args)
	{
		std::string sep = " ";
		((std::cout << sep << args), ...);
	}

	template<typename... List>
	inline constexpr unsigned int
	sum(const List&... list)
	{
		return (0 + ... + sizeof(List));
	}

	template<bool _bCond>
	struct when;

	template<typename _type>
	struct always
	{
	private:
		template<typename...>
		struct impl
		{
			using type = _type;
		};

	public:
		template<typename... _types>
		using apply = impl<_types...>;
	};

	template<typename _type, typename... _types>
	using well_formed_t =
		typename always<_type>::template apply<_types...>::type;

	template<typename... _types>
	using when_valid = well_formed_t<when<true>, _types...>;


} // namespace rider::faiz


namespace rider::faiz::meta
{

	namespace detail
	{
		template<typename T>
		constexpr T*
		_nullptr_v()
		{
			return nullptr;
		}
	} // namespace detail

	struct nil_
	{};

	namespace lazy
	{
		template<typename T>
		using _t = defer<_t, T>;
	} // namespace lazy

	template<typename T>
	using inc
		= integral_constant<decltype(T::type::value + 1), T::type::value + 1>;

	template<typename T>
	using dec
		= integral_constant<decltype(T::type::value - 1), T::type::value - 1>;

	template<typename T, typename U>
	using plus = integral_constant<decltype(T::type::value + U::type::value),
		T::type::value + U::type::value>;

	template<typename T, typename U>
	using minus = integral_constant<decltype(T::type::value - U::type::value),
		T::type::value - U::type::value>;

	template<typename T, typename U>
	using multiplies
		= integral_constant<decltype(T::type::value * U::type::value),
			T::type::value * U::type::value>;

	template<typename T, typename U>
	using divides = integral_constant<decltype(T::type::value / U::type::value),
		T::type::value / U::type::value>;

	template<typename T>
	using negate
		= integral_constant<decltype(-T::type::value), -T::type::value>;

	template<typename T, typename U>
	using modulus = integral_constant<decltype(T::type::value % U::type::value),
		T::type::value % U::type::value>;

	template<typename T, typename U>
	using equal_to = bool_<T::type::value == U::type::value>;

	template<typename T, typename U>
	using not_equal_to = bool_<T::type::value != U::type::value>;

	template<typename T, typename U>
	using greater = bool_<(T::type::value > U::type::value)>;

	template<typename T, typename U>
	using less = bool_<(T::type::value < U::type::value)>;

	template<typename T, typename U>
	using greater_equal = bool_<(T::type::value >= U::type::value)>;

	template<typename T, typename U>
	using less_equal = bool_<(T::type::value <= U::type::value)>;

	template<typename T, typename U>
	using bit_and = integral_constant<decltype(T::type::value & U::type::value),
		T::type::value & U::type::value>;

	template<typename T, typename U>
	using bit_or = integral_constant<decltype(T::type::value | U::type::value),
		T::type::value | U::type::value>;

	template<typename T, typename U>
	using bit_xor = integral_constant<decltype(T::type::value ^ U::type::value),
		T::type::value ^ U::type::value>;

	template<typename T>
	using bit_not
		= integral_constant<decltype(~T::type::value), ~T::type::value>;

	namespace lazy
	{
		template<typename T>
		using inc = defer<inc, T>;

		template<typename T>
		using dec = defer<dec, T>;

		template<typename T, typename U>
		using plus = defer<plus, T, U>;

		template<typename T, typename U>
		using minus = defer<minus, T, U>;

		template<typename T, typename U>
		using multiplies = defer<multiplies, T, U>;

		template<typename T, typename U>
		using divides = defer<divides, T, U>;

		template<typename T>
		using negate = defer<negate, T>;

		template<typename T, typename U>
		using modulus = defer<modulus, T, U>;

		template<typename T, typename U>
		using equal_to = defer<equal_to, T, U>;

		template<typename T, typename U>
		using not_equal_to = defer<not_equal_to, T, U>;

		template<typename T, typename U>
		using greater = defer<greater, T, U>;

		template<typename T, typename U>
		using less = defer<less, T, U>;

		template<typename T, typename U>
		using greater_equal = defer<greater_equal, T, U>;

		template<typename T, typename U>
		using less_equal = defer<less_equal, T, U>;

		template<typename T, typename U>
		using bit_and = defer<bit_and, T, U>;

		template<typename T, typename U>
		using bit_or = defer<bit_or, T, U>;

		template<typename T, typename U>
		using bit_xor = defer<bit_xor, T, U>;

		template<typename T>
		using bit_not = defer<bit_not, T>;
	} // namespace lazy


	template<typename F, typename... Args>
	using invoke = typename F::template invoke<Args...>;

	namespace lazy
	{
		template<typename F, typename... Args>
		using invoke = defer<invoke, F, Args...>;
	} // namespace lazy

	template<typename T>
	struct id
	{
		template<typename...>
		using invoke = T;
		using type = T;
	};

	template<typename T>
	using id_t = _t<id<T>>;

	namespace lazy
	{
		template<typename T>
		using id = defer<id, T>;
	} // namespace lazy

	namespace detail
	{
		template<typename, typename = void>
		struct is_trait_
		{
			using type = false_;
		};

		template<typename T>
		struct is_trait_<T, void_t<typename T::type>>
		{
			using type = true_;
		};

		template<typename, typename = void>
		struct is_callable_
		{
			using type = false_;
		};

		template<typename T>
		struct is_callable_<T, void_t<quote<T::template invoke>>>
		{
			using type = true_;
		};

		template<template<typename...> class C,
			typename... Ts,
			template<typename...> class D = C>
		id<D<Ts...>>
		try_defer_(int);
		template<template<typename...> class C, typename... Ts>
		nil_
		try_defer_(long);

		template<template<typename...> class C, typename... Ts>
		using defer_ = decltype(detail::try_defer_<C, Ts...>(0));

		template<typename T,
			template<T...>
			class C,
			T... Is,
			template<T...> class D = C>
		id<D<Is...>>
		try_defer_i_(int);
		template<typename T, template<T...> class C, T... Is>
		nil_
		try_defer_i_(long);

		template<typename T, template<T...> class C, T... Is>
		using defer_i_ = decltype(detail::try_defer_i_<T, C, Is...>(0));

		template<typename T>
		using _t_t = _t<_t<T>>;
	} // namespace detail

	template<typename T>
	using is_trait = _t<detail::is_trait_<T>>;

	template<typename T>
	using is_callable = _t<detail::is_callable_<T>>;

	template<template<typename...> class C, typename... Ts>
	struct defer : detail::defer_<C, Ts...>
	{};

	template<typename T, template<T...> class C, T... Is>
	struct defer_i : detail::defer_i_<T, C, Is...>
	{};

	template<template<typename...> class C, typename... Ts>
	using defer_trait = defer<detail::_t_t, detail::defer_<C, Ts...>>;

	template<class T>
	using sizeof_ = size_t_<sizeof(T)>;

	template<class T>
	using alignof_ = size_t_<alignof(T)>;

	namespace lazy
	{
		template<typename T>
		using sizeof_ = defer<sizeof_, T>;

		template<typename T>
		using alignof_ = defer<alignof_, T>;
	} // namespace lazy


	namespace detail
	{
		template<typename, template<typename...> class>
		struct is_ : false_
		{};

		template<typename... Ts, template<typename...> class C>
		struct is_<C<Ts...>, C> : true_
		{};
	} // namespace detail


	template<typename T, template<typename...> class C>
	using is = _t<detail::is_<T, C>>;


	template<typename... Fs>
	struct compose
	{};

	template<typename F0>
	struct compose<F0>
	{
		template<typename... Ts>
		using invoke = invoke<F0, Ts...>;
	};

	template<typename F0, typename... Fs>
	struct compose<F0, Fs...>
	{
		template<typename... Ts>
		using invoke = invoke<F0, invoke<compose<Fs...>, Ts...>>;
	};

	namespace lazy
	{
		template<typename... Fns>
		using compose = defer<compose, Fns...>;
	} // namespace lazy

	template<template<typename...> class C>
	struct quote
	{
		// Indirection through defer here needed to avoid Core issue 1430
		// http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
		template<typename... Ts>
		using invoke = _t<detail::defer_<C, Ts...>>;
	};

	template<typename T, template<T...> class C>
	struct quote_i
	{
		// Indirection through defer_i here needed to avoid Core issue 1430
		// http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
		template<typename... Ts>
		using invoke = _t<detail::defer_i_<T, C, Ts::type::value...>>;
	};

	template<template<typename...> class C>
	using quote_trait = compose<quote<_t>, quote<C>>;

	template<typename T, template<T...> class C>
	using quote_trait_i = compose<quote<_t>, quote_i<T, C>>;

	template<typename F, typename... Ts>
	struct bind_front
	{
		template<typename... Us>
		using invoke = invoke<F, Ts..., Us...>;
	};

	template<typename F, typename... Us>
	struct bind_back
	{
		template<typename... Ts>
		using invoke = invoke<F, Ts..., Us...>;
	};

	namespace lazy
	{
		template<typename Fn, typename... Ts>
		using bind_front = defer<bind_front, Fn, Ts...>;

		template<typename Fn, typename... Ts>
		using bind_back = defer<bind_back, Fn, Ts...>;
	} // namespace lazy

	namespace extension
	{
		template<typename F, typename List>
		struct apply
		{};

		template<typename F, typename Ret, typename... Args>
		struct apply<F, Ret(Args...)> : lazy::invoke<F, Ret, Args...>
		{};

		template<typename F, template<typename...> class T, typename... Ts>
		struct apply<F, T<Ts...>> : lazy::invoke<F, Ts...>
		{};

		template<typename F, typename T, T... Is>
		struct apply<F, integer_sequence<T, Is...>>
			: lazy::invoke<F, integral_constant<T, Is>...>
		{};
	} // namespace extension

	template<typename C, typename List>
	using apply = _t<extension::apply<C, List>>;

	namespace lazy
	{
		template<typename F, typename List>
		using apply = defer<apply, F, List>;
	}

	template<typename F, typename Q = quote<list>>
	using curry = compose<F, Q>;

	template<typename F>
	using uncurry = bind_front<quote<apply>, F>;

	namespace lazy
	{
		template<typename F, typename Q = quote<list>>
		using curry = defer<curry, F, Q>;

		template<typename F>
		using uncurry = defer<uncurry, F>;
	} // namespace lazy

	template<typename F>
	struct flip
	{
	private:
		template<typename... Ts>
		struct impl
		{};
		template<typename A, typename B, typename... Ts>
		struct impl<A, B, Ts...> : lazy::invoke<F, B, A, Ts...>
		{};

	public:
		template<typename... Ts>
		using invoke = _t<impl<Ts...>>;
	};

	namespace lazy
	{
		template<typename F>
		using flip = defer<flip, F>;
	} // namespace lazy

	namespace detail
	{
		template<typename...>
		struct on_
		{};
		template<typename F, typename... Gs>
		struct on_<F, Gs...>
		{
			template<typename... Ts>
			using invoke = invoke<F, invoke<compose<Gs...>, Ts>...>;
		};
	} // namespace detail

	template<typename... Fs>
	using on = detail::on_<Fs...>;

	namespace lazy
	{
		template<typename F, typename G>
		using on = defer<on, F, G>;
	} // namespace lazy

	namespace detail
	{
		template<typename, typename = bool>
		struct _if_
		{};

		template<typename If>
		struct _if_<list<If>, decltype(bool(If::type::value))>
			: enable_if<If::type::value>
		{};

		template<typename If, typename Then>
		struct _if_<list<If, Then>, decltype(bool(If::type::value))>
			: enable_if<If::type::value, Then>
		{};

		template<typename If, typename Then, typename Else>
		struct _if_<list<If, Then, Else>, decltype(bool(If::type::value))>
			: conditional<If::type::value, Then, Else>
		{};
	} // namespace detail

	template<typename... Args>
	using if_ = _t<detail::_if_<list<Args...>>>;

	template<bool If, typename... Args>
	using if_c = _t<detail::_if_<list<bool_<If>, Args...>>>;

	namespace lazy
	{
		template<typename... Args>
		using if_ = defer<if_, Args...>;

		template<bool If, typename... Args>
		using if_c = if_<bool_<If>, Args...>;
	} // namespace lazy

	namespace detail
	{
		template<bool>
		struct _and_
		{
			template<class...>
			using invoke = true_;
		};

		template<>
		struct _and_<false>
		{
			template<typename Bool, typename... Bools>
			using invoke = invoke<if_c<!Bool::type::value,
									  id<false_>,
									  _and_<0 == sizeof...(Bools)>>,
				Bools...>;
		};

		template<bool>
		struct _or_
		{
			template<class = void>
			using invoke = false_;
		};

		template<>
		struct _or_<false>
		{
			template<typename Bool_, typename... Bools>
			using invoke = invoke<
				if_c<Bool_::type::value, id<true_>, _or_<0 == sizeof...(Bools)>>,
				Bools...>;
		};
	} // namespace detail

	template<bool Bool_>
	using not_c = bool_<!Bool_>;

	template<typename Bool_>
	using not_ = not_c<Bool_::type::value>;

	template<bool... Bools>
	using and_c = is_same<integer_sequence<bool, Bools...>,
		integer_sequence<bool, (Bools || true)...>>;

	template<typename... Bools>
	using strict_and = and_c<Bools::type::value...>;

	template<typename... Bools>
	using and_ = _t<
		defer<detail::_and_<0 == sizeof...(Bools)>::template invoke, Bools...>>;


	template<bool... Bools>
	using or_c = not_<is_same<integer_sequence<bool, Bools...>,
		integer_sequence<bool, (Bools && false)...>>>;

	template<typename... Bools>
	using strict_or = or_c<Bools::type::value...>;

	template<typename... Bools>
	using or_ = _t<
		defer<detail::_or_<0 == sizeof...(Bools)>::template invoke, Bools...>>;

	namespace lazy
	{
		template<typename... Bools>
		using and_ = defer<and_, Bools...>;

		template<typename... Bools>
		using or_ = defer<or_, Bools...>;

		template<typename Bool_>
		using not_ = defer<not_, Bool_>;

		template<typename... Bools>
		using strict_and = defer<strict_and, Bools...>;

		template<typename... Bools>
		using strict_or = defer<strict_or, Bools...>;
	} // namespace lazy

	namespace detail
	{
		template<typename Fun, typename T0>
		struct compose1_
		{
			template<typename X>
			using invoke = invoke<Fun, _t<X>, T0>;
		};

		template<typename Fun,
			typename T0,
			typename T1,
			typename T2,
			typename T3,
			typename T4,
			typename T5,
			typename T6,
			typename T7,
			typename T8,
			typename T9>
		struct compose10_
		{
			template<typename X, typename Y>
			using F = invoke<Fun, X, Y>;

			template<typename S>
			using invoke = F<
				F<F<F<F<F<F<F<F<F<_t<S>, T0>, T1>, T2>, T3>, T4>, T5>, T6>, T7>,
					T8>,
				T9>;
		};

		template<typename, typename, typename>
		struct fold_
		{};

		template<typename State, typename Fun>
		struct fold_<list<>, State, Fun> : State
		{};

		template<typename Head, typename... List, typename State, typename Fun>
		struct fold_<list<Head, List...>, State, Fun>
			: fold_<list<List...>,
				  lazy::invoke<compose1_<Fun, Head>, State>,
				  Fun>
		{};

		template<typename T0,
			typename T1,
			typename T2,
			typename T3,
			typename T4,
			typename T5,
			typename T6,
			typename T7,
			typename T8,
			typename T9,
			typename... List,
			typename State,
			typename Fun>
		struct fold_<list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, List...>,
			State,
			Fun>
			: fold_<list<List...>,
				  lazy::invoke<
					  compose10_<Fun, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>,
					  State>,
				  Fun>
		{};
	} // namespace detail

	template<typename List, typename State, typename Fun>
	using fold = _t<detail::fold_<List, id<State>, Fun>>;

	template<typename List, typename State, typename Fun>
	using accumulate = fold<List, State, Fun>;

	namespace lazy
	{
		template<typename List, typename State, typename Fun>
		using fold = defer<fold, List, State, Fun>;

		template<typename List, typename State, typename Fun>
		using accumulate = defer<accumulate, List, State, Fun>;
	} // namespace lazy

	namespace detail
	{
		template<typename List, typename State, typename Fun>
		struct reverse_fold_
		{};

		template<typename State, typename Fun>
		struct reverse_fold_<list<>, State, Fun>
		{
			using type = State;
		};

		template<typename Head, typename... List, typename State, typename Fun>
		struct reverse_fold_<list<Head, List...>, State, Fun>
			: lazy::invoke<compose1_<Fun, Head>,
				  reverse_fold_<list<List...>, State, Fun>>
		{};

		template<typename T0,
			typename T1,
			typename T2,
			typename T3,
			typename T4,
			typename T5,
			typename T6,
			typename T7,
			typename T8,
			typename T9,
			typename... List,
			typename State,
			typename Fun>
		struct reverse_fold_<
			list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, List...>,
			State,
			Fun>
			: lazy::invoke<
				  compose10_<Fun, T9, T8, T7, T6, T5, T4, T3, T2, T1, T0>,
				  reverse_fold_<list<List...>, State, Fun>>
		{};
	} // namespace detail

	template<typename List, typename State, typename Fun>
	using reverse_fold = _t<detail::reverse_fold_<List, State, Fun>>;

	namespace lazy
	{
		template<typename List, typename State, typename Fun>
		using reverse_fold = defer<reverse_fold, List, State, Fun>;
	} // namespace lazy

	using npos = size_t_<faiz::size_t(-1)>;

	template<typename... Ts>
	struct list
	{
		using type = list;
		/// \return `sizeof...(Ts)`
		static constexpr faiz::size_t
		size() noexcept
		{
			return sizeof...(Ts);
		}
	};

	template<typename List>
	using size = size_t_<List::size()>;

	namespace lazy
	{
		template<typename List>
		using size = defer<size, List>;
	} // namespace lazy

	namespace detail
	{
		template<typename... Lists>
		struct concat_
		{};

		template<>
		struct concat_<>
		{
			using type = list<>;
		};

		template<typename... List1>
		struct concat_<list<List1...>>
		{
			using type = list<List1...>;
		};

		template<typename... List1, typename... List2, typename... Rest>
		struct concat_<list<List1...>, list<List2...>, Rest...>
			: concat_<list<List1..., List2...>, Rest...>
		{};

		template<typename... List1,
			typename... List2,
			typename... List3,
			typename... List4,
			typename... List5,
			typename... List6,
			typename... List7,
			typename... List8,
			typename... List9,
			typename... List10,
			typename... Rest>
		struct concat_<list<List1...>,
			list<List2...>,
			list<List3...>,
			list<List4...>,
			list<List5...>,
			list<List6...>,
			list<List7...>,
			list<List8...>,
			list<List9...>,
			list<List10...>,
			Rest...> : concat_<list<List1...,
								   List2...,
								   List3...,
								   List4...,
								   List5...,
								   List6...,
								   List7...,
								   List8...,
								   List9...,
								   List10...>,
						   Rest...>
		{};
	} // namespace detail

	template<typename... Lists>
	using concat = _t<detail::concat_<Lists...>>;

	namespace lazy
	{
		template<typename... Lists>
		using concat = defer<concat, Lists...>;
	} // namespace lazy

	template<typename ListOfLists>
	using join = apply<quote<concat>, ListOfLists>;

	namespace lazy
	{
		template<typename ListOfLists>
		using join = defer<join, ListOfLists>;
	} // namespace lazy

	namespace detail
	{
		template<typename, typename = void>
		struct transform_
		{};

		template<typename... Ts, typename Fun>
		struct transform_<list<list<Ts...>, Fun>, void_t<invoke<Fun, Ts>...>>
		{
			using type = list<invoke<Fun, Ts>...>;
		};

		template<typename... Ts0, typename... Ts1, typename Fun>
		struct transform_<list<list<Ts0...>, list<Ts1...>, Fun>,
			void_t<invoke<Fun, Ts0, Ts1>...>>
		{
			using type = list<invoke<Fun, Ts0, Ts1>...>;
		};
	} // namespace detail

	template<typename... Args>
	using transform = _t<detail::transform_<list<Args...>>>;

	namespace lazy
	{
		template<typename... Args>
		using transform = defer<transform, Args...>;
	} // namespace lazy

	namespace detail
	{
		template<typename T, faiz::size_t>
		using first_ = T;

		template<typename T, typename Ints>
		struct repeat_n_c_
		{};

		template<typename T, faiz::size_t... Is>
		struct repeat_n_c_<T, index_sequence<Is...>>
		{
			using type = list<first_<T, Is>...>;
		};
	} // namespace detail

	template<faiz::size_t N, typename T = void>
	using repeat_n_c = _t<detail::repeat_n_c_<T, make_index_sequence<N>>>;

	template<typename N, typename T = void>
	using repeat_n = repeat_n_c<N::type::value, T>;

	namespace lazy
	{
		template<typename N, typename T = void>
		using repeat_n = defer<repeat_n, N, T>;

		template<faiz::size_t N, typename T = void>
		using repeat_n_c = defer<repeat_n, size_t_<N>, T>;
	} // namespace lazy

	namespace detail
	{
		template<typename VoidPtrs>
		struct at_impl_;

		template<typename... VoidPtrs>
		struct at_impl_<list<VoidPtrs...>>
		{
			static nil_
			eval(...);

			template<typename T, typename... Us>
			static T
			eval(VoidPtrs..., T*, Us*...);
		};

		template<typename List, faiz::size_t N>
		struct at_
		{};

		template<typename... Ts, faiz::size_t N>
		struct at_<list<Ts...>, N>
			: decltype(at_impl_<repeat_n_c<N, void*>>::eval(
				  static_cast<id<Ts>*>(nullptr)...))
		{};
	} // namespace detail

	template<typename List, faiz::size_t N>
	using at_c = _t<detail::at_<List, N>>;

	template<typename List, typename N>
	using at = at_c<List, N::type::value>;

	namespace lazy
	{
		template<typename List, typename N>
		using at = defer<at, List, N>;
	} // namespace lazy

	namespace detail
	{
		template<typename VoidPtrs>
		struct drop_impl_
		{
			static nil_
			eval(...);
		};

		template<typename... VoidPtrs>
		struct drop_impl_<list<VoidPtrs...>>
		{
			static nil_
			eval(...);

			template<typename... Ts>
			static id<list<Ts...>>
			eval(VoidPtrs..., id<Ts>*...);
		};

		template<>
		struct drop_impl_<list<>>
		{
			template<typename... Ts>
			static id<list<Ts...>>
			eval(id<Ts>*...);
		};

		template<typename List, typename N>
		struct drop_
		{};

		template<typename... Ts, typename N>
		struct drop_<list<Ts...>, N>
			: decltype(drop_impl_<repeat_n<N, void*>>::eval(
				  detail::_nullptr_v<id<Ts>>()...))
		{};
	} // namespace detail

	template<typename List, typename N>
	using drop = _t<detail::drop_<List, N>>;

	template<typename List, faiz::size_t N>
	using drop_c = _t<detail::drop_<List, size_t_<N>>>;

	namespace lazy
	{
		template<typename List, typename N>
		using drop = defer<drop, List, N>;
	} // namespace lazy

	namespace detail
	{
		template<typename List>
		struct front_
		{};

		template<typename Head, typename... List>
		struct front_<list<Head, List...>>
		{
			using type = Head;
		};
	} // namespace detail

	template<typename List>
	using front = _t<detail::front_<List>>;

	namespace lazy
	{
		template<typename List>
		using front = defer<front, List>;
	} // namespace lazy

	namespace detail
	{
		template<typename List>
		struct back_
		{};

		template<typename Head, typename... List>
		struct back_<list<Head, List...>>
		{
			using type = at_c<list<Head, List...>, sizeof...(List)>;
		};
	} // namespace detail

	template<typename List>
	using back = _t<detail::back_<List>>;

	namespace lazy
	{
		template<typename List>
		using back = defer<back, List>;
	} // namespace lazy

	namespace detail
	{
		template<typename List, typename T>
		struct push_front_
		{};

		template<typename... List, typename T>
		struct push_front_<list<List...>, T>
		{
			using type = list<T, List...>;
		};
	} // namespace detail

	template<typename List, typename T>
	using push_front = _t<detail::push_front_<List, T>>;

	namespace lazy
	{
		template<typename List, typename T>
		using push_front = defer<push_front, List, T>;
	} // namespace lazy

	namespace detail
	{
		template<typename List>
		struct pop_front_
		{};

		template<typename Head, typename... List>
		struct pop_front_<list<Head, List...>>
		{
			using type = list<List...>;
		};
	} // namespace detail

	template<typename List>
	using pop_front = _t<detail::pop_front_<List>>;

	namespace lazy
	{
		template<typename List>
		using pop_front = defer<pop_front, List>;
	} // namespace lazy

	namespace detail
	{
		template<typename List, typename T>
		struct push_back_
		{};

		template<typename... List, typename T>
		struct push_back_<list<List...>, T>
		{
			using type = list<List..., T>;
		};
	} // namespace detail

	template<typename List, typename T>
	using push_back = _t<detail::push_back_<List, T>>;

	namespace lazy
	{
		template<typename List, typename T>
		using push_back = defer<push_back, List, T>;
	} // namespace lazy

	namespace detail
	{
		template<typename T, typename U>
		using min_ = if_<less<U, T>, U, T>;

		template<typename T, typename U>
		using max_ = if_<less<U, T>, T, U>;
	} // namespace detail

	template<typename... Ts>
	using min
		= fold<pop_front<list<Ts...>>, front<list<Ts...>>, quote<detail::min_>>;

	template<typename... Ts>
	using max
		= fold<pop_front<list<Ts...>>, front<list<Ts...>>, quote<detail::max_>>;

	namespace lazy
	{
		template<typename... Ts>
		using min = defer<min, Ts...>;

		template<typename... Ts>
		using max = defer<max, Ts...>;
	} // namespace lazy

	template<typename List>
	using empty = bool_<0 == size<List>::type::value>;

	namespace lazy
	{
		template<typename List>
		using empty = defer<empty, List>;
	} // namespace lazy

	template<typename F, typename S>
	using pair = list<F, S>;

	template<typename Pair>
	using first = front<Pair>;

	template<typename Pair>
	using second = front<pop_front<Pair>>;

	namespace lazy
	{
		template<typename Pair>
		using first = defer<first, Pair>;

		template<typename Pair>
		using second = defer<second, Pair>;
	} // namespace lazy

	namespace detail
	{
		constexpr faiz::size_t
		find_index_i_(
			bool const* const first, bool const* const last, faiz::size_t N = 0)
		{
			return first == last ?
				npos::value :
				*first ? N : find_index_i_(first + 1, last, N + 1);
		}

		template<typename List, typename T>
		struct find_index_
		{};

		template<typename V>
		struct find_index_<list<>, V>
		{
			using type = npos;
		};

		template<typename... T, typename V>
		struct find_index_<list<T...>, V>
		{
			static constexpr bool s_v[] = {is_same<T, V>::value...};
			using type = size_t_<find_index_i_(s_v, s_v + sizeof...(T))>;
		};
	} // namespace detail

	template<typename List, typename T>
	using find_index = _t<detail::find_index_<List, T>>;

	namespace lazy
	{
		template<typename List, typename T>
		using find_index = defer<find_index, List, T>;
	} // namespace lazy

	namespace detail
	{
		constexpr faiz::size_t
		reverse_find_index_i_(
			bool const* const first, bool const* const last, faiz::size_t N)
		{
			return first == last ?
				npos::value :
				*(last - 1) ? N - 1 :
							  reverse_find_index_i_(first, last - 1, N - 1);
		}

		template<typename List, typename T>
		struct reverse_find_index_
		{};

		template<typename V>
		struct reverse_find_index_<list<>, V>
		{
			using type = npos;
		};

		template<typename... T, typename V>
		struct reverse_find_index_<list<T...>, V>
		{
			static constexpr bool s_v[] = {is_same<T, V>::value...};
			using type = size_t_<reverse_find_index_i_(
				s_v, s_v + sizeof...(T), sizeof...(T))>;
		};
	} // namespace detail

	template<typename List, typename T>
	using reverse_find_index = _t<detail::reverse_find_index_<List, T>>;

	namespace lazy
	{
		template<typename List, typename T>
		using reverse_find_index = defer<reverse_find_index, List, T>;
	} // namespace lazy

	template<typename List, typename T>
	using find = drop<List, min<find_index<List, T>, size<List>>>;

	namespace lazy
	{
		template<typename List, typename T>
		using find = defer<find, List, T>;
	} // namespace lazy

	template<typename List, typename T>
	using reverse_find
		= drop<List, min<reverse_find_index<List, T>, size<List>>>;

	namespace lazy
	{
		template<typename List, typename T>
		using reverse_find = defer<reverse_find, List, T>;
	} // namespace lazy

	namespace detail
	{
		constexpr bool const*
		find_if_i_(bool const* const begin, bool const* const end)
		{
			return begin == end || *begin ? begin : find_if_i_(begin + 1, end);
		}

		template<typename List, typename Fun, typename = void>
		struct find_if_
		{};

		template<typename Fun>
		struct find_if_<list<>, Fun>
		{
			using type = list<>;
		};

		template<typename... List, typename Fun>
		struct find_if_<list<List...>,
			Fun,
			void_t<integer_sequence<bool,
				bool(invoke<Fun, List>::type::value)...>>>
		{
			static constexpr bool s_v[] = {invoke<Fun, List>::type::value...};
			using type = drop_c<list<List...>,
				detail::find_if_i_(s_v, s_v + sizeof...(List)) - s_v>;
		};
	} // namespace detail

	template<typename List, typename Fun>
	using find_if = _t<detail::find_if_<List, Fun>>;

	namespace lazy
	{
		template<typename List, typename Fun>
		using find_if = defer<find_if, List, Fun>;
	} // namespace lazy

	namespace detail
	{
		constexpr bool const*
		reverse_find_if_i_(bool const* const begin,
			bool const* const pos,
			bool const* const end)
		{
			return begin == pos ?
				end :
				*(pos - 1) ? pos - 1 : reverse_find_if_i_(begin, pos - 1, end);
		}

		template<typename List, typename Fun, typename = void>
		struct reverse_find_if_
		{};

		template<typename Fun>
		struct reverse_find_if_<list<>, Fun>
		{
			using type = list<>;
		};

		template<typename... List, typename Fun>
		struct reverse_find_if_<list<List...>,
			Fun,
			void_t<integer_sequence<bool,
				bool(invoke<Fun, List>::type::value)...>>>
		{
			static constexpr bool s_v[] = {invoke<Fun, List>::type::value...};
			using type = drop_c<list<List...>,
				detail::reverse_find_if_i_(
					s_v, s_v + sizeof...(List), s_v + sizeof...(List))
					- s_v>;
		};
	} // namespace detail

	template<typename List, typename Fun>
	using reverse_find_if = _t<detail::reverse_find_if_<List, Fun>>;

	namespace lazy
	{
		template<typename List, typename Fun>
		using reverse_find_if = defer<reverse_find_if, List, Fun>;
	} // namespace lazy

	namespace detail
	{
		template<typename List, typename T, typename U>
		struct replace_
		{};

		template<typename... List, typename T, typename U>
		struct replace_<list<List...>, T, U>
		{
			using type = list<if_<is_same<T, List>, U, List>...>;
		};
	} // namespace detail

	template<typename List, typename T, typename U>
	using replace = _t<detail::replace_<List, T, U>>;

	namespace lazy
	{
		template<typename List, typename T, typename U>
		using replace = defer<replace, T, U>;
	} // namespace lazy

	namespace detail
	{
		template<typename List, typename C, typename U, typename = void>
		struct replace_if_
		{};

		template<typename... List, typename C, typename U>
		struct replace_if_<list<List...>,
			C,
			U,
			void_t<
				integer_sequence<bool, bool(invoke<C, List>::type::value)...>>>
		{
			using type = list<if_<invoke<C, List>, U, List>...>;
		};
	} // namespace detail

	template<typename List, typename C, typename U>
	using replace_if = _t<detail::replace_if_<List, C, U>>;

	namespace lazy
	{
		template<typename List, typename C, typename U>
		using replace_if = defer<replace_if, C, U>;
	} // namespace lazy

	namespace detail
	{
		constexpr faiz::size_t
		count_i_(bool const* const begin, bool const* const end, faiz::size_t n)
		{
			return begin == end ? n :
								  detail::count_i_(begin + 1, end, n + *begin);
		}

		template<typename List, typename T, typename = void>
		struct count_
		{};

		template<typename T>
		struct count_<list<>, T>
		{
			using type = size_t_<0>;
		};

		template<typename... List, typename T>
		struct count_<list<List...>, T>
		{
			static constexpr bool s_v[] = {is_same<T, List>::value...};
			using type
				= size_t_<detail::count_i_(s_v, s_v + sizeof...(List), 0u)>;
		};
	} // namespace detail

	template<typename List, typename T>
	using count = _t<detail::count_<List, T>>;

	namespace lazy
	{
		template<typename List, typename T>
		using count = defer<count, List, T>;
	} // namespace lazy

	namespace detail
	{
		template<typename List, typename Fn, typename = void>
		struct count_if_
		{};

		template<typename Fn>
		struct count_if_<list<>, Fn>
		{
			using type = size_t_<0>;
		};

		template<typename... List, typename Fn>
		struct count_if_<list<List...>,
			Fn,
			void_t<
				integer_sequence<bool, bool(invoke<Fn, List>::type::value)...>>>
		{
			static constexpr bool s_v[] = {invoke<Fn, List>::type::value...};
			using type
				= size_t_<detail::count_i_(s_v, s_v + sizeof...(List), 0u)>;
		};
	} // namespace detail

	template<typename List, typename Fn>
	using count_if = _t<detail::count_if_<List, Fn>>;

	namespace lazy
	{
		template<typename List, typename Fn>
		using count_if = defer<count_if, List, Fn>;
	} // namespace lazy

	namespace detail
	{
		template<typename Pred>
		struct filter_
		{
			template<typename A>
			using invoke = if_c<invoke<Pred, A>::type::value, list<A>, list<>>;
		};
	} // namespace detail

	template<typename List, typename Pred>
	using filter = join<transform<List, detail::filter_<Pred>>>;

	namespace lazy
	{
		template<typename List, typename Pred>
		using filter = defer<filter, List, Pred>;
	} // namespace lazy

	namespace detail
	{
		template<typename T>
		struct static_const
		{
			static constexpr T value{};
		};

		// Avoid potential ODR violations with global objects:
		template<typename T>
		constexpr T static_const<T>::value;
	} // namespace detail

	namespace detail
	{
		struct for_each_fn
		{
			template<class UnaryFunction, class... Args>
			constexpr auto
			operator()(list<Args...>, UnaryFunction f) const -> UnaryFunction
			{
				return (void)faiz::initializer_list<int>{
						   ((void)f(Args{}), 0)...},
					   f;
			}
		};
	} // namespace detail

	namespace
	{
		constexpr auto&& for_each
			= detail::static_const<detail::for_each_fn>::value;
	} // namespace

	template<typename ListOfLists>
	using transpose = fold<ListOfLists,
		repeat_n<size<front<ListOfLists>>, list<>>,
		bind_back<quote<transform>, quote<push_back>>>;

	namespace lazy
	{
		template<typename ListOfLists>
		using transpose = defer<transpose, ListOfLists>;
	} // namespace lazy

	template<typename Fun, typename ListOfLists>
	using zip_with = transform<transpose<ListOfLists>, uncurry<Fun>>;

	namespace lazy
	{
		template<typename Fun, typename ListOfLists>
		using zip_with = defer<zip_with, Fun, ListOfLists>;
	} // namespace lazy

	template<typename ListOfLists>
	using zip = transpose<ListOfLists>;

	namespace lazy
	{
		template<typename ListOfLists>
		using zip = defer<zip, ListOfLists>;
	} // namespace lazy

	namespace detail
	{
		// Indirection here needed to avoid Core issue 1430
		// http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
		template<typename Sequence>
		struct as_list_ : lazy::invoke<uncurry<quote<list>>, Sequence>
		{};
	} // namespace detail

	template<typename Sequence>
	using as_list = _t<detail::as_list_<remove_cvref_t<Sequence>>>;

	namespace lazy
	{
		template<typename Sequence>
		using as_list = defer<as_list, Sequence>;
	} // namespace lazy

	namespace detail
	{
		template<typename List, typename State = list<>>
		struct reverse_ : lazy::fold<List, State, quote<push_front>>
		{};

		template<typename T0,
			typename T1,
			typename T2,
			typename T3,
			typename T4,
			typename T5,
			typename T6,
			typename T7,
			typename T8,
			typename T9,
			typename... Ts,
			typename... Us>
		struct reverse_<list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, Ts...>,
			list<Us...>>
			: reverse_<list<Ts...>,
				  list<T9, T8, T7, T6, T5, T4, T3, T2, T1, T0, Us...>>
		{};
	} // namespace detail

	template<typename List>
	using reverse = _t<detail::reverse_<List>>;

	namespace lazy
	{
		template<typename List>
		using reverse = defer<reverse, List>;
	} // namespace lazy

	template<typename F>
	using not_fn = compose<quote<not_>, F>;

	namespace lazy
	{
		template<typename F>
		using not_fn = defer<not_fn, F>;
	} // namespace lazy

	template<typename List, typename F>
	using all_of = empty<find_if<List, not_fn<F>>>;

	namespace lazy
	{
		/// \sa 'meta::all_of'
		/// \ingroup lazy_query
		template<typename List, typename Fn>
		using all_of = defer<all_of, List, Fn>;
	} // namespace lazy

	template<typename List, typename F>
	using any_of = not_<empty<find_if<List, F>>>;

	namespace lazy
	{
		template<typename List, typename Fn>
		using any_of = defer<any_of, List, Fn>;
	} // namespace lazy

	template<typename List, typename F>
	using none_of = empty<find_if<List, F>>;

	namespace lazy
	{
		template<typename List, typename Fn>
		using none_of = defer<none_of, List, Fn>;
	} // namespace lazy

	template<typename List, typename T>
	using in = not_<empty<find<List, T>>>;

	namespace lazy
	{
		template<typename List, typename T>
		using in = defer<in, List, T>;
	} // namespace lazy

	namespace detail
	{
		template<typename List>
		struct inherit_
		{};

		template<typename... List>
		struct inherit_<list<List...>> : List...
		{
			using type = inherit_;
		};
	} // namespace detail

	template<typename List>
	using inherit = _t<detail::inherit_<List>>;

	namespace lazy
	{
		template<typename List>
		using inherit = defer<inherit, List>;
	} // namespace lazy

	namespace detail
	{
		template<typename Set, typename T>
		struct in_
		{};

		template<typename... Set, typename T>
		struct in_<list<Set...>, T>
			: faiz::is_base_of<id<T>, inherit<list<id<Set>...>>>
		{};

		template<typename Set, typename T>
		struct insert_back_
		{};

		template<typename... Set, typename T>
		struct insert_back_<list<Set...>, T>
		{
			using type
				= if_<in_<list<Set...>, T>, list<Set...>, list<Set..., T>>;
		};
	} // namespace detail

	template<typename List>
	using unique = fold<List, list<>, quote_trait<detail::insert_back_>>;

	namespace lazy
	{
		template<typename List>
		using unique = defer<unique, List>;
	} // namespace lazy

	namespace detail
	{
		template<typename Pred>
		struct partition_
		{
			template<typename, typename, typename = void>
			struct impl
			{};
			template<typename... Yes, typename... No, typename A>
			struct impl<pair<list<Yes...>, list<No...>>,
				A,
				void_t<bool_<invoke<Pred, A>::type::value>>>
			{
				using type = if_<invoke<Pred, A>,
					pair<list<Yes..., A>, list<No...>>,
					pair<list<Yes...>, list<No..., A>>>;
			};
			template<typename State, typename A>
			using invoke = _t<impl<State, A>>;
		};
	} // namespace detail

	template<typename List, typename Pred>
	using partition
		= fold<List, pair<list<>, list<>>, detail::partition_<Pred>>;

	namespace lazy
	{
		template<typename List, typename Pred>
		using partition = defer<partition, List, Pred>;
	} // namespace lazy

	namespace detail
	{
		template<typename, typename, typename = void>
		struct sort_
		{};

		template<typename Pred>
		struct sort_<list<>, Pred>
		{
			using type = list<>;
		};

		template<typename A, typename Pred>
		struct sort_<list<A>, Pred>
		{
			using type = list<A>;
		};

		template<typename A, typename B, typename... List, typename Pred>
		struct sort_<list<A, B, List...>,
			Pred,
			void_t<
				_t<sort_<first<partition<list<B, List...>, bind_back<Pred, A>>>,
					Pred>>>>
		{
			using P = partition<list<B, List...>, bind_back<Pred, A>>;
			using type = concat<_t<sort_<first<P>, Pred>>,
				list<A>,
				_t<sort_<second<P>, Pred>>>;
		};
	} // namespace detail

	template<typename List, typename Pred>
	using sort = _t<detail::sort_<List, Pred>>;

	namespace lazy
	{
		template<typename List, typename Pred>
		using sort = defer<sort, List, Pred>;
	} // namespace lazy

	namespace detail
	{
		template<typename T, int = 0>
		struct protect_;

		template<typename, int = 0>
		struct vararg_;

		template<typename T, int = 0>
		struct is_valid_;

		// Returns which branch to evaluate
		template<typename If, typename... Ts>
		using lazy_if_ = lazy::_t<defer<_if_, list<If, protect_<Ts>...>>>;

		template<typename A, typename T, typename F, typename Ts>
		struct subst1_
		{
			using type = list<list<T>>;
		};
		template<typename T, typename F, typename Ts>
		struct subst1_<F, T, F, Ts>
		{
			using type = list<>;
		};
		template<typename A, typename T, typename F, typename Ts>
		struct subst1_<vararg_<A>, T, F, Ts>
		{
			using type = list<Ts>;
		};

		template<typename As, typename Ts>
		using substitutions_ = push_back<
			join<transform<
				concat<As, repeat_n_c<size<Ts>{} + 2 - size<As>{}, back<As>>>,
				concat<Ts, repeat_n_c<2, back<As>>>,
				bind_back<quote_trait<subst1_>,
					back<As>,
					drop_c<Ts, size<As>{} - 2>>>>,
			list<back<As>>>;

		template<typename As, typename Ts>
		using substitutions
			= invoke<if_c<(size<Ts>{} + 2 >= size<As>{}), quote<substitutions_>>,
				As,
				Ts>;

		template<typename T>
		struct is_vararg_ : false_
		{};
		template<typename T>
		struct is_vararg_<vararg_<T>> : true_
		{};

		template<typename Tags>
		using is_variadic_
			= is_vararg_<at<push_front<Tags, void>, dec<size<Tags>>>>;

		template<typename Tags, bool IsVariadic = is_variadic_<Tags>::value>
		struct lambda_;

		// Non-variadic lambda implementation
		template<typename... As>
		struct lambda_<list<As...>, false>
		{
		private:
			static constexpr faiz::size_t arity = sizeof...(As) - 1;
			using Tags
				= list<As...>; // Includes the lambda body as the last arg!
			using F = back<Tags>;
			template<typename T, typename Args>
			struct impl;
			template<typename T, typename Args>
			using lazy_impl_ = lazy::_t<defer<impl, T, protect_<Args>>>;
			template<typename, typename, typename = void>
			struct subst_
			{};
			template<template<typename...> class C,
				typename... Ts,
				typename Args>
			struct subst_<defer<C, Ts...>,
				Args,
				void_t<C<_t<impl<Ts, Args>>...>>>
			{
				using type = C<_t<impl<Ts, Args>>...>;
			};
			template<typename T, template<T...> class C, T... Is, typename Args>
			struct subst_<defer_i<T, C, Is...>, Args, void_t<C<Is...>>>
			{
				using type = C<Is...>;
			};
			template<typename T, typename Args>
			struct impl : if_c<(reverse_find_index<Tags, T>() != npos()),
							  lazy::at<Args, reverse_find_index<Tags, T>>,
							  id<T>>
			{};
			template<typename T, typename Args>
			struct impl<protect_<T>, Args>
			{
				using type = T;
			};
			template<typename T, typename Args>
			struct impl<is_valid_<T>, Args>
			{
				using type = is_trait<impl<T, Args>>;
			};
			template<typename If, typename... Ts, typename Args>
			struct impl<defer<if_, If, Ts...>, Args> // Short-circuit if_
				: impl<lazy_impl_<lazy_if_<If, Ts...>, Args>, Args>
			{};
			template<typename Bool_, typename... Ts, typename Args>
			struct impl<defer<and_, Bool_, Ts...>, Args> // Short-circuit
														 // and_
				: impl<lazy_impl_<
						   lazy_if_<Bool_, lazy::and_<Ts...>, protect_<false_>>,
						   Args>,
					  Args>
			{};
			template<typename Bool_, typename... Ts, typename Args>
			struct impl<defer<or_, Bool_, Ts...>, Args> // Short-circuit or_
				: impl<lazy_impl_<
						   lazy_if_<Bool_, protect_<true_>, lazy::or_<Ts...>>,
						   Args>,
					  Args>
			{};
			template<template<typename...> class C,
				typename... Ts,
				typename Args>
			struct impl<defer<C, Ts...>, Args> : subst_<defer<C, Ts...>, Args>
			{};
			template<typename T, template<T...> class C, T... Is, typename Args>
			struct impl<defer_i<T, C, Is...>, Args>
				: subst_<defer_i<T, C, Is...>, Args>
			{};
			template<template<typename...> class C,
				typename... Ts,
				typename Args>
			struct impl<C<Ts...>, Args> : subst_<defer<C, Ts...>, Args>
			{};
			template<typename... Ts, typename Args>
			struct impl<lambda_<list<Ts...>, false>, Args>
			{
				using type
					= compose<uncurry<lambda_<list<As..., Ts...>, false>>,
						curry<bind_front<quote<concat>, Args>>>;
			};
			template<typename... Bs, typename Args>
			struct impl<lambda_<list<Bs...>, true>, Args>
			{
				using type
					= compose<typename lambda_<list<As..., Bs...>, true>::thunk,
						bind_front<quote<concat>, transform<Args, quote<list>>>,
						curry<bind_front<quote<substitutions>, list<Bs...>>>>;
			};

		public:
			template<typename... Ts>
			using invoke
				= _t<if_c<sizeof...(Ts) == arity, impl<F, list<Ts..., F>>>>;
		};

		template<typename... As>
		struct lambda_<list<As...>, true>
		{
		private:
			template<typename T, bool IsVar>
			friend struct lambda_;
			using Tags
				= list<As...>; // Includes the lambda body as the last arg!
			template<typename T, typename Args>
			struct impl;
			template<typename Args>
			using eval_impl_ = bind_back<quote_trait<impl>, Args>;
			template<typename T, typename Args>
			using lazy_impl_ = lazy::_t<defer<impl, T, protect_<Args>>>;
			template<template<typename...> class C, typename Args, typename Ts>
			using try_subst_
				= apply<quote<C>, join<transform<Ts, eval_impl_<Args>>>>;
			template<typename, typename, typename = void>
			struct subst_
			{};
			template<template<typename...> class C,
				typename... Ts,
				typename Args>
			struct subst_<defer<C, Ts...>,
				Args,
				void_t<try_subst_<C, Args, list<Ts...>>>>
			{
				using type = list<try_subst_<C, Args, list<Ts...>>>;
			};
			template<typename T, template<T...> class C, T... Is, typename Args>
			struct subst_<defer_i<T, C, Is...>, Args, void_t<C<Is...>>>
			{
				using type = list<C<Is...>>;
			};
			template<typename T, typename Args>
			struct impl : if_c<(reverse_find_index<Tags, T>() != npos()),
							  lazy::at<Args, reverse_find_index<Tags, T>>,
							  id<list<T>>>
			{};
			template<typename T, typename Args>
			struct impl<protect_<T>, Args>
			{
				using type = list<T>;
			};
			template<typename T, typename Args>
			struct impl<is_valid_<T>, Args>
			{
				using type = list<is_trait<impl<T, Args>>>;
			};
			template<typename If, typename... Ts, typename Args>
			struct impl<defer<if_, If, Ts...>, Args> // Short-circuit if_
				: impl<lazy_impl_<lazy_if_<If, Ts...>, Args>, Args>
			{};
			template<typename Bool_, typename... Ts, typename Args>
			struct impl<defer<and_, Bool_, Ts...>, Args> // Short-circuit
														 // and_
				: impl<lazy_impl_<
						   lazy_if_<Bool_, lazy::and_<Ts...>, protect_<false_>>,
						   Args>,
					  Args>
			{};
			template<typename Bool_, typename... Ts, typename Args>
			struct impl<defer<or_, Bool_, Ts...>, Args> // Short-circuit or_
				: impl<lazy_impl_<
						   lazy_if_<Bool_, protect_<true_>, lazy::or_<Ts...>>,
						   Args>,
					  Args>
			{};
			template<template<typename...> class C,
				typename... Ts,
				typename Args>
			struct impl<defer<C, Ts...>, Args> : subst_<defer<C, Ts...>, Args>
			{};
			template<typename T, template<T...> class C, T... Is, typename Args>
			struct impl<defer_i<T, C, Is...>, Args>
				: subst_<defer_i<T, C, Is...>, Args>
			{};
			template<template<typename...> class C,
				typename... Ts,
				typename Args>
			struct impl<C<Ts...>, Args> : subst_<defer<C, Ts...>, Args>
			{};
			template<typename... Bs, bool IsVar, typename Args>
			struct impl<lambda_<list<Bs...>, IsVar>, Args>
			{
				using type = list<
					compose<typename lambda_<list<As..., Bs...>, true>::thunk,
						bind_front<quote<concat>, Args>,
						curry<bind_front<quote<substitutions>, list<Bs...>>>>>;
			};
			struct thunk
			{
				template<typename S, typename R = _t<impl<back<Tags>, S>>>
				using invoke = if_c<size<R>{} == 1, front<R>>;
			};

		public:
			template<typename... Ts>
			using invoke = invoke<thunk, substitutions<Tags, list<Ts...>>>;
		};
	} // namespace detail

	template<typename... Ts>
	using lambda = if_c<(sizeof...(Ts) > 0), detail::lambda_<list<Ts...>>>;

	template<typename T>
	using is_valid = detail::is_valid_<T>;

	template<typename T>
	using vararg = detail::vararg_<T>;

	template<typename T>
	using protect = detail::protect_<T>;

	template<typename Tag, typename Value>
	struct var;

	namespace detail
	{
		template<typename... As>
		struct let_
		{};
		template<typename Fn>
		struct let_<Fn>
		{
			using type = lazy::invoke<lambda<Fn>>;
		};
		template<typename Tag, typename Value, typename... Rest>
		struct let_<var<Tag, Value>, Rest...>
		{
			using type = lazy::invoke<lambda<Tag, _t<let_<Rest...>>>, Value>;
		};
	} // namespace detail

	template<typename... As>
	using let = _t<_t<detail::let_<As...>>>;

	namespace lazy
	{
		template<typename... As>
		using let = defer<let, As...>;
	} // namespace lazy

	inline namespace placeholders
	{
		struct _a;
		struct _b;
		struct _c;
		struct _d;
		struct _e;
		struct _f;
		struct _g;
		struct _h;
		struct _i;

		using _args = vararg<void>;
		using _args_a = vararg<_a>;
		using _args_b = vararg<_b>;
		using _args_c = vararg<_c>;
	} // namespace placeholders

	namespace detail
	{
		template<typename M2, typename M>
		struct cartesian_product_fn
		{
			template<typename X>
			struct lambda0
			{
				template<typename Xs>
				using lambda1 = list<push_front<Xs, X>>;
				using type = join<transform<M2, quote<lambda1>>>;
			};
			using type = join<transform<M, quote_trait<lambda0>>>;
		};
	} // namespace detail

	template<typename ListOfLists>
	using cartesian_product = reverse_fold<ListOfLists,
		list<list<>>,
		quote_trait<detail::cartesian_product_fn>>;

	namespace lazy
	{
		template<typename ListOfLists>
		using cartesian_product = defer<cartesian_product, ListOfLists>;
	} // namespace lazy

	namespace detail
	{
		template<bool>
		struct add_const_if
		{
			template<typename T>
			using invoke = T const;
		};
		template<>
		struct add_const_if<false>
		{
			template<typename T>
			using invoke = T;
		};
	} // namespace detail
	template<bool If>
	using add_const_if_c = detail::add_const_if<If>;
	template<typename If>
	using add_const_if = add_const_if_c<If::type::value>;

	template<bool If, typename T>
	using const_if_c = invoke<add_const_if_c<If>, T>;
	template<typename If, typename T>
	using const_if = invoke<add_const_if<If>, T>;

	namespace detail
	{
		template<typename State, typename Ch>
		using atoi_ = if_c<(Ch::value >= '0' && Ch::value <= '9'),
			integral_constant<typename State::value_type,
				State::value * 10 + (Ch::value - '0')>>;
	}

	inline namespace literals
	{
		template<char... Chs>
		constexpr fold<list<char_<Chs>...>, size_t_<0>, quote<detail::atoi_>>
		operator"" _z()
		{
			return {};
		}
	} // namespace literals

	template<typename... Ts>
	constexpr int
	ignore_unused(Ts&&...)
	{
		return 42;
	}

} // namespace rider::faiz::meta


namespace rider::faiz
{
	template<typename _type, typename... _types>
	struct are_same : logic::and_<is_same<_type, _types>...>
	{};

	//! \brief 判断第一个参数在之后参数指定的类型中出现。
	template<typename _type, typename... _types>
	struct is_in_types : or_<is_same<_type, _types...>>
	{};
} // namespace rider::faiz

#endif
