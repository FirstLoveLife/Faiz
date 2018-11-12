#ifndef CSTRING
#define CSTRING
#include "rider/faiz/cstddef.hpp"
namespace rider::faiz
{

    // Copies count characters from the object pointed to by src to the object
    // pointed to by dest. Both objects are reinterpreted as arrays of unsigned
    // char.
    // The objects may overlap: copying takes place as if the characters were
    // copied to a temporary character array and then the characters were copied
    // from the array to dest.
    void* memmove(void* dest, const void* src, faiz::size_t count)
    {
        auto d = static_cast<char*>(dest);
        auto s = static_cast<char const*>(src);

        if (s < d)
        {
            s += count;
            d += count;
            while (count--)
            {
                *--d = *--s;
            }
        }
        else
        {
            while (count--)
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
    void* wmemmove(wchar_t* dest, const wchar_t* src, faiz::size_t count)
    {
        auto d = static_cast<wchar_t*>(dest);
        auto s = static_cast<wchar_t const*>(src);

        if (s < d)
        {
            s += count;
            d += count;
            while (count--)
            {
                *--d = *--s;
            }
        }
        else
        {
            while (count--)
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

    void* memset(void* dest, int ch, size_t n)
    {
        size_t i;

        for (i = 0; i < n; ++i)
        {
            ((unsigned char*)dest)[i] = ch;
        }

        return dest;
    }
} // namespace rider::faiz
#endif
