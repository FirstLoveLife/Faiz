#ifndef STRING_VIEW
#define STRING_VIEW
#include "rider/faiz/algorithm.hpp"
#include "rider/faiz/char_traits.hpp"
#include "rider/faiz/iterator.hpp"
#include "rider/faiz/macros.hpp"
#include "rider/faiz/math/RelationAlgebra.hpp"
#include "rider/faiz/type_traits.hpp"

// TODO: conclude
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0903r2.pdf

namespace Rider::Faiz
{
	Tpl<Typ Char, class Traits = std::char_traits<Char>> class basic_string_view
		: private totally_ordered<basic_string_view<Char, Traits>>
	{
	public:
		using traits_type = Traits;
		using value_type = Char;
		using pointer = Char*;
		using const_pointer = const Char*;
		using reference = Char&;
		using const_reference = const Char&;
		using const_iterator = IMPL(const Char*);
		using iterator = const_iterator;
		using const_reverse_iterator = Faiz::reverse_iterator<const_iterator>;
		using reverse_iterator = const_reverse_iterator;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		static cexp size_type npos = size_type(-1);

	private:
		const_pointer data_;
		size_type size_;

	public:
		cexp
		basic_string_view() noexcept
			: data_(), size_()
		{}

		cexp
		basic_string_view(const Char* str)
			: data_(str), size_(traits_type::length(str))
		{}

		cexp
		basic_string_view(const Char* str, size_type len)
			: data_(str), size_(len)
		{}
		cexp
		basic_string_view(const basic_string_view&) noexcept
			= default;

		cexp basic_string_view&
		operator=(const basic_string_view&) noexcept
			= default;

		cexp const_iterator
		begin() const noexcept
		{
			return data_;
		}

		cexp const_iterator
		end() const noexcept
		{
			return data_ + size_;
		}

		cexp const_iterator
		cbegin() const noexcept
		{
			return data_;
		}

		cexp const_iterator
		cend() const noexcept
		{
			return data_ + size_;
		}

		cexp const_reverse_iterator
		rbegin() const noexcept
		{
			return const_reverse_iterator(end());
		}

		cexp const_reverse_iterator
		rend() const noexcept
		{
			return const_reverse_iterator(begin());
		}

		cexp const_reverse_iterator
		crbegin() const noexcept
		{
			return const_reverse_iterator(cend());
		}

		cexp const_reverse_iterator
		crend() const noexcept
		{
			return const_reverse_iterator(cbegin());
		}

		cexp size_type
		size() const noexcept
		{
			return size_;
		}

		cexp size_type
		length() const noexcept
		{
			return size_;
		}

		cexp size_type
		max_size() const noexcept
		{
			// NOTE: This is same to libc++ but not libstdc++.
			return std::numeric_limits<size_type>::max();
		}

		cexp bool
		empty() const noexcept
		{
			return size_ == 0;
		}

		cexp const_reference operator[](size_type pos) const
		{
			return yconstraint(pos < size_), data_[pos];
		}

		cexp const_reference
		at(size_type pos) const
		{
			return pos < size_ ?
				data_[pos] :
				(throw std::out_of_range("string_view::at"), data_[0]);
		}

		cexp const_reference
		front() const
		{
			return yconstraint(0 < size_), data_[0];
		}

		cexp const_reference
		back() const
		{
			return yconstraint(0 < size_), data_[size_ - 1];
		}

		cexp const_pointer
		data() const noexcept
		{
			return data_;
		}

		cexp void
		remove_prefix(size_type n) noexcept
		{
			yconstraint(n <= size_), yunseq(data_ += n, size_ -= n);
		}

		cexp void
		remove_suffix(size_type n) noexcept
		{
			yconstraint(n <= size_), size_ -= n;
		}

		cexp void
		swap(basic_string_view& s) noexcept
		{
			std::swap(data_, s.data_), std::swap(size_, s.size_);
		}


		size_type
		copy(Char* s, size_type n, size_type pos = 0) const
		{
			yconstraint(s);
			if(pos <= size_)
			{
				const auto rlen(std::min<size_type>(n, size_ - pos));

				Faiz::copy_n(data_ + pos, rlen, s);
				return rlen;
			}
			throw std::out_of_range("string_view::copy");
		}

		cexp basic_string_view
		substr(size_type pos = 0, size_type n = npos) const
		{
			return pos <= size_ ?
				basic_string_view(
					data_ + pos, std::min(n, size_type(size_ - pos))) :
				throw std::out_of_range("string_view::substr");
		}

		cexp int
		compare(basic_string_view s) const noexcept
		{
			return compare_remained(s,
				Traits::compare(
					data(), s.data(), std::min<size_type>(size(), s.size())));
		}
		cexp int
		compare(size_type pos1, size_type n1, basic_string_view s) const
		{
			return substr(pos1, n1).compare(s);
		}
		cexp int
		compare(size_type pos1,
			size_type n1,
			basic_string_view s,
			size_type pos2,
			size_type n2) const
		{
			return substr(pos1, n1).compare(s.substr(pos2, n2));
		}
		cexp int
		compare(const Char* s) const
		{
			return compare(basic_string_view(s));
		}

		cexp int
		compare(size_type pos1, size_type n1, const Char* s) const
		{
			return substr(pos1, n1).compare(basic_string_view(s));
		}

		cexp int
		compare(size_type pos1, size_type n1, const Char* s, size_type n2) const
		{
			return substr(pos1, n1).compare(basic_string_view(s, n2));
		}

	private:
		cexp int
		compare_remained(basic_string_view s, int ret) const noexcept
		{
			return ret == 0 ?
				(size() == s.size() ? 0 : (size() < s.size() ? -1 : 1)) :
				ret;
		}

	public:
		cexp bool
		starts_with(basic_string_view __s) const noexcept
		{
			return size() >= __s.size() && compare(0, __s.size(), __s) == 0;
		}

		cexp bool
		starts_with(value_type __c) const noexcept
		{
			return !empty() && Traits::eq(front(), __c);
		}

		cexp bool
		starts_with(const value_type* __s) const noexcept
		{
			return starts_with(basic_string_view(__s));
		}

		cexp bool
		ends_with(basic_string_view __s) const noexcept
		{
			return size() >= __s.size()
				&& compare(size() - __s.size(), npos, __s) == 0;
		}

		cexp bool
		ends_with(value_type __c) const noexcept
		{
			return !empty() && Traits::eq(back(), __c);
		}

		cexp bool
		ends_with(const value_type* __s) const noexcept
		{
			return ends_with(basic_string_view(__s));
		}


#define Impl_StringView_search_fn_head(_n, _attr, _spec, ...) \
	cexp _attr size_type _n(__VA_ARGS__) const _spec \
	{
#define Impl_StringView_search1(_n, _arg) \
	Impl_StringView_search_fn_head(_n, \
		, \
		noexcept, \
		basic_string_view s, \
		size_type pos = _arg) return Faiz:: \
		str_##_n<value_type, size_type, traits_type, npos>( \
			data_, size_, s.data_, pos, s.size_); \
	}

