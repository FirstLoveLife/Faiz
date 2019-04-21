#ifndef __RANGE_H_
#define __RANGE_H_
#include "rider/faiz/faiz_fwd_meta.hpp"

namespace Rider::Faiz::meta
{


	Tpl<Typ... Ts> struct list;

	Tpl<Typ T> struct id;

	Tpl<Tpl<Typ...> class> struct quote;

	Tpl<Typ T, Tpl<T...> class F> struct quote_i;

	Tpl<Typ... Fs> struct compose;

	Tpl<Tpl<Typ...> class C, Typ... Ts> struct defer;

	Tpl<Typ T, Tpl<T...> class C, T... Is> struct defer_i;

	namespace extension
	{
		Tpl<Typ F, Typ List> struct apply;
	}

} // namespace Rider::Faiz::meta
namespace Rider::Faiz::meta
{

	namespace detail
	{
		Tpl<Typ T> cexp T*
		_nullptr_v()
		{
			return nullptr;
		}
	} // namespace detail

	struct nil_
	{};

	namespace lazy
	{
		Tpl<Typ T> using _t = defer<_t, T>;
	} // namespace lazy

	Tpl<Typ T> using inc
		= integral_constant<decltype(T::type::value + 1), T::type::value + 1>;

	Tpl<Typ T> using dec
		= integral_constant<decltype(T::type::value - 1), T::type::value - 1>;

	Tpl<Typ T, Typ U> using plus
		= integral_constant<decltype(T::type::value + U::type::value),
			T::type::value + U::type::value>;

	Tpl<Typ T, Typ U> using minus
		= integral_constant<decltype(T::type::value - U::type::value),
			T::type::value - U::type::value>;

	Tpl<Typ T, Typ U> using multiplies
		= integral_constant<decltype(T::type::value * U::type::value),
			T::type::value * U::type::value>;

	Tpl<Typ T, Typ U> using divides
		= integral_constant<decltype(T::type::value / U::type::value),
			T::type::value / U::type::value>;

	Tpl<Typ T> using negate
		= integral_constant<decltype(-T::type::value), -T::type::value>;

	Tpl<Typ T, Typ U> using modulus
		= integral_constant<decltype(T::type::value % U::type::value),
			T::type::value % U::type::value>;

	Tpl<Typ T, Typ U> using equal_to = bool_<T::type::value == U::type::value>;

	Tpl<Typ T, Typ U> using not_equal_to
		= bool_<T::type::value != U::type::value>;

	Tpl<Typ T, Typ U> using greater = bool_<(T::type::value > U::type::value)>;

	Tpl<Typ T, Typ U> using less = bool_<(T::type::value < U::type::value)>;

	Tpl<Typ T, Typ U> using greater_equal
		= bool_<(T::type::value >= U::type::value)>;

	Tpl<Typ T, Typ U> using less_equal
		= bool_<(T::type::value <= U::type::value)>;

	Tpl<Typ T, Typ U> using bit_and
		= integral_constant<decltype(T::type::value & U::type::value),
			T::type::value & U::type::value>;

	Tpl<Typ T, Typ U> using bit_or
		= integral_constant<decltype(T::type::value | U::type::value),
			T::type::value | U::type::value>;

	Tpl<Typ T, Typ U> using bit_xor
		= integral_constant<decltype(T::type::value ^ U::type::value),
			T::type::value ^ U::type::value>;

	Tpl<Typ T> using bit_not
		= integral_constant<decltype(~T::type::value), ~T::type::value>;

	namespace lazy
	{
		Tpl<Typ T> using inc = defer<inc, T>;

		Tpl<Typ T> using dec = defer<dec, T>;

		Tpl<Typ T, Typ U> using plus = defer<plus, T, U>;

		Tpl<Typ T, Typ U> using minus = defer<minus, T, U>;

		Tpl<Typ T, Typ U> using multiplies = defer<multiplies, T, U>;

		Tpl<Typ T, Typ U> using divides = defer<divides, T, U>;

		Tpl<Typ T> using negate = defer<negate, T>;

		Tpl<Typ T, Typ U> using modulus = defer<modulus, T, U>;

		Tpl<Typ T, Typ U> using equal_to = defer<equal_to, T, U>;

		Tpl<Typ T, Typ U> using not_equal_to = defer<not_equal_to, T, U>;

		Tpl<Typ T, Typ U> using greater = defer<greater, T, U>;

		Tpl<Typ T, Typ U> using less = defer<less, T, U>;

		Tpl<Typ T, Typ U> using greater_equal = defer<greater_equal, T, U>;

		Tpl<Typ T, Typ U> using less_equal = defer<less_equal, T, U>;

		Tpl<Typ T, Typ U> using bit_and = defer<bit_and, T, U>;

		Tpl<Typ T, Typ U> using bit_or = defer<bit_or, T, U>;

		Tpl<Typ T, Typ U> using bit_xor = defer<bit_xor, T, U>;

		Tpl<Typ T> using bit_not = defer<bit_not, T>;
	} // namespace lazy


	Tpl<Typ F, Typ... Args> using invoke = Typ F::Tpl invoke<Args...>;

	namespace lazy
	{
		Tpl<Typ F, Typ... Args> using invoke = defer<invoke, F, Args...>;
	} // namespace lazy

	Tpl<Typ T> struct id : type_identity<T>
	{
		Tpl<Typ...> using invoke = T;
	};

	Tpl<Typ T> using id_t = _t<id<T>>;

	namespace lazy
	{
		Tpl<Typ T> using id = defer<id, T>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ, Typ = void> struct is_trait_ : type_identity<false_>
		{};

		Tpl<Typ T> struct is_trait_<T, void_t<Typ T::type>>
			: type_identity<true_>
		{};

		Tpl<Typ, Typ = void> struct is_callable_ : type_identity<false_>
		{};

		Tpl<Typ T> struct is_callable_<T, void_t<quote<T::Tpl invoke>>>
			: type_identity<true_>
		{};

		Tpl<Tpl<Typ...> class C, Typ... Ts, Tpl<Typ...> class D = C>
			id<D<Ts...>>
			try_defer_(int);
		Tpl<Tpl<Typ...> class C, Typ... Ts> nil_
		try_defer_(long);

		Tpl<Tpl<Typ...> class C, Typ... Ts> using defer_
			= decltype(detail::try_defer_<C, Ts...>(0));

		Tpl<Typ T, Tpl<T...> class C, T... Is, Tpl<T...> class D = C>
			id<D<Is...>>
			try_defer_i_(int);
		Tpl<Typ T, Tpl<T...> class C, T... Is> nil_
		try_defer_i_(long);

		Tpl<Typ T, Tpl<T...> class C, T... Is> using defer_i_
			= decltype(detail::try_defer_i_<T, C, Is...>(0));

