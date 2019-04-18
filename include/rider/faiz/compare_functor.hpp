#ifndef COMPARE_FUNCTOR
#define COMPARE_FUNCTOR
#include "rider/faiz/faiz_fwd.hpp"
#include "rider/faiz/macros.hpp"
#include "rider/faiz/utility.hpp"
namespace Rider::Faiz
{
	// both call_traits::param_type and this conditional check are ok, but
	// check is more simple.
	tpl<typ T> struct check : Faiz::conditional<is_arithmetic_v<T>, T, check<T>>
	{};

	tpl<typ T>
	using check_t = _t<check<T>>;

	tpl<typ T> struct equal_to
	{
		cexp bool
		operator()(check_t<T> x, check_t<T> y) const
		{
			return x == y;
		}
	};

	tpl<> struct equal_to<void>
	{
		using is_transparent = int;
		tpl<typ A, typ B> cexp decltype(auto)
		operator()(A&& a, B&& b) const
		{
			return Faiz::forward<A>(a) == Faiz::forward<B>(b);
		}
	};


	tpl<typ T> struct not_equal_to
	{
		cfn
		operator()(check_t<T> x, check_t<T> y) const->bool
		{
			return x != y;
		}
	};

	tpl<> struct not_equal_to<void>
	{
		using is_transparent = int;
		tpl<typ T, typ U> cdfn
		operator()(T&& t, U&& u) const
			noexcept(noexcept(Faiz::forward<T>(t) != Faiz::forward<U>(u)))
		{
			return Faiz::forward<T>(t) != Faiz::forward<U>(u);
		}
	};

	tpl<typ T> struct greater
	{
		cfn
		operator()(check_t<T> x, check_t<T> y) const -> bool
		{
			return x > y;
		}
	};

	tpl<> struct greater<void>
	{
		using is_transparent = void;

		tpl<typ A, typ B> cdfn
		operator()(A&& a, B&& b) const
		{
			return Faiz::forward<A>(a) > Faiz::forward<B>(b);
		}
	};

	tpl<typ T = void> struct less
	{
		cfn
		operator()(check_t<T> x, check_t<T> y) const->bool
		{
			return x < y;
		}
	};

	// The member type is_transparent indicates to the caller that this function
	// object is a transparent function object:
	// it accepts arguments of arbitrary types and uses perfect forwarding,
	// which avoids unnecessary copying and conversion when the function object
	// is used in heterogeneous context, or with rvalue arguments. In
	// particular, tpl functions such as `std::set::find` and
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
	//  general tpl for std::less<...> does not define a nested type
	//  is_transparent but the std::less<void> specialization does. That is, no,
	//  you don't get a transparent operator by default.
	//
	// [this answer:](https://stackoverflow.com/a/20383136/6949852)
	// * If you don't use a "transparent functor" (i.e. one that defines a
	// is_transparent type) then the containers behave the same as they've
	// always done, and that's still the default.
	// * Iff you choose to use std::less<> (which is new for C++14) or another
	// "transparent functor" type then you get the new functionality.
	tpl<> struct less<void>
	{
		using is_transparent = void;
		tpl<typ A, typ B> cdfn
		operator()(A&& a, B&& b) const
		{
			return Faiz::forward<A>(a) < Faiz::forward<B>(b);
		}
	};


	tpl<typ T> struct greater_equal
	{
		cfn
		operator()(check_t<T> x, check_t<T> y) const -> bool
		{
			return x >= y;
		}
	};

	tpl<> struct greater_equal<void>
	{
		using is_transparent = void;
		tpl<typ A, typ B> cdfn
		operator()(A&& a, B&& b) const
		{
			return Faiz::forward<A>(a) >= Faiz::forward<B>(b);
		}
	};


	tpl<typ T> struct less_equal
	{
		cfn
		operator()(check_t<T> x, check_t<T> y) const -> bool
		{
			return x <= y;
		}
	};

	tpl<> struct less_equal<void>
	{
		using is_transparent = void;
		tpl<typ A, typ B> cdfn
		operator()(A&& a, B&& b) const
		{
			return Faiz::forward<A>(a) <= Faiz::forward<B>(b);
		}
	};


} // namespace Rider::Faiz


#endif
