#ifndef __RANGE_H_
#define __RANGE_H_
#include "rider/faiz/faiz_fwd_meta.hpp"

namespace Rider::Faiz::meta
{


	tpl<typ... Ts> struct list;

	tpl<typ T> struct id;

	tpl<tpl<typ...> class> struct quote;

	tpl<typ T, tpl<T...> class F> struct quote_i;

	tpl<typ... Fs> struct compose;

	tpl<tpl<typ...> class C, typ... Ts> struct defer;

	tpl<typ T, tpl<T...> class C, T... Is> struct defer_i;

	namespace extension
	{
		tpl<typ F, typ List> struct apply;
	}

} // namespace Rider::Faiz::meta
namespace Rider::Faiz::meta
{

	namespace detail
	{
		tpl<typ T> cexp T*
		_nullptr_v()
		{
			return nullptr;
		}
	} // namespace detail

	struct nil_
	{};

	namespace lazy
	{
		tpl<typ T> using _t = defer<_t, T>;
	} // namespace lazy

	tpl<typ T> using inc
		= integral_constant<decltype(T::type::value + 1), T::type::value + 1>;

	tpl<typ T> using dec
		= integral_constant<decltype(T::type::value - 1), T::type::value - 1>;

	tpl<typ T, typ U> using plus
		= integral_constant<decltype(T::type::value + U::type::value),
			T::type::value + U::type::value>;

	tpl<typ T, typ U> using minus
		= integral_constant<decltype(T::type::value - U::type::value),
			T::type::value - U::type::value>;

	tpl<typ T, typ U> using multiplies
		= integral_constant<decltype(T::type::value * U::type::value),
			T::type::value * U::type::value>;

	tpl<typ T, typ U> using divides
		= integral_constant<decltype(T::type::value / U::type::value),
			T::type::value / U::type::value>;

	tpl<typ T> using negate
		= integral_constant<decltype(-T::type::value), -T::type::value>;

	tpl<typ T, typ U> using modulus
		= integral_constant<decltype(T::type::value % U::type::value),
			T::type::value % U::type::value>;

	tpl<typ T, typ U> using equal_to = bool_<T::type::value == U::type::value>;

	tpl<typ T, typ U> using not_equal_to
		= bool_<T::type::value != U::type::value>;

	tpl<typ T, typ U> using greater = bool_<(T::type::value > U::type::value)>;

	tpl<typ T, typ U> using less = bool_<(T::type::value < U::type::value)>;

	tpl<typ T, typ U> using greater_equal
		= bool_<(T::type::value >= U::type::value)>;

	tpl<typ T, typ U> using less_equal
		= bool_<(T::type::value <= U::type::value)>;

	tpl<typ T, typ U> using bit_and
		= integral_constant<decltype(T::type::value & U::type::value),
			T::type::value & U::type::value>;

	tpl<typ T, typ U> using bit_or
		= integral_constant<decltype(T::type::value | U::type::value),
			T::type::value | U::type::value>;

	tpl<typ T, typ U> using bit_xor
		= integral_constant<decltype(T::type::value ^ U::type::value),
			T::type::value ^ U::type::value>;

	tpl<typ T> using bit_not
		= integral_constant<decltype(~T::type::value), ~T::type::value>;

	namespace lazy
	{
		tpl<typ T> using inc = defer<inc, T>;

		tpl<typ T> using dec = defer<dec, T>;

		tpl<typ T, typ U> using plus = defer<plus, T, U>;

		tpl<typ T, typ U> using minus = defer<minus, T, U>;

		tpl<typ T, typ U> using multiplies = defer<multiplies, T, U>;

		tpl<typ T, typ U> using divides = defer<divides, T, U>;

		tpl<typ T> using negate = defer<negate, T>;

		tpl<typ T, typ U> using modulus = defer<modulus, T, U>;

		tpl<typ T, typ U> using equal_to = defer<equal_to, T, U>;

		tpl<typ T, typ U> using not_equal_to = defer<not_equal_to, T, U>;

		tpl<typ T, typ U> using greater = defer<greater, T, U>;

		tpl<typ T, typ U> using less = defer<less, T, U>;

		tpl<typ T, typ U> using greater_equal = defer<greater_equal, T, U>;

		tpl<typ T, typ U> using less_equal = defer<less_equal, T, U>;

		tpl<typ T, typ U> using bit_and = defer<bit_and, T, U>;

		tpl<typ T, typ U> using bit_or = defer<bit_or, T, U>;

		tpl<typ T, typ U> using bit_xor = defer<bit_xor, T, U>;

		tpl<typ T> using bit_not = defer<bit_not, T>;
	} // namespace lazy


	tpl<typ F, typ... Args> using invoke = typ F::tpl invoke<Args...>;

	namespace lazy
	{
		tpl<typ F, typ... Args> using invoke = defer<invoke, F, Args...>;
	} // namespace lazy

	tpl<typ T> struct id : type_identity<T>
	{
		tpl<typ...> using invoke = T;
	};

	tpl<typ T> using id_t = _t<id<T>>;

	namespace lazy
	{
		tpl<typ T> using id = defer<id, T>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ, typ = void> struct is_trait_ : type_identity<false_>
		{};

		tpl<typ T> struct is_trait_<T, void_t<typ T::type>>
			: type_identity<true_>
		{};

		tpl<typ, typ = void> struct is_callable_ : type_identity<false_>
		{};

		tpl<typ T> struct is_callable_<T, void_t<quote<T::tpl invoke>>>
			: type_identity<true_>
		{};

		tpl<tpl<typ...> class C, typ... Ts, tpl<typ...> class D = C>
			id<D<Ts...>>
			try_defer_(int);
		tpl<tpl<typ...> class C, typ... Ts> nil_
		try_defer_(long);

		tpl<tpl<typ...> class C, typ... Ts> using defer_
			= decltype(detail::try_defer_<C, Ts...>(0));

		tpl<typ T, tpl<T...> class C, T... Is, tpl<T...> class D = C>
			id<D<Is...>>
			try_defer_i_(int);
		tpl<typ T, tpl<T...> class C, T... Is> nil_
		try_defer_i_(long);

		tpl<typ T, tpl<T...> class C, T... Is> using defer_i_
			= decltype(detail::try_defer_i_<T, C, Is...>(0));

		tpl<typ T> using _t_t = _t<_t<T>>;
	} // namespace detail

	tpl<typ T> using is_trait = _t<detail::is_trait_<T>>;

	tpl<typ T> using is_callable = _t<detail::is_callable_<T>>;

	tpl<tpl<typ...> class C, typ... Ts> struct defer : detail::defer_<C, Ts...>
	{};

