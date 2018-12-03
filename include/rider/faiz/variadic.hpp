#ifndef VARIADIC
#define VARIADIC
#include "rider/faiz/faiz_fwd.hpp"
namespace rider::faiz::fseq
{

	template<typename T, typename... Ts>
	constexpr type_identity<T>
	front(empty_base<T, Ts...>) noexcept
	{
		return {};
	}

	template<typename T, typename... Ts>
	constexpr empty_base<Ts...>
	pop_front(empty_base<T, Ts...>) noexcept
	{
		return {};
	}

	template<typename Function, typename State>
	constexpr State
	fold(Function, State s, empty_base<>) noexcept
	{
		return s;
	}
	template<typename Function, typename State, typename T, typename... Ts>
	constexpr auto
	fold(Function f, State s, empty_base<T, Ts...> xs) noexcept
		-> decltype(fold(f, f(s, front(xs)), pop_front(xs)))
	{
		return fold(f, f(s, front(xs)), pop_front(xs));
	}


	template<typename Function, typename State>
	constexpr State
	vfold(Function, State s) noexcept
	{
		return s;
	}
	template<typename Function, typename State, typename T, typename... Ts>
	constexpr State
	vfold(Function f, State s, T x, Ts... xs) noexcept
	{
		return vfold(f, f(s, x), xs...);
	}

} // namespace rider::faiz::fseq

namespace rider::faiz::vseq
{
	template<class Function, typename... _tParams>
	using apply = typename Function::template apply<_tParams...>;

	template<class Function, typename... _tParams>
	using apply_t = _t<apply<Function, _tParams...>>;


#define Impl_Variadic_SeqOp(_n, _tparams, _targs) \
	template<_tparams, typename = void> \
	struct _n : _n<_targs, when<true>> \
	{}; \
\
	template<_tparams, typename _tEnabler = void> \
	using _n##_t = _t<_n<_targs, _tEnabler>>;

#define Impl_Variadic_SeqOpU(_n) Impl_Variadic_SeqOp(_n, class Seq, Seq)

#define Impl_Variadic_SeqOpB(_n) \
	Impl_Variadic_SeqOp(_n, class Seq1 COMMA class Seq2, Seq1 COMMA Seq2)

#define Impl_Variadic_SeqOpI(_n) \
	Impl_Variadic_SeqOp(_n, class Seq COMMA size_t _vIdx, Seq COMMA _vIdx)

#define Impl_Variadic_SeqOpN(_n) \
	Impl_Variadic_SeqOp(_n, size_t _vN COMMA class Seq, _vN COMMA Seq)

	// clang-format off
	Impl_Variadic_SeqOp(defer,
						template<typename...> class _gOp COMMA class Seq,
						_gOp COMMA Seq)

    // Same as:
	// 	template<template<typename...> class _gOp, class Seq, typename = void>
	// 	struct defer : defer<_gOp, Seq, when<true>>
	// {};
	// 	template<template<typename...> class _gOp, class Seq, typename _tEnabler = void> \
	// 	using defer_t = _t<defer<_gOp, Seq, _tEnabler>>;

			//  clang-format on

	template<template<typename...> class _gOp,
			 template<typename...>
			 class _gSeq,
			 typename... Ts>
	struct defer<_gOp, _gSeq<Ts...>,  when_valid<_gOp<Ts...>>>
	{
		using type = _gOp<Ts...>;
	};

// Same as:
// template<typename T, template<T...> class _gOp, class Seq, typename = void>
// 	struct defer_i : defer_i<T, _gOp, Seq, when<true>>
// 	{};
// 	template<typename T, template<T...> class _gOp, class Seq, typename _tEnabler = void>
// 	using defer_i_t = _t<defer_i<T, _gOp, Seq, _tEnabler>>;


// clang-format off
	Impl_Variadic_SeqOp(defer_i,
						typename T COMMA template<T...>
						class _gOp COMMA class Seq,
						T COMMA _gOp COMMA Seq)

    template<typename _tInt,
			 template<_tInt...> class _gOp,
			 template<typename, _tInt...>
			 class _gSeq,
			 _tInt... _vSeq>
    struct defer_i<_tInt,
				   _gOp,
				   _gSeq<_tInt, _vSeq...>,
				   when_valid<_gOp<_vSeq...>>>
	{
		using type = _gOp<_vSeq...>;
	};


