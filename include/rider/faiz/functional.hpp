#ifndef FUNCTIONAL
#define FUNCTIONAL
/*
Don't implement myself: not_fn, invoke
 */
#include <exception> // for std::except
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"

#include <functional>
namespace rider::faiz
{
	// both call_traits::param_type and this conditional check are ok, but
	// check is more simple.
	template<typename T>
	struct check : faiz::conditional<faiz::is_arithmetic<T>::value, T, check<T>>
	{};


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
	struct plus
	{
		constexpr T
		operator()(check<T> x, check<T> y) const
		{
			return x + y;
		}
	};

	template<>
	struct plus<void>
	{
		using is_transparent = void;
		template<typename T, typename U>
		constexpr decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(faiz::forward<T>(t) + faiz::forward<U>(u)))
		{
			return faiz::forward<T>(t) + faiz::forward<U>(u);
		}
	};

	template<typename T>
	struct minus
	{
		constexpr T
		operator()(check<T> x, check<T> y) const
		{
			return x - y;
		}
	};

	template<>
	struct minus<void>
	{
		using is_transparent = void;
		template<typename T, typename U>
		constexpr decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(faiz::forward<T>(t) - faiz::forward<U>(u)))
		{
			return faiz::forward<T>(t) - faiz::forward<U>(u);
		}
	};

	template<typename T>
	struct multiplies
	{
		constexpr T
		operator()(check<T> x, check<T> y) const
		{
			return x * y;
		}
	};

	template<>
	struct multiplies<void>
	{
		using is_transparent = void;
		template<typename T, typename U>
		constexpr decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(faiz::forward<T>(t) * faiz::forward<U>(u)))
		{
			return faiz::forward<T>(t) * faiz::forward<U>(u);
		}
	};


	template<typename T>
	struct divides
	{
		constexpr T
		operator()(check<T> x, check<T> y) const
		{
			return x / y;
		}
	};

	template<>
	struct divides<void>
	{
		using is_transparent = void;
		template<typename T, typename U>
		constexpr decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(faiz::forward<T>(t) / faiz::forward<U>(u)))
		{
			return faiz::forward<T>(t) / faiz::forward<U>(u);
		}
	};


	template<typename T>
	struct modulus
	{
		constexpr T
		operator()(check<T> x, check<T> y) const
		{
			return x % y;
		}
	};

	template<>
	struct modulus<void>
	{
		using is_transparent = void;
		template<typename A, typename B>
		constexpr decltype(auto)
		operator()(A&& a, B&& b) const
		{
			return faiz::forward<A>(a) % faiz::forward<B>(b);
		}
	};


	template<typename T>
	struct negate
	{
		constexpr T
		operator()(check<T> x) const
		{
			return -x;
		}
	};

	template<>
	struct negate<void>
	{
		using is_transparent = void;
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
	struct equal_to
	{
		constexpr bool
		operator()(check<T> x, check<T> y) const
		{
			return x == y;
		}
	};

	template<>
	struct equal_to<void>
	{
		using is_transparent = int;
		template<typename A, typename B>
		constexpr decltype(auto)
		operator()(A&& a, B&& b) const
		{
			return faiz::forward<A>(a) == faiz::forward<B>(b);
		}
	};


	template<typename T>
	struct not_equal_to
	{
		constexpr bool
		operator()(check<T> x, check<T> y) const
		{
			return x != y;
		}
	};

	template<>
	struct not_equal_to<void>
	{
		using is_transparent = int;
		template<typename T, typename U>
		constexpr decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(faiz::forward<T>(t) != faiz::forward<U>(u)))
		{
			return faiz::forward<T>(t) != faiz::forward<U>(u);
		}
	};

	template<typename T>
	struct greater
	{
		constexpr bool
		operator()(check<T> x, check<T> y) const
		{
			return x > y;
		}
	};

	template<>
	struct greater<void>
	{
		using is_transparent = void;

		template<typename A, typename B>
		constexpr decltype(auto)
		operator()(A&& a, B&& b) const
		{
			return faiz::forward<A>(a) > faiz::forward<B>(b);
		}
	};

	template<typename T>
	struct less
	{
		constexpr bool
		operator()(check<T> x, check<T> y) const
		{
			return x < y;
		}
	};

	// The member type is_transparent indicates to the caller that this function
	// object is a transparent function object:
	// it accepts arguments of arbitrary types and uses perfect forwarding,
	// which avoids unnecessary copying and conversion when the function object
	// is used in heterogeneous context, or with rvalue arguments. In
	// particular, template functions such as `std::set::find` and
	// `std::set::lower_bound` make use of this member type on their Compare
	// types.
	//
	//  https://stackoverflow.com/a/18940595/6949852
	//
	//    [n3421](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3421.htm)
	//    [n3465](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3465.pdf)
	//
	// [this
	// comment:](https://stackoverflow.com/questions/20317413/what-are-transparent-comparators#comment30322069_20318064)
	//
	//  the transparent behavior is enable when is_transparent is defined in the
	//  comparison function object according to 23.2.4 [associative.reqmts]
	//  paragraph 13. The default comparison function objects is std::less<Key>
	//  according to 23.4.2 [associative.map.syn] and 23.4.3
	//  [associative.set.syn]. According to 20.10.5 [comparison] paragraph 4 the
	//  general template for std::less<...> does not define a nested type
	//  is_transparent but the std::less<void> specialization does. That is, no,
	//  you don't get a transparent operator by default.
	//
	// [this answer:](https://stackoverflow.com/a/20383136/6949852)
	// * If you don't use a "transparent functor" (i.e. one that defines a
	// is_transparent type) then the containers behave the same as they've
	// always done, and that's still the default.
	// * Iff you choose to use std::less<> (which is new for C++14) or another
	// "transparent functor" type then you get the new functionality.
	template<>
	struct less<void>
	{
		using is_transparent = void;
		template<typename A, typename B>
		constexpr decltype(auto)
		operator()(A&& a, B&& b) const
		{
			return faiz::forward<A>(a) < faiz::forward<B>(b);
		}
	};


	template<typename T>
	struct greater_equal
	{
		constexpr bool
		operator()(check<T> x, check<T> y) const
		{
			return x >= y;
		}
	};

	template<>
	struct greater_equal<void>
	{
		using is_transparent = void;
		template<typename A, typename B>
		constexpr decltype(auto)
		operator()(A&& a, B&& b) const
		{
			return faiz::forward<A>(a) >= faiz::forward<B>(b);
		}
	};


	template<typename T>
	struct less_equal
	{
		constexpr bool
		operator()(check<T> x, check<T> y) const
		{
			return x <= y;
		}
	};

	template<>
	struct less_equal<void>
	{
		using is_transparent = void;
		template<typename A, typename B>
		constexpr decltype(auto)
		operator()(A&& a, B&& b) const
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
	struct logical_and
	{
		constexpr bool
		operator()(check<T> x, check<T> y) const
		{
			return x && y;
		}
	};

	template<>
	struct logical_and<void>
	{
		using is_transparent = void;
		template<typename T, typename U>
		constexpr decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(faiz::forward<T>(t) && faiz::forward<U>(u)))
		{
			return faiz::forward<T>(t) && faiz::forward<U>(u);
		}
	};

	template<typename T>
	struct logical_or
	{
		constexpr bool
		operator()(check<T> x, check<T> y) const
		{
			return x || y;
		}
	};

	template<>
	struct logical_or<void>
	{
		using is_transparent = void;
		template<typename T, typename U>
		constexpr decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(faiz::forward<T>(t) || faiz::forward<U>(u)))
		{
			return faiz::forward<T>(t) || faiz::forward<U>(u);
		}
	};

	template<typename T>
	struct logical_not
	{
		constexpr bool
		operator()(check<T> x) const
		{
			return !x;
		}
	};


	template<>
	struct logical_not<void>
	{
		using is_transparent = void;
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
	struct bit_and
	{
		constexpr T
		operator()(check<T> x, check<T> y) const
		{
			return x & y;
		}
	};

	template<>
	struct bit_and<void>
	{
		using is_transparent = void;
		template<typename T, typename U>
		constexpr decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(faiz::forward<T>(t) & faiz::forward<U>(u)))
		{
			return faiz::forward<T>(t) & faiz::forward<U>(u);
		}
	};

	template<typename T>
	struct bit_or
	{
		constexpr T
		operator()(check<T> x, check<T> y) const
		{
			return x | y;
		}
	};

	template<>
	struct bit_or<void>
	{
		using is_transparent = void;
		template<typename T, typename U>
		constexpr decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(faiz::forward<T>(t) | faiz::forward<U>(u)))
		{
			return faiz::forward<T>(t) | faiz::forward<U>(u);
		}
	};

	template<typename T>
	struct bit_xor
	{
		constexpr T
		operator()(check<T> x, check<T> y) const
		{
			return x ^ y;
		}
	};

	template<>
	struct bit_xor<void>
	{
		using is_transparent = void;
		template<typename T, typename U>
		constexpr decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(faiz::forward<T>(t) ^ faiz::forward<U>(u)))
		{
			return faiz::forward<T>(t) ^ faiz::forward<U>(u);
		}
	};

	template<typename T>
	struct bit_not
	{
		constexpr T
		operator()(check<T> x) const
		{
			return ~x;
		}
	};


	template<>
	struct bit_not<void>
	{
		using is_transparent = void;
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
	class bad_function_call : public std::exception
	{
		~bad_function_call() noexcept
		{}

		const char*
		what() const noexcept
		{
			return "std::bad_function_call";
		}
	};

} // namespace rider::faiz
#endif