	tpl<typ T, tpl<T...> class C, T... Is> struct defer_i
		: detail::defer_i_<T, C, Is...>
	{};

	tpl<tpl<typ...> class C, typ... Ts> using defer_trait
		= defer<detail::_t_t, detail::defer_<C, Ts...>>;

	tpl<typ T> using sizeof_ = size_t_<sizeof(T)>;

	tpl<typ T> using alignof_ = size_t_<alignof(T)>;

	namespace lazy
	{
		tpl<typ T> using sizeof_ = defer<sizeof_, T>;

		tpl<typ T> using alignof_ = defer<alignof_, T>;
	} // namespace lazy


	namespace detail
	{
		tpl<typ, tpl<typ...> class> struct is_ : false_
		{};

		tpl<typ... Ts, tpl<typ...> class C> struct is_<C<Ts...>, C> : true_
		{};
	} // namespace detail


	tpl<typ T, tpl<typ...> class C> using is = _t<detail::is_<T, C>>;


	tpl<typ... Fs> struct compose
	{};

	tpl<typ F0> struct compose<F0>
	{
		tpl<typ... Ts> using invoke = invoke<F0, Ts...>;
	};

	tpl<typ F0, typ... Fs> struct compose<F0, Fs...>
	{
		tpl<typ... Ts> using invoke = invoke<F0, invoke<compose<Fs...>, Ts...>>;
	};

	namespace lazy
	{
		tpl<typ... Fns> using compose = defer<compose, Fns...>;
	} // namespace lazy

	tpl<tpl<typ...> class C> struct quote
	{
		// Indirection through defer here needed to avoid Core issue 1430
		// http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
		tpl<typ... Ts> using invoke = _t<detail::defer_<C, Ts...>>;
	};

	tpl<typ T, tpl<T...> class C> struct quote_i
	{
		// Indirection through defer_i here needed to avoid Core issue 1430
		// http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
		tpl<typ... Ts> using invoke
			= _t<detail::defer_i_<T, C, Ts::type::value...>>;
	};

	tpl<tpl<typ...> class C> using quote_trait = compose<quote<_t>, quote<C>>;

	tpl<typ T, tpl<T...> class C> using quote_trait_i
		= compose<quote<_t>, quote_i<T, C>>;

	tpl<typ F, typ... Ts> struct bind_front
	{
		tpl<typ... Us> using invoke = invoke<F, Ts..., Us...>;
	};

	tpl<typ F, typ... Us> struct bind_back
	{
		tpl<typ... Ts> using invoke = invoke<F, Ts..., Us...>;
	};

	namespace lazy
	{
		tpl<typ Fn, typ... Ts> using bind_front = defer<bind_front, Fn, Ts...>;

		tpl<typ Fn, typ... Ts> using bind_back = defer<bind_back, Fn, Ts...>;
	} // namespace lazy

	namespace extension
	{
		tpl<typ F, typ List> struct apply
		{};

		tpl<typ F, typ Ret, typ... Args> struct apply<F, Ret(Args...)>
			: lazy::invoke<F, Ret, Args...>
		{};

		tpl<typ F, tpl<typ...> typ T, typ... Ts> struct apply<F, T<Ts...>>
			: lazy::invoke<F, Ts...>
		{};

		tpl<typ F, typ T, T... Is> struct apply<F, integer_sequence<T, Is...>>
			: lazy::invoke<F, integral_constant<T, Is>...>
		{};
	} // namespace extension

	tpl<typ C, typ List> using apply = _t<extension::apply<C, List>>;

	namespace lazy
	{
		tpl<typ F, typ List> using apply = defer<apply, F, List>;
	}

	tpl<typ F, typ Q = quote<list>> using curry = compose<F, Q>;

	tpl<typ F> using uncurry = bind_front<quote<apply>, F>;

	namespace lazy
	{
		tpl<typ F, typ Q = quote<list>> using curry = defer<curry, F, Q>;

		tpl<typ F> using uncurry = defer<uncurry, F>;
	} // namespace lazy

	tpl<typ F> struct flip
	{
	private:
		tpl<typ... Ts> struct impl
		{};
		tpl<typ A, typ B, typ... Ts> struct impl<A, B, Ts...>
			: lazy::invoke<F, B, A, Ts...>
		{};

	public:
		tpl<typ... Ts> using invoke = _t<impl<Ts...>>;
	};

	namespace lazy
	{
		tpl<typ F> using flip = defer<flip, F>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ...> struct on_
		{};
		tpl<typ F, typ... Gs> struct on_<F, Gs...>
		{
			tpl<typ... Ts> using invoke
				= invoke<F, invoke<compose<Gs...>, Ts>...>;
		};
	} // namespace detail

	tpl<typ... Fs> using on = detail::on_<Fs...>;

	namespace lazy
	{
		tpl<typ F, typ G> using on = defer<on, F, G>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ, typ = bool> struct _if_
		{};

		tpl<typ If> struct _if_<list<If>, decltype(bool(If::type::value))>
			: enable_if<If::type::value>
		{};

		tpl<typ If, typ Then> struct _if_<list<If, Then>,
			decltype(bool(If::type::value))> : enable_if<If::type::value, Then>
		{};

		tpl<typ If, typ Then, typ Else> struct _if_<list<If, Then, Else>,
			decltype(bool(If::type::value))>
			: conditional<If::type::value, Then, Else>
		{};
	} // namespace detail

	tpl<typ... Args> using if_ = _t<detail::_if_<list<Args...>>>;

	tpl<bool If, typ... Args> using if_c
		= _t<detail::_if_<list<bool_<If>, Args...>>>;

	namespace lazy
	{
		tpl<typ... Args> using if_ = defer<if_, Args...>;

		tpl<bool If, typ... Args> using if_c = if_<bool_<If>, Args...>;
	} // namespace lazy

	namespace detail
	{
		tpl<bool> struct _and_
		{
			tpl<class...> using invoke = true_;
		};

		tpl<> struct _and_<false>
		{
			tpl<typ Bool, typ... Bools> using invoke
				= invoke<if_c<!Bool::type::value,
							 id<false_>,
							 _and_<0 == sizeof...(Bools)>>,
					Bools...>;
		};

		tpl<bool> struct _or_
		{
			tpl<class = void> using invoke = false_;
		};

		tpl<> struct _or_<false>
		{
			tpl<typ Bool_, typ... Bools> using invoke = invoke<
				if_c<Bool_::type::value, id<true_>, _or_<0 == sizeof...(Bools)>>,
				Bools...>;
		};
	} // namespace detail