		Tpl<Typ T> using _t_t = _t<_t<T>>;
	} // namespace detail

	Tpl<Typ T> using is_trait = _t<detail::is_trait_<T>>;

	Tpl<Typ T> using is_callable = _t<detail::is_callable_<T>>;

	Tpl<Tpl<Typ...> class C, Typ... Ts> struct defer : detail::defer_<C, Ts...>
	{};

	Tpl<Typ T, Tpl<T...> class C, T... Is> struct defer_i
		: detail::defer_i_<T, C, Is...>
	{};

	Tpl<Tpl<Typ...> class C, Typ... Ts> using defer_trait
		= defer<detail::_t_t, detail::defer_<C, Ts...>>;

	Tpl<Typ T> using sizeof_ = size_t_<sizeof(T)>;

	Tpl<Typ T> using alignof_ = size_t_<alignof(T)>;

	namespace lazy
	{
		Tpl<Typ T> using sizeof_ = defer<sizeof_, T>;

		Tpl<Typ T> using alignof_ = defer<alignof_, T>;
	} // namespace lazy


	namespace detail
	{
		Tpl<Typ, Tpl<Typ...> class> struct is_ : false_
		{};

		Tpl<Typ... Ts, Tpl<Typ...> class C> struct is_<C<Ts...>, C> : true_
		{};
	} // namespace detail


	Tpl<Typ T, Tpl<Typ...> class C> using is = _t<detail::is_<T, C>>;


	Tpl<Typ... Fs> struct compose
	{};

	Tpl<Typ F0> struct compose<F0>
	{
		Tpl<Typ... Ts> using invoke = invoke<F0, Ts...>;
	};

	Tpl<Typ F0, Typ... Fs> struct compose<F0, Fs...>
	{
		Tpl<Typ... Ts> using invoke = invoke<F0, invoke<compose<Fs...>, Ts...>>;
	};

	namespace lazy
	{
		Tpl<Typ... Fns> using compose = defer<compose, Fns...>;
	} // namespace lazy

	Tpl<Tpl<Typ...> class C> struct quote
	{
		// Indirection through defer here needed to avoid Core issue 1430
		// http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
		Tpl<Typ... Ts> using invoke = _t<detail::defer_<C, Ts...>>;
	};

	Tpl<Typ T, Tpl<T...> class C> struct quote_i
	{
		// Indirection through defer_i here needed to avoid Core issue 1430
		// http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
		Tpl<Typ... Ts> using invoke
			= _t<detail::defer_i_<T, C, Ts::type::value...>>;
	};

	Tpl<Tpl<Typ...> class C> using quote_trait = compose<quote<_t>, quote<C>>;

	Tpl<Typ T, Tpl<T...> class C> using quote_trait_i
		= compose<quote<_t>, quote_i<T, C>>;

	Tpl<Typ F, Typ... Ts> struct bind_front
	{
		Tpl<Typ... Us> using invoke = invoke<F, Ts..., Us...>;
	};

	Tpl<Typ F, Typ... Us> struct bind_back
	{
		Tpl<Typ... Ts> using invoke = invoke<F, Ts..., Us...>;
	};

	namespace lazy
	{
		Tpl<Typ Fn, Typ... Ts> using bind_front = defer<bind_front, Fn, Ts...>;

		Tpl<Typ Fn, Typ... Ts> using bind_back = defer<bind_back, Fn, Ts...>;
	} // namespace lazy

	namespace extension
	{
		Tpl<Typ F, Typ List> struct apply
		{};

		Tpl<Typ F, Typ Ret, Typ... Args> struct apply<F, Ret(Args...)>
			: lazy::invoke<F, Ret, Args...>
		{};

		Tpl<Typ F, Tpl<Typ...> Typ T, Typ... Ts> struct apply<F, T<Ts...>>
			: lazy::invoke<F, Ts...>
		{};

		Tpl<Typ F, Typ T, T... Is> struct apply<F, integer_sequence<T, Is...>>
			: lazy::invoke<F, integral_constant<T, Is>...>
		{};
	} // namespace extension

	Tpl<Typ C, Typ List> using apply = _t<extension::apply<C, List>>;

	namespace lazy
	{
		Tpl<Typ F, Typ List> using apply = defer<apply, F, List>;
	}

	Tpl<Typ F, Typ Q = quote<list>> using curry = compose<F, Q>;

	Tpl<Typ F> using uncurry = bind_front<quote<apply>, F>;

	namespace lazy
	{
		Tpl<Typ F, Typ Q = quote<list>> using curry = defer<curry, F, Q>;

		Tpl<Typ F> using uncurry = defer<uncurry, F>;
	} // namespace lazy

	Tpl<Typ F> struct flip
	{
	private:
		Tpl<Typ... Ts> struct impl
		{};
		Tpl<Typ A, Typ B, Typ... Ts> struct impl<A, B, Ts...>
			: lazy::invoke<F, B, A, Ts...>
		{};

	public:
		Tpl<Typ... Ts> using invoke = _t<impl<Ts...>>;
	};

	namespace lazy
	{
		Tpl<Typ F> using flip = defer<flip, F>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ...> struct on_
		{};
		Tpl<Typ F, Typ... Gs> struct on_<F, Gs...>
		{
			Tpl<Typ... Ts> using invoke
				= invoke<F, invoke<compose<Gs...>, Ts>...>;
		};
	} // namespace detail

	Tpl<Typ... Fs> using on = detail::on_<Fs...>;

	namespace lazy
	{
		Tpl<Typ F, Typ G> using on = defer<on, F, G>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ, Typ = bool> struct _if_
		{};

		Tpl<Typ If> struct _if_<list<If>, decltype(bool(If::type::value))>
			: enable_if<If::type::value>
		{};

		Tpl<Typ If, Typ Then> struct _if_<list<If, Then>,
			decltype(bool(If::type::value))> : enable_if<If::type::value, Then>
		{};

		Tpl<Typ If, Typ Then, Typ Else> struct _if_<list<If, Then, Else>,
			decltype(bool(If::type::value))>
			: conditional<If::type::value, Then, Else>
		{};
	} // namespace detail

	Tpl<Typ... Args> using if_ = _t<detail::_if_<list<Args...>>>;

	Tpl<bool If, Typ... Args> using if_c
		= _t<detail::_if_<list<bool_<If>, Args...>>>;

	namespace lazy
	{
		Tpl<Typ... Args> using if_ = defer<if_, Args...>;

		Tpl<bool If, Typ... Args> using if_c = if_<bool_<If>, Args...>;
	} // namespace lazy

	namespace detail
	{
		Tpl<bool> struct _and_
		{
			Tpl<class...> using invoke = true_;
		};

