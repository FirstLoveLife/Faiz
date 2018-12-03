#ifndef MEMORY
#define MEMORY
#include "rider/faiz/type_traits.hpp"
/*
https://softwareengineering.stackexchange.com/questions/274801/raw-weak-ptr-unique-ptr-shared-ptr-etc-how-to-choose-them-wisely

TODO: allocator_traits: https://www.youtube.com/watch?v=ybaE9qlhHvw begin from
58
 */
namespace rider::faiz
{

	template<class T>
	T*
	addressof(T& arg)
	{
		return reinterpret_cast<T*>(
			&const_cast<char&>(reinterpret_cast<const volatile char&>(arg)));
	}

	template<class Targ, class = void>
	struct has_element_type : faiz::false_type
	{};

	template<class Targ>
	struct has_element_type<Targ, faiz::void_t<typename Targ::element_type>>
		: faiz::true_type
	{};

	template<class Tptr, bool = has_element_type<Tptr>::value>
	struct pointer_traits_element_type;

	template<class Tptr>
	struct pointer_traits_element_type<Tptr, true>
		: type_identity<typename Tptr::element_type>
	{};


	template<template<class, class...> class Tmeta, class Targ, class... Targs>
	struct pointer_traits_element_type<Tmeta<Targ, Targs...>, true>
		: type_identity<typename Tmeta<Targ, Targs...>::element_type>
	{};

	template<template<class, class...> class Tmeta, class Targ, class... Targs>
	struct pointer_traits_element_type<Tmeta<Targ, Targs...>, false>
		: type_identity<Targ>
	{};

	template<class Targ, class = void>
	struct has_difference_type : false_type
	{};

	template<class Targ>
	struct has_difference_type<Targ, void_t<typename Targ::difference_type>>
		: true_type
	{};

	template<class Tptr, bool = has_difference_type<Tptr>::value>
	struct pointer_traits_difference_type : type_identity<ptrdiff_t>
	{};

	template<class Tptr>
	struct pointer_traits_difference_type<Tptr, true>
		: type_identity<typename Tptr::difference_type>
	{};

	template<class Targ, class Tup>
	struct has_rebind
	{
	private:
		struct two
		{
			char lx;
			char lxx;
		};
		template<class _Xp>
		static two
		test(...);
		template<class _Xp>
		static char
		test(typename _Xp::template rebind<Tup>* = 0);

	public:
		static constexpr bool value = sizeof(test<Targ>(0)) == 1;
	};

	template<class Targ, class Tup, bool = has_rebind<Targ, Tup>::value>
	struct pointer_traits_rebind
		: type_identity<typename Targ::template rebind<Tup>>
	{};

	template<template<class, class...> class Tmeta,
		class Targ,
		class... Targs,
		class Tup>
	struct pointer_traits_rebind<Tmeta<Targ, Targs...>, Tup, true>
		: type_identity<typename Tmeta<Targ, Targs...>::template rebind<Tup>>
	{};

	template<template<class, class...> class Tmeta,
		class Targ,
		class... Targs,
		class Tup>
	struct pointer_traits_rebind<Tmeta<Targ, Targs...>, Tup, false>
		: type_identity<Tmeta<Tup, Targs...>>
	{};


	template<class Tptr>
	struct pointer_traits
	{
		using pointer = Tptr;
		// `Ptr::element_type` if present. Otherwise T if Ptr is a template
		// instantiation `Template<T, Args...>`
		using element_type = _t<pointer_traits_element_type<pointer>>;
		// `Ptr::difference_type` if present, otherwise std::ptrdiff_t
		using difference_type = _t<pointer_traits_difference_type<pointer>>;

		// `Ptr::rebind<U>` if exists, otherwise `Template<U, Args...>` if Ptr
		// is a template instantiation `Template<T, Args...>`
		template<class Tup>
		using rebind = _t<pointer_traits_rebind<pointer, Tup>>;

	private:
		struct nat
		{};

	public:
		static auto
		pointer_to(conditional_t<is_void_v<element_type>, nat, element_type>& r)
		{
			return pointer::pointer_to(r);
		}
	};

	template<class Targ>
	struct pointer_traits<Targ*>
	{
		using pointer = Targ*;
		using element_type = Targ;
		using difference_type = ptrdiff_t;

		template<class Tup>
		using rebind = Tup*;

	private:
		struct nat
		{};

	public:
		static pointer
		pointer_to(conditional_t<is_void_v<element_type>, nat, element_type>&
				r) noexcept
		{
			return faiz::addressof(r);
		}
	};

} // namespace rider::faiz
#endif