	template<template<typename...> class _gOp, typename... Ts>
	struct vdefer : defer<_gOp, empty_base<Ts...>>
	{};

	template<class Function, class _tParamsList>
	using defer_apply_t = _t<defer_t<Function::template apply, _tParamsList>>;


	template<class Function, typename... _tParams>
	struct bind_back
	{
		template<typename... Ts>
		using apply = vseq::apply<Function, Ts..., _tParams...>;
	};


	template<class Function, typename... _tParams>
	struct bind_front
	{
		template<typename... Ts>
		using apply = vseq::apply<Function, _tParams..., Ts...>;
	};


	template<template<typename...> class _gOp>
	struct _a
	{
		template<typename... Ts>
		using apply = vdefer<_gOp, Ts...>;
	};


	template<class Function>
	struct _q
	{
		// NOTE: Ideally, the template argument should be limited to enable
		// check.
		//	However this is impossible since the arity of 'Function::apply'
		// is 	unknown.
		template<typename... Ts>
		using apply = vdefer<apply, Function, Ts...>;
	};

	Impl_Variadic_SeqOpU(ctor_of)

		template<template<typename...> class _gSeq, typename... Ts>
		struct ctor_of<_gSeq<Ts...>>
	{
		using type = _a<_gSeq>;
	};


	Impl_Variadic_SeqOpU(params_of)

	template<template<typename...> class _gSeq, typename... Ts>
	struct params_of<_gSeq<Ts...>>
	{
		using type = empty_base<Ts...>;
	};


	template<class Seq, class _tParamsList>
	using instance_apply_t = defer_apply_t<ctor_of_t<Seq>, _tParamsList>;


	template<class, typename = void>
	struct is_instance : false_
	{};

	template<class Seq>
	struct is_instance<Seq,
		 enable_if_t<is_same<Seq,
			_t<vdefer<instance_apply_t, Seq, params_of_t<Seq>>>>::value>>
		: true_
	{};


	template<class... Seqs>
	using same_instance = are_same<ctor_of_t<Seqs>...>;


	template<typename... Seqs>
	using enable_for_instances =  enable_when<
		logic::and_<is_instance<Seqs>...,
					same_instance<Seqs...>>::value>;


	Impl_Variadic_SeqOpU(clear)

	template<class Seq>
	struct clear<Seq, when<true>>
	{
		using type = apply_t<ctor_of_t<Seq>>;
	};


	Impl_Variadic_SeqOpB(concat)

	template<typename... Ts1, typename... Ts2>
	struct concat<empty_base<Ts1...>, empty_base<Ts2...>>
	{
		using type = empty_base<Ts1..., Ts2...>;
	};

	template<class Seq1, class Seq2>
	struct concat<Seq1, Seq2, enable_for_instances<Seq1, Seq2>>
	{
		using type = instance_apply_t<Seq1,
			concat_t<params_of_t<Seq1>, params_of_t<Seq2>>>;
	};


	Impl_Variadic_SeqOp(fmap, class _tCtor COMMA class Seq, _tCtor COMMA Seq)

	template<class _tCtor, typename... Ts>
	struct fmap<_tCtor, empty_base<Ts...>>
	{
		using type = apply_t<_tCtor, Ts...>;
	};

	template<class _tCtor, class Seq>
	struct fmap<_tCtor, Seq, enable_for_instances<Seq>>
	{
		using type = defer_apply_t<_tCtor, params_of_t<Seq>>;
	};


	Impl_Variadic_SeqOpU(front)

	template<typename _tHead, typename... _tTail>
	struct front<empty_base<_tHead, _tTail...>>
	{
		using type = _tHead;
	};

	template<class Seq>
	struct front<Seq, enable_for_instances<Seq>>
	{
		using type = front_t<params_of_t<Seq>>;
	};


	Impl_Variadic_SeqOpU(pop_front)

	template<typename _tHead, typename... _tTail>
	struct pop_front<empty_base<_tHead, _tTail...>>
	{
		using type = empty_base<_tTail...>;
	};

	template<class Seq>
	struct pop_front<Seq, enable_for_instances<Seq>>
	{
		using type = instance_apply_t<Seq, pop_front_t<params_of_t<Seq>>>;
	};


