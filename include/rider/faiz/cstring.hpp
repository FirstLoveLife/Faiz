#ifndef CSTRING
#define CSTRING
#include "rider/faiz/algorithm.hpp"
#include "rider/faiz/cstddef.hpp" // for size_t
#include "rider/faiz/debug.hpp"
#include "rider/faiz/macros.hpp"
namespace Rider::Faiz
{
	Tpl<class CharT> class char_traits;

	// Copies count characters from the object pointed to by src to the object
	// pointed to by dest. Both objects are reinterpreted as arrays of unsigned
	// char.
	// The objects may overlap: copying takes place as if the characters were
	// copied to a temporary character array and then the characters were copied
	// from the array to dest.
	void*
	memmove(void* dest, const void* src, Faiz::size_t count)
	{
		auto d = static_cast<char*>(dest);
		auto s = static_cast<char const*>(src);

		if(s < d)
		{
			s += count;
			d += count;
			while(count--)
			{
				*--d = *--s;
			}
		}
		else
		{
			while(count--)
			{
				*d++ = *s++;
			}
		}

		return dest;
	}

	// Copies exactly count successive wide characters from the wide character
	// array pointed to by src to the wide character array pointed to by dest.
	// If count is zero, the function does nothing.
	// The arrays may overlap: copying takes place as if the wide characters
	// were copied to a temporary wide character array and then copied from the
	// temporary array to dest.
	void*
	wmemmove(wchar_t* dest, const wchar_t* src, Faiz::size_t count)
	{
		auto d = static_cast<wchar_t*>(dest);
		auto s = static_cast<wchar_t const*>(src);

		if(s < d)
		{
			s += count;
			d += count;
			while(count--)
			{
				*--d = *--s;
			}
		}
		else
		{
			while(count--)
			{
				*d++ = *s++;
			}
		}

		return dest;
	}

	// I don't want to implement memcpy, because I am not able to optimize it.
	void* __attribute__((weak)) memcpy(void* s1, const void* s2, size_t n)
	{
		return memmove(s1, s2, n);
	}

	void* __attribute__((weak))
	wmemcpy(wchar_t* s1, const wchar_t* s2, size_t n)
	{
		return wmemmove(s1, s2, n);
	}

	void*
	memset(void* dest, int ch, size_t n)
	{
		size_t i;

		for(i = 0; i < n; ++i)
		{
			((unsigned char*)dest)[i] = ch;
		}

		return dest;
	}


	Tpl<Typ Char,
		Typ Size,
		class Traits = Faiz::char_traits<Char>,
		Size NPos = Size(-1)>
		cexp Size
		str_find(const Char* p, Size sz, Char c, Size pos) noexcept
	{
		Expects(p || sz == 0);
		if(pos < sz)
			if(const Char* ret = Traits::find(p + pos, sz - pos, c))
				return Size(ret - p);
		return NPos;
	}
	Tpl<Typ Char,
		Typ Size,
		class Traits = Faiz::char_traits<Char>,
		Size NPos = Size(-1)>
		cexp Size
		str_find(
			const Char* p, Size sz, const Char* s, Size pos, Size n) noexcept
	{
		Expects((p || sz == 0) && (s || n == 0));
		if(n == 0)
			return pos <= sz ? pos : NPos;
		if(n <= sz)
			for(; pos <= sz - n; ++pos)
				if(Traits::eq(p[pos], s[0])
					&& Traits::compare(p + pos + 1, s + 1, n - 1) == 0)
					return pos;
		return NPos;
	}

	Tpl<Typ Char,
		Typ Size,
		class Traits = Faiz::char_traits<Char>,
		Size NPos = Size(-1)>
		cexp Size
		str_rfind(const Char* p, Size sz, Char c, Size pos) noexcept
	{
		Expects(p || sz == 0);
		if(0 < sz)
		{
			if(pos < --sz)
				sz = pos;
			for(++sz; sz-- > 0;)
				if(Traits::eq(*(p + sz), c))
					return sz;
		}
		return NPos;
	}
	Tpl<Typ Char,
		Typ Size,
		class Traits = Faiz::char_traits<Char>,
		Size NPos = Size(-1)>
		cexp Size
		str_rfind(
			const Char* p, Size sz, const Char* s, Size pos, Size n) noexcept
	{
		Expects((p || sz == 0) && (s || n == 0));
		if(n <= sz)
		{
			pos = Faiz::min(Size(sz - n), pos);
			do
				if(Traits::compare(p + pos, s, n) == 0)
					return pos;
			while(pos-- > 0);
		}
		return NPos;
	}

	Tpl<Typ Char,
		Typ Size,
		class Traits = Faiz::char_traits<Char>,
		Size NPos = Size(-1)>
		cexp Size
		str_find_first_of(
			const Char* p, Size sz, const Char* s, Size pos, Size n) noexcept
	{
		Expects((p || sz == 0) && (s || n == 0));
		if(n != 0)
		{
			for(; pos < sz; ++pos)
				if(Traits::find(s, n, p[pos]))
					return pos;
		}
		return NPos;
	}

	Tpl<Typ Char,
		Typ Size,
		class Traits = Faiz::char_traits<Char>,
		Size NPos = Size(-1)>
		cexp Size
		str_find_last_of(
			const Char* p, Size sz, const Char* s, Size pos, Size n) noexcept
	{
		Expects((p || sz == 0) && (s || n == 0));
		if(sz != 0 && n != 0)
		{
			if(--sz > pos)
				sz = pos;
			do
				if(Traits::find(s, n, p[sz]))
					return sz;
			while(sz-- != 0);
		}
		return NPos;
	}

	Tpl<Typ Char,
		Typ Size,
		class Traits = Faiz::char_traits<Char>,
		Size NPos = Size(-1)>
		cexp Size
		str_find_first_not_of(const Char* p, Size sz, Char c, Size pos) noexcept
	{
		Expects(p || sz == 0);
		for(; pos < sz; ++pos)
			if(!Traits::eq(p[pos], c))
				return pos;
		return NPos;
	}
	Tpl<Typ Char,
		Typ Size,
		class Traits = Faiz::char_traits<Char>,
		Size NPos = Size(-1)>
		cexp Size
		str_find_first_not_of(
			const Char* p, Size sz, const Char* s, Size pos, Size n) noexcept
	{
		Expects((p || sz == 0) && (s || n == 0));
		for(; pos < sz; ++pos)
			if(!Traits::find(s, n, p[pos]))
				return pos;
		return NPos;
	}

	Tpl<Typ Char,
		Typ Size,
		class Traits = Faiz::char_traits<Char>,
		Size NPos = Size(-1)>
		cexp Size
		str_find_last_not_of(const Char* p, Size sz, Char c, Size pos) noexcept
	{
		Expects(p || sz == 0);
		if(sz != 0)
		{
			if(--sz > pos)
				sz = pos;
			do
				if(!Traits::eq(p[sz], c))
					return sz;
			while(sz-- != 0);
		}
		return NPos;
	}
	Tpl<Typ Char,
		Typ Size,
		class Traits = Faiz::char_traits<Char>,
		Size NPos = Size(-1)>
		cexp Size
		str_find_last_not_of(
			const Char* p, Size sz, const Char* s, Size pos, Size n) noexcept
	{
		Expects((p || sz == 0) && (s || n == 0));
		if(sz != 0)
		{
			if(--sz > pos)
				sz = pos;
			do
				if(!Traits::find(s, n, p[sz]))
					return sz;
			while(sz-- != 0);
		}
		return NPos;
	}

} // namespace Rider::Faiz
#endif
