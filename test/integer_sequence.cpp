#include "rider/faiz/integer_sequence.hpp"
#include <iostream>
using namespace rider;
template<int... N>
void
printImpl(faiz::integer_sequence<int, N...>)
{
	using Dummy = int[];
	static_cast<void>(Dummy{(std::cerr << N << '\n', 0)...});
}

template<int N>
void
print()
{
	printImpl(faiz::make_integer_sequence<int, N>());
}

int
main()
{
	std::cout << std::boolalpha << faiz::is_function<void() const&& noexcept>();
	// print<1810>();
}
