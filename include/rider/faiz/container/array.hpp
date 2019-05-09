#ifndef FAIZ_FIXARRAY
#define FAIZ_FIXARRAY
#include "rider/faiz/algorithm.hpp"
#include "rider/faiz/debug.hpp"
#include "rider/faiz/math/RelationAlgebra.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"
#include <initializer_list>
#include <iterator>
#include <stdexcept>
// Fixed:
// FixedArray a{1, 2, 3, 4, 5} ✓
// FixedArray<int, 5> a{1, 2, 3, 4, 5} ✓
// FixedArray<int, 5> a{1, 2, 3, 4} X
// FixedArray<int, 5> a{1, 2, 3, 4, 6} X
//
// non-Fixed:
// array a{1, 2, 3, 4, 5} ✓
// array<int, 5> a{1, 2, 3, 4, 5} ✓
// array<int, 5> a{1, 2, 3, 4} ✓
// array<int, 5> a{1, 2, 3, 4, 6} ✓

// TODO: use compare_3way and operator<=> after clang, gcc, msvc all implement
// them completely

namespace Rider::Faiz::FixedDetail
{
	Tpl<Typ T, size_t N> using always_t = T;

	template<typename T, typename Seq>
	struct FixedArray_impl;

	template<typename T, size_t... Is>
	struct FixedArray_impl<T, index_sequence<Is...>>
	{
		FixedArray_impl(always_t<T, Is>... args) : elem{args...}
		{}

		constexpr FixedArray_impl() = default;
		constexpr static size_t N = sizeof...(Is) == 0 ? 1 : sizeof...(Is);

	protected:
		T elem[N];
	};

} // namespace Rider::Faiz::FixedDetail
namespace Rider::Faiz
{
	// # FixedArray-CRTP based array
	// template argument deduction ✓
	// FixedArray a{1, 2, 3, 4, 5} ✓
	// FixedArray<int, 5> a{1, 2, 3, 4, 5} ✓
	// FixedArray<int, 5> a{1, 2, 3, 4} X
	// FixedArray<int, 5> a{1, 2, 3, 4, 6} X
	using Math::AbstractAlgebra::RelationAlgebra::totally_ordered;
	Tpl<Typ T, size_t N> struct FixedArray
		: FixedDetail::FixedArray_impl<T, make_index_sequence<N>>,
		  totally_ordered<FixedArray<T, N>>
	{
		using value_type = T;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using const_iterator = const value_type*;
		using iterator = value_type*;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		using FixedDetail::FixedArray_impl<T,
			make_index_sequence<N>>::FixedArray_impl;
		using FixedDetail::FixedArray_impl<T, make_index_sequence<N>>::elem;

		cfn
		size() const->size_type
		{
			return N;
		}

		cfn
		at(size_type pos)->reference
		{
			if(pos >= N)
			{
				throw std::out_of_range("FixedArray::at");
			}
			return elem[pos];
		}

		cfn
		at(size_type pos) const->const_reference
		{
			if(pos >= N)
			{
				throw std::out_of_range("FixedArray::at");
			}
			return elem[pos];
		}

		cfn
		back()->reference
		{
			Expects(N > 0);
			return N ? *(end() - 1) : *end();
		}

		cfn
		back() const->const_reference
		{
			Expects(N > 0);
			return N ? *(end() - 1) : *end();
		}


		cfn
		operator[](size_type pos)->reference
		{
			Expects(N > 0);
			return elem[pos];
		}

		cfn
		operator[](size_type pos) const->const_reference
		{
			Expects(N > 0);
			return elem[pos];
		}

		cfn
		front()->reference
		{
			Expects(N > 0);
			return *begin();
		}

		cfn
		front() const->const_reference
		{
			Expects(N > 0);
			return *begin();
		}

		[[nodiscard]] cfn
		empty() const noexcept->bool
		{
			return begin() == end();
		}

		cfn
		data() noexcept->pointer
		{
			return elem;
		}

		cfn
		data() const noexcept->const_pointer
		{
			return elem;
		}

		cfn
		begin() noexcept->iterator
		{
			return iterator(data());
		}
		cfn
		begin() const noexcept->const_iterator
		{
			return const_iterator(data());
		}
		cfn
		cbegin() const noexcept->const_iterator
		{
			return begin();
		}

		cfn
		end() noexcept->iterator
		{
			return iterator(data() + N);
		}
		cfn
		end() const noexcept->const_iterator
		{
			return const_iterator(data() + N);
		}
		cfn
		cend() const noexcept->const_iterator
		{
			return end();
		}

		cfn
		rbegin() noexcept->reverse_iterator
		{
			return reverse_iterator(end());
		}

		cfn
		rbegin() const noexcept->const_reverse_iterator
		{
			return const_reverse_iterator(end());
		}

		cfn
		rend() noexcept->reverse_iterator
		{
			return reverse_iterator(begin());
		}

		cfn
		rend() const noexcept->const_reverse_iterator
		{
			return const_reverse_iterator(begin());
		}

		cfn
		max_size() const noexcept->size_type
		{
			return N;
		}

		cfn
		fill(const T& value)
		{
			fill_n(elem, N, value);
		}
	};
	// FixedArray a{1, 2, 3, 5, x}; // OK, creates std::FixedArray<int, 5>
	// FixedArray b{1, 2u}; // Error, all arguments must have same type
	Tpl<Typ T, Typ... U>
	FixedArray(T, U...)
		->FixedArray<enable_if_t<(are_same_v<T, U...>), T>, 1 + sizeof...(U)>;