		Tpl<> struct _and_<false>
		{
			Tpl<Typ Bool, Typ... Bools> using invoke
				= invoke<if_c<!Bool::type::value,
							 id<false_>,
							 _and_<0 == sizeof...(Bools)>>,
					Bools...>;
		};

		Tpl<bool> struct _or_
		{
			Tpl<class = void> using invoke = false_;
		};

		Tpl<> struct _or_<false>
		{
			Tpl<Typ Bool_, Typ... Bools> using invoke = invoke<
				if_c<Bool_::type::value, id<true_>, _or_<0 == sizeof...(Bools)>>,
				Bools...>;
		};
	} // namespace detail

	Tpl<bool Bool_> using not_c = bool_<!Bool_>;

	Tpl<Typ Bool_> using not_ = not_c<Bool_::type::value>;

	// Tpl<bool... Bools>
	// using and_c = is_same<integer_sequence<bool, Bools...>,
	// 	integer_sequence<bool, (Bools || true)...>>;
	Tpl<bool... B> using and_c = bool_<(B && ...)>;
	Tpl<bool... B> inline cexp bool and_t = bool_<(B && ...)>::value;

	Tpl<Typ... Bools> using strict_and = and_c<Bools::type::value...>;
	Tpl<Typ... Bools> inline cexp bool strict_and_v
		= and_c<Bools::type::value...>::value;

	Tpl<Typ... Bools> using and_
		= _t<defer<detail::_and_<0 == sizeof...(Bools)>::Tpl invoke, Bools...>>;


	Tpl<bool... B> struct fold_and : bool_<(B && ...)>
	{};

	Tpl<bool... Bools> using or_c
		= not_<is_same<integer_sequence<bool, Bools...>,
			integer_sequence<bool, (Bools && false)...>>>;

	Tpl<Typ... Bools> using strict_or = or_c<Bools::type::value...>;

	Tpl<Typ... Bools> using or_
		= _t<defer<detail::_or_<0 == sizeof...(Bools)>::Tpl invoke, Bools...>>;

	namespace lazy
	{
		Tpl<Typ... Bools> using and_ = defer<and_, Bools...>;

		Tpl<Typ... Bools> using or_ = defer<or_, Bools...>;

		Tpl<Typ Bool_> using not_ = defer<not_, Bool_>;

		Tpl<Typ... Bools> using strict_and = defer<strict_and, Bools...>;

		Tpl<Typ... Bools> using strict_or = defer<strict_or, Bools...>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ Fun, Typ T0> struct compose1_
		{
			Tpl<Typ X> using invoke = invoke<Fun, _t<X>, T0>;
		};

		Tpl<Typ Fun,
			Typ T0,
			Typ T1,
			Typ T2,
			Typ T3,
			Typ T4,
			Typ T5,
			Typ T6,
			Typ T7,
			Typ T8,
			Typ T9> struct compose10_
		{
			Tpl<Typ X, Typ Y> using F = invoke<Fun, X, Y>;

			Tpl<Typ S> using invoke = F<
				F<F<F<F<F<F<F<F<F<_t<S>, T0>, T1>, T2>, T3>, T4>, T5>, T6>, T7>,
					T8>,
				T9>;
		};

		Tpl<Typ, Typ, Typ> struct fold_
		{};

		Tpl<Typ State, Typ Fun> struct fold_<list<>, State, Fun> : State
		{};

		Tpl<Typ Head,
			Typ... List,
			Typ State,
			Typ Fun> struct fold_<list<Head, List...>, State, Fun>
			: fold_<list<List...>,
				  lazy::invoke<compose1_<Fun, Head>, State>,
				  Fun>
		{};

		Tpl<Typ T0,
			Typ T1,
			Typ T2,
			Typ T3,
			Typ T4,
			Typ T5,
			Typ T6,
			Typ T7,
			Typ T8,
			Typ T9,
			Typ... List,
			Typ State,
			Typ Fun> struct
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

	Tpl<Typ List, Typ State, Typ Fun> using fold
		= _t<detail::fold_<List, id<State>, Fun>>;

	Tpl<Typ List, Typ State, Typ Fun> using accumulate = fold<List, State, Fun>;

	namespace lazy
	{
		Tpl<Typ List, Typ State, Typ Fun> using fold
			= defer<fold, List, State, Fun>;

		Tpl<Typ List, Typ State, Typ Fun> using accumulate
			= defer<accumulate, List, State, Fun>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ List, Typ State, Typ Fun> struct reverse_fold_
		{};

		Tpl<Typ State, Typ Fun> struct reverse_fold_<list<>, State, Fun>
			: type_identity<State>
		{};

		Tpl<Typ Head,
			Typ... List,
			Typ State,
			Typ Fun> struct reverse_fold_<list<Head, List...>, State, Fun>
			: lazy::invoke<compose1_<Fun, Head>,
				  reverse_fold_<list<List...>, State, Fun>>
		{};

		Tpl<Typ T0,
			Typ T1,
			Typ T2,
			Typ T3,
			Typ T4,
			Typ T5,
			Typ T6,
			Typ T7,
			Typ T8,
			Typ T9,
			Typ... List,
			Typ State,
			Typ Fun> struct
			reverse_fold_<list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, List...>,
				State,
				Fun>
			: lazy::invoke<
				  compose10_<Fun, T9, T8, T7, T6, T5, T4, T3, T2, T1, T0>,
				  reverse_fold_<list<List...>, State, Fun>>
		{};
	} // namespace detail

	Tpl<Typ List, Typ State, Typ Fun> using reverse_fold
		= _t<detail::reverse_fold_<List, State, Fun>>;

	namespace lazy
	{
		Tpl<Typ List, Typ State, Typ Fun> using reverse_fold
			= defer<reverse_fold, List, State, Fun>;
	} // namespace lazy

	using npos = size_t_<size_t(-1)>;

	Tpl<Typ... Ts> struct list
	{
		using type = list;
		/// \return `sizeof...(Ts)`
		static cexp size_t
		size() noexcept
		{
			return sizeof...(Ts);
		}
	};

	Tpl<Typ List> using size = size_t_<List::size()>;

	namespace lazy
	{
		Tpl<Typ List> using size = defer<size, List>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ... Lists> struct concat_
		{};

		Tpl<> struct concat_<> : type_identity<list<>>
		{};

		Tpl<Typ... List1> struct concat_<list<List1...>>
			: type_identity<list<List1...>>
		{};

		Tpl<Typ... List1,
			Typ... List2,
			Typ... Rest> struct concat_<list<List1...>, list<List2...>, Rest...>
			: concat_<list<List1..., List2...>, Rest...>
		{};

