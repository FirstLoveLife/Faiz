#ifndef CHAR_TRAITS
#define CHAR_TRAITS

#include "rider/faiz/cstring.hpp"
#include "rider/faiz/cwchar.hpp"
#include "rider/faiz/debug.hpp"

#include <ios>
namespace rider::faiz
{
	template<class CharT>
	struct char_traits
	{
		typedef CharT char_type;
		typedef int int_type;
		typedef std::streamoff off_type;
		typedef std::streampos pos_type;
		typedef mbstate_t state_type;

		static inline void constexpr assign(
			char_type& c1, const char_type& c2) noexcept
		{
			c1 = c2;
		}
		static constexpr bool
		eq(char_type c1, char_type c2) noexcept
		{
			return c1 == c2;
		}
		static constexpr bool
		lt(char_type c1, char_type c2) noexcept
		{
			return c1 < c2;
		}

		static constexpr int
		compare(const char_type* s1, const char_type* s2, size_t n);
		static constexpr size_t
		length(const char_type* s);
		static constexpr const char_type*
		find(const char_type* s, size_t n, const char_type& a);
		static char_type*
		move(char_type* s1, const char_type* s2, size_t n);

		static char_type*
		copy(char_type* s1, const char_type* s2, size_t n);

		static char_type*
		assign(char_type* s, size_t n, char_type a);

		static constexpr int_type
		not_eof(int_type c) noexcept
		{
			return eq_int_type(c, eof()) ? ~eof() : c;
		}
		static constexpr char_type
		to_char_type(int_type c) noexcept
		{
			return char_type(c);
		}
		static constexpr int_type
		to_int_type(char_type c) noexcept
		{
			return int_type(c);
		}
		static constexpr bool
		eq_int_type(int_type c1, int_type c2) noexcept
		{
			return c1 == c2;
		}
		static constexpr int_type
		eof() noexcept
		{
			return int_type(EOF);
		}
	};

	template<class CharT>
	constexpr int
	char_traits<CharT>::compare(
		const char_type* s1, const char_type* s2, size_t n)
	{
		for(; n; --n, ++s1, ++s2)
		{
			if(lt(*s1, *s2))
				return -1;
			if(lt(*s2, *s1))
				return 1;
		}
		return 0;
	}

	template<class CharT>
	constexpr size_t
	char_traits<CharT>::length(const char_type* s)
	{
		size_t len = 0;
		for(; !eq(*s, char_type(0)); ++s)
			++len;
		return len;
	}

	template<class CharT>
	constexpr const CharT*
	char_traits<CharT>::find(const char_type* s, size_t n, const char_type& a)
	{
		for(; n; --n)
		{
			if(eq(*s, a))
				return s;
			++s;
		}
		return 0;
	}

	template<class CharT>
	CharT*
	char_traits<CharT>::move(char_type* s1, const char_type* s2, size_t n)
	{
		char_type* r = s1;
		if(s1 < s2)
		{
			for(; n; --n, ++s1, ++s2)
				assign(*s1, *s2);
		}
		else if(s2 < s1)
		{
			s1 += n;
			s2 += n;
			for(; n; --n)
				assign(*--s1, *--s2);
		}
		return r;
	}

	template<class CharT>
	inline CharT*
	char_traits<CharT>::copy(char_type* s1, const char_type* s2, size_t n)
	{
		assertWithLog(
			s2 < s1 || s2 >= s1 + n, "char_traits::copy overlapped range");
		char_type* r = s1;
		for(; n; --n, ++s1, ++s2)
			assign(*s1, *s2);
		return r;
	}

	template<class CharT>
	inline CharT*
	char_traits<CharT>::assign(char_type* s, size_t n, char_type a)
	{
		char_type* r = s;
		for(; n; --n, ++s)
			assign(*s, a);
		return r;
	}

	// char_traits<char>

	template<>
	struct char_traits<char>
	{
		typedef char char_type;
		typedef int int_type;
		typedef std::streamoff off_type;
		typedef std::streampos pos_type;
		typedef mbstate_t state_type;

