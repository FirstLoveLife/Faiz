#ifndef ZSTRING
#define ZSTRING
#include "rider/faiz/string_view.hpp"
namespace rider::faiz
{
	template<typename charT, typename traits = std::char_traits<charT>>
	class basic_zstring_view : private basic_string_view<charT, traits>
	{
	public:
		using base_view_type = basic_string_view<charT, traits>;

		using base_view_type::traits_type;
		using base_view_type::value_type;
		using base_view_type::pointer;
		using base_view_type::const_pointer;
		using base_view_type::reference;
		using base_view_type::const_reference;

		using base_view_type::const_iterator;
		using base_view_type::iterator;
		using base_view_type::const_reverse_iterator;
		using base_view_type::reverse_iterator;

		using typename base_view_type::size_type;
		using base_view_type::difference_type;

		using base_view_type::npos;

		basic_zstring_view(const charT* str) : base_view_type(str)
		{}
		constexpr explicit basic_zstring_view(const charT* str, size_type len)
			: base_view_type(str, len)
		{}
		constexpr explicit basic_zstring_view(const base_view_type& view)
			: base_view_type(view)
		{}

		constexpr basic_zstring_view(const basic_zstring_view&) noexcept
			= default;
		basic_zstring_view&
		operator=(const basic_zstring_view&) noexcept
			= default;

		using base_view_type::begin;
		using base_view_type::end;
		using base_view_type::cbegin;
		using base_view_type::cend;
		using base_view_type::rbegin;
		using base_view_type::rend;
		using base_view_type::crbegin;
		using base_view_type::crend;

		using base_view_type::size;
		using base_view_type::length;
		using base_view_type::max_size;
		using base_view_type::empty;

		using base_view_type::operator[];
		using base_view_type::at;
		using base_view_type::front;
		using base_view_type::back;
		using base_view_type::data;

		using base_view_type::remove_prefix;

		//`using base_view_type::remove_suffix`; Intentionally not provided.

		/// Creates a `basic_string_view` that lacks the last few characters.
		constexpr basic_string_view<charT, traits>
		view_suffix(size_type n) const
		{
			return basic_string_view<charT, traits>(data(), size() - n);
		}

		using base_view_type::swap;

		template<class Allocator = std::allocator<charT>>
		std::basic_string<charT, traits, Allocator>
		to_string(const Allocator& a = Allocator()) const
		{
			return std::basic_string<charT, traits, Allocator>(
				begin(), end(), a);
		}

		constexpr operator base_view_type() const
		{
			return base_view_type(data(), size());
		}

		using base_view_type::to_string;

		using base_view_type::copy;

		using base_view_type::substr;

		using base_view_type::operator==;
		using base_view_type::operator!=;
		using base_view_type::compare;
	};
} // namespace rider::faiz
#endif
