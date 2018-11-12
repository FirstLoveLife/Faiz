#ifndef OPERATOR
#	define OPERATOR
#	include "rider/faiz/faiz.hpp"
#	include "rider/faiz/integer_sequence.hpp"
#	include "rider/faiz/type_traits.hpp"
#	include "rider/faiz/utility.hpp"
#	include "rider/faiz/variadic.hpp"

namespace rider::faiz::dep_ops
{
	using no_constfn = IMPL(false_);
	using use_constfn = IMPL(true_);
} // namespace rider::faiz::dep_ops

namespace rider::faiz
{
#	define ImplOperatorsDeOpt class _tOpt = IMPL(use_constfn)
#	define ImplOperators_H_n(_args) template<class _type, _args>
#	define ImplOperators_H1 ImplOperators_H_n(ImplOperatorsDeOpt)
#	define ImplOperators_H2 \
		ImplOperators_H_n(typename _type2 COMMA ImplOperatorsDeOpt)
#	define ImplOperators_H2_de \
		ImplOperators_H_n(typename _type2 = _type COMMA ImplOperatorsDeOpt)
#	define ImplOperators_H2_Alias(_name, ...) \
		ImplOperators_H2 IMPL(using) _name = __VA_ARGS__;
#	define ImplOperators_H2_Alias_de(_name, ...) \
		ImplOperators_H2_de IMPL(using) _name = __VA_ARGS__;
#	define ImplOperators_H3 \
		ImplOperators_H_n( \
			typename _type2 COMMA typename _type3 COMMA ImplOperatorsDeOpt)
} // namespace rider::faiz

namespace rider::faiz::details
{
	template<typename _type = void>
	struct op_idt
	{
		using type = _type;
	};

	template<>
	struct op_idt<void>;

	template<typename _type = void>
	using op_idt_t = _t<op_idt<_type>>;

	template<class... _types>
	struct ebases : _types...
	{};

	template<size_t _vN, class _type, typename _type2, class _tOpt>
	struct bin_ops;

	template<class, typename, class, class>
	struct ops_seq;

	template<class _type, typename _type2, class _tOpt, size_t... _vSeq>
	struct ops_seq<_type, _type2, _tOpt, index_sequence<_vSeq...>>
	{
		using type = ebases<bin_ops<_vSeq, _type, _type2, _tOpt>...>;
	};

#	define ImplOperators_f(_c, _op, _tRet, _expr, ...) \
		friend _c _tRet operator _op(__VA_ARGS__) noexcept(noexcept(_expr)) \
		{ \
			return (_expr); \
		}

#	define ImplOperators_bin_ts(_n, _f, _c, _opt, ...) \
		ImplOperators_H_n( \
			typename _type2) struct bin_ops<_n, _type, _type2, dep_ops::_opt> \
		{ \
			template<IMPL(typename = void)> \
			_f(_c, __VA_ARGS__) \
		};
#	define ImplOperators_bin_spec(_n, _f, ...) \
		ImplOperators_bin_ts(_n, _f, inline, no_constfn, __VA_ARGS__) \
			ImplOperators_bin_ts(_n, _f, constexpr, use_constfn, __VA_ARGS__)

#	define ImplOperators_cmpf(_c, _op, _expr, _ptp, _ptp2) \
		ImplOperators_f(_c, _op, bool, _expr, const _ptp& x, const _ptp2& y)

#	define ImplOperators_cmp(_n, _op, _expr, _ptp, _ptp2) \
		ImplOperators_bin_spec(_n, ImplOperators_cmpf, _op, _expr, _ptp, _ptp2)

	// clang-format off
		// TODO: Add strictly partial order comparison support to reduce
		// duplicate code
		//	between 'less_than_comparable' and 'partially_ordered'.

		// NOTE: Range 0-5 is reserved for %less_than_comparable. Range 2-3 is
		// also
		//	used by %partially_ordered.
	ImplOperators_cmp(0, <=, !bool(x > y), _type, _type2)
	ImplOperators_cmp(1, >=, !bool(x < y), _type, _type2)
	ImplOperators_cmp(2, >, y < x, _type2, _type)
	ImplOperators_cmp(3, <, y> x, _type2, _type)
	ImplOperators_cmp(4, <=, !bool(y < x), _type2, _type)
	ImplOperators_cmp(5, >=, !bool(y > x), _type2, _type)

	// NOTE: Range 6-8 is reserved for %equality_comparable.
	ImplOperators_cmp(6, ==, y == x, _type2, _type)
	ImplOperators_cmp(7, !=, !bool(x == y), _type2, _type)
	ImplOperators_cmp(8, !=, !bool(x == y), _type, _type2)