	Impl_Variadic_SeqOp(push_back,
						class Seq COMMA typename _tItem,
						Seq COMMA _tItem)

	template<typename... Ts, typename T>
	struct push_back<empty_base<Ts...>, T>
	{
		using type = empty_base<Ts..., T>;
	};

	template<class Seq, typename T>
	struct push_back<Seq, T, enable_for_instances<Seq>>
	{
		using type = instance_apply_t<Seq, push_back_t<params_of_t<Seq>, T>>;
	};


	Impl_Variadic_SeqOp(push_front,
						class Seq COMMA typename _tItem,
						Seq COMMA _tItem)

	template<typename... Ts, typename T>
	struct push_front<empty_base<Ts...>, T>
	{
		using type = empty_base<T, Ts...>;
	};

	template<class Seq, typename T>
	struct push_front<Seq, T, enable_for_instances<Seq>>
	{
		using type = instance_apply_t<Seq, push_front_t<params_of_t<Seq>, T>>;
	};


	Impl_Variadic_SeqOpU(seq_size)

	template<typename... Ts>
	struct seq_size<empty_base<Ts...>> : size_t_<sizeof...(Ts)>
	{};

	template<class Seq>
	struct seq_size<Seq, enable_for_instances<Seq>>
		: seq_size_t<params_of_t<Seq>>
	{};

	Impl_Variadic_SeqOpI(at)

	template<class Seq, size_t _vIdx>
	struct at<Seq, _vIdx, when<true>> : at<pop_front_t<Seq>, _vIdx - 1>
	{};

	template<class Seq>
	struct at<Seq, 0, when<true>> : front<Seq>
	{};


	Impl_Variadic_SeqOpU(back)

	template<class Seq>
	struct back<Seq, when<true>> : at<Seq, seq_size<Seq>::value - 1>
	{};


	Impl_Variadic_SeqOpN(split_n)

	template<size_t _vN, class Seq>
	struct split_n<_vN, Seq, when<true>>
	{
	private:
		using half = split_n<_vN / 2, Seq>;
		using last = split_n<_vN - _vN / 2, typename half::tail>;

	public:
		using type = concat_t<_t<half>, _t<last>>;
		using tail = typename last::tail;
	};

	template<class Seq>
	struct split_n<0, Seq, when<true>>
	{
		using type = clear_t<Seq>;
		using tail = Seq;
	};

	template<class Seq>
	struct split_n<1, Seq, when<true>>
	{
		using type = push_front_t<clear_t<Seq>, front_t<Seq>>;
		using tail = pop_front_t<Seq>;
	};


	Impl_Variadic_SeqOpU(pop_back)

	template<class Seq>
	struct pop_back<Seq, when<true>>
	: split_n<seq_size<Seq>::value - 1, Seq>
	{};


	Impl_Variadic_SeqOp(erase,
		class Seq COMMA size_t _vIdx COMMA size_t _vEnd = _vIdx + 1,
		Seq COMMA _vIdx COMMA _vEnd)

	template<class Seq, size_t _vIdx, size_t _vEnd>
	struct erase<Seq, _vIdx, _vEnd, when<true>>
	{
		static_assert(_vIdx <= _vEnd, "Invalid range found.");

	public:
		using type = concat_t<split_n_t<_vIdx, Seq>,
							  typename split_n<_vEnd,
											   Seq>::tail>;
	};

	Impl_Variadic_SeqOp(find, class Seq COMMA typename T, Seq COMMA T)

	namespace details
	{

		template<size_t _vN, class Seq, typename T>
		struct find
		{
			static constexpr const size_t value
				= is_same<front_t<Seq>, T>::value ?
				0 :
				find<_vN - 1, pop_front_t<Seq>, T>::value + 1;
		};

		template<class Seq, typename T>
		struct find<0, Seq, T>
		{
			static constexpr const size_t value = 0;
		};

	} // namespace details;

	template<class Seq, typename T>
	struct find<Seq, T, when<true>>
		: size_t_<details::find<seq_size<Seq>::value, Seq, T>::value>
	{};


	Impl_Variadic_SeqOpU(deduplicate)

	namespace details
	{

		template<size_t, class Seq>
		struct deduplicate
		{
		private:
			using head = front_t<Seq>;
			using tail = pop_front_t<Seq>;
			using sub = deduplicate_t<tail>;

