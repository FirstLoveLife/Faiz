#ifndef MEMORY
#define MEMORY
#include "rider/faiz/macros.hpp"
#include "rider/faiz/type_traits.hpp"
/*
https://softwareengineering.stackexchange.com/questions/274801/raw-weak-ptr-unique-ptr-shared-ptr-etc-how-to-choose-them-wisely

TODO: allocator_traits: https://www.youtube.com/watch?v=ybaE9qlhHvw begin from
58
 */
namespace Rider::Faiz
{

	tpl<class T> T*
	addressof(T& arg)
	{
		return reinterpret_cast<T*>(
			&const_cast<char&>(reinterpret_cast<const volatile char&>(arg)));
	}

	tpl<class Targ, class = void> struct has_element_type : Faiz::false_type
	{};

	tpl<class Targ> struct has_element_type<Targ,
		Faiz::void_t<typ Targ::element_type>> : Faiz::true_type
	{};

	tpl<class Tptr,
		bool
		= has_element_type<Tptr>::value> struct pointer_traits_element_type;

	tpl<class Tptr> struct pointer_traits_element_type<Tptr, true>
		: type_identity<typ Tptr::element_type>
	{};


	tpl<tpl<class, class...> class Tmeta,
		class Targ,
		class... Targs> struct pointer_traits_element_type<Tmeta<Targ, Targs...>,
		true> : type_identity<typ Tmeta<Targ, Targs...>::element_type>
	{};

	tpl<tpl<class, class...> class Tmeta,
		class Targ,
		class... Targs> struct pointer_traits_element_type<Tmeta<Targ, Targs...>,
		false> : type_identity<Targ>
	{};

	tpl<class Targ, class = void> struct has_difference_type : false_type
	{};

	tpl<class Targ> struct has_difference_type<Targ,
		void_t<typ Targ::difference_type>> : true_type
	{};

	tpl<class Tptr,
		bool
		= has_difference_type<Tptr>::value> struct pointer_traits_difference_type
		: type_identity<ptrdiff_t>
	{};

	tpl<class Tptr> struct pointer_traits_difference_type<Tptr, true>
		: type_identity<typ Tptr::difference_type>
	{};

	tpl<class Targ, class Tup> struct has_rebind
	{
	private:
		struct two
		{
			char lx;
			char lxx;
		};
		tpl<class _Xp> static two
		test(...);
		tpl<class _Xp> static char
		test(typ _Xp::tpl rebind<Tup>* = 0);

	public:
		static cexp bool value = sizeof(test<Targ>(0)) == 1;
	};

	tpl<class Targ,
		class Tup,
		bool = has_rebind<Targ, Tup>::value> struct pointer_traits_rebind
		: type_identity<typ Targ::tpl rebind<Tup>>
	{};

	tpl<tpl<class, class...> class Tmeta,
		class Targ,
		class... Targs,
		class Tup> struct pointer_traits_rebind<Tmeta<Targ, Targs...>, Tup, true>
		: type_identity<typ Tmeta<Targ, Targs...>::tpl rebind<Tup>>
	{};

	tpl<tpl<class, class...> class Tmeta,
		class Targ,
		class... Targs,
		class Tup> struct pointer_traits_rebind<Tmeta<Targ, Targs...>,
		Tup,
		false> : type_identity<Tmeta<Tup, Targs...>>
	{};


	tpl<class Tptr> struct pointer_traits
	{
		using pointer = Tptr;
		// `Ptr::element_type` if present. Otherwise T if Ptr is a tpl
		// instantiation `Tpl<T, Args...>`
		using element_type = _t<pointer_traits_element_type<pointer>>;
		// `Ptr::difference_type` if present, otherwise std::ptrdiff_t
		using difference_type = _t<pointer_traits_difference_type<pointer>>;

		// `Ptr::rebind<U>` if exists, otherwise `Tpl<U, Args...>` if Ptr
		// is a tpl instantiation `Tpl<T, Args...>`
		tpl<class Tup> using rebind = _t<pointer_traits_rebind<pointer, Tup>>;

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

	tpl<class Targ> struct pointer_traits<Targ*>
	{
		using pointer = Targ*;
		using element_type = Targ;
		using difference_type = ptrdiff_t;

		tpl<class Tup> using rebind = Tup*;

	private:
		struct nat
		{};

	public:
		static pointer
		pointer_to(conditional_t<is_void_v<element_type>, nat, element_type>&
				r) noexcept
		{
			return Faiz::addressof(r);
		}
	};

} // namespace Rider::Faiz
#endif