		// The sequence of preprocessing tokens bounded by the outside-most
		// matching parentheses forms the list of arguments for the
		// function-like macro. The individual arguments within the list are
		// separated by comma preprocessing tokens.
#define Impl_StringView_search34(_n, _arg) \
	Impl_StringView_search_fn_head(_n, \
		, \
		, \
		const Char* s, \
		size_type pos, \
		size_type n) return yconstraint(s), \
		_n(basic_string_view(s, n), pos); \
	} \
	Impl_StringView_search_fn_head( \
		_n, , , const Char* s, size_type pos = _arg) return _n(s, \
		pos, \
		traits_type::length(s)); \
	}
#define Impl_StringView_search_mf(_n, _arg) \
	Impl_StringView_search1(_n, _arg) Impl_StringView_search_fn_head( \
		_n, , noexcept, Char c, size_type pos = _arg) return Faiz:: \
		str_##_n<value_type, size_type, traits_type, npos>( \
			data_, size_, c, pos); \
	} \
	Impl_StringView_search34(_n, _arg)
#define Impl_StringView_search_mf2(_n, _arg, _n2) \
	Impl_StringView_search1(_n, _arg) Impl_StringView_search_fn_head( \
		_n, , noexcept, Char c, size_type pos = _arg) return _n2(c, pos); \
	} \
	Impl_StringView_search34(_n, _arg)


		// clang-format off
		Impl_StringView_search_mf(find, 0)

		Impl_StringView_search_mf(rfind, npos)

		Impl_StringView_search_mf2(find_first_of, 0, find)

		Impl_StringView_search_mf2(find_last_of, npos, rfind)

		Impl_StringView_search_mf(find_first_not_of, 0)

		Impl_StringView_search_mf(find_last_not_of, npos)