		Tpl<Typ... List1,
			Typ... List2,
			Typ... List3,
			Typ... List4,
			Typ... List5,
			Typ... List6,
			Typ... List7,
			Typ... List8,
			Typ... List9,
			Typ... List10,
			Typ... Rest> struct concat_<list<List1...>,
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

	Tpl<Typ... Lists> using concat = _t<detail::concat_<Lists...>>;

	namespace lazy
	{
		Tpl<Typ... Lists> using concat = defer<concat, Lists...>;
	} // namespace lazy

	Tpl<Typ ListOfLists> using join = apply<quote<concat>, ListOfLists>;

	namespace lazy
	{
		Tpl<Typ ListOfLists> using join = defer<join, ListOfLists>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ, Typ = void> struct transform_
		{};

		Tpl<Typ... Ts, Typ Fun> struct transform_<list<list<Ts...>, Fun>,
			void_t<invoke<Fun, Ts>...>>
			: type_identity<list<invoke<Fun, Ts>...>>
		{};

		Tpl<Typ... Ts0, Typ... Ts1, Typ Fun> struct transform_<
			list<list<Ts0...>, list<Ts1...>, Fun>,
			void_t<invoke<Fun, Ts0, Ts1>...>>
			: type_identity<list<invoke<Fun, Ts0, Ts1>...>>
		{};
	} // namespace detail

	Tpl<Typ... Args> using transform = _t<detail::transform_<list<Args...>>>;

	namespace lazy
	{
		Tpl<Typ... Args> using transform = defer<transform, Args...>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ T, size_t> using first_ = T;

		Tpl<Typ T, Typ Ints> struct repeat_n_c_
		{};

		Tpl<Typ T, size_t... Is> struct repeat_n_c_<T, index_sequence<Is...>>
			: type_identity<list<first_<T, Is>...>>
		{};
	} // namespace detail

	Tpl<size_t N, Typ T = void> using repeat_n_c
		= _t<detail::repeat_n_c_<T, make_index_sequence<N>>>;

	Tpl<Typ N, Typ T = void> using repeat_n = repeat_n_c<N::type::value, T>;

	namespace lazy
	{
		Tpl<Typ N, Typ T = void> using repeat_n = defer<repeat_n, N, T>;

		Tpl<size_t N, Typ T = void> using repeat_n_c
			= defer<repeat_n, size_t_<N>, T>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ VoidPtrs> struct at_impl_;

		Tpl<Typ... VoidPtrs> struct at_impl_<list<VoidPtrs...>>
		{
			static nil_
			eval(...);

			Tpl<Typ T, Typ... Us> static T
			eval(VoidPtrs..., T*, Us*...);
		};

		Tpl<Typ List, size_t N> struct at_
		{};

		Tpl<Typ... Ts, size_t N> struct at_<list<Ts...>, N>
			: decltype(at_impl_<repeat_n_c<N, void*>>::eval(
				  static_cast<id<Ts>*>(nullptr)...))
		{};
	} // namespace detail

	Tpl<Typ List, size_t N> using at_c = _t<detail::at_<List, N>>;

	Tpl<Typ List, Typ N> using at = at_c<List, N::type::value>;

	namespace lazy
	{
		Tpl<Typ List, Typ N> using at = defer<at, List, N>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ VoidPtrs> struct drop_impl_
		{
			static nil_
			eval(...);
		};

		Tpl<Typ... VoidPtrs> struct drop_impl_<list<VoidPtrs...>>
		{
			static nil_
			eval(...);

			Tpl<Typ... Ts> static id<list<Ts...>>
			eval(VoidPtrs..., id<Ts>*...);
		};

		Tpl<> struct drop_impl_<list<>>
		{
			Tpl<Typ... Ts> static id<list<Ts...>>
			eval(id<Ts>*...);
		};

		Tpl<Typ List, Typ N> struct drop_
		{};

		Tpl<Typ... Ts, Typ N> struct drop_<list<Ts...>, N>
			: decltype(drop_impl_<repeat_n<N, void*>>::eval(
				  detail::_nullptr_v<id<Ts>>()...))
		{};
	} // namespace detail

	Tpl<Typ List, Typ N> using drop = _t<detail::drop_<List, N>>;

	Tpl<Typ List, size_t N> using drop_c = _t<detail::drop_<List, size_t_<N>>>;

	namespace lazy
	{
		Tpl<Typ List, Typ N> using drop = defer<drop, List, N>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ List> struct front_
		{};

		Tpl<Typ Head, Typ... List> struct front_<list<Head, List...>>
			: type_identity<Head>
		{};
	} // namespace detail

	Tpl<Typ List> using front = _t<detail::front_<List>>;

	namespace lazy
	{
		Tpl<Typ List> using front = defer<front, List>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ List> struct back_
		{};

		Tpl<Typ Head, Typ... List> struct back_<list<Head, List...>>
			: type_identity<at_c<list<Head, List...>, sizeof...(List)>>
		{};
	} // namespace detail

	Tpl<Typ List> using back = _t<detail::back_<List>>;

	namespace lazy
	{
		Tpl<Typ List> using back = defer<back, List>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ List, Typ T> struct push_front_
		{};

		Tpl<Typ... List, Typ T> struct push_front_<list<List...>, T>
			: type_identity<list<T, List...>>
		{};
	} // namespace detail

	Tpl<Typ List, Typ T> using push_front = _t<detail::push_front_<List, T>>;

	namespace lazy
	{
		Tpl<Typ List, Typ T> using push_front = defer<push_front, List, T>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ List> struct pop_front_
		{};

		Tpl<Typ Head, Typ... List> struct pop_front_<list<Head, List...>>
			: type_identity<list<List...>>
		{};
	} // namespace detail

	Tpl<Typ List> using pop_front = _t<detail::pop_front_<List>>;

	namespace lazy
	{
		Tpl<Typ List> using pop_front = defer<pop_front, List>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ List, Typ T> struct push_back_
		{};

		Tpl<Typ... List, Typ T> struct push_back_<list<List...>, T>
			: type_identity<list<List..., T>>
		{};
	} // namespace detail

	Tpl<Typ List, Typ T> using push_back = _t<detail::push_back_<List, T>>;

	namespace lazy
	{
		Tpl<Typ List, Typ T> using push_back = defer<push_back, List, T>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ T, Typ U> using min_ = if_<less<U, T>, U, T>;

		Tpl<Typ T, Typ U> using max_ = if_<less<U, T>, T, U>;
	} // namespace detail

	Tpl<Typ... Ts> using min
		= fold<pop_front<list<Ts...>>, front<list<Ts...>>, quote<detail::min_>>;

	Tpl<Typ... Ts> using max
		= fold<pop_front<list<Ts...>>, front<list<Ts...>>, quote<detail::max_>>;

