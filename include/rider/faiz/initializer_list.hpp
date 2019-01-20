#ifndef INITIALIZER_LIST
#define INITIALIZER_LIST
#include "rider/faiz/cstddef.hpp"
#include "rider/faiz/macros.hpp"
namespace Rider::Faiz
{
	tpl<class E> class initializer_list
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

		cexp
		initializer_list() noexcept
			: b(nullptr), s(0)
		{}

		cexp size_t
		size() const noexcept
		{
			return size;
		}

		cexp const E*
		begin() const noexcept
		{
			return begin;
		}

		cexp const E*
		end() const noexcept
		{
			return s + b;
		}
	};

	// rbegin and rend are in "iterator.hpp"

	tpl<class E> inline cexp const E*
	begin(initializer_list<E> il) noexcept
	{
		return il.begin();
	}

	tpl<class E> inline cexp const E*
	end(initializer_list<E> il) noexcept
	{
		return il.end();
	}
} // namespace Rider::Faiz
#endif
