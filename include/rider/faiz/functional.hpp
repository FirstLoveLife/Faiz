#ifndef FUNCTIONAL
#define FUNCTIONAL
/*
Don't implement myself: not_fn, invoke
 */

#include "rider/faiz/exception.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"

#include <functional>
namespace rider::faiz
{
    // both call_traits::param_type and this conditional check are ok, but
    // check is more simple.
    template<typename T>
    struct check : faiz::conditional<faiz::is_arithmetic<T>::value, T, check<T>>
    {
    };
    template<typename Arg, typename Result>
    struct unary_function
    {
        using argument_type = Arg;

        using result_type = Result;
    };

    template<typename Arg1, typename Arg2, typename Result>
    struct binary_function
    {
        using firstargument_type = Arg1;

        using secondargument_type = Arg2;

        using result_type = Result;
    };

    struct _is_transparent; // undefined

    template<typename T = void>
    struct plus;

    template<typename T = void>
    struct minus;

    template<typename T = void>
    struct multiplies;

    template<typename T = void>
    struct divides;

    template<typename T = void>
    struct modulus;

    template<typename T = void>
    struct negate;

    template<typename T>
    struct plus : public binary_function<T, T, T>
    {
        constexpr T operator()(check<T> x, check<T> y) const
        {
            return x + y;
        }
    };