	namespace lazy
	{
		Tpl<Typ... Ts> using min = defer<min, Ts...>;

		Tpl<Typ... Ts> using max = defer<max, Ts...>;
	} // namespace lazy

	Tpl<Typ List> using empty = bool_<0 == size<List>::type::value>;

	namespace lazy
	{
		Tpl<Typ List> using empty = defer<empty, List>;
	} // namespace lazy

	Tpl<Typ F, Typ S> using pair = list<F, S>;

	Tpl<Typ Pair> using first = front<Pair>;

	Tpl<Typ Pair> using second = front<pop_front<Pair>>;

	namespace lazy
	{
		Tpl<Typ Pair> using first = defer<first, Pair>;

		Tpl<Typ Pair> using second = defer<second, Pair>;
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

		Tpl<Typ List, Typ T> struct find_index_
		{};

		Tpl<Typ V> struct find_index_<list<>, V> : type_identity<npos>
		{};

		Tpl<Typ... T, Typ V> struct find_index_<list<T...>, V>
		{
			static cexp bool s_v[] = {is_same<T, V>::value...};
			using type = size_t_<find_index_i_(s_v, s_v + sizeof...(T))>;
		};
	} // namespace detail

	Tpl<Typ List, Typ T> using find_index = _t<detail::find_index_<List, T>>;

	namespace lazy
	{
		Tpl<Typ List, Typ T> using find_index = defer<find_index, List, T>;
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

		Tpl<Typ List, Typ T> struct reverse_find_index_
		{};

		Tpl<Typ V> struct reverse_find_index_<list<>, V> : type_identity<npos>
		{};

		Tpl<Typ... T, Typ V> struct reverse_find_index_<list<T...>, V>
		{
			static cexp bool s_v[] = {is_same<T, V>::value...};
			using type = size_t_<reverse_find_index_i_(
				s_v, s_v + sizeof...(T), sizeof...(T))>;
		};
	} // namespace detail

	Tpl<Typ List, Typ T> using reverse_find_index
		= _t<detail::reverse_find_index_<List, T>>;

	namespace lazy
	{
		Tpl<Typ List, Typ T> using reverse_find_index
			= defer<reverse_find_index, List, T>;
	} // namespace lazy

	Tpl<Typ List, Typ T> using find
		= drop<List, min<find_index<List, T>, size<List>>>;

	namespace lazy
	{
		Tpl<Typ List, Typ T> using find = defer<find, List, T>;
	} // namespace lazy

	Tpl<Typ List, Typ T> using reverse_find
		= drop<List, min<reverse_find_index<List, T>, size<List>>>;

	namespace lazy
	{
		Tpl<Typ List, Typ T> using reverse_find = defer<reverse_find, List, T>;
	} // namespace lazy

	namespace detail
	{
		cexp bool const*
		find_if_i_(bool const* const begin, bool const* const end)
		{
			return begin == end || *begin ? begin : find_if_i_(begin + 1, end);
		}

		Tpl<Typ List, Typ Fun, Typ = void> struct find_if_
		{};

		Tpl<Typ Fun> struct find_if_<list<>, Fun> : type_identity<list<>>
		{};

		Tpl<Typ... List, Typ Fun> struct find_if_<list<List...>,
			Fun,
			void_t<integer_sequence<bool,
				bool(invoke<Fun, List>::type::value)...>>>
		{
			static cexp bool s_v[] = {invoke<Fun, List>::type::value...};
			using type = drop_c<list<List...>,
				detail::find_if_i_(s_v, s_v + sizeof...(List)) - s_v>;
		};
	} // namespace detail

	Tpl<Typ List, Typ Fun> using find_if = _t<detail::find_if_<List, Fun>>;

	namespace lazy
	{
		Tpl<Typ List, Typ Fun> using find_if = defer<find_if, List, Fun>;
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

		Tpl<Typ List, Typ Fun, Typ = void> struct reverse_find_if_
		{};

		Tpl<Typ Fun> struct reverse_find_if_<list<>, Fun>
			: type_identity<list<>>
		{};

		Tpl<Typ... List, Typ Fun> struct reverse_find_if_<list<List...>,
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

	Tpl<Typ List, Typ Fun> using reverse_find_if
		= _t<detail::reverse_find_if_<List, Fun>>;

	namespace lazy
	{
		Tpl<Typ List, Typ Fun> using reverse_find_if
			= defer<reverse_find_if, List, Fun>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ List, Typ T, Typ U> struct replace_
		{};

		Tpl<Typ... List, Typ T, Typ U> struct replace_<list<List...>, T, U>
			: type_identity<list<if_<is_same<T, List>, U, List>...>>
		{};
	} // namespace detail

	Tpl<Typ List, Typ T, Typ U> using replace
		= _t<detail::replace_<List, T, U>>;

	namespace lazy
	{
		Tpl<Typ List, Typ T, Typ U> using replace = defer<replace, T, U>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ List, Typ C, Typ U, Typ = void> struct replace_if_
		{};

		Tpl<Typ... List, Typ C, Typ U> struct replace_if_<list<List...>,
			C,
			U,
			void_t<
				integer_sequence<bool, bool(invoke<C, List>::type::value)...>>>
		{
			using type = list<if_<invoke<C, List>, U, List>...>;
		};
	} // namespace detail

	Tpl<Typ List, Typ C, Typ U> using replace_if
		= _t<detail::replace_if_<List, C, U>>;

	namespace lazy
	{
		Tpl<Typ List, Typ C, Typ U> using replace_if = defer<replace_if, C, U>;
	} // namespace lazy

	namespace detail
	{
		cexp size_t
		count_i_(bool const* const begin, bool const* const end, size_t n)
		{
			return begin == end ? n :
								  detail::count_i_(begin + 1, end, n + *begin);
		}

		Tpl<Typ List, Typ T, Typ = void> struct count_
		{};

		Tpl<Typ T> struct count_<list<>, T> : type_identity<size_t_<0>>
		{};

		Tpl<Typ... List, Typ T> struct count_<list<List...>, T>
		{
			static cexp bool s_v[] = {is_same<T, List>::value...};
			using type
				= size_t_<detail::count_i_(s_v, s_v + sizeof...(List), 0u)>;
		};
	} // namespace detail

	Tpl<Typ List, Typ T> using count = _t<detail::count_<List, T>>;

	namespace lazy
	{
		Tpl<Typ List, Typ T> using count = defer<count, List, T>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ List, Typ Fn, Typ = void> struct count_if_
		{};

		Tpl<Typ Fn> struct count_if_<list<>, Fn> : type_identity<size_t_<0>>
		{};