	tpl<bool Bool_> using not_c = bool_<!Bool_>;

	tpl<typ Bool_> using not_ = not_c<Bool_::type::value>;

	// tpl<bool... Bools>
	// using and_c = is_same<integer_sequence<bool, Bools...>,
	// 	integer_sequence<bool, (Bools || true)...>>;
	tpl<bool... B> using and_c = bool_<(B && ...)>;
	tpl<bool... B> inline cexp bool and_t = bool_<(B && ...)>::value;

	tpl<typ... Bools> using strict_and = and_c<Bools::type::value...>;
	tpl<typ... Bools> inline cexp bool strict_and_v
		= and_c<Bools::type::value...>::value;

	tpl<typ... Bools> using and_
		= _t<defer<detail::_and_<0 == sizeof...(Bools)>::tpl invoke, Bools...>>;


	tpl<bool... B> struct fold_and : bool_<(B && ...)>
	{};

	tpl<bool... Bools> using or_c
		= not_<is_same<integer_sequence<bool, Bools...>,
			integer_sequence<bool, (Bools && false)...>>>;

	tpl<typ... Bools> using strict_or = or_c<Bools::type::value...>;

	tpl<typ... Bools> using or_
		= _t<defer<detail::_or_<0 == sizeof...(Bools)>::tpl invoke, Bools...>>;

	namespace lazy
	{
		tpl<typ... Bools> using and_ = defer<and_, Bools...>;

		tpl<typ... Bools> using or_ = defer<or_, Bools...>;

		tpl<typ Bool_> using not_ = defer<not_, Bool_>;

		tpl<typ... Bools> using strict_and = defer<strict_and, Bools...>;

		tpl<typ... Bools> using strict_or = defer<strict_or, Bools...>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ Fun, typ T0> struct compose1_
		{
			tpl<typ X> using invoke = invoke<Fun, _t<X>, T0>;
		};

		tpl<typ Fun,
			typ T0,
			typ T1,
			typ T2,
			typ T3,
			typ T4,
			typ T5,
			typ T6,
			typ T7,
			typ T8,
			typ T9> struct compose10_
		{
			tpl<typ X, typ Y> using F = invoke<Fun, X, Y>;

			tpl<typ S> using invoke = F<
				F<F<F<F<F<F<F<F<F<_t<S>, T0>, T1>, T2>, T3>, T4>, T5>, T6>, T7>,
					T8>,
				T9>;
		};

		tpl<typ, typ, typ> struct fold_
		{};

		tpl<typ State, typ Fun> struct fold_<list<>, State, Fun> : State
		{};

		tpl<typ Head,
			typ... List,
			typ State,
			typ Fun> struct fold_<list<Head, List...>, State, Fun>
			: fold_<list<List...>,
				  lazy::invoke<compose1_<Fun, Head>, State>,
				  Fun>
		{};

		tpl<typ T0,
			typ T1,
			typ T2,
			typ T3,
			typ T4,
			typ T5,
			typ T6,
			typ T7,
			typ T8,
			typ T9,
			typ... List,
			typ State,
			typ Fun> struct
			fold_<list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, List...>,
				State,
				Fun>
			: fold_<list<List...>,
				  lazy::invoke<
					  compose10_<Fun, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>,
					  State>,
				  Fun>
		{};
	} // namespace detail

	tpl<typ List, typ State, typ Fun> using fold
		= _t<detail::fold_<List, id<State>, Fun>>;

	tpl<typ List, typ State, typ Fun> using accumulate = fold<List, State, Fun>;

	namespace lazy
	{
		tpl<typ List, typ State, typ Fun> using fold
			= defer<fold, List, State, Fun>;

		tpl<typ List, typ State, typ Fun> using accumulate
			= defer<accumulate, List, State, Fun>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ List, typ State, typ Fun> struct reverse_fold_
		{};

		tpl<typ State, typ Fun> struct reverse_fold_<list<>, State, Fun>
			: type_identity<State>
		{};

		tpl<typ Head,
			typ... List,
			typ State,
			typ Fun> struct reverse_fold_<list<Head, List...>, State, Fun>
			: lazy::invoke<compose1_<Fun, Head>,
				  reverse_fold_<list<List...>, State, Fun>>
		{};

		tpl<typ T0,
			typ T1,
			typ T2,
			typ T3,
			typ T4,
			typ T5,
			typ T6,
			typ T7,
			typ T8,
			typ T9,
			typ... List,
			typ State,
			typ Fun> struct
			reverse_fold_<list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, List...>,
				State,
				Fun>
			: lazy::invoke<
				  compose10_<Fun, T9, T8, T7, T6, T5, T4, T3, T2, T1, T0>,
				  reverse_fold_<list<List...>, State, Fun>>
		{};
	} // namespace detail

	tpl<typ List, typ State, typ Fun> using reverse_fold
		= _t<detail::reverse_fold_<List, State, Fun>>;

	namespace lazy
	{
		tpl<typ List, typ State, typ Fun> using reverse_fold
			= defer<reverse_fold, List, State, Fun>;
	} // namespace lazy

	using npos = size_t_<size_t(-1)>;

	tpl<typ... Ts> struct list
	{
		using type = list;
		/// \return `sizeof...(Ts)`
		static cexp size_t
		size() noexcept
		{
			return sizeof...(Ts);
		}
	};

	tpl<typ List> using size = size_t_<List::size()>;

	namespace lazy
	{
		tpl<typ List> using size = defer<size, List>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ... Lists> struct concat_
		{};

		tpl<> struct concat_<> : type_identity<list<>>
		{};

		tpl<typ... List1> struct concat_<list<List1...>>
			: type_identity<list<List1...>>
		{};

		tpl<typ... List1,
			typ... List2,
			typ... Rest> struct concat_<list<List1...>, list<List2...>, Rest...>
			: concat_<list<List1..., List2...>, Rest...>
		{};

		tpl<typ... List1,
			typ... List2,
			typ... List3,
			typ... List4,
			typ... List5,
			typ... List6,
			typ... List7,
			typ... List8,
			typ... List9,
			typ... List10,
			typ... Rest> struct concat_<list<List1...>,
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

	tpl<typ... Lists> using concat = _t<detail::concat_<Lists...>>;

	namespace lazy
	{
		tpl<typ... Lists> using concat = defer<concat, Lists...>;
	} // namespace lazy

	tpl<typ ListOfLists> using join = apply<quote<concat>, ListOfLists>;

	namespace lazy
	{
		tpl<typ ListOfLists> using join = defer<join, ListOfLists>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ, typ = void> struct transform_
		{};

