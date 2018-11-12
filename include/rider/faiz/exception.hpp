#ifndef EXCEPTION
#define EXCEPTION
#include "rider/faiz/cstddef.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"
namespace rider::faiz
{
    // https://stackoverflow.com/questions/29052647/c11-introduced-exception-constructors-taking-const-char-but-why/29052781
    //
    class exception
    {
    public:
        exception() noexcept
        {
        }
        virtual ~exception() noexcept;
        virtual const char* what() const noexcept;
    };

    class bad_exception : public exception
    {
    public:
        bad_exception() noexcept
        {
        }
        virtual ~bad_exception() noexcept;
        virtual const char* what() const noexcept
        {
            return "std::bad_exception";
        }
    };

    using terminate_handler = void (*)();

    terminate_handler set_terminate(terminate_handler) noexcept;
    terminate_handler get_terminate() noexcept;
    void terminate() noexcept;

    bool uncaught_exception() noexcept;
    int uncaught_exceptions() noexcept;

    class exception_ptr;

    exception_ptr current_exception() noexcept;
    void rethrow_exception(exception_ptr);

    class exception_ptr
    {
        void* ptr;

    public:
        exception_ptr() noexcept : ptr()
        {
        }
        exception_ptr(nullptr_t) noexcept : ptr()
        {
        }

        exception_ptr(const exception_ptr&) noexcept;
        exception_ptr& operator=(const exception_ptr&) noexcept;
        ~exception_ptr() noexcept;

        explicit operator bool() const noexcept
        {
            return ptr != nullptr;
        }

        friend bool
        operator==(const exception_ptr& x, const exception_ptr& y) noexcept
        {
            return x.ptr == y.ptr;
        }

        friend bool
        operator!=(const exception_ptr& x, const exception_ptr& y) noexcept
        {
            return !(x == y);
        }

        friend exception_ptr current_exception() noexcept;
        friend void rethrow_exception(exception_ptr);
    };

    template<class E>
    exception_ptr make_exception_ptr(E e) noexcept
    {
        try
        {
            throw e;
        }
        catch (...)
        {
            return current_exception();
        }
    }

    class nested_exception
    {
        exception_ptr ptr;

    public:
        nested_exception() noexcept;
        //     nested_exception(const nested_exception&) noexcept = default;
        //     nested_exception& operator=(const nested_exception&) noexcept =
        //     default;
        virtual ~nested_exception() noexcept;

        // access functions
        void rethrow_nested() const;
        exception_ptr nested_ptr() const noexcept
        {
            return ptr;
        }
    };

    template<class p>
    struct nested : public p, public nested_exception
    {
        explicit nested(const p& t) : p(t)
        {
        }
    };

    template<class p, class U, bool>
    struct throw_with_nested_aux;

    template<class p, class U>
    struct throw_with_nested_aux<p, U, true>
    {
        static inline void do_throw(p&& t)
        {
            throw nested<U>(faiz::forward<p>(t));
        }
    };

    template<class p, class U>
    struct throw_with_nested_aux<p, U, false>
    {
        static inline void do_throw(p&& t)
        {
            throw faiz::forward<p>(t);
        }
    };

    template<class p>
    void throw_with_nested(p&& t)
    {
        using U = decay_t<p>;
        static_assert(is_copy_constructible_v<U>,
            "type thrown must be CopyConstructible");
        throw_with_nested_aux<p,
            U,
            is_class_v<U> && !is_base_of_v<nested_exception, U> &&
                !is_final_v<U>>::do_throw(faiz::forward<p>(t));
    }

    template<class From, class To>
    struct can_dynamic_cast
        : public integral_constant<bool,
              is_polymorphic_v<From> &&
                  (!is_base_of_v<To, From> ||
                      is_convertible_v<const From*, const To*>)>
    {
    };

    template<class E>
    inline void rethrow_if_nested(const E& e,
        typename enable_if<
            can_dynamic_cast<E, nested_exception>::value>::type* = 0)
    {
        const nested_exception* nep =
            dynamic_cast<const nested_exception*>(faiz::addressof(e));
        if (nep)
            nep->rethrow_nested();
    }

    template<class E>
    inline void rethrow_if_nested(const E&,
        typename enable_if<
            !can_dynamic_cast<E, nested_exception>::value>::type* = 0)
    {
    }

} // namespace rider::faiz
#endif
