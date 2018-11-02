#ifndef SCTDDEF
#    define SCTDDEF
namespace rider::faiz
{

    using size_t = __SIZE_TYPE__;

    // faiz::ptrdiff_t is the signed integer type of the result of subtracting
    // two pointers.
    using ptrdiff_t = __PTRDIFF_TYPE__;
#    if __cplusplus >= 201103L
    struct nullptr_t
    {
        void* lx;

        struct nat
        {
            int for_bool_;
        };

        constexpr nullptr_t() : lx(0)
        {
        }
        constexpr nullptr_t(int nat::*) : lx(0)
        {
        }

        constexpr operator int nat::*() const
        {
            return 0;
        }

        template<class T>
        constexpr operator T*() const
        {
            return 0;
        }

        template<class T, class U>
        operator T U::*() const
        {
            return 0;
        }

        friend constexpr bool operator==(nullptr_t, nullptr_t)
        {
            return true;
        }
        friend constexpr bool operator!=(nullptr_t, nullptr_t)
        {
            return false;
        }
#    else
    using nullptr_t = decltype(nullptr);
#    endif

    }; // namespace rider::faiz

    inline constexpr nullptr_t get_nullptr_t()
    {
        return nullptr_t(0);
    }
#    define nullptr get_nullptr_t()
} // namespace rider::faiz
#endif