		tpl<typ... Ts, typ Fun> struct transform_<list<list<Ts...>, Fun>,
			void_t<invoke<Fun, Ts>...>>
			: type_identity<list<invoke<Fun, Ts>...>>
		{};

		tpl<typ... Ts0, typ... Ts1, typ Fun> struct transform_<
			list<list<Ts0...>, list<Ts1...>, Fun>,
			void_t<invoke<Fun, Ts0, Ts1>...>>
			: type_identity<list<invoke<Fun, Ts0, Ts1>...>>
		{};
	} // namespace detail

	tpl<typ... Args> using transform = _t<detail::transform_<list<Args...>>>;

	namespace lazy
	{
		tpl<typ... Args> using transform = defer<transform, Args...>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ T, size_t> using first_ = T;

		tpl<typ T, typ Ints> struct repeat_n_c_
		{};

		tpl<typ T, size_t... Is> struct repeat_n_c_<T, index_sequence<Is...>>
			: type_identity<list<first_<T, Is>...>>
		{};
	} // namespace detail

	tpl<size_t N, typ T = void> using repeat_n_c
		= _t<detail::repeat_n_c_<T, make_index_sequence<N>>>;

	tpl<typ N, typ T = void> using repeat_n = repeat_n_c<N::type::value, T>;

	namespace lazy
	{
		tpl<typ N, typ T = void> using repeat_n = defer<repeat_n, N, T>;

		tpl<size_t N, typ T = void> using repeat_n_c
			= defer<repeat_n, size_t_<N>, T>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ VoidPtrs> struct at_impl_;

		tpl<typ... VoidPtrs> struct at_impl_<list<VoidPtrs...>>
		{
			static nil_
			eval(...);

			tpl<typ T, typ... Us> static T
			eval(VoidPtrs..., T*, Us*...);
		};

		tpl<typ List, size_t N> struct at_
		{};

		tpl<typ... Ts, size_t N> struct at_<list<Ts...>, N>
			: decltype(at_impl_<repeat_n_c<N, void*>>::eval(
				  static_cast<id<Ts>*>(nullptr)...))
		{};
	} // namespace detail

	tpl<typ List, size_t N> using at_c = _t<detail::at_<List, N>>;

	tpl<typ List, typ N> using at = at_c<List, N::type::value>;

	namespace lazy
	{
		tpl<typ List, typ N> using at = defer<at, List, N>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ VoidPtrs> struct drop_impl_
		{
			static nil_
			eval(...);
		};

		tpl<typ... VoidPtrs> struct drop_impl_<list<VoidPtrs...>>
		{
			static nil_
			eval(...);

			tpl<typ... Ts> static id<list<Ts...>>
			eval(VoidPtrs..., id<Ts>*...);
		};

		tpl<> struct drop_impl_<list<>>
		{
			tpl<typ... Ts> static id<list<Ts...>>
			eval(id<Ts>*...);
		};

		tpl<typ List, typ N> struct drop_
		{};

		tpl<typ... Ts, typ N> struct drop_<list<Ts...>, N>
			: decltype(drop_impl_<repeat_n<N, void*>>::eval(
				  detail::_nullptr_v<id<Ts>>()...))
		{};
	} // namespace detail

	tpl<typ List, typ N> using drop = _t<detail::drop_<List, N>>;

	tpl<typ List, size_t N> using drop_c = _t<detail::drop_<List, size_t_<N>>>;

	namespace lazy
	{
		tpl<typ List, typ N> using drop = defer<drop, List, N>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ List> struct front_
		{};

		tpl<typ Head, typ... List> struct front_<list<Head, List...>>
			: type_identity<Head>
		{};
	} // namespace detail

	tpl<typ List> using front = _t<detail::front_<List>>;

	namespace lazy
	{
		tpl<typ List> using front = defer<front, List>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ List> struct back_
		{};

		tpl<typ Head, typ... List> struct back_<list<Head, List...>>
			: type_identity<at_c<list<Head, List...>, sizeof...(List)>>
		{};
	} // namespace detail

	tpl<typ List> using back = _t<detail::back_<List>>;

	namespace lazy
	{
		tpl<typ List> using back = defer<back, List>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ List, typ T> struct push_front_
		{};

		tpl<typ... List, typ T> struct push_front_<list<List...>, T>
			: type_identity<list<T, List...>>
		{};
	} // namespace detail

	tpl<typ List, typ T> using push_front = _t<detail::push_front_<List, T>>;

	namespace lazy
	{
		tpl<typ List, typ T> using push_front = defer<push_front, List, T>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ List> struct pop_front_
		{};

		tpl<typ Head, typ... List> struct pop_front_<list<Head, List...>>
			: type_identity<list<List...>>
		{};
	} // namespace detail

	tpl<typ List> using pop_front = _t<detail::pop_front_<List>>;

	namespace lazy
	{
		tpl<typ List> using pop_front = defer<pop_front, List>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ List, typ T> struct push_back_
		{};

		tpl<typ... List, typ T> struct push_back_<list<List...>, T>
			: type_identity<list<List..., T>>
		{};
	} // namespace detail

	tpl<typ List, typ T> using push_back = _t<detail::push_back_<List, T>>;

	namespace lazy
	{
		tpl<typ List, typ T> using push_back = defer<push_back, List, T>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ T, typ U> using min_ = if_<less<U, T>, U, T>;

		tpl<typ T, typ U> using max_ = if_<less<U, T>, T, U>;
	} // namespace detail

	tpl<typ... Ts> using min
		= fold<pop_front<list<Ts...>>, front<list<Ts...>>, quote<detail::min_>>;

	tpl<typ... Ts> using max
		= fold<pop_front<list<Ts...>>, front<list<Ts...>>, quote<detail::max_>>;

	namespace lazy
	{
		tpl<typ... Ts> using min = defer<min, Ts...>;

		tpl<typ... Ts> using max = defer<max, Ts...>;
	} // namespace lazy

	tpl<typ List> using empty = bool_<0 == size<List>::type::value>;

	namespace lazy
	{
		tpl<typ List> using empty = defer<empty, List>;
	} // namespace lazy

	tpl<typ F, typ S> using pair = list<F, S>;

	tpl<typ Pair> using first = front<Pair>;

	tpl<typ Pair> using second = front<pop_front<Pair>>;

	namespace lazy
	{
		tpl<typ Pair> using first = defer<first, Pair>;

		tpl<typ Pair> using second = defer<second, Pair>;
	} // namespace lazy

	namespace detail
	{
		cexp size_t
		find_index_i_(
			bool const* const first, bool const* const last, size_t N = 0)
		{
			return first == last ?
				npos::value :
				*first ? N : find_index_i_(first + 1, last, N + 1);
		}