// size_type find_last_not_of(basic_string_view s, size_type pos = npos) const
// noexcept
// {
//     return Faiz::
//         str_find_last_not_of<value_type, size_type, traits_type, npos>(
//                 data_, size_, s.data_, pos, s.size_);
// }

// size_type find_last_not_of(Char c, size_type pos = npos) const noexcept
// {
//     return Faiz::
// 		str_find_last_not_of<value_type, size_type, traits_type, npos>(
// 			data_, size_, c, pos);
// }

// size_type find_last_not_of(const Char* s,
// 		size_type pos, \
// 		size_type n) const
// {
//     return yconstraint(s),
// 		_n(basic_string_view(s, n), pos);
// }

// size_type find_last_not_of(const Char* s, size_type pos = _arg) const
// {
//     return _n(s, pos, traits_type::length(s));
// }


// clang-format on
#undef Impl_StringView_search_mf
#undef Impl_StringView_search_mf2
#undef Impl_StringView_search1
#undef Impl_StringView_search34
#undef Impl_StringView_search_fn_head
	};

	//! \relates basic_string_view
	//@{
	Tpl<Typ Char, class Traits> cexp bool
	operator==(basic_string_view<Char, Traits> x,
		basic_string_view<Char, Traits> y) noexcept
	{
		return x.compare(y) == 0;
	}
	Tpl<Typ Char, class Traits> cexp bool
	operator==(basic_string_view<Char, Traits> x,
		IMPL(decay_t<basic_string_view<Char, Traits>>) y) noexcept
	{
		return x.compare(y) == 0;
	}
	//! \since build 642
	Tpl<Typ Char, class Traits> cexp bool
	operator==(IMPL(decay_t<basic_string_view<Char, Traits>>) x,
		basic_string_view<Char, Traits> y) noexcept
	{
		return x.compare(y) == 0;
	}

	Tpl<Typ Char, class Traits> cexp bool
	operator<(basic_string_view<Char, Traits> x,
		basic_string_view<Char, Traits> y) noexcept
	{
		return x.compare(y) < 0;
	}
	//! \since build 642
	Tpl<Typ Char, class Traits> cexp bool
	operator<(basic_string_view<Char, Traits> x,
		IMPL(decay_t<basic_string_view<Char, Traits>>) y) noexcept
	{
		return x.compare(y) < 0;
	}
	//! \since build 642
	Tpl<Typ Char, class Traits> cexp bool
	operator<(IMPL(decay_t<basic_string_view<Char, Traits>>) x,
		basic_string_view<Char, Traits> y) noexcept
	{
		return x.compare(y) < 0;
	}

	Tpl<Typ Char, class Traits> std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
		basic_string_view<Char, Traits> str)
	{
		// XXX: Better implementation?
		return os << std::basic_string<Char, Traits>(str.data(), str.size());
	}
	//@}
	//@}
	using string_view = basic_string_view<char>;
	using u16string_view = basic_string_view<char16_t>;
	using u32string_view = basic_string_view<char32_t>;
	using wstring_view = basic_string_view<wchar_t>;

	Tpl<class _tString> using string_view_t
		= basic_string_view<Typ _tString::value_type>;

} // namespace Rider::Faiz


#endif