	// Tpl<Typ T, Typ... U> FixedArray(std::initializer_list<T>(T, U...))
	// 	->FixedArray<enable_if_t<(are_same_v<T, U...>), T>, 1 + sizeof...(U)>;

	namespace Fixed::details
	{
		Tpl<Typ> struct is_ref_wrapper : false_
		{};
		Tpl<Typ T> struct is_ref_wrapper<std::reference_wrapper<T>> : true_
		{};

		Tpl<Typ T> using not_ref_wrapper = negation<is_ref_wrapper<decay_t<T>>>;

		Tpl<Typ D, Typ...> struct return_type_helper : type_identity<D>
		{};

		Tpl<Typ... Types> struct return_type_helper<void, Types...>
			: std::common_type<Types...>
		{
			static_assert(conjunction_v<not_ref_wrapper<Types>...>,
				"Types cannot contain reference_wrappers when D is void");
		};

		Tpl<Typ D, Typ... Types> using return_type
			= FixedArray<_t<return_type_helper<D, Types...>>, sizeof...(Types)>;
	} // namespace Fixed::details

	Tpl<Typ D = void, Typ... Types> cfn
	MakeFixedArray(Types&&... t)->Fixed::details::return_type<D, Types...>
	{
		return {Faiz::forward<Types>(t)...};
	}
	Tpl<Typ T, size_t N> cfn
	operator==(const FixedArray<T, N>& lhs, const FixedArray<T, N>& rhs)->bool
	{
		return equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	Tpl<Typ T, size_t N> cfn
	operator<(const FixedArray<T, N>& lhs, const FixedArray<T, N>& rhs)->bool
	{
		return lexicographical_compare(
			lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}
} // namespace Rider::Faiz


namespace Rider::Faiz
{
	Tpl<Typ T, size_t N> struct array
	{
		using value_type = T;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using const_iterator = const value_type*;
		using iterator = value_type*;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		constexpr static size_t l = N == 0 ? 1 : N;
		T elem[l];

	public:
		cfn
		size() const->size_type
		{
			return N;
		}

		cfn
		at(size_type pos)->reference
		{
			if(pos >= N)
			{
				throw std::out_of_range("array::at");
			}
			return elem[pos];
		}

		cfn
		at(size_type pos) const->const_reference
		{
			if(pos >= N)
			{
				throw std::out_of_range("array::at");
			}
			return elem[pos];
		}

		cfn
		back()->reference
		{
			Expects(N > 0);
			return N ? *(end() - 1) : *end();
		}

		cfn
		back() const->const_reference
		{
			Expects(N > 0);
			return N ? *(end() - 1) : *end();
		}


		cfn
		operator[](size_type pos)->reference
		{
			Expects(N > 0);
			return elem[pos];
		}

		cfn
		operator[](size_type pos) const->const_reference
		{
			Expects(N > 0);
			return elem[pos];
		}

		cfn
		front()->reference
		{
			Expects(N > 0);
			return *begin();
		}

		cfn
		front() const->const_reference
		{
			Expects(N > 0);
			return *begin();
		}

		[[nodiscard]] cfn
		empty() const noexcept->bool
		{
			return begin() == end();
		}

		cfn
		data() noexcept->pointer
		{
			return elem;
		}

		cfn
		data() const noexcept->const_pointer
		{
			return elem;
		}

		cfn
		begin() noexcept->iterator
		{
			return iterator(data());
		}
		cfn
		begin() const noexcept->const_iterator
		{
			return const_iterator(data());
		}
		cfn
		cbegin() const noexcept->const_iterator
		{
			return begin();
		}

		cfn
		end() noexcept->iterator
		{
			return iterator(data() + N);
		}
		cfn
		end() const noexcept->const_iterator
		{
			return const_iterator(data() + N);
		}
		cfn
		cend() const noexcept->const_iterator
		{
			return end();
		}

		cfn
		rbegin() noexcept->reverse_iterator
		{
			return reverse_iterator(end());
		}

		cfn
		rbegin() const noexcept->const_reverse_iterator
		{
			return const_reverse_iterator(end());
		}

		cfn
		rend() noexcept->reverse_iterator
		{
			return reverse_iterator(begin());
		}

		cfn
		rend() const noexcept->const_reverse_iterator
		{
			return const_reverse_iterator(begin());
		}

		cfn
		max_size() const noexcept->size_type
		{
			return N;
		}

		cfn
		fill(const T& value)
		{
			fill_n(elem, N, value);
		}
	};
	// array a{1, 2, 3, 5, x}; // OK, creates std::array<int, 5>
	// array b{1, 2u}; // Error, all arguments must have same type
	Tpl<Typ T, Typ... U>
	array(T, U...)
		->array<enable_if_t<(are_same_v<T, U...>), T>, 1 + sizeof...(U)>;


	namespace details
	{
		Tpl<Typ> struct is_ref_wrapper : false_
		{};
		Tpl<Typ T> struct is_ref_wrapper<std::reference_wrapper<T>> : true_
		{};

		Tpl<Typ T> using not_ref_wrapper = negation<is_ref_wrapper<decay_t<T>>>;

		Tpl<Typ D, Typ...> struct return_type_helper : type_identity<D>
		{};

		Tpl<Typ... Types> struct return_type_helper<void, Types...>
			: std::common_type<Types...>
		{
			static_assert(conjunction_v<not_ref_wrapper<Types>...>,
				"Types cannot contain reference_wrappers when D is void");
		};

		Tpl<Typ D, Typ... Types> using return_type
			= array<_t<return_type_helper<D, Types...>>, sizeof...(Types)>;
	} // namespace details

	Tpl<Typ D = void, Typ... Types> cfn
	make_array(Types&&... t)->details::return_type<D, Types...>
	{
		return {Faiz::forward<Types>(t)...};
	}

	// Establishes an equivalence relation, that is, it satisfies the following
	// properties:
	// - For all values of a, a == a yields true.
	// - If a == b, then b == a
	// - If a == b and b == c, then a == c
	Tpl<Typ T, size_t N> cfn
	operator==(const array<T, N>& lhs, const array<T, N>& rhs)->bool
	{
		return equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	Tpl<Typ T, size_t N> cfn
	operator!=(const array<T, N>& lhs, const array<T, N>& rhs)->bool
	{
		return not(lhs == rhs);
	}


	// Establishes strict weak ordering relation with the following properties
	// - For all a, !(a < a)
	// - If a < b then !(b < a)
	// - If a < b and b < c then a < c
	// - Defining equiv(a, b) as !(a < b) && !(b < a), if equiv(a, b) and
	// equiv(b, c), then equiv(a, c)
	//
	// The ordering relation must establish total order.
	Tpl<Typ T, size_t N> cfn
	operator<(const array<T, N>& lhs, const array<T, N>& rhs)->bool
	{
		return lexicographical_compare(
			lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	Tpl<Typ T, size_t N> cfn
	operator>(const array<T, N>& lhs, const array<T, N>& rhs)->bool
	{
		return lhs != rhs and not(lhs < rhs);
	}

	Tpl<Typ T, size_t N> cfn
	operator>=(const array<T, N>& lhs, const array<T, N>& rhs)->bool
	{
		return not(lhs < rhs);
	}

	Tpl<Typ T, size_t N> cfn
	operator<=(const array<T, N>& lhs, const array<T, N>& rhs)->bool
	{
		return not(lhs > rhs);
	}
} // namespace Rider::Faiz

#endif
