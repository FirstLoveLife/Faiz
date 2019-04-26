#ifndef ALGORITHM
#define ALGORITHM
#include "rider/faiz/macros.hpp"
namespace Rider::Faiz
{
	Tpl<Typ T> cexp const T&
	min(const T& a, const T& b)
	{
		return (b < a) ? b : a;
	}
	Tpl<Typ T> cexp const T&
	max(const T& a, const T& b)
	{
		return (b < a) ? a : b;
	}

	Tpl<Typ T, Typ Cmp> cexp const T&
	min(const T& a, const T& b, Cmp less)
	{
		return less(b, a) ? b : a;
	}
	Tpl<Typ T, Typ Cmp> cexp const T&
	max(const T& a, const T& b, Cmp less)
	{
		return less(b, a) ? a : b;
	}

	//  Copies exactly count values from the range beginning at first to the
	// range beginning at result. Formally, for each non-negative integer i < n,
	// performs *(result + i) = *(first + i). Overlap of ranges is not
	// permitted.
	//
	// ```cpp
	// std::string in = "1234567890";
	// std::string out;
	// std::copy_n(in.begin(), 4, std::back_inserter(out));
	// std::cout << out << '\n';
	// ```
	// Output:
	// 1234
	Tpl<Typ tInputIt, Typ Size, Typ OutputIt> cfn
	copy_n(tInputIt first, Size count, OutputIt result)->OutputIt
	{
		if(count > 0)
		{
			*result++ = *first;
			for(Size i = 1; i < count; ++i)
			{
				*result++ = *++first;
			}
		}
		return result;
	}

	Tpl<unsigned N, Typ T> cfn
	power(T x)->T
	{
		if(N == 0u)
			return T(1);
		else if(N % 2u == 0u)
			return power<N / 2>(x * x);
		else
			return x * power<N - 1>(x);
	}

	Tpl<Typ tInputIt1, Typ tInputIt2> cfn
	lexicographical_compare(
		tInputIt1 first1, tInputIt1 last1, tInputIt2 first2, tInputIt2 last2)
		->bool
	{
		for(; first1 != last1 or first2 != last2; first1++, first2++)
		{
			if(*first1 < *first2)
			{
				return true;
			}
			if(*first1 > *first2)
			{
				return false;
			}
		}
		return first1 == last1 && first2 != last2;
	}
	Tpl<Typ tInputIt1, Typ tInputIt2, Typ tCompare> cfn
	lexicographical_compare(tInputIt1 first1,
		tInputIt1 last1,
		tInputIt2 first2,
		tInputIt2 last2,
		tCompare comp)
		->bool
	{
		for(; first1 != last1 or first2 != last2; first1++, first2++)
		{
			if(cmp(*first1, *first2))
			{
				return true;
			}
			if(cmp(*first1, *first2))
			{
				return false;
			}
		}
		return first1 == last1 && first2 != last2;
	}

	Tpl<Typ tInputIt1, Typ tInputIt2> cfn
	equal(tInputIt1 first1, tInputIt1 last1, tInputIt2 first2)->bool
	{
		for(; first1 != last1; first1++, first2++)
		{
			if(*first1 != *first2)
			{
				return false;
			}
		}
		return true;
	}

	Tpl<Typ tInputIt1, Typ tInputIt2, Typ tBinaryPredicate> cfn
	equal(tInputIt1 first1,
		tInputIt1 last1,
		tInputIt2 first2,
		tBinaryPredicate cmp)
		->bool
	{
		for(; first1 != last1; first1++, first2++)
		{
			if(not cmp(*first1, *first2))
			{
				return false;
			}
		}
		return true;
	}

	Tpl<Typ tInputIt1, Typ tInputIt2> cfn
	equal(tInputIt1 first1, tInputIt1 last1, tInputIt2 first2, tInputIt2 last2)
		->bool
	{
		for(; first1 != last1 and first2 != last2; first1++, first2++)
		{
			if(*first1 != *first2)
			{
				return false;
			}
		}
		return first1 == last1 and first2 == last2;
	}


	Tpl<Typ tInputIt1, Typ tInputIt2, Typ tBinaryPredicate> cfn
	equal(tInputIt1 first1,
		tInputIt1 last1,
		tInputIt2 first2,
		tInputIt2 last2,
		tBinaryPredicate cmp)
		->bool
	{
		for(; first1 != last1 && first2 != last2; first1++, first2++)
		{
			if(not cmp(*first1, *first2))
			{
				return false;
			}
		}
		return first1 == last1 and first2 == last2;
	}

	Tpl<Typ tOutputIt, Typ tSize, Typ T> tOutputIt
	fill_n(tOutputIt first, tSize count, const T& value)
	{
		for(tSize i = 0; i < count; i++)
		{
			*first++ = value;
		}
		return first;
	}
} // namespace Rider::Faiz
#endif