		static constexpr void
		assign(char_type& c1, const char_type& c2) noexcept
		{
			c1 = c2;
		}
		static constexpr bool
		eq(char_type c1, char_type c2) noexcept
		{
			return c1 == c2;
		}
		static constexpr bool
		lt(char_type c1, char_type c2) noexcept
		{
			return (unsigned char)c1 < (unsigned char)c2;
		}

		static constexpr int
		compare(const char_type* s1, const char_type* s2, size_t n) noexcept;
		static inline size_t constexpr length(const char_type* s) noexcept
		{
			return __builtin_strlen(s);
		}
		static constexpr const char_type*
		find(const char_type* s, size_t n, const char_type& a) noexcept;
		static inline char_type*
		move(char_type* s1, const char_type* s2, size_t n) noexcept
		{
			return n == 0 ? s1 : (char_type*)memmove(s1, s2, n);
		}
		static inline char_type*
		copy(char_type* s1, const char_type* s2, size_t n) noexcept
		{
			Expects(s2 < s1 || s2 >= s1 + n);
			return n == 0 ? s1 : (char_type*)memcpy(s1, s2, n);
		}
		static inline char_type*
		assign(char_type* s, size_t n, char_type a) noexcept
		{
			return n == 0 ? s : (char_type*)memset(s, to_int_type(a), n);
		}

		static constexpr int_type
		not_eof(int_type c) noexcept
		{
			return eq_int_type(c, eof()) ? ~eof() : c;
		}
		static constexpr char_type
		to_char_type(int_type c) noexcept
		{
			return char_type(c);
		}
		static constexpr int_type
		to_int_type(char_type c) noexcept
		{
			return int_type((unsigned char)c);
		}
		static constexpr bool
		eq_int_type(int_type c1, int_type c2) noexcept
		{
			return c1 == c2;
		}
		static constexpr int_type
		eof() noexcept
		{
			return int_type(EOF);
		}
	};

	constexpr int
	char_traits<char>::compare(
		const char_type* s1, const char_type* s2, size_t n) noexcept
	{
		if(n == 0)
			return 0;
		for(; n; --n, ++s1, ++s2)
		{
			if(lt(*s1, *s2))
				return -1;
			if(lt(*s2, *s1))
				return 1;
		}
		return 0;
	}

	constexpr const char*
	char_traits<char>::find(
		const char_type* s, size_t n, const char_type& a) noexcept
	{
		if(n == 0)
			return nullptr;
		for(; n; --n)
		{
			if(eq(*s, a))
				return s;
			++s;
		}
		return nullptr;
	}


	template<>
	struct char_traits<wchar_t>
	{
		typedef wchar_t char_type;
		typedef wint_t int_type;
		typedef std::streamoff off_type;
		typedef std::streampos pos_type;
		typedef mbstate_t state_type;

		static constexpr void
		assign(char_type& c1, const char_type& c2) noexcept
		{
			c1 = c2;
		}
		static constexpr bool
		eq(char_type c1, char_type c2) noexcept
		{
			return c1 == c2;
		}
		static constexpr bool
		lt(char_type c1, char_type c2) noexcept
		{
			return c1 < c2;
		}

		static constexpr int
		compare(const char_type* s1, const char_type* s2, size_t n) noexcept;
		static constexpr size_t
		length(const char_type* s) noexcept;
		static constexpr const char_type*
		find(const char_type* s, size_t n, const char_type& a) noexcept;
		static inline char_type*
		move(char_type* s1, const char_type* s2, size_t n) noexcept
		{
			return n == 0 ? s1 : (char_type*)wmemmove(s1, s2, n);
		}
		static inline char_type*
		copy(char_type* s1, const char_type* s2, size_t n) noexcept
		{
			Expects(s2 < s1 || s2 >= s1 + n);
			return n == 0 ? s1 : (char_type*)wmemcpy(s1, s2, n);
		}
		static inline char_type*
		assign(char_type* s, size_t n, char_type a) noexcept
		{
			return n == 0 ? s : (char_type*)wmemset(s, a, n);
		}