		tpl<typ List, typ T> struct find_index_
		{};

		tpl<typ V> struct find_index_<list<>, V> : type_identity<npos>
		{};

		tpl<typ... T, typ V> struct find_index_<list<T...>, V>
		{
			static cexp bool s_v[] = {is_same<T, V>::value...};
			using type = size_t_<find_index_i_(s_v, s_v + sizeof...(T))>;
		};
	} // namespace detail

	tpl<typ List, typ T> using find_index = _t<detail::find_index_<List, T>>;

	namespace lazy
	{
		tpl<typ List, typ T> using find_index = defer<find_index, List, T>;
	} // namespace lazy

	namespace detail
	{
		cexp size_t
		reverse_find_index_i_(
			bool const* const first, bool const* const last, size_t N)
		{
			return first == last ?
				npos::value :
				*(last - 1) ? N - 1 :
							  reverse_find_index_i_(first, last - 1, N - 1);
		}

		tpl<typ List, typ T> struct reverse_find_index_
		{};

		tpl<typ V> struct reverse_find_index_<list<>, V> : type_identity<npos>
		{};

		tpl<typ... T, typ V> struct reverse_find_index_<list<T...>, V>
		{
			static cexp bool s_v[] = {is_same<T, V>::value...};
			using type = size_t_<reverse_find_index_i_(
				s_v, s_v + sizeof...(T), sizeof...(T))>;
		};
	} // namespace detail

	tpl<typ List, typ T> using reverse_find_index
		= _t<detail::reverse_find_index_<List, T>>;

	namespace lazy
	{
		tpl<typ List, typ T> using reverse_find_index
			= defer<reverse_find_index, List, T>;
	} // namespace lazy

	tpl<typ List, typ T> using find
		= drop<List, min<find_index<List, T>, size<List>>>;

	namespace lazy
	{
		tpl<typ List, typ T> using find = defer<find, List, T>;
	} // namespace lazy

	tpl<typ List, typ T> using reverse_find
		= drop<List, min<reverse_find_index<List, T>, size<List>>>;

	namespace lazy
	{
		tpl<typ List, typ T> using reverse_find = defer<reverse_find, List, T>;
	} // namespace lazy

	namespace detail
	{
		cexp bool const*
		find_if_i_(bool const* const begin, bool const* const end)
		{
			return begin == end || *begin ? begin : find_if_i_(begin + 1, end);
		}

		tpl<typ List, typ Fun, typ = void> struct find_if_
		{};

		tpl<typ Fun> struct find_if_<list<>, Fun> : type_identity<list<>>
		{};

		tpl<typ... List, typ Fun> struct find_if_<list<List...>,
			Fun,
			void_t<integer_sequence<bool,
				bool(invoke<Fun, List>::type::value)...>>>
		{
			static cexp bool s_v[] = {invoke<Fun, List>::type::value...};
			using type = drop_c<list<List...>,
				detail::find_if_i_(s_v, s_v + sizeof...(List)) - s_v>;
		};
	} // namespace detail

	tpl<typ List, typ Fun> using find_if = _t<detail::find_if_<List, Fun>>;

	namespace lazy
	{
		tpl<typ List, typ Fun> using find_if = defer<find_if, List, Fun>;
	} // namespace lazy

	namespace detail
	{
		cexp bool const*
		reverse_find_if_i_(bool const* const begin,
			bool const* const pos,
			bool const* const end)
		{
			return begin == pos ?
				end :
				*(pos - 1) ? pos - 1 : reverse_find_if_i_(begin, pos - 1, end);
		}

		tpl<typ List, typ Fun, typ = void> struct reverse_find_if_
		{};

		tpl<typ Fun> struct reverse_find_if_<list<>, Fun>
			: type_identity<list<>>
		{};

		tpl<typ... List, typ Fun> struct reverse_find_if_<list<List...>,
			Fun,
			void_t<integer_sequence<bool,
				bool(invoke<Fun, List>::type::value)...>>>
		{
			static cexp bool s_v[] = {invoke<Fun, List>::type::value...};
			using type = drop_c<list<List...>,
				detail::reverse_find_if_i_(
					s_v, s_v + sizeof...(List), s_v + sizeof...(List))
					- s_v>;
		};
	} // namespace detail

	tpl<typ List, typ Fun> using reverse_find_if
		= _t<detail::reverse_find_if_<List, Fun>>;

	namespace lazy
	{
		tpl<typ List, typ Fun> using reverse_find_if
			= defer<reverse_find_if, List, Fun>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ List, typ T, typ U> struct replace_
		{};

		tpl<typ... List, typ T, typ U> struct replace_<list<List...>, T, U>
			: type_identity<list<if_<is_same<T, List>, U, List>...>>
		{};
	} // namespace detail

	tpl<typ List, typ T, typ U> using replace
		= _t<detail::replace_<List, T, U>>;

	namespace lazy
	{
		tpl<typ List, typ T, typ U> using replace = defer<replace, T, U>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ List, typ C, typ U, typ = void> struct replace_if_
		{};

		tpl<typ... List, typ C, typ U> struct replace_if_<list<List...>,
			C,
			U,
			void_t<
				integer_sequence<bool, bool(invoke<C, List>::type::value)...>>>
		{
			using type = list<if_<invoke<C, List>, U, List>...>;
		};
	} // namespace detail

	tpl<typ List, typ C, typ U> using replace_if
		= _t<detail::replace_if_<List, C, U>>;

	namespace lazy
	{
		tpl<typ List, typ C, typ U> using replace_if = defer<replace_if, C, U>;
	} // namespace lazy

	namespace detail
	{
		cexp size_t
		count_i_(bool const* const begin, bool const* const end, size_t n)
		{
			return begin == end ? n :
								  detail::count_i_(begin + 1, end, n + *begin);
		}

		tpl<typ List, typ T, typ = void> struct count_
		{};

		tpl<typ T> struct count_<list<>, T> : type_identity<size_t_<0>>
		{};

		tpl<typ... List, typ T> struct count_<list<List...>, T>
		{
			static cexp bool s_v[] = {is_same<T, List>::value...};
			using type
				= size_t_<detail::count_i_(s_v, s_v + sizeof...(List), 0u)>;
		};
	} // namespace detail

	tpl<typ List, typ T> using count = _t<detail::count_<List, T>>;

	namespace lazy
	{
		tpl<typ List, typ T> using count = defer<count, List, T>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ List, typ Fn, typ = void> struct count_if_
		{};

		tpl<typ Fn> struct count_if_<list<>, Fn> : type_identity<size_t_<0>>
		{};