		Tpl<Typ... List, Typ Fn> struct count_if_<list<List...>,
			Fn,
			void_t<
				integer_sequence<bool, bool(invoke<Fn, List>::type::value)...>>>
		{
			static cexp bool s_v[] = {invoke<Fn, List>::type::value...};
			using type
				= size_t_<detail::count_i_(s_v, s_v + sizeof...(List), 0u)>;
		};
	} // namespace detail

	Tpl<Typ List, Typ Fn> using count_if = _t<detail::count_if_<List, Fn>>;

	namespace lazy
	{
		Tpl<Typ List, Typ Fn> using count_if = defer<count_if, List, Fn>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ Pred> struct filter_
		{
			Tpl<Typ A> using invoke
				= if_c<invoke<Pred, A>::type::value, list<A>, list<>>;
		};
	} // namespace detail

	Tpl<Typ List, Typ Pred> using filter
		= join<transform<List, detail::filter_<Pred>>>;

	namespace lazy
	{
		Tpl<Typ List, Typ Pred> using filter = defer<filter, List, Pred>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ T> struct static_const
		{
			static cexp T value{};
		};

		// Avoid potential ODR violations with global objects:
		Tpl<Typ T> cexp T static_const<T>::value;
	} // namespace detail

	namespace detail
	{
		struct for_each_fn
		{
			Tpl<class UnaryFunction, class... Args> cfn
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

	Tpl<Typ ListOfLists> using transpose = fold<ListOfLists,
		repeat_n<size<front<ListOfLists>>, list<>>,
		bind_back<quote<transform>, quote<push_back>>>;

	namespace lazy
	{
		Tpl<Typ ListOfLists> using transpose = defer<transpose, ListOfLists>;
	} // namespace lazy

	Tpl<Typ Fun, Typ ListOfLists> using zip_with
		= transform<transpose<ListOfLists>, uncurry<Fun>>;

	namespace lazy
	{
		Tpl<Typ Fun, Typ ListOfLists> using zip_with
			= defer<zip_with, Fun, ListOfLists>;
	} // namespace lazy

	Tpl<Typ ListOfLists> using zip = transpose<ListOfLists>;

	namespace lazy
	{
		Tpl<Typ ListOfLists> using zip = defer<zip, ListOfLists>;
	} // namespace lazy

	namespace detail
	{
		// Indirection here needed to avoid Core issue 1430
		// http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
		Tpl<Typ Sequence> struct as_list_
			: lazy::invoke<uncurry<quote<list>>, Sequence>
		{};
	} // namespace detail

	Tpl<Typ Sequence> using as_list
		= _t<detail::as_list_<remove_cvref_t<Sequence>>>;

	namespace lazy
	{
		Tpl<Typ Sequence> using as_list = defer<as_list, Sequence>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ List, Typ State = list<>> struct reverse_
			: lazy::fold<List, State, quote<push_front>>
		{};

		Tpl<Typ T0,
			Typ T1,
			Typ T2,
			Typ T3,
			Typ T4,
			Typ T5,
			Typ T6,
			Typ T7,
			Typ T8,
			Typ T9,
			Typ... Ts,
			Typ... Us> struct
			reverse_<list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, Ts...>,
				list<Us...>>
			: reverse_<list<Ts...>,
				  list<T9, T8, T7, T6, T5, T4, T3, T2, T1, T0, Us...>>
		{};
	} // namespace detail

	Tpl<Typ List> using reverse = _t<detail::reverse_<List>>;

	namespace lazy
	{
		Tpl<Typ List> using reverse = defer<reverse, List>;
	} // namespace lazy

	Tpl<Typ F> using not_fn = compose<quote<not_>, F>;

	namespace lazy
	{
		Tpl<Typ F> using not_fn = defer<not_fn, F>;
	} // namespace lazy

	Tpl<Typ List, Typ F> using all_of = empty<find_if<List, not_fn<F>>>;

	namespace lazy
	{
		/// \sa 'meta::all_of'
		/// \ingroup lazy_query
		Tpl<Typ List, Typ Fn> using all_of = defer<all_of, List, Fn>;
	} // namespace lazy

	Tpl<Typ List, Typ F> using any_of = not_<empty<find_if<List, F>>>;

	namespace lazy
	{
		Tpl<Typ List, Typ Fn> using any_of = defer<any_of, List, Fn>;
	} // namespace lazy

	Tpl<Typ List, Typ F> using none_of = empty<find_if<List, F>>;

	namespace lazy
	{
		Tpl<Typ List, Typ Fn> using none_of = defer<none_of, List, Fn>;
	} // namespace lazy

	Tpl<Typ List, Typ T> using in = not_<empty<find<List, T>>>;

	namespace lazy
	{
		Tpl<Typ List, Typ T> using in = defer<in, List, T>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ List> struct inherit_
		{};

		Tpl<Typ... List> struct inherit_<list<List...>> : List...
		{
			using type = inherit_;
		};
	} // namespace detail

	Tpl<Typ List> using inherit = _t<detail::inherit_<List>>;

	namespace lazy
	{
		Tpl<Typ List> using inherit = defer<inherit, List>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ Set, Typ T> struct in_
		{};

		Tpl<Typ... Set, Typ T> struct in_<list<Set...>, T>
			: Faiz::is_base_of<id<T>, inherit<list<id<Set>...>>>
		{};

		Tpl<Typ Set, Typ T> struct insert_back_
		{};

		Tpl<Typ... Set, Typ T> struct insert_back_<list<Set...>, T>
		{
			using type
				= if_<in_<list<Set...>, T>, list<Set...>, list<Set..., T>>;
		};
	} // namespace detail

	Tpl<Typ List> using unique
		= fold<List, list<>, quote_trait<detail::insert_back_>>;

	namespace lazy
	{
		Tpl<Typ List> using unique = defer<unique, List>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ Pred> struct partition_
		{
			Tpl<Typ, Typ, Typ = void> struct impl
			{};
			Tpl<Typ... Yes, Typ... No, Typ A> struct impl<
				pair<list<Yes...>, list<No...>>,
				A,
				void_t<bool_<invoke<Pred, A>::type::value>>>
			{
				using type = if_<invoke<Pred, A>,
					pair<list<Yes..., A>, list<No...>>,
					pair<list<Yes...>, list<No..., A>>>;
			};
			Tpl<Typ State, Typ A> using invoke = _t<impl<State, A>>;
		};
	} // namespace detail

	Tpl<Typ List, Typ Pred> using partition
		= fold<List, pair<list<>, list<>>, detail::partition_<Pred>>;

	namespace lazy
	{
		Tpl<Typ List, Typ Pred> using partition = defer<partition, List, Pred>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ, Typ, Typ = void> struct sort_
		{};

		Tpl<Typ Pred> struct sort_<list<>, Pred> : type_identity<list<>>
		{};

