#ifndef ALGORITHM
#define ALGORITHM
#include <iostream>
namespace Rider::Faiz
{
	template<class T>
	constexpr const T&
	min(const T& a, const T& b)
	{
		return (b < a) ? b : a;
	}
	template<class T>
	constexpr const T&
	max(const T& a, const T& b)
	{
		return (b < a) ? a : b;
	}

	template<class T, class Cmp>
	constexpr const T&
	min(const T& a, const T& b, Cmp less)
	{
		return less(b, a) ? b : a;
	}
	template<class T, class Cmp>
	constexpr const T&
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
	template<class InputIt, class Size, class OutputIt>
	constexpr OutputIt
	copy_n(InputIt first, Size count, OutputIt result)
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

	template<unsigned N, class T>
	T
	power(T x)
	{
		if constexpr(N == 0u)
			return T(1);
		else if constexpr(N % 2u == 0u)
			return power<N / 2>(x * x);
		else
			return x * power<N - 1>(x);
	}
} // namespace Rider::Faiz
#endif
