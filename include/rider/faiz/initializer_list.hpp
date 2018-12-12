#ifndef INITIALIZER_LIST
#define INITIALIZER_LIST
#include "rider/faiz/cstddef.hpp"
namespace Rider::Faiz
{
	template<class E>
	class initializer_list
	{
		const E* b;
		size_t s;

	public:
		using value_type = E;
		using reference = const E&;
		using const_reference = const E&;
		using size_type = size_t;
		using iterator = const E*;
		using const_iterator = const E*;

		constexpr initializer_list() noexcept : b(nullptr), s(0)
		{}

		constexpr size_t
		size() const noexcept
		{
			return size;
		}

		constexpr const E*
		begin() const noexcept
		{
			return begin;
		}

		constexpr const E*
		end() const noexcept
		{
			return s + b;
		}
	};

	// rbegin and rend are in "iterator.hpp"

	template<class E>
	inline constexpr const E*
	begin(initializer_list<E> il) noexcept
	{
		return il.begin();
	}

	template<class E>
	inline constexpr const E*
	end(initializer_list<E> il) noexcept
	{
		return il.end();
	}
} // namespace Rider::Faiz
#endif