		Tpl<Typ A, Typ Pred> struct sort_<list<A>, Pred>
			: type_identity<list<A>>
		{};

		Tpl<Typ A, Typ B, Typ... List, Typ Pred> struct sort_<
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

	Tpl<Typ List, Typ Pred> using sort = _t<detail::sort_<List, Pred>>;

	namespace lazy
	{
		Tpl<Typ List, Typ Pred> using sort = defer<sort, List, Pred>;
	} // namespace lazy

	namespace detail
	{
		Tpl<Typ T, int = 0> struct protect_;

		Tpl<Typ, int = 0> struct vararg_;

		Tpl<Typ T, int = 0> struct is_valid_;

		// Returns which branch to evaluate
		Tpl<Typ If, Typ... Ts> using lazy_if_
			= lazy::_t<defer<_if_, list<If, protect_<Ts>...>>>;

		Tpl<Typ A, Typ T, Typ F, Typ Ts> struct subst1_
			: type_identity<list<list<T>>>
		{};
		Tpl<Typ T, Typ F, Typ Ts> struct subst1_<F, T, F, Ts>
			: type_identity<list<>>
		{};
		Tpl<Typ A, Typ T, Typ F, Typ Ts> struct subst1_<vararg_<A>, T, F, Ts>
			: type_identity<list<Ts>>
		{};

		Tpl<Typ As, Typ Ts> using substitutions_ = push_back<
			join<transform<
				concat<As, repeat_n_c<size<Ts>{} + 2 - size<As>{}, back<As>>>,
				concat<Ts, repeat_n_c<2, back<As>>>,
				bind_back<quote_trait<subst1_>,
					back<As>,
					drop_c<Ts, size<As>{} - 2>>>>,
			list<back<As>>>;

		Tpl<Typ As, Typ Ts> using substitutions
			= invoke<if_c<(size<Ts>{} + 2 >= size<As>{}), quote<substitutions_>>,
				As,
				Ts>;

		Tpl<Typ T> struct is_vararg_ : false_
		{};
		Tpl<Typ T> struct is_vararg_<vararg_<T>> : true_
		{};

		Tpl<Typ Tags> using is_variadic_
			= is_vararg_<at<push_front<Tags, void>, dec<size<Tags>>>>;

		Tpl<Typ Tags,
			bool IsVariadic = is_variadic_<Tags>::value> struct lambda_;

		// Non-variadic lambda implementation
		Tpl<Typ... As> struct lambda_<list<As...>, false>
		{
		private:
			static cexp size_t arity = sizeof...(As) - 1;
			using Tags
				= list<As...>; // Includes the lambda body as the last arg!
			using F = back<Tags>;
			Tpl<Typ T, Typ Args> struct impl;
			Tpl<Typ T, Typ Args> using lazy_impl_
				= lazy::_t<defer<impl, T, protect_<Args>>>;
			Tpl<Typ, Typ, Typ = void> struct subst_
			{};
			Tpl<Tpl<Typ...> class C, Typ... Ts, Typ Args> struct subst_<
				defer<C, Ts...>,
				Args,
				void_t<C<_t<impl<Ts, Args>>...>>>
				: type_identity<C<_t<impl<Ts, Args>>...>>
			{};
			Tpl<Typ T, Tpl<T...> class C, T... Is, Typ Args> struct subst_<
				defer_i<T, C, Is...>,
				Args,
				void_t<C<Is...>>> : type_identity<C<Is...>>
			{};
			Tpl<Typ T, Typ Args> struct impl
				: if_c<(reverse_find_index<Tags, T>() != npos()),
					  lazy::at<Args, reverse_find_index<Tags, T>>,
					  id<T>>
			{};
			Tpl<Typ T, Typ Args> struct impl<protect_<T>, Args>
				: type_identity<T>
			{};
			Tpl<Typ T, Typ Args> struct impl<is_valid_<T>, Args>
				: type_identity<is_trait<impl<T, Args>>>
			{};
			Tpl<Typ If, Typ... Ts, Typ Args> struct impl<defer<if_, If, Ts...>,
				Args> // Short-circuit
					  // if_
				: impl<lazy_impl_<lazy_if_<If, Ts...>, Args>, Args>
			{};
			Tpl<Typ Bool_, Typ... Ts, Typ Args> struct impl<
				defer<and_, Bool_, Ts...>,
				Args> // Short-circuit
					  // and_
				: impl<lazy_impl_<
						   lazy_if_<Bool_, lazy::and_<Ts...>, protect_<false_>>,
						   Args>,
					  Args>
			{};
			Tpl<Typ Bool_, Typ... Ts, Typ Args> struct impl<
				defer<or_, Bool_, Ts...>,
				Args> // Short-circuit or_
				: impl<lazy_impl_<
						   lazy_if_<Bool_, protect_<true_>, lazy::or_<Ts...>>,
						   Args>,
					  Args>
			{};
			Tpl<Tpl<Typ...> class C,
				Typ... Ts,
				Typ Args> struct impl<defer<C, Ts...>, Args>
				: subst_<defer<C, Ts...>, Args>
			{};
			Tpl<Typ T, Tpl<T...> class C, T... Is, Typ Args> struct impl<
				defer_i<T, C, Is...>,
				Args> : subst_<defer_i<T, C, Is...>, Args>
			{};
			Tpl<Tpl<Typ...> class C, Typ... Ts, Typ Args> struct impl<C<Ts...>,
				Args> : subst_<defer<C, Ts...>, Args>
			{};
			Tpl<Typ... Ts, Typ Args> struct impl<lambda_<list<Ts...>, false>,
				Args>
			{
				using type
					= compose<uncurry<lambda_<list<As..., Ts...>, false>>,
						curry<bind_front<quote<concat>, Args>>>;
			};
			Tpl<Typ... Bs, Typ Args> struct impl<lambda_<list<Bs...>, true>,
				Args>
			{
				using type
					= compose<Typ lambda_<list<As..., Bs...>, true>::thunk,
						bind_front<quote<concat>, transform<Args, quote<list>>>,
						curry<bind_front<quote<substitutions>, list<Bs...>>>>;
			};

		public:
			Tpl<Typ... Ts> using invoke
				= _t<if_c<sizeof...(Ts) == arity, impl<F, list<Ts..., F>>>>;
		};