    template<>
    struct plus<void>
    {
        template<typename T, typename U>
        constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(faiz::forward<T>(t) + faiz::forward<U>(u)))
        {
            return faiz::forward<T>(t) + faiz::forward<U>(u);
        }
    };

    template<typename T>
    struct minus : public binary_function<T, T, T>
    {
        constexpr T operator()(check<T> x, check<T> y) const
        {
            return x - y;
        }
    };

    template<>
    struct minus<void>
    {
        template<typename T, typename U>
        constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(faiz::forward<T>(t) - faiz::forward<U>(u)))
        {
            return faiz::forward<T>(t) - faiz::forward<U>(u);
        }
    };

    template<typename T>
    struct multiplies : public binary_function<T, T, T>
    {
        constexpr T operator()(check<T> x, check<T> y) const
        {
            return x * y;
        }
    };

    template<>
    struct multiplies<void>
    {
        template<typename T, typename U>
        constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(faiz::forward<T>(t) * faiz::forward<U>(u)))
        {
            return faiz::forward<T>(t) * faiz::forward<U>(u);
        }
    };


    template<typename T>
    struct divides : public binary_function<T, T, T>
    {
        constexpr T operator()(check<T> x, check<T> y) const
        {
            return x / y;
        }
    };

    template<>
    struct divides<void>
    {
        template<typename T, typename U>
        constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(faiz::forward<T>(t) / faiz::forward<U>(u)))
        {
            return faiz::forward<T>(t) / faiz::forward<U>(u);
        }
    };


    template<typename T>
    struct modulus : public binary_function<T, T, T>
    {
        constexpr T operator()(check<T> x, check<T> y) const
        {
            return x % y;
        }
    };

    template<>
    struct modulus<void>
    {
        typedef int is_transparent;
        template<typename A, typename B>
        constexpr decltype(auto) operator()(A&& a, B&& b) const
        {
            return faiz::forward<A>(a) % faiz::forward<B>(b);
        }
    };


    template<typename T>
    struct negate : public unary_function<T, T>
    {
        constexpr T operator()(check<T> x) const
        {
            return -x;
        }
    };

    template<>
    struct negate<void>
    {
        template<typename T>
        constexpr decltype(auto)
        operator()(T&& t) const noexcept(noexcept(-faiz::forward<T>(t)))
        {
            return -faiz::forward<T>(t);
        }
    };

    template<typename T = void>
    struct equal_to;

    template<typename T = void>
    struct not_equal_to;

    template<typename T = void>
    struct greater;

    template<typename T = void>
    struct less;

    template<typename T = void>
    struct greater_equal;

    template<typename T = void>
    struct less_equal;

    template<typename T>
    struct equal_to : public binary_function<T, T, bool>
    {
        constexpr bool operator()(check<T> x, check<T> y) const
        {
            return x == y;
        }
    };

    template<>
    struct equal_to<void>
    {
        typedef int is_transparent;
        template<typename A, typename B>
        constexpr decltype(auto) operator()(A&& a, B&& b) const
        {
            return faiz::forward<A>(a) == faiz::forward<B>(b);
        }
    };

    template<>
    struct not_equal_to<void>
    {
        template<typename T, typename U>
        constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(faiz::forward<T>(t) != faiz::forward<U>(u)))
        {
            return faiz::forward<T>(t) != faiz::forward<U>(u);
        }
    };

    template<typename T>
    struct not_equal_to : public binary_function<T, T, bool>
    {
        constexpr bool operator()(check<T> x, check<T> y) const
        {
            return x != y;
        }
    };


    template<typename T>
    struct greater : public binary_function<T, T, bool>
    {
        constexpr bool operator()(check<T> x, check<T> y) const
        {
            return x > y;
        }
    };

    template<>
    struct greater<void>
    {
        template<typename A, typename B>
        constexpr decltype(auto) operator()(A&& a, B&& b) const
        {
            return faiz::forward<A>(a) > faiz::forward<B>(b);
        }
    };

    template<typename T>
    struct less : public binary_function<T, T, bool>
    {
        constexpr bool operator()(check<T> x, check<T> y) const
        {
            return x < y;
        }
    };

    template<>
    struct less<void>
    {
        template<typename A, typename B>
        constexpr decltype(auto) operator()(A&& a, B&& b) const
        {
            return faiz::forward<A>(a) < faiz::forward<B>(b);
        }
    };


    template<typename T>
    struct greater_equal : public binary_function<T, T, bool>
    {
        constexpr bool operator()(check<T> x, check<T> y) const
        {
            return x >= y;
        }
    };

    template<>
    struct greater_equal<void>
    {
        template<typename A, typename B>
        constexpr decltype(auto) operator()(A&& a, B&& b) const
        {
            return faiz::forward<A>(a) >= faiz::forward<B>(b);
        }
    };


    template<typename T>
    struct less_equal : public binary_function<T, T, bool>
    {
        constexpr bool operator()(check<T> x, check<T> y) const
        {
            return x <= y;
        }
    };

    template<>
    struct less_equal<void>
    {
        template<typename A, typename B>
        constexpr decltype(auto) operator()(A&& a, B&& b) const
        {
            return faiz::forward<A>(a) <= faiz::forward<B>(b);
        }
    };


    template<typename T = void>
    struct logical_and;

    template<typename T = void>
    struct logical_or;

    template<typename T = void>
    struct logical_not;

    template<typename T>
    struct logical_and : public binary_function<T, T, bool>
    {
        constexpr bool operator()(check<T> x, check<T> y) const
        {
            return x && y;
        }
    };

    template<>
    struct logical_and<void>
    {
        template<typename T, typename U>
        constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(faiz::forward<T>(t) && faiz::forward<U>(u)))
        {
            return faiz::forward<T>(t) && faiz::forward<U>(u);
        }
    };

    template<typename T>
    struct logical_or : public binary_function<T, T, bool>
    {
        constexpr bool operator()(check<T> x, check<T> y) const
        {
            return x || y;
        }
    };

    template<>
    struct logical_or<void>
    {
        template<typename T, typename U>
        constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(faiz::forward<T>(t) || faiz::forward<U>(u)))
        {
            return faiz::forward<T>(t) || faiz::forward<U>(u);
        }
    };

    template<typename T>
    struct logical_not : public unary_function<T, bool>
    {
        constexpr bool operator()(check<T> x) const
        {
            return !x;
        }
    };


    template<>
    struct logical_not<void>
    {
        template<typename T>
        constexpr decltype(auto)
        operator()(T&& t) const noexcept(noexcept(!faiz::forward<T>(t)))
        {
            return !faiz::forward<T>(t);
        }
    };

    template<typename T = void>
    struct bit_and;

    template<typename T = void>
    struct bit_or;

    template<typename T = void>
    struct bit_xor;

    template<typename T = void>
    struct bit_not;

    template<typename T>
    struct bit_and : public binary_function<T, T, T>
    {
        constexpr T operator()(check<T> x, check<T> y) const
        {
            return x & y;
        }
    };

    template<>
    struct bit_and<void>
    {
        template<typename T, typename U>
        constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(faiz::forward<T>(t) & faiz::forward<U>(u)))
        {
            return faiz::forward<T>(t) & faiz::forward<U>(u);
        }
    };

    template<typename T>
    struct bit_or : public binary_function<T, T, T>
    {
        constexpr T operator()(check<T> x, check<T> y) const
        {
            return x | y;
        }
    };

    template<>
    struct bit_or<void>
    {
        template<typename T, typename U>
        constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(faiz::forward<T>(t) | faiz::forward<U>(u)))
        {
            return faiz::forward<T>(t) | faiz::forward<U>(u);
        }
    };

    template<typename T>
    struct bit_xor : public binary_function<T, T, T>
    {
        constexpr T operator()(check<T> x, check<T> y) const
        {
            return x ^ y;
        }
    };

    template<>
    struct bit_xor<void>
    {
        template<typename T, typename U>
        constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(faiz::forward<T>(t) ^ faiz::forward<U>(u)))
        {
            return faiz::forward<T>(t) ^ faiz::forward<U>(u);
        }
    };

    template<typename T>
    struct bit_not : public unary_function<T, T>
    {
        constexpr T operator()(check<T> x) const
        {
            return ~x;
        }
    };


    template<>
    struct bit_not<void>
    {
        template<typename T>
        constexpr decltype(auto)
        operator()(T&& t) const noexcept(noexcept(~faiz::forward<T>(t)))
        {
            return ~faiz::forward<T>(t);
        }
    };


    // std::bad_function_call is the type of the exception thrown by
    // std::function::operator() if the function wrapper has no
    // target.std::bad_function_call is the type of the exception thrown by
    // std::function::operator() if the function wrapper has no target.
    class bad_function_call : public exception
    {
        ~bad_function_call() noexcept
        {
        }

        const char* what() const noexcept
        {
            return "std::bad_function_call";
        }
    };

} // namespace rider::faiz
#endif
