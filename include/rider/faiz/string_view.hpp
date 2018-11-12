#ifndef STRING_VIEW
#define STRING_VIEW
#include "rider/faiz/algorithm.hpp"
#include "rider/faiz/char_traits.hpp"
#include "rider/faiz/faiz.hpp" // for IMPL
#include "rider/faiz/iterator.hpp"
#include "rider/faiz/operators.hpp"
#include "rider/faiz/type_traits.hpp"

namespace rider::faiz
{
	template<typename _tChar, class _tTraits = std::char_traits<_tChar>>
	class basic_string_view
		: private totally_ordered<basic_string_view<_tChar, _tTraits>>
	{
	public:
		using traits_type = _tTraits;
		using value_type = _tChar;
		using pointer = _tChar*;
		using const_pointer = const _tChar*;
		using reference = _tChar&;
		using const_reference = const _tChar&;
		using const_iterator = IMPL(const _tChar*);
		using iterator = const_iterator;
		using const_reverse_iterator = faiz::reverse_iterator<const_iterator>;
		using reverse_iterator = const_reverse_iterator;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		static constexpr size_type npos = size_type(-1);

	private:
		const_pointer data_;
		size_type size_;

	public:
		constexpr basic_string_view() noexcept : data_(), size_()
		{}

		constexpr basic_string_view(const _tChar* str)
			: data_(str), size_(traits_type::length(str))
		{}

		constexpr basic_string_view(const _tChar* str, size_type len)
			: data_(str), size_(len)
		{}
		constexpr basic_string_view(const basic_string_view&) noexcept
			= default;

		basic_string_view&
		operator=(const basic_string_view&) noexcept
			= default;

		constexpr const_iterator
		begin() const noexcept
		{
			return data_;
		}

		constexpr const_iterator
		end() const noexcept
		{
			return data_ + size_;
		}

		constexpr const_iterator
		cbegin() const noexcept
		{
			return data_;
		}

		constexpr const_iterator
		cend() const noexcept
		{
			return data_ + size_;
		}

		constexpr const_reverse_iterator
		rbegin() const noexcept
		{
			return const_reverse_iterator(end());
		}

		constexpr const_reverse_iterator
		rend() const noexcept
		{
			return const_reverse_iterator(begin());
		}

		constexpr const_reverse_iterator
		crbegin() const noexcept
		{
			return const_reverse_iterator(cend());
		}

		constexpr const_reverse_iterator
		crend() const noexcept
		{
			return const_reverse_iterator(cbegin());
		}

		constexpr size_type
		size() const noexcept
		{
			return size_;
		}

		constexpr size_type
		length() const noexcept
		{
			return size_;
		}

		constexpr size_type
		max_size() const noexcept
		{
			// NOTE: This is same to libc++ but not libstdc++.
			return std::numeric_limits<size_type>::max();
		}

		constexpr bool
		empty() const noexcept
		{
			return size_ == 0;
		}

		constexpr const_reference operator[](size_type pos) const
		{
			return yconstraint(pos < size_), data_[pos];
		}

		constexpr const_reference
		at(size_type pos) const
		{
			return pos < size_ ?
				data_[pos] :
				(throw std::out_of_range("string_view::at"), data_[0]);
		}

		constexpr const_reference
		front() const
		{
			return yconstraint(0 < size_), data_[0];
		}

		constexpr const_reference
		back() const
		{
			return yconstraint(0 < size_), data_[size_ - 1];
		}

		constexpr const_pointer
		data() const noexcept
		{
			return data_;
		}

		void
		remove_prefix(size_type n) noexcept
		{
			yconstraint(n <= size_), yunseq(data_ += n, size_ -= n);
		}

		void
		remove_suffix(size_type n) noexcept
		{
			yconstraint(n <= size_), size_ -= n;
		}

		void
		swap(basic_string_view& s) noexcept
		{
			std::swap(data_, s.data_), std::swap(size_, s.size_);
		}


		size_type
		copy(_tChar* s, size_type n, size_type pos = 0) const
		{
			yconstraint(s);
			if(pos <= size_)
			{
				const auto rlen(std::min<size_type>(n, size_ - pos));

				faiz::copy_n(data_ + pos, rlen, s);
				return rlen;
			}
			throw std::out_of_range("string_view::copy");
		}

		constexpr basic_string_view
		substr(size_type pos = 0, size_type n = npos) const
		{
			return pos <= size_ ?
				basic_string_view(
					data_ + pos, std::min(n, size_type(size_ - pos))) :
				throw std::out_of_range("string_view::substr");
		}

		constexpr int
		compare(basic_string_view s) const noexcept
		{
			return compare_remained(s,
				_tTraits::compare(
					data(), s.data(), std::min<size_type>(size(), s.size())));
		}
		constexpr int
		compare(size_type pos1, size_type n1, basic_string_view s) const
		{
			return substr(pos1, n1).compare(s);
		}
		constexpr int
		compare(size_type pos1,
			size_type n1,
			basic_string_view s,
			size_type pos2,
			size_type n2) const
		{
			return substr(pos1, n1).compare(s.substr(pos2, n2));
		}
		constexpr int
		compare(const _tChar* s) const
		{
			return compare(basic_string_view(s));
		}

		constexpr int
		compare(size_type pos1, size_type n1, const _tChar* s) const
		{
			return substr(pos1, n1).compare(basic_string_view(s));
		}

		constexpr int
		compare(
			size_type pos1, size_type n1, const _tChar* s, size_type n2) const
		{
			return substr(pos1, n1).compare(basic_string_view(s, n2));
		}

	private:
		constexpr int
		compare_remained(basic_string_view s, int ret) const noexcept
		{
			return ret == 0 ?
				(size() == s.size() ? 0 : (size() < s.size() ? -1 : 1)) :
				ret;
		}

	public:
#define Impl_StringView_search_fn_head(_n, _attr, _spec, ...) \
	_attr size_type _n(__VA_ARGS__) const _spec \
	{
#define Impl_StringView_search1(_n, _arg) \
	Impl_StringView_search_fn_head(_n, \
		, \
		noexcept, \
		basic_string_view s, \
		size_type pos = _arg) return ystdex:: \
		str_##_n<value_type, size_type, traits_type, npos>( \
			data_, size_, s.data_, pos, s.size_); \
	}
#define Impl_StringView_search34(_n, _arg) \
	Impl_StringView_search_fn_head(_n, \
		, \
		, \
		const _tChar* s, \
		size_type pos, \
		size_type n) return yconstraint(s), \
		_n(basic_string_view(s, n), pos); \
	} \
	Impl_StringView_search_fn_head( \
		_n, , , const _tChar* s, size_type pos = _arg) return _n(s, \
		pos, \
		traits_type::length(s)); \
	}
