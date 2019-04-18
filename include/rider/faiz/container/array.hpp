#ifndef FAIZ_ARRAY
#define FAIZ_ARRAY
#include "rider/faiz/algorithm.hpp"
#include "rider/faiz/debug.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"
#include <initializer_list>
#include <iterator>
#include <stdexcept>
// TODO:
// tpl<typ T, typ... U>
// array(totally_ordered<
// 		  array<enable_if_t<(is_same_v<T, U> and ...), T>,
// 	  1 + sizeof...(U)>>, std::initializer_list<T>(T, U...))
// 	->array<enable_if_t<(is_same_v<T, U> and ...), T>,
// 		1 + sizeof...(U)>;
// FIXME: array is not implemtable via CRTP( private
// totally_ordered<array<T, N>>) and user defined template deduction guide:
// https://stackoverflow.com/questions/55592478/how-to-avoid-when-using-aggregate-initialization-ith-empty-base-class
// https://stackoverflow.com/questions/55592146/deduction-guide-breaks-for-templated-derived-class
//
// TODO: use compare_3way and operator<=> after clang, gcc, msvc all implement
// them completely

namespace Rider::Faiz
{
	tpl<typ T, size_t N> struct array
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
	tpl<typ T, typ... U>
	array(T, U...)
		->array<enable_if_t<(are_same_v<T, U...>), T>, 1 + sizeof...(U)>;


	namespace details
	{
		tpl<typ> struct is_ref_wrapper : false_
		{};
		tpl<typ T> struct is_ref_wrapper<std::reference_wrapper<T>> : true_
		{};

		tpl<typ T> using not_ref_wrapper
			= negation<is_ref_wrapper<std::decay_t<T>>>;

		tpl<typ D, typ...> struct return_type_helper : type_identity<D>
		{};

		tpl<typ... Types> struct return_type_helper<void, Types...>
			: std::common_type<Types...>
		{
			static_assert(conjunction_v<not_ref_wrapper<Types>...>,
				"Types cannot contain reference_wrappers when D is void");
		};

		tpl<typ D, typ... Types> using return_type
			= array<_t<return_type_helper<D, Types...>>, sizeof...(Types)>;
	} // namespace details

	tpl<typ D = void, typ... Types> cfn
	make_array(Types&&... t)->details::return_type<D, Types...>
	{
		return {Faiz::forward<Types>(t)...};
	}

	// Establishes an equivalence relation, that is, it satisfies the following
	// properties:
	// - For all values of a, a == a yields true.
	// - If a == b, then b == a
	// - If a == b and b == c, then a == c
	tpl<typ T, size_t N> cfn
	operator==(const array<T, N>& lhs, const array<T, N>& rhs)->bool
	{
		return equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	tpl<typ T, size_t N> cfn
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
	tpl<typ T, size_t N> cfn
	operator<(const array<T, N>& lhs, const array<T, N>& rhs)->bool
	{
		return lexicographical_compare(
			lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	tpl<typ T, size_t N> cfn
	operator>(const array<T, N>& lhs, const array<T, N>& rhs)->bool
	{
		return lhs != rhs and not(lhs < rhs);
	}

	tpl<typ T, size_t N> cfn
	operator>=(const array<T, N>& lhs, const array<T, N>& rhs)->bool
	{
		return not(lhs < rhs);
	}

	tpl<typ T, size_t N> cfn
	operator<=(const array<T, N>& lhs, const array<T, N>& rhs)->bool
	{
		return not(lhs > rhs);
	}
} // namespace Rider::Faiz

#endif
