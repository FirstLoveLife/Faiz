#ifndef FUNCTIONAL
#define FUNCTIONAL
/*
Don't implement myself: not_fn, invoke
 */
#include "rider/faiz/macros.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"
#include <exception> // for std::except

#include <functional>
namespace Rider::Faiz
{
	// both call_traits::param_type and this conditional check are ok, but
	// check is more simple.
	Tpl<Typ T> struct check
		: Faiz::conditional<Faiz::is_arithmetic<T>::value, T, check<T>>
	{};


	Tpl<Typ T = void> struct plus;

	Tpl<Typ T = void> struct minus;

	Tpl<Typ T = void> struct multiplies;

	Tpl<Typ T = void> struct divides;

	Tpl<Typ T = void> struct modulus;

	Tpl<Typ T = void> struct negate;

	Tpl<Typ T> struct plus
	{
		cexp T
		operator()(check<T> x, check<T> y) const
		{
			return x + y;
		}
	};

	Tpl<> struct plus<void>
	{
		using is_transparent = void;
		Tpl<Typ T, Typ U> cexp decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(Faiz::forward<T>(t) + Faiz::forward<U>(u)))
		{
			return Faiz::forward<T>(t) + Faiz::forward<U>(u);
		}
	};

	Tpl<Typ T> struct minus
	{
		cexp T
		operator()(check<T> x, check<T> y) const
		{
			return x - y;
		}
	};

	Tpl<> struct minus<void>
	{
		using is_transparent = void;
		Tpl<Typ T, Typ U> cexp decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(Faiz::forward<T>(t) - Faiz::forward<U>(u)))
		{
			return Faiz::forward<T>(t) - Faiz::forward<U>(u);
		}
	};

	Tpl<Typ T> struct multiplies
	{
		cexp T
		operator()(check<T> x, check<T> y) const
		{
			return x * y;
		}
	};

	Tpl<> struct multiplies<void>
	{
		using is_transparent = void;
		Tpl<Typ T, Typ U> cexp decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(Faiz::forward<T>(t) * Faiz::forward<U>(u)))
		{
			return Faiz::forward<T>(t) * Faiz::forward<U>(u);
		}
	};


	Tpl<Typ T> struct divides
	{
		cexp T
		operator()(check<T> x, check<T> y) const
		{
			return x / y;
		}
	};

	Tpl<> struct divides<void>
	{
		using is_transparent = void;
		Tpl<Typ T, Typ U> cexp decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(Faiz::forward<T>(t) / Faiz::forward<U>(u)))
		{
			return Faiz::forward<T>(t) / Faiz::forward<U>(u);
		}
	};


	Tpl<Typ T> struct modulus
	{
		cexp T
		operator()(check<T> x, check<T> y) const
		{
			return x % y;
		}
	};

	Tpl<> struct modulus<void>
	{
		using is_transparent = void;
		Tpl<Typ A, Typ B> cexp decltype(auto)
		operator()(A&& a, B&& b) const
		{
			return Faiz::forward<A>(a) % Faiz::forward<B>(b);
		}
	};


	Tpl<Typ T> struct negate
	{
		cexp T
		operator()(check<T> x) const
		{
			return -x;
		}
	};

	Tpl<> struct negate<void>
	{
		using is_transparent = void;
		Tpl<Typ T> cexp decltype(auto)
		operator()(T&& t) const noexcept(noexcept(-Faiz::forward<T>(t)))
		{
			return -Faiz::forward<T>(t);
		}
	};


	Tpl<Typ T> struct equal_to
	{
		cexp bool
		operator()(check<T> x, check<T> y) const
		{
			return x == y;
		}
	};

	Tpl<> struct equal_to<void>
	{
		using is_transparent = int;
		Tpl<Typ A, Typ B> cexp decltype(auto)
		operator()(A&& a, B&& b) const
		{
			return Faiz::forward<A>(a) == Faiz::forward<B>(b);
		}
	};


	Tpl<Typ T> struct not_equal_to
	{
		cexp bool
		operator()(check<T> x, check<T> y) const
		{
			return x != y;
		}
	};

	Tpl<> struct not_equal_to<void>
	{
		using is_transparent = int;
		Tpl<Typ T, Typ U> cexp decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(Faiz::forward<T>(t) != Faiz::forward<U>(u)))
		{
			return Faiz::forward<T>(t) != Faiz::forward<U>(u);
		}
	};

	Tpl<Typ T> struct greater
	{
		cexp bool
		operator()(check<T> x, check<T> y) const
		{
			return x > y;
		}
	};

	Tpl<> struct greater<void>
	{
		using is_transparent = void;

		Tpl<Typ A, Typ B> cexp decltype(auto)
		operator()(A&& a, B&& b) const
		{
			return Faiz::forward<A>(a) > Faiz::forward<B>(b);
		}
	};

	Tpl<Typ T> struct less
	{
		cexp bool
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
	// particular, Tpl functions such as `std::set::find` and
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
	//  general Tpl for std::less<...> does not define a nested type
	//  is_transparent but the std::less<void> specialization does. That is, no,
	//  you don't get a transparent operator by default.
	//
	// [this answer:](https://stackoverflow.com/a/20383136/6949852)
	// * If you don't use a "transparent functor" (i.e. one that defines a
	// is_transparent type) then the containers behave the same as they've
	// always done, and that's still the default.
	// * Iff you choose to use std::less<> (which is new for C++14) or another
	// "transparent functor" type then you get the new functionality.
	Tpl<> struct less<void>
	{
		using is_transparent = void;
		Tpl<Typ A, Typ B> cexp decltype(auto)
		operator()(A&& a, B&& b) const
		{
			return Faiz::forward<A>(a) < Faiz::forward<B>(b);
		}
	};


	Tpl<Typ T> struct greater_equal
	{
		cexp bool
		operator()(check<T> x, check<T> y) const
		{
			return x >= y;
		}
	};

	Tpl<> struct greater_equal<void>
	{
		using is_transparent = void;
		Tpl<Typ A, Typ B> cexp decltype(auto)
		operator()(A&& a, B&& b) const
		{
			return Faiz::forward<A>(a) >= Faiz::forward<B>(b);
		}
	};


	Tpl<Typ T> struct less_equal
	{
		cexp bool
		operator()(check<T> x, check<T> y) const
		{
			return x <= y;
		}
	};

	Tpl<> struct less_equal<void>
	{
		using is_transparent = void;
		Tpl<Typ A, Typ B> cexp decltype(auto)
		operator()(A&& a, B&& b) const
		{
			return Faiz::forward<A>(a) <= Faiz::forward<B>(b);
		}
	};


	Tpl<Typ T = void> struct logical_and;

	Tpl<Typ T = void> struct logical_or;

	Tpl<Typ T = void> struct logical_not;

	Tpl<Typ T> struct logical_and
	{
		cexp bool
		operator()(check<T> x, check<T> y) const
		{
			return x && y;
		}
	};

	Tpl<> struct logical_and<void>
	{
		using is_transparent = void;
		Tpl<Typ T, Typ U> cexp decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(Faiz::forward<T>(t) && Faiz::forward<U>(u)))
		{
			return Faiz::forward<T>(t) && Faiz::forward<U>(u);
		}
	};

	Tpl<Typ T> struct logical_or
	{
		cexp bool
		operator()(check<T> x, check<T> y) const
		{
			return x || y;
		}
	};

	Tpl<> struct logical_or<void>
	{
		using is_transparent = void;
		Tpl<Typ T, Typ U> cexp decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(Faiz::forward<T>(t) || Faiz::forward<U>(u)))
		{
			return Faiz::forward<T>(t) || Faiz::forward<U>(u);
		}
	};

	Tpl<Typ T> struct logical_not
	{
		cexp bool
		operator()(check<T> x) const
		{
			return !x;
		}
	};


	Tpl<> struct logical_not<void>
	{
		using is_transparent = void;
		Tpl<Typ T> cexp decltype(auto)
		operator()(T&& t) const noexcept(noexcept(!Faiz::forward<T>(t)))
		{
			return !Faiz::forward<T>(t);
		}
	};

	Tpl<Typ T = void> struct bit_and;

	Tpl<Typ T = void> struct bit_or;

	Tpl<Typ T = void> struct bit_xor;

	Tpl<Typ T = void> struct bit_not;

	Tpl<Typ T> struct bit_and
	{
		cexp T
		operator()(check<T> x, check<T> y) const
		{
			return x & y;
		}
	};

	Tpl<> struct bit_and<void>
	{
		using is_transparent = void;
		Tpl<Typ T, Typ U> cexp decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(Faiz::forward<T>(t) & Faiz::forward<U>(u)))
		{
			return Faiz::forward<T>(t) & Faiz::forward<U>(u);
		}
	};

	Tpl<Typ T> struct bit_or
	{
		cexp T
		operator()(check<T> x, check<T> y) const
		{
			return x | y;
		}
	};

	Tpl<> struct bit_or<void>
	{
		using is_transparent = void;
		Tpl<Typ T, Typ U> cexp decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(Faiz::forward<T>(t) | Faiz::forward<U>(u)))
		{
			return Faiz::forward<T>(t) | Faiz::forward<U>(u);
		}
	};

	Tpl<Typ T> struct bit_xor
	{
		cexp T
		operator()(check<T> x, check<T> y) const
		{
			return x ^ y;
		}
	};

	Tpl<> struct bit_xor<void>
	{
		using is_transparent = void;
		Tpl<Typ T, Typ U> cexp decltype(auto)
		operator()(T&& t, U&& u) const
			noexcept(noexcept(Faiz::forward<T>(t) ^ Faiz::forward<U>(u)))
		{
			return Faiz::forward<T>(t) ^ Faiz::forward<U>(u);
		}
	};

	Tpl<Typ T> struct bit_not
	{
		cexp T
		operator()(check<T> x) const
		{
			return ~x;
		}
	};


	Tpl<> struct bit_not<void>
	{
		using is_transparent = void;
		Tpl<Typ T> cexp decltype(auto)
		operator()(T&& t) const noexcept(noexcept(~Faiz::forward<T>(t)))
		{
			return ~Faiz::forward<T>(t);
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

} // namespace Rider::Faiz


#endif
