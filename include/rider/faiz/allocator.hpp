#ifndef ALLOCATOR
#define ALLOCATOR
#include "rider/faiz/cstddef.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"
namespace rider::faiz
{
    // The **std::allocator** class template is the default Allocator used by
    // all standard library containers if no user-specified allocator is
    // provided. The default allocator is stateless, that is, all instances of
    // the given allocator are interchangeable, compare equal and can deallocate
    // memory allocated by any other instance of the same allocator type.
    //
    template<class T>
    class allocator
    {
    public:
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using value_type = T;

        using propagate_on_container_move_assignment = true_type;
        using is_always_equal = true_type;

        template<class U>
        struct rebind
        {
            using other = allocator<U>;
        };

        constexpr allocator() noexcept
        {
        }

        template<class U>
        constexpr allocator(const allocator<U>&) noexcept
        {
        }

        pointer address(reference x) const noexcept
        {
            return faiz::addressof(x);
        }
        const_pointer address(const_reference x) const noexcept
        {
            return faiz::addressof(x);
        }
        pointer allocate(size_type n, allocator<void>::const_pointer = 0)
        {
            if (n > max_size())
                length_error("allocator<T>::allocate(size_t n)"
                             " 'n' exceeds maximum supported size");
            return static_cast<pointer>(
                faiz::__libcpp_allocate(n * sizeof(T), __alignof(T)));
        }
        void deallocate(pointer p, size_type n) noexcept
        {
            faiz::__libcpp_deallocate((void*)p, n * sizeof(T), __alignof(T));
        }
        size_type max_size() const noexcept
        {
            return size_type(~0) / sizeof(T);
        }
#if !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES) && \
    !defined(_LIBCPP_HAS_NO_VARIADICS)
        template<class U, class... _Args>
        _LIBCPP_INLINE_VISIBILITY void construct(U* p, _Args&&... __args)
        {
            ::new ((void*)p) U(faiz::forward<_Args>(__args)...);
        }
#else // !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES) &&
      // !defined(_LIBCPP_HAS_NO_VARIADICS)
        _LIBCPP_INLINE_VISIBILITY
        void construct(pointer p)
        {
            ::new ((void*)p) T();
        }
#    if defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)

        template<class _A0>
        _LIBCPP_INLINE_VISIBILITY void construct(pointer p, _A0& __a0)
        {
            ::new ((void*)p) T(__a0);
        }
        template<class _A0>
        _LIBCPP_INLINE_VISIBILITY void construct(pointer p, const _A0& __a0)
        {
            ::new ((void*)p) T(__a0);
        }
#    endif // defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)
        template<class _A0, class _A1>
        _LIBCPP_INLINE_VISIBILITY void
        construct(pointer p, _A0& __a0, _A1& __a1)
        {
            ::new ((void*)p) T(__a0, __a1);
        }
        template<class _A0, class _A1>
        _LIBCPP_INLINE_VISIBILITY void
        construct(pointer p, const _A0& __a0, _A1& __a1)
        {
            ::new ((void*)p) T(__a0, __a1);
        }
        template<class _A0, class _A1>
        _LIBCPP_INLINE_VISIBILITY void
        construct(pointer p, _A0& __a0, const _A1& __a1)
        {
            ::new ((void*)p) T(__a0, __a1);
        }
        template<class _A0, class _A1>
        _LIBCPP_INLINE_VISIBILITY void
        construct(pointer p, const _A0& __a0, const _A1& __a1)
        {
            ::new ((void*)p) T(__a0, __a1);
        }
#endif // !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES) &&
       // !defined(_LIBCPP_HAS_NO_VARIADICS)
        _LIBCPP_INLINE_VISIBILITY void destroy(pointer p)
        {
            p->~T();
        }
    };
} // namespace rider::faiz
#endif