		static constexpr int_type
		not_eof(int_type c) noexcept
		{
			return eq_int_type(c, eof()) ? ~eof() : c;
		}
		static constexpr char_type
		to_char_type(int_type c) noexcept
		{
			return char_type(c);
		}
		static constexpr int_type
		to_int_type(char_type c) noexcept
		{
			return int_type(c);
		}
		static constexpr bool
		eq_int_type(int_type c1, int_type c2) noexcept
		{
			return c1 == c2;
		}
		static constexpr int_type
		eof() noexcept
		{
			return int_type(WEOF);
		}
	};

	constexpr int
	char_traits<wchar_t>::compare(
		const char_type* s1, const char_type* s2, size_t n) noexcept
	{
		if(n == 0)
			return 0;
		for(; n; --n, ++s1, ++s2)
		{
			if(lt(*s1, *s2))
				return -1;
			if(lt(*s2, *s1))
				return 1;
		}
		return 0;
	}

	constexpr size_t
	char_traits<wchar_t>::length(const char_type* s) noexcept
	{
		size_t len = 0;
		for(; !eq(*s, char_type(0)); ++s)
			++len;
		return len;
	}

	constexpr const wchar_t*
	char_traits<wchar_t>::find(
		const char_type* s, size_t n, const char_type& a) noexcept
	{
		if(n == 0)
			return nullptr;
		for(; n; --n)
		{
			if(eq(*s, a))
				return s;
			++s;
		}
		return nullptr;
	}


	template<>
	struct char_traits<char16_t>
	{
		typedef char16_t char_type;
		typedef uint_least16_t int_type;
		typedef std::streamoff off_type;
		typedef std::u16streampos pos_type;
		typedef mbstate_t state_type;

		static constexpr void
		assign(char_type& c1, const char_type& c2) noexcept
		{
			c1 = c2;
		}
		static constexpr bool
		eq(char_type c1, char_type c2) noexcept
		{
			return c1 == c2;
		}
		static constexpr bool
		lt(char_type c1, char_type c2) noexcept
		{
			return c1 < c2;
		}

		static constexpr int
		compare(const char_type* s1, const char_type* s2, size_t n) noexcept;
		static constexpr size_t
		length(const char_type* s) noexcept;
		static constexpr const char_type*
		find(const char_type* s, size_t n, const char_type& a) noexcept;

		static char_type*
		move(char_type* s1, const char_type* s2, size_t n) noexcept;

		static char_type*
		copy(char_type* s1, const char_type* s2, size_t n) noexcept;

		static char_type*
		assign(char_type* s, size_t n, char_type a) noexcept;

		static constexpr int_type
		not_eof(int_type c) noexcept
		{
			return eq_int_type(c, eof()) ? ~eof() : c;
		}
		static constexpr char_type
		to_char_type(int_type c) noexcept
		{
			return char_type(c);
		}
		static constexpr int_type
		to_int_type(char_type c) noexcept
		{
			return int_type(c);
		}
		static constexpr bool
		eq_int_type(int_type c1, int_type c2) noexcept
		{
			return c1 == c2;
		}
		static constexpr int_type
		eof() noexcept
		{
			return int_type(0xFFFF);
		}
	};

	constexpr int
	char_traits<char16_t>::compare(
		const char_type* s1, const char_type* s2, size_t n) noexcept
	{
		for(; n; --n, ++s1, ++s2)
		{
			if(lt(*s1, *s2))
				return -1;
			if(lt(*s2, *s1))
				return 1;
		}
		return 0;
	}

	constexpr size_t
	char_traits<char16_t>::length(const char_type* s) noexcept
	{
		size_t len = 0;
		for(; !eq(*s, char_type(0)); ++s)
			++len;
		return len;
	}

	constexpr const char16_t*
	char_traits<char16_t>::find(
		const char_type* s, size_t n, const char_type& a) noexcept
	{
		for(; n; --n)
		{
			if(eq(*s, a))
				return s;
			++s;
		}
		return 0;
	}

	inline char16_t*
	char_traits<char16_t>::move(
		char_type* s1, const char_type* s2, size_t n) noexcept
	{
		char_type* r = s1;
		if(s1 < s2)
		{
			for(; n; --n, ++s1, ++s2)
				assign(*s1, *s2);
		}
		else if(s2 < s1)
		{
			s1 += n;
			s2 += n;
			for(; n; --n)
				assign(*--s1, *--s2);
		}
		return r;
	}