#define Impl_StringView_search_mf(_n, _arg) \
	Impl_StringView_search1(_n, _arg) Impl_StringView_search_fn_head( \
		_n, , noexcept, _tChar c, size_type pos = _arg) return ystdex:: \
		str_##_n<value_type, size_type, traits_type, npos>( \
			data_, size_, c, pos); \
	} \
	Impl_StringView_search34(_n, _arg)
#define Impl_StringView_search_mf2(_n, _arg, _n2) \
	Impl_StringView_search1(_n, _arg) Impl_StringView_search_fn_head( \
		_n, , noexcept, _tChar c, size_type pos = _arg) return _n2(c, pos); \
	} \
	Impl_StringView_search34(_n, _arg)

		// clang-format off
		Impl_StringView_search_mf(find, 0)

			Impl_StringView_search_mf(rfind, npos)

				Impl_StringView_search_mf2(find_first_of, 0, find)

					Impl_StringView_search_mf2(find_last_of, npos, rfind)

						Impl_StringView_search_mf(find_first_not_of, 0)

							Impl_StringView_search_mf(find_last_not_of, npos)

// clang-format on
#undef Impl_StringView_search_mf
#undef Impl_StringView_search_mf2
#undef Impl_StringView_search1
#undef Impl_StringView_search34
#undef Impl_StringView_search_fn_head
	};

	//! \relates basic_string_view
	//@{
	template<typename _tChar, class _tTraits>
	constexpr bool
	operator==(basic_string_view<_tChar, _tTraits> x,
		basic_string_view<_tChar, _tTraits> y) noexcept
	{
		return x.compare(y) == 0;
	}
	template<typename _tChar, class _tTraits>
	constexpr bool
	operator==(basic_string_view<_tChar, _tTraits> x,
		IMPL(decay_t<basic_string_view<_tChar, _tTraits>>) y) noexcept
	{
		return x.compare(y) == 0;
	}
	//! \since build 642
	template<typename _tChar, class _tTraits>
	constexpr bool
	operator==(IMPL(decay_t<basic_string_view<_tChar, _tTraits>>) x,
		basic_string_view<_tChar, _tTraits> y) noexcept
	{
		return x.compare(y) == 0;
	}

	template<typename _tChar, class _tTraits>
	constexpr bool
	operator<(basic_string_view<_tChar, _tTraits> x,
		basic_string_view<_tChar, _tTraits> y) noexcept
	{
		return x.compare(y) < 0;
	}
	//! \since build 642
	template<typename _tChar, class _tTraits>
	constexpr bool
	operator<(basic_string_view<_tChar, _tTraits> x,
		IMPL(decay_t<basic_string_view<_tChar, _tTraits>>) y) noexcept
	{
		return x.compare(y) < 0;
	}
	//! \since build 642
	template<typename _tChar, class _tTraits>
	constexpr bool
	operator<(IMPL(decay_t<basic_string_view<_tChar, _tTraits>>) x,
		basic_string_view<_tChar, _tTraits> y) noexcept
	{
		return x.compare(y) < 0;
	}

	template<typename _tChar, class _tTraits>
	std::basic_ostream<_tChar, _tTraits>&
	operator<<(std::basic_ostream<_tChar, _tTraits>& os,
		basic_string_view<_tChar, _tTraits> str)
	{
		// XXX: Better implementation?
		return os
			<< std::basic_string<_tChar, _tTraits>(str.data(), str.size());
	}
	//@}
	//@}
	using string_view = basic_string_view<char>;
	using u16string_view = basic_string_view<char16_t>;
	using u32string_view = basic_string_view<char32_t>;
	using wstring_view = basic_string_view<wchar_t>;

} // namespace rider::faiz

template<class _tString>
using string_view_t = basic_string_view<typename _tString::value_type>;

#endif