		tpl<typ... List, typ Fn> struct count_if_<list<List...>,
			Fn,
			void_t<
				integer_sequence<bool, bool(invoke<Fn, List>::type::value)...>>>
		{
			static cexp bool s_v[] = {invoke<Fn, List>::type::value...};
			using type
				= size_t_<detail::count_i_(s_v, s_v + sizeof...(List), 0u)>;
		};
	} // namespace detail

	tpl<typ List, typ Fn> using count_if = _t<detail::count_if_<List, Fn>>;

	namespace lazy
	{
		tpl<typ List, typ Fn> using count_if = defer<count_if, List, Fn>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ Pred> struct filter_
		{
			tpl<typ A> using invoke
				= if_c<invoke<Pred, A>::type::value, list<A>, list<>>;
		};
	} // namespace detail

	tpl<typ List, typ Pred> using filter
		= join<transform<List, detail::filter_<Pred>>>;

	namespace lazy
	{
		tpl<typ List, typ Pred> using filter = defer<filter, List, Pred>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ T> struct static_const
		{
			static cexp T value{};
		};

		// Avoid potential ODR violations with global objects:
		tpl<typ T> cexp T static_const<T>::value;
	} // namespace detail

	namespace detail
	{
		struct for_each_fn
		{
			tpl<class UnaryFunction, class... Args> cfn
			operator()(list<Args...>, UnaryFunction f) const->UnaryFunction
			{
				return (void)std::initializer_list<int>{
						   ((void)f(Args{}), 0)...},
					   f;
			}
		};
	} // namespace detail

	namespace
	{
		cval&& for_each = detail::static_const<detail::for_each_fn>::value;
	} // namespace

	tpl<typ ListOfLists> using transpose = fold<ListOfLists,
		repeat_n<size<front<ListOfLists>>, list<>>,
		bind_back<quote<transform>, quote<push_back>>>;

	namespace lazy
	{
		tpl<typ ListOfLists> using transpose = defer<transpose, ListOfLists>;
	} // namespace lazy

	tpl<typ Fun, typ ListOfLists> using zip_with
		= transform<transpose<ListOfLists>, uncurry<Fun>>;

	namespace lazy
	{
		tpl<typ Fun, typ ListOfLists> using zip_with
			= defer<zip_with, Fun, ListOfLists>;
	} // namespace lazy

	tpl<typ ListOfLists> using zip = transpose<ListOfLists>;

	namespace lazy
	{
		tpl<typ ListOfLists> using zip = defer<zip, ListOfLists>;
	} // namespace lazy

	namespace detail
	{
		// Indirection here needed to avoid Core issue 1430
		// http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
		tpl<typ Sequence> struct as_list_
			: lazy::invoke<uncurry<quote<list>>, Sequence>
		{};
	} // namespace detail

	tpl<typ Sequence> using as_list
		= _t<detail::as_list_<remove_cvref_t<Sequence>>>;

	namespace lazy
	{
		tpl<typ Sequence> using as_list = defer<as_list, Sequence>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ List, typ State = list<>> struct reverse_
			: lazy::fold<List, State, quote<push_front>>
		{};

		tpl<typ T0,
			typ T1,
			typ T2,
			typ T3,
			typ T4,
			typ T5,
			typ T6,
			typ T7,
			typ T8,
			typ T9,
			typ... Ts,
			typ... Us> struct
			reverse_<list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, Ts...>,
				list<Us...>>
			: reverse_<list<Ts...>,
				  list<T9, T8, T7, T6, T5, T4, T3, T2, T1, T0, Us...>>
		{};
	} // namespace detail

	tpl<typ List> using reverse = _t<detail::reverse_<List>>;

	namespace lazy
	{
		tpl<typ List> using reverse = defer<reverse, List>;
	} // namespace lazy

	tpl<typ F> using not_fn = compose<quote<not_>, F>;

	namespace lazy
	{
		tpl<typ F> using not_fn = defer<not_fn, F>;
	} // namespace lazy

	tpl<typ List, typ F> using all_of = empty<find_if<List, not_fn<F>>>;

	namespace lazy
	{
		/// \sa 'meta::all_of'
		/// \ingroup lazy_query
		tpl<typ List, typ Fn> using all_of = defer<all_of, List, Fn>;
	} // namespace lazy

	tpl<typ List, typ F> using any_of = not_<empty<find_if<List, F>>>;

	namespace lazy
	{
		tpl<typ List, typ Fn> using any_of = defer<any_of, List, Fn>;
	} // namespace lazy

	tpl<typ List, typ F> using none_of = empty<find_if<List, F>>;

	namespace lazy
	{
		tpl<typ List, typ Fn> using none_of = defer<none_of, List, Fn>;
	} // namespace lazy

	tpl<typ List, typ T> using in = not_<empty<find<List, T>>>;

	namespace lazy
	{
		tpl<typ List, typ T> using in = defer<in, List, T>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ List> struct inherit_
		{};

		tpl<typ... List> struct inherit_<list<List...>> : List...
		{
			using type = inherit_;
		};
	} // namespace detail

	tpl<typ List> using inherit = _t<detail::inherit_<List>>;

	namespace lazy
	{
		tpl<typ List> using inherit = defer<inherit, List>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ Set, typ T> struct in_
		{};

		tpl<typ... Set, typ T> struct in_<list<Set...>, T>
			: Faiz::is_base_of<id<T>, inherit<list<id<Set>...>>>
		{};

		tpl<typ Set, typ T> struct insert_back_
		{};

		tpl<typ... Set, typ T> struct insert_back_<list<Set...>, T>
		{
			using type
				= if_<in_<list<Set...>, T>, list<Set...>, list<Set..., T>>;
		};
	} // namespace detail

	tpl<typ List> using unique
		= fold<List, list<>, quote_trait<detail::insert_back_>>;

	namespace lazy
	{
		tpl<typ List> using unique = defer<unique, List>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ Pred> struct partition_
		{
			tpl<typ, typ, typ = void> struct impl
			{};
			tpl<typ... Yes, typ... No, typ A> struct impl<
				pair<list<Yes...>, list<No...>>,
				A,
				void_t<bool_<invoke<Pred, A>::type::value>>>
			{
				using type = if_<invoke<Pred, A>,
					pair<list<Yes..., A>, list<No...>>,
					pair<list<Yes...>, list<No..., A>>>;
			};
			tpl<typ State, typ A> using invoke = _t<impl<State, A>>;
		};
	} // namespace detail

	tpl<typ List, typ Pred> using partition
		= fold<List, pair<list<>, list<>>, detail::partition_<Pred>>;

