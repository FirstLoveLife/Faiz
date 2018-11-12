#ifndef ALGORITHM
#define ALGORITHM
#include <iostream>
namespace rider::faiz
{
	// Copies exactly count values from the range beginning at first to the
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
} // namespace rider::faiz
#endif