	inline char16_t*
	char_traits<char16_t>::copy(
		char_type* s1, const char_type* s2, size_t n) noexcept
	{
		Expects(s2 < s1 || s2 >= s1 + n);
		char_type* r = s1;
		for(; n; --n, ++s1, ++s2)
			assign(*s1, *s2);
		return r;
	}

	inline char16_t*
	char_traits<char16_t>::assign(char_type* s, size_t n, char_type a) noexcept
	{
		char_type* r = s;
		for(; n; --n, ++s)
			assign(*s, a);
		return r;
	}

	template<>
	struct char_traits<char32_t>
	{
		typedef char32_t char_type;
		typedef uint_least32_t int_type;
		typedef std::streamoff off_type;
		typedef std::u32streampos pos_type;
		typedef mbstate_t state_type;

		static constexpr void
		assign(char_type& c1, const char_type& c2) noexcept
		{
			c1 = c2;
		}
		static constexpr bool
		eq(char_type c1, char_type c2) noexcept
		{
			return c1 == c2;
		}
		static constexpr bool
		lt(char_type c1, char_type c2) noexcept
		{
			return c1 < c2;
		}

		static constexpr int
		compare(const char_type* s1, const char_type* s2, size_t n) noexcept;
		static constexpr size_t
		length(const char_type* s) noexcept;
		static constexpr const char_type*
		find(const char_type* s, size_t n, const char_type& a) noexcept;

		static char_type*
		move(char_type* s1, const char_type* s2, size_t n) noexcept;

		static char_type*
		copy(char_type* s1, const char_type* s2, size_t n) noexcept;

		static char_type*
		assign(char_type* s, size_t n, char_type a) noexcept;

		static constexpr int_type
		not_eof(int_type c) noexcept
		{
			return eq_int_type(c, eof()) ? ~eof() : c;
		}
		static constexpr char_type
		to_char_type(int_type c) noexcept
		{
			return char_type(c);
		}
		static constexpr int_type
		to_int_type(char_type c) noexcept
		{
			return int_type(c);
		}
		static constexpr bool
		eq_int_type(int_type c1, int_type c2) noexcept
		{
			return c1 == c2;
		}
		static constexpr int_type
		eof() noexcept
		{
			return int_type(0xFFFFFFFF);
		}
	};

	constexpr int
	char_traits<char32_t>::compare(
		const char_type* s1, const char_type* s2, size_t n) noexcept
	{
		for(; n; --n, ++s1, ++s2)
		{
			if(lt(*s1, *s2))
				return -1;
			if(lt(*s2, *s1))
				return 1;
		}
		return 0;
	}

	constexpr size_t
	char_traits<char32_t>::length(const char_type* s) noexcept
	{
		size_t len = 0;
		for(; !eq(*s, char_type(0)); ++s)
			++len;
		return len;
	}

	constexpr const char32_t*
	char_traits<char32_t>::find(
		const char_type* s, size_t n, const char_type& a) noexcept
	{
		for(; n; --n)
		{
			if(eq(*s, a))
				return s;
			++s;
		}
		return 0;
	}

	inline char32_t*
	char_traits<char32_t>::move(
		char_type* s1, const char_type* s2, size_t n) noexcept
	{
		char_type* r = s1;
		if(s1 < s2)
		{
			for(; n; --n, ++s1, ++s2)
				assign(*s1, *s2);
		}
		else if(s2 < s1)
		{
			s1 += n;
			s2 += n;
			for(; n; --n)
				assign(*--s1, *--s2);
		}
		return r;
	}

	inline char32_t*
	char_traits<char32_t>::copy(
		char_type* s1, const char_type* s2, size_t n) noexcept
	{
		Expects(s2 < s1 || s2 >= s1 + n);
		char_type* r = s1;
		for(; n; --n, ++s1, ++s2)
			assign(*s1, *s2);
		return r;
	}

	inline char32_t*
	char_traits<char32_t>::assign(char_type* s, size_t n, char_type a) noexcept
	{
		char_type* r = s;
		for(; n; --n, ++s)
			assign(*s, a);
		return r;
	}
} // namespace rider::faiz
#endif