	namespace lazy
	{
		tpl<typ List, typ Pred> using partition = defer<partition, List, Pred>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ, typ, typ = void> struct sort_
		{};

		tpl<typ Pred> struct sort_<list<>, Pred> : type_identity<list<>>
		{};

		tpl<typ A, typ Pred> struct sort_<list<A>, Pred>
			: type_identity<list<A>>
		{};

		tpl<typ A, typ B, typ... List, typ Pred> struct sort_<
			list<A, B, List...>,
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

	tpl<typ List, typ Pred> using sort = _t<detail::sort_<List, Pred>>;

	namespace lazy
	{
		tpl<typ List, typ Pred> using sort = defer<sort, List, Pred>;
	} // namespace lazy

	namespace detail
	{
		tpl<typ T, int = 0> struct protect_;

		tpl<typ, int = 0> struct vararg_;

		tpl<typ T, int = 0> struct is_valid_;

		// Returns which branch to evaluate
		tpl<typ If, typ... Ts> using lazy_if_
			= lazy::_t<defer<_if_, list<If, protect_<Ts>...>>>;

		tpl<typ A, typ T, typ F, typ Ts> struct subst1_
			: type_identity<list<list<T>>>
		{};
		tpl<typ T, typ F, typ Ts> struct subst1_<F, T, F, Ts>
			: type_identity<list<>>
		{};
		tpl<typ A, typ T, typ F, typ Ts> struct subst1_<vararg_<A>, T, F, Ts>
			: type_identity<list<Ts>>
		{};

		tpl<typ As, typ Ts> using substitutions_ = push_back<
			join<transform<
				concat<As, repeat_n_c<size<Ts>{} + 2 - size<As>{}, back<As>>>,
				concat<Ts, repeat_n_c<2, back<As>>>,
				bind_back<quote_trait<subst1_>,
					back<As>,
					drop_c<Ts, size<As>{} - 2>>>>,
			list<back<As>>>;

		tpl<typ As, typ Ts> using substitutions
			= invoke<if_c<(size<Ts>{} + 2 >= size<As>{}), quote<substitutions_>>,
				As,
				Ts>;

		tpl<typ T> struct is_vararg_ : false_
		{};
		tpl<typ T> struct is_vararg_<vararg_<T>> : true_
		{};

		tpl<typ Tags> using is_variadic_
			= is_vararg_<at<push_front<Tags, void>, dec<size<Tags>>>>;

		tpl<typ Tags,
			bool IsVariadic = is_variadic_<Tags>::value> struct lambda_;

		// Non-variadic lambda implementation
		tpl<typ... As> struct lambda_<list<As...>, false>
		{
		private:
			static cexp size_t arity = sizeof...(As) - 1;
			using Tags
				= list<As...>; // Includes the lambda body as the last arg!
			using F = back<Tags>;
			tpl<typ T, typ Args> struct impl;
			tpl<typ T, typ Args> using lazy_impl_
				= lazy::_t<defer<impl, T, protect_<Args>>>;
			tpl<typ, typ, typ = void> struct subst_
			{};
			tpl<tpl<typ...> class C, typ... Ts, typ Args> struct subst_<
				defer<C, Ts...>,
				Args,
				void_t<C<_t<impl<Ts, Args>>...>>>
				: type_identity<C<_t<impl<Ts, Args>>...>>
			{};
			tpl<typ T, tpl<T...> class C, T... Is, typ Args> struct subst_<
				defer_i<T, C, Is...>,
				Args,
				void_t<C<Is...>>> : type_identity<C<Is...>>
			{};
			tpl<typ T, typ Args> struct impl
				: if_c<(reverse_find_index<Tags, T>() != npos()),
					  lazy::at<Args, reverse_find_index<Tags, T>>,
					  id<T>>
			{};
			tpl<typ T, typ Args> struct impl<protect_<T>, Args>
				: type_identity<T>
			{};
			tpl<typ T, typ Args> struct impl<is_valid_<T>, Args>
				: type_identity<is_trait<impl<T, Args>>>
			{};
			tpl<typ If, typ... Ts, typ Args> struct impl<defer<if_, If, Ts...>,
				Args> // Short-circuit
					  // if_
				: impl<lazy_impl_<lazy_if_<If, Ts...>, Args>, Args>
			{};
			tpl<typ Bool_, typ... Ts, typ Args> struct impl<
				defer<and_, Bool_, Ts...>,
				Args> // Short-circuit
					  // and_
				: impl<lazy_impl_<
						   lazy_if_<Bool_, lazy::and_<Ts...>, protect_<false_>>,
						   Args>,
					  Args>
			{};
			tpl<typ Bool_, typ... Ts, typ Args> struct impl<
				defer<or_, Bool_, Ts...>,
				Args> // Short-circuit or_
				: impl<lazy_impl_<
						   lazy_if_<Bool_, protect_<true_>, lazy::or_<Ts...>>,
						   Args>,
					  Args>
			{};
			tpl<tpl<typ...> class C,
				typ... Ts,
				typ Args> struct impl<defer<C, Ts...>, Args>
				: subst_<defer<C, Ts...>, Args>
			{};
			tpl<typ T, tpl<T...> class C, T... Is, typ Args> struct impl<
				defer_i<T, C, Is...>,
				Args> : subst_<defer_i<T, C, Is...>, Args>
			{};
			tpl<tpl<typ...> class C, typ... Ts, typ Args> struct impl<C<Ts...>,
				Args> : subst_<defer<C, Ts...>, Args>
			{};
			tpl<typ... Ts, typ Args> struct impl<lambda_<list<Ts...>, false>,
				Args>
			{
				using type
					= compose<uncurry<lambda_<list<As..., Ts...>, false>>,
						curry<bind_front<quote<concat>, Args>>>;
			};
			tpl<typ... Bs, typ Args> struct impl<lambda_<list<Bs...>, true>,
				Args>
			{
				using type
					= compose<typ lambda_<list<As..., Bs...>, true>::thunk,
						bind_front<quote<concat>, transform<Args, quote<list>>>,
						curry<bind_front<quote<substitutions>, list<Bs...>>>>;
			};

		public:
			tpl<typ... Ts> using invoke
				= _t<if_c<sizeof...(Ts) == arity, impl<F, list<Ts..., F>>>>;
		};