		public:
			using type = meta::if_c<vseq::find<tail, head>::value
					== seq_size<tail>::value,
				push_front_t<sub, head>,
				sub>;
		};

		template<class Seq>
		struct deduplicate<0, Seq>
		{
			using type = Seq;
		};

		template<class Seq>
		struct deduplicate<1, Seq>
		{
			using type = Seq;
		};

	} // namespace details;

	template<class Seq>
	struct deduplicate<Seq, when<true>>
	{
		using type = _t<details::deduplicate<seq_size<Seq>::value, Seq>>;
	};


	Impl_Variadic_SeqOpN(join_n)

	template<size_t _vN, class Seq>
	struct join_n<_vN, Seq, when<true>>
	{
	private:
		using unit = Seq;
		using half = join_n_t<_vN / 2, unit>;

	public:
		using type = concat_t<concat_t<half, half>, join_n_t<_vN % 2, unit>>;
	};

	template<class Seq>
	struct join_n<0, Seq>
	{
		using type = clear_t<Seq>;
	};

	template<class Seq>
	struct join_n<1, Seq>
	{
		using type = Seq;
	};


	Impl_Variadic_SeqOp(project,
						class Seq COMMA class _tIdxSeq,
						Seq COMMA _tIdxSeq)


	Impl_Variadic_SeqOpU(reverse)

	template<class Seq>
	struct reverse<Seq, when<true>>
	{
		using type = meta::if_c<seq_size<Seq>::value == 0,
								   clear_t<Seq>,
								   concat_t<reverse_t<pop_front_t<Seq>>,
											front_t<Seq>>>;
	};


	Impl_Variadic_SeqOpU(unique)

	namespace details
	{

		template<class Seq, size_t _vN = seq_size<Seq>::value>
		struct unique
		{
		private:
			using split = split_n<_vN / 2, Seq>;
			using half = _t<unique<typename split::type>>;
			using last = _t<unique<typename split::tail>>;
			using half_back = back_t<half>;
			using last_front = front_t<last>;

		public:
			using type = concat_t<half,
								  meta::if_<is_same<half_back, last_front>,
					   pop_front_t<last>, last>>;
		};

		template<class Seq>
		struct unique<Seq, 0>
		{
			using type = clear_t<Seq>;
		};

		template<class Seq>
		struct unique<Seq, 1>
		{
			using type = Seq;
		};

	} // namespace details;

	template<class Seq>
	struct unique<Seq, when<true>>
	{
		using type = _t<details::unique<Seq>>;
	};


	Impl_Variadic_SeqOp(fold,
						class BinaryFunction COMMA typename State COMMA class Seq,
						BinaryFunction COMMA State COMMA Seq)

		namespace details
	{
		template<class BinaryFunction, typename State, typename Seq, size_t Size>
		struct fold
		{
		private:
			using parts = split_n<Size / 2, Seq>;
			using head = _t<parts>;
			using tail = typename parts::tail;

		public:
			using type = fold_t<BinaryFunction,
				fold_t<BinaryFunction, State, head>,
				tail>;
		};

		template<class BinaryFunction, typename State, typename Seq>
		struct fold<BinaryFunction, State, Seq, 0>
		{
			using type = State;
		};

		template<class BinaryFunction, typename State, typename Seq>
		struct fold<BinaryFunction, State, Seq, 1>
		{
			using type = apply_t<BinaryFunction, State, front_t<Seq>>;
		};

	} // namespace details;

	template<class BinaryFunction, typename State, typename Seq>
	struct fold<BinaryFunction, State, Seq, enable_for_instances<Seq>>
		: details::fold<BinaryFunction, State, Seq, seq_size_t<Seq>::value>
	{};


	// clang-format off
Impl_Variadic_SeqOpB(vec_add)


Impl_Variadic_SeqOpB(vec_subtract)
	// clang-format on

#undef Impl_Variadic_SeqOpN
#undef Impl_Variadic_SeqOpI
#undef Impl_Variadic_SeqOpU
#undef Impl_Variadic_SeqOp

} // namespace rider::faiz::vseq

namespace rider::faiz
{
	// namespace fseq
	using vseq::vdefer;
} // namespace rider::faiz

#endif