		Tpl<Typ... As> struct lambda_<list<As...>, true>
		{
		private:
			Tpl<Typ T, bool IsVar> friend struct lambda_;
			using Tags
				= list<As...>; // Includes the lambda body as the last arg!
			Tpl<Typ T, Typ Args> struct impl;
			Tpl<Typ Args> using eval_impl_ = bind_back<quote_trait<impl>, Args>;
			Tpl<Typ T, Typ Args> using lazy_impl_
				= lazy::_t<defer<impl, T, protect_<Args>>>;
			Tpl<Tpl<Typ...> class C, Typ Args, Typ Ts> using try_subst_
				= apply<quote<C>, join<transform<Ts, eval_impl_<Args>>>>;
			Tpl<Typ, Typ, Typ = void> struct subst_
			{};
			Tpl<Tpl<Typ...> class C, Typ... Ts, Typ Args> struct subst_<
				defer<C, Ts...>,
				Args,
				void_t<try_subst_<C, Args, list<Ts...>>>>
				: type_identity<list<try_subst_<C, Args, list<Ts...>>>>
			{};
			Tpl<Typ T, Tpl<T...> class C, T... Is, Typ Args> struct subst_<
				defer_i<T, C, Is...>,
				Args,
				void_t<C<Is...>>> : type_identity<list<C<Is...>>>
			{};
			Tpl<Typ T, Typ Args> struct impl
				: if_c<(reverse_find_index<Tags, T>() != npos()),
					  lazy::at<Args, reverse_find_index<Tags, T>>,
					  id<list<T>>>
			{};
			Tpl<Typ T, Typ Args> struct impl<protect_<T>, Args>
				: type_identity<list<T>>
			{};
			Tpl<Typ T, Typ Args> struct impl<is_valid_<T>, Args>
				: type_identity<list<is_trait<impl<T, Args>>>>
			{};
			Tpl<Typ If, Typ... Ts, Typ Args> struct impl<defer<if_, If, Ts...>,
				Args> // Short-circuit
					  // if_
				: impl<lazy_impl_<lazy_if_<If, Ts...>, Args>, Args>
			{};
			Tpl<Typ Bool_, Typ... Ts, Typ Args> struct impl<
				defer<and_, Bool_, Ts...>,
				Args> // Short-circuit
					  // and_
				: impl<lazy_impl_<
						   lazy_if_<Bool_, lazy::and_<Ts...>, protect_<false_>>,
						   Args>,
					  Args>
			{};
			Tpl<Typ Bool_, Typ... Ts, Typ Args> struct impl<
				defer<or_, Bool_, Ts...>,
				Args> // Short-circuit or_
				: impl<lazy_impl_<
						   lazy_if_<Bool_, protect_<true_>, lazy::or_<Ts...>>,
						   Args>,
					  Args>
			{};
			Tpl<Tpl<Typ...> class C,
				Typ... Ts,
				Typ Args> struct impl<defer<C, Ts...>, Args>
				: subst_<defer<C, Ts...>, Args>
			{};
			Tpl<Typ T, Tpl<T...> class C, T... Is, Typ Args> struct impl<
				defer_i<T, C, Is...>,
				Args> : subst_<defer_i<T, C, Is...>, Args>
			{};
			Tpl<Tpl<Typ...> class C, Typ... Ts, Typ Args> struct impl<C<Ts...>,
				Args> : subst_<defer<C, Ts...>, Args>
			{};
			Tpl<Typ... Bs,
				bool IsVar,
				Typ Args> struct impl<lambda_<list<Bs...>, IsVar>, Args>
			{
				using type
					= list<compose<Typ lambda_<list<As..., Bs...>, true>::thunk,
						bind_front<quote<concat>, Args>,
						curry<bind_front<quote<substitutions>, list<Bs...>>>>>;
			};
			struct thunk
			{
				Tpl<Typ S, Typ R = _t<impl<back<Tags>, S>>> using invoke
					= if_c<size<R>{} == 1, front<R>>;
			};

		public:
			Tpl<Typ... Ts> using invoke
				= invoke<thunk, substitutions<Tags, list<Ts...>>>;
		};
	} // namespace detail

	Tpl<Typ... Ts> using lambda
		= if_c<(sizeof...(Ts) > 0), detail::lambda_<list<Ts...>>>;

	Tpl<Typ T> using is_valid = detail::is_valid_<T>;

	Tpl<Typ T> using vararg = detail::vararg_<T>;

	Tpl<Typ T> using protect = detail::protect_<T>;

	Tpl<Typ Tag, Typ Value> struct var;

	namespace detail
	{
		Tpl<Typ... As> struct let_
		{};
		Tpl<Typ Fn> struct let_<Fn> : type_identity<lazy::invoke<lambda<Fn>>>
		{};
		Tpl<Typ Tag, Typ Value, Typ... Rest> struct let_<var<Tag, Value>,
			Rest...>
			: type_identity<lazy::invoke<lambda<Tag, _t<let_<Rest...>>>, Value>>
		{};
	} // namespace detail

	Tpl<Typ... As> using let = _t<_t<detail::let_<As...>>>;

	namespace lazy
	{
		Tpl<Typ... As> using let = defer<let, As...>;
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
		Tpl<Typ M2, Typ M> struct cartesian_product_fn
		{
			Tpl<Typ X> struct lambda0
			{
				Tpl<Typ Xs> using lambda1 = list<push_front<Xs, X>>;
				using type = join<transform<M2, quote<lambda1>>>;
			};
			using type = join<transform<M, quote_trait<lambda0>>>;
		};
	} // namespace detail

	Tpl<Typ ListOfLists> using cartesian_product = reverse_fold<ListOfLists,
		list<list<>>,
		quote_trait<detail::cartesian_product_fn>>;

	namespace lazy
	{
		Tpl<Typ ListOfLists> using cartesian_product
			= defer<cartesian_product, ListOfLists>;
	} // namespace lazy

	namespace detail
	{
		Tpl<bool> struct add_const_if
		{
			Tpl<Typ T> using invoke = T const;
		};
		Tpl<> struct add_const_if<false>
		{
			Tpl<Typ T> using invoke = T;
		};
	} // namespace detail
	Tpl<bool If> using add_const_if_c = detail::add_const_if<If>;
	Tpl<Typ If> using add_const_if = add_const_if_c<If::type::value>;

	Tpl<bool If, Typ T> using const_if_c = invoke<add_const_if_c<If>, T>;
	Tpl<Typ If, Typ T> using const_if = invoke<add_const_if<If>, T>;

	namespace detail
	{
		Tpl<Typ State, Typ Ch> using atoi_
			= if_c<(Ch::value >= '0' && Ch::value <= '9'),
				integral_constant<Typ State::value_type,
					State::value * 10 + (Ch::value - '0')>>;
	}

	inline namespace literals
	{
		Tpl<char... Chs>
			cexp fold<list<char_<Chs>...>, size_t_<0>, quote<detail::atoi_>>
			operator"" _z()
		{
			return {};
		}
	} // namespace literals

	Tpl<Typ... Ts> cexp int
	ignore_unused(Ts&&...)
	{
		return 42;
	}

} // namespace Rider::Faiz::meta


#endif // __RANGE_H_