		tpl<typ... As> struct lambda_<list<As...>, true>
		{
		private:
			tpl<typ T, bool IsVar> friend struct lambda_;
			using Tags
				= list<As...>; // Includes the lambda body as the last arg!
			tpl<typ T, typ Args> struct impl;
			tpl<typ Args> using eval_impl_ = bind_back<quote_trait<impl>, Args>;
			tpl<typ T, typ Args> using lazy_impl_
				= lazy::_t<defer<impl, T, protect_<Args>>>;
			tpl<tpl<typ...> class C, typ Args, typ Ts> using try_subst_
				= apply<quote<C>, join<transform<Ts, eval_impl_<Args>>>>;
			tpl<typ, typ, typ = void> struct subst_
			{};
			tpl<tpl<typ...> class C, typ... Ts, typ Args> struct subst_<
				defer<C, Ts...>,
				Args,
				void_t<try_subst_<C, Args, list<Ts...>>>>
				: type_identity<list<try_subst_<C, Args, list<Ts...>>>>
			{};
			tpl<typ T, tpl<T...> class C, T... Is, typ Args> struct subst_<
				defer_i<T, C, Is...>,
				Args,
				void_t<C<Is...>>> : type_identity<list<C<Is...>>>
			{};
			tpl<typ T, typ Args> struct impl
				: if_c<(reverse_find_index<Tags, T>() != npos()),
					  lazy::at<Args, reverse_find_index<Tags, T>>,
					  id<list<T>>>
			{};
			tpl<typ T, typ Args> struct impl<protect_<T>, Args>
				: type_identity<list<T>>
			{};
			tpl<typ T, typ Args> struct impl<is_valid_<T>, Args>
				: type_identity<list<is_trait<impl<T, Args>>>>
			{};
			tpl<typ If, typ... Ts, typ Args> struct impl<defer<if_, If, Ts...>,
				Args> // Short-circuit
					  // if_
				: impl<lazy_impl_<lazy_if_<If, Ts...>, Args>, Args>
			{};
			tpl<typ Bool_, typ... Ts, typ Args> struct impl<
				defer<and_, Bool_, Ts...>,
				Args> // Short-circuit
					  // and_
				: impl<lazy_impl_<
						   lazy_if_<Bool_, lazy::and_<Ts...>, protect_<false_>>,
						   Args>,
					  Args>
			{};
			tpl<typ Bool_, typ... Ts, typ Args> struct impl<
				defer<or_, Bool_, Ts...>,
				Args> // Short-circuit or_
				: impl<lazy_impl_<
						   lazy_if_<Bool_, protect_<true_>, lazy::or_<Ts...>>,
						   Args>,
					  Args>
			{};
			tpl<tpl<typ...> class C,
				typ... Ts,
				typ Args> struct impl<defer<C, Ts...>, Args>
				: subst_<defer<C, Ts...>, Args>
			{};
			tpl<typ T, tpl<T...> class C, T... Is, typ Args> struct impl<
				defer_i<T, C, Is...>,
				Args> : subst_<defer_i<T, C, Is...>, Args>
			{};
			tpl<tpl<typ...> class C, typ... Ts, typ Args> struct impl<C<Ts...>,
				Args> : subst_<defer<C, Ts...>, Args>
			{};
			tpl<typ... Bs,
				bool IsVar,
				typ Args> struct impl<lambda_<list<Bs...>, IsVar>, Args>
			{
				using type
					= list<compose<typ lambda_<list<As..., Bs...>, true>::thunk,
						bind_front<quote<concat>, Args>,
						curry<bind_front<quote<substitutions>, list<Bs...>>>>>;
			};
			struct thunk
			{
				tpl<typ S, typ R = _t<impl<back<Tags>, S>>> using invoke
					= if_c<size<R>{} == 1, front<R>>;
			};

		public:
			tpl<typ... Ts> using invoke
				= invoke<thunk, substitutions<Tags, list<Ts...>>>;
		};
	} // namespace detail

	tpl<typ... Ts> using lambda
		= if_c<(sizeof...(Ts) > 0), detail::lambda_<list<Ts...>>>;

	tpl<typ T> using is_valid = detail::is_valid_<T>;

	tpl<typ T> using vararg = detail::vararg_<T>;

	tpl<typ T> using protect = detail::protect_<T>;

	tpl<typ Tag, typ Value> struct var;

	namespace detail
	{
		tpl<typ... As> struct let_
		{};
		tpl<typ Fn> struct let_<Fn> : type_identity<lazy::invoke<lambda<Fn>>>
		{};
		tpl<typ Tag, typ Value, typ... Rest> struct let_<var<Tag, Value>,
			Rest...>
			: type_identity<lazy::invoke<lambda<Tag, _t<let_<Rest...>>>, Value>>
		{};
	} // namespace detail

	tpl<typ... As> using let = _t<_t<detail::let_<As...>>>;

	namespace lazy
	{
		tpl<typ... As> using let = defer<let, As...>;
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
		tpl<typ M2, typ M> struct cartesian_product_fn
		{
			tpl<typ X> struct lambda0
			{
				tpl<typ Xs> using lambda1 = list<push_front<Xs, X>>;
				using type = join<transform<M2, quote<lambda1>>>;
			};
			using type = join<transform<M, quote_trait<lambda0>>>;
		};
	} // namespace detail

	tpl<typ ListOfLists> using cartesian_product = reverse_fold<ListOfLists,
		list<list<>>,
		quote_trait<detail::cartesian_product_fn>>;

	namespace lazy
	{
		tpl<typ ListOfLists> using cartesian_product
			= defer<cartesian_product, ListOfLists>;
	} // namespace lazy

	namespace detail
	{
		tpl<bool> struct add_const_if
		{
			tpl<typ T> using invoke = T const;
		};
		tpl<> struct add_const_if<false>
		{
			tpl<typ T> using invoke = T;
		};
	} // namespace detail
	tpl<bool If> using add_const_if_c = detail::add_const_if<If>;
	tpl<typ If> using add_const_if = add_const_if_c<If::type::value>;

	tpl<bool If, typ T> using const_if_c = invoke<add_const_if_c<If>, T>;
	tpl<typ If, typ T> using const_if = invoke<add_const_if<If>, T>;

	namespace detail
	{
		tpl<typ State, typ Ch> using atoi_
			= if_c<(Ch::value >= '0' && Ch::value <= '9'),
				integral_constant<typ State::value_type,
					State::value * 10 + (Ch::value - '0')>>;
	}

	inline namespace literals
	{
		tpl<char... Chs>
			cexp fold<list<char_<Chs>...>, size_t_<0>, quote<detail::atoi_>>
			operator"" _z()
		{
			return {};
		}
	} // namespace literals

	tpl<typ... Ts> cexp int
	ignore_unused(Ts&&...)
	{
		return 42;
	}

} // namespace Rider::Faiz::meta


#endif // __RANGE_H_