	// NOTE: Range 9-10 is reserved for %equivalent.
	ImplOperators_cmp(9, !=, !bool(x < y) && !bool(x > y), _type, _type2)
	ImplOperators_cmp(10, !=, !bool(x < y) && !bool(y < x), _type, _type2)

	// NOTE: Range 11-14 is reserved for %partially_ordered.
	ImplOperators_cmp(11, <=, bool(x < y) || bool(x == y), _type, _type2)
	ImplOperators_cmp(12, >=, bool(x > y) || bool(x == y), _type, _type2)
	ImplOperators_cmp(13, <=, bool(y > x) || bool(y == x), _type2, _type)
	ImplOperators_cmp(14, >=, bool(y < x) || bool(y == x), _type2, _type)
#undef ImplOperators_cmp
#undef ImplOperators_cmpf

	// NOTE: Offset range 0-1 is reserved for general binary operations.
	// Range 2-3
	//	is for commutative operations. Range 4-5 is for non commutative
	//"left" 	operations.
	constexpr size_t ops_bin_id(size_t id, size_t off) noexcept
	{
		return (id + 1) * 16 + off;
	}

	template<size_t _vID, size_t... _vSeq>
	using ops_bin_id_seq = index_sequence<ops_bin_id(_vID, _vSeq)...>;

#	define ImplOperators_bin_tmpl(_id, _off, _op, ...) \
		ImplOperators_bin_spec(ops_bin_id(_id COMMA _off), \
			ImplOperators_f, \
			_op, \
			_type, \
			__VA_ARGS__)
#	define ImplOperators_bin(_id, _op) \
		ImplOperators_bin_tmpl( \
			_id, 0, _op, x _op## = y, _type x, const _type2& y) \
			ImplOperators_bin_tmpl( \
				_id, 1, _op, x _op## = faiz::move(y), _type x, _type2 && y)
#	define ImplOperators_con(_id, _op) \
		ImplOperators_bin(_id, _op) ImplOperators_bin_tmpl( \
			_id, 2, _op, y _op## = x, const _type2& x, _type y) \
			ImplOperators_bin_tmpl( \
				_id, 3, _op, y _op## = faiz::move(x), _type2 && x, _type y)
#	define ImplOperators_left(_id, _op) \
		ImplOperators_bin(_id, _op) ImplOperators_bin_tmpl( \
			_id, 4, _op, _type(x) _op## = y, const _type2& x, const _type& y) \
			ImplOperators_bin_tmpl(_id, \
				5, \
				_op, \
				_type(x) _op## = faiz::move(y), \
				const _type2& x, \
				_type&& y)

	// 1/8 of ImplOperators_con(0, +)
	//template<class _type, typename _type2>
	// struct bin_ops<ops_bin_id(0 COMMA 0), _type, _type2, dep_ops::no_constfn>
	// {
	//     friend inline _type					\
	// 		operator +(_type x, const _type2& y) noexcept(noexcept(x += y))
	//     {
	//     	return (x += y);
	//     }
	// };

	ImplOperators_con(0, +)
	ImplOperators_left(1, -)
	ImplOperators_con(2, *)
	ImplOperators_left(3, /)
	ImplOperators_left(4, %)
	ImplOperators_con(5, ^)
	ImplOperators_con(6, &)
	ImplOperators_con(7, |)
	ImplOperators_bin(8, <<)
	ImplOperators_bin(9, >>)

#undef ImplOperators_left
#undef ImplOperators_con
#undef ImplOperators_bin
#undef ImplOperators_bin_tmpl

#undef ImplOperators_bin_spec
#undef ImplOperators_bin_ts
#undef ImplOperators_f

	template<class _type,
			 typename _type2,
			 class _tOpt,
			 template<typename...> class... _gOps>
	using flat_ops =
		vseq::defer_t<ebases,
					  vseq::fold_t<
						  vseq::_a<vseq::concat_t>,
						  empty_base<>,
						  empty_base<vseq::defer_t<
										 empty_base,
										 _gOps<_type, _type2, _tOpt>>...>>>;

}

namespace rider::faiz::dep_ops
{
#define ImplOperators_Compare(_name, _bseq, _bseq_s) \
	ImplOperators_H2_Alias_de(_name, \
		_t<details::ops_seq<_type, \
			_type2, \
			_tOpt, \
			cond_t<is_same<_type, _type2>, \
				index_sequence<_bseq_s>, \
				index_sequence<_bseq>>>>)

	ImplOperators_Compare(less_than_comparable,
						  0 COMMA 1 COMMA 2 COMMA 3 COMMA 4 COMMA 5,
						  2 COMMA 4 COMMA 1)

	ImplOperators_Compare(equality_comparable,
						  6 COMMA 7 COMMA 8,
						  7)

	ImplOperators_Compare(equivalent, 9, 10)

	ImplOperators_Compare(partially_ordered,
						  11 COMMA 12 COMMA 2 COMMA 3 COMMA 13 COMMA 14,
						  2 COMMA 11 COMMA 14)

#undef ImplOperators_Compare

#define ImplOperators_Binary_Tmpl(_id, _name, _bseq) \
	ImplOperators_H2_Alias_de(_name, \
		_t<details::ops_seq<_type, \
			_type2, \
			_tOpt, \
			details::ops_bin_id_seq<_id, _bseq>>>)
#define ImplOperators_Binary(_id, _name) \
	ImplOperators_Binary_Tmpl(_id, _name, 0 COMMA 1)
#define ImplOperators_Commutative(_id, _name) \
	ImplOperators_H2_Alias_de(_name, \
		_t<details::ops_seq<_type, \
			_type2, \
			_tOpt, \
			cond_t<is_same<_type, _type2>, \
				details::ops_bin_id_seq<_id, 0 COMMA 1>, \
				details::ops_bin_id_seq<_id, 0 COMMA 1 COMMA 2 COMMA 3>>>>)

#define ImplOperators_Left(_id, _name) \
	ImplOperators_H2_Alias_de(_name, \
		_t<details::ops_seq<_type, \
			_type2, \
			_tOpt, \
			details::ops_bin_id_seq<_id, 0 COMMA 1>>>) \
		ImplOperators_H2_Alias_de(_name##_left, \
								  _t<details::ops_seq<_type, \
								  _type2, \
								  _tOpt, \
								  details::ops_bin_id_seq<_id, 4 COMMA 5>>>)

	ImplOperators_Commutative(0, addable)
	ImplOperators_Left(1, subtractable)
	ImplOperators_Commutative(2, multipliable)
	ImplOperators_Left(3, dividable)
	ImplOperators_Left(4, modable)
	ImplOperators_Binary(5, left_shiftable)
	ImplOperators_Binary(6, right_shiftable)
	ImplOperators_Commutative(7, xorable)
	ImplOperators_Commutative(8, andable)
	ImplOperators_Commutative(9, orable)

#undef ImplOperators_Binary_Tmpl
#undef ImplOperators_Left
#undef ImplOperators_Commutative
#undef ImplOperators_Binary

	ImplOperators_H1
	IMPL(struct) incrementable
	{
		friend _type operator++(_type& x, int)
			noexcept(noexcept(_type(x))
					 && noexcept(++x)
					 && noexcept(_type(faiz::declval<_type>())))
		{
			_type t(x);
			++x;
			return t;
		}
	};

	ImplOperators_H1 IMPL(struct) decrementable
	{
		friend _type
			operator--(_type& x, int) noexcept(noexcept(_type(x)) && noexcept(--x)
											   && noexcept(_type(faiz::declval<_type>())))
		{
			_type t(x);
			++x;
			return t;
		}
	};

	ImplOperators_H2 IMPL(struct) dereferenceable
	{
		// TODO: Add non-const
		// overloaded version? SFINAE?
		// TODO: Use '_tOpt'.
		constexpr decltype(faiz::addressof(faiz::declval<const _type2&>()))
		operator->() const noexcept(*faiz::declval<const _type&>())
		{
			return faiz::addressof(*static_cast<const _type&>(*this));
		}
	}
;

	ImplOperators_H3 IMPL(struct) indexable{
        // TODO: Add non-const overloaded
		// version? SFINAE?
		// TODO: Use '_tOpt'.
		constexpr _type3
		operator[](_type2 n) const noexcept(*(faiz::declval<const _type&>() + n))
		{
			return *(static_cast<const _type&>(*this) + n);
		}
	};

// clang-format on
#	define ImplOperatorsFlatAlias2_de(_name, ...) \
		ImplOperators_H2_Alias_de( \
			_name, details::flat_ops<_type, _type2, _tOpt, __VA_ARGS__>)
// clang-format off
//
//template<class _type, typename _type2 = _type, class _tOpt = use_constfn>
// using less_than_comparable =
//     _t<details::ops_seq<_type,
//                         _type2,
//                         _tOpt,
//                         cond_t<is_same<_type, _type2>,
//                                index_sequence<2, 4, 1>,
//                                index_sequence<0, 1, 2, 3, 4, 5>>>>;

// template<class _type, typename _type2 = _type, class _tOpt = use_constfn>
// using equality_comparable =
//     _t<details::ops_seq<_type,
//                         _type2,
//                         _tOpt,
//                         cond_t<is_same<_type, _type2>,
//                                index_sequence<7>,
//                                index_sequence<6, 7, 8>>>>;

// template<class _type,
// 				 typename _type2,
// 				 class _tOpt,
// 				 template<typename...>
// 				 class... _gOps>
// 		using flat_ops =
// 			vseq::defer_t<ebases,
// 						  vseq::fold_t<
// 							  vseq::_a<vseq::concat_t>,
// 							  empty_base<>,
// 							  empty_base<vseq::defer_t<
// 											 empty_base,
// 											 _gOps<_type, _type2, _tOpt>>...>>>;


// template<class _type, typename _type2 = _type, class _tOpt = use_constfn>
// using totally_ordered =
//     details::flat_ops<_type, _type2, _tOpt, less_than_comparable, equality_comparable>

ImplOperatorsFlatAlias2_de(totally_ordered,
						   less_than_comparable,
						   equality_comparable)

ImplOperatorsFlatAlias2_de(additive,
						   addable,
						   subtractable)

ImplOperatorsFlatAlias2_de(multiplicative,
						   multipliable,
						   dividable)

ImplOperatorsFlatAlias2_de(integer_multiplicative,
						   multiplicative,
						   modable)

ImplOperatorsFlatAlias2_de(arithmetic,
						   additive,
						   multiplicative)

ImplOperatorsFlatAlias2_de(integer_arithmetic,
						   additive,
						   integer_multiplicative)

ImplOperatorsFlatAlias2_de(bitwise,
						   xorable,
						   andable,
						   orable)

ImplOperators_H1
IMPL(using) unit_steppable = details::ebases<incrementable<_type>,
											  decrementable<_type>>;

ImplOperatorsFlatAlias2_de(shiftable,
						   left_shiftable,
						   right_shiftable)

ImplOperatorsFlatAlias2_de(ring_operators,
						   additive,
						   subtractable_left,
						   multipliable)

ImplOperatorsFlatAlias2_de(ordered_ring_operators,
						   ring_operators,
						   totally_ordered)

ImplOperatorsFlatAlias2_de(field_operators,
						   ring_operators,
						   dividable,
						   dividable_left)

ImplOperatorsFlatAlias2_de(ordered_field_operators,
						   field_operators,
						   totally_ordered)

ImplOperatorsFlatAlias2_de(euclidean_ring_operators,
						   ring_operators,
						   dividable,
						   dividable_left,
						   modable,
						   modable_left)

ImplOperatorsFlatAlias2_de(ordered_euclidean_ring_operators,
						   totally_ordered,
						   euclidean_ring_operators)

ImplOperators_H2_Alias(input_iteratable,
					   details::ebases<
					   equality_comparable<_type, _type, _tOpt>,
					   incrementable<_type>,
					   dereferenceable<_type, _type2, _tOpt>>)

ImplOperators_H2_Alias(output_iteratable,
					   details::ebases<incrementable<_type>>)

ImplOperators_H2_Alias(forward_iteratable,
					   details::ebases<input_iteratable<_type,
					   _type2,
					   _tOpt>>)

ImplOperators_H2_Alias(
	bidirectional_iteratable,
	details::ebases<
	                forward_iteratable<_type,
                                       _type2,
	                                   _tOpt>,
	                decrementable<_type>>)

ImplOperators_H3
IMPL(using) random_access_iteratable =
	details::ebases<
	               bidirectional_iteratable<_type, _type3, _tOpt>,
	               less_than_comparable<_type, _type, _tOpt>,
	               additive<_type, _type2, _tOpt>,
	               indexable<_type, _type2, _type, _tOpt>>;

ImplOperatorsFlatAlias2_de(operators,
						   totally_ordered,
						   integer_arithmetic,
						   bitwise)

// clang-format on
} // namespace dep_ops;

namespace rider::faiz
{
	using namespace dep_ops;
#	undef ImplOperators_H3
#	undef ImplOperators_H2_Alias_de
#	undef ImplOperators_H2_Alias
#	undef ImplOperators_H2
#	undef ImplOperators_H1
#	undef ImplOperators_H_n
#	undef ImplOperatorsDeOpt
} // namespace rider::faiz
#endif

// clang-format off
