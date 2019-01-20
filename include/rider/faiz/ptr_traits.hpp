#ifndef PTR_TRAITS
#define PTR_TRAITS
#include "rider/faiz/cstddef.hpp"
#include "rider/faiz/macros.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"
namespace Rider::Faiz
{
	class undefined;

	tpl<typ T> struct get_first_arg
	{
		using type = undefined;
	};
	tpl<tpl<typ, typ...> class Tpl, typ T, typ... Types> struct get_first_arg<
		Tpl<T, Types...>>
	{
		using type = T;
	};

	tpl<typ T> using get_first_arg_t = typ get_first_arg<T>::type;

	// Given Tpl<T, ...> and U return Tpl<U, ...>, otherwise invalid.
	tpl<typ T, typ U> struct replace_first_arg
	{};

	tpl<tpl<typ, typ...> class Tpl,
		typ U,
		typ T,
		typ... Types> struct replace_first_arg<Tpl<T, Types...>, U>
	{
		using type = Tpl<U, Types...>;
	};

	tpl<typ T, typ U> using replace_first_arg_t
		= typ replace_first_arg<T, U>::type;

	tpl<typ T> using make_not_void
		= typ Faiz::conditional<is_void_v<T>, undefined, T>::type;

	/**
	 * @brief  Uniform interface to all pointer-like types
	 * @ingroup pointer_abstractions
	 */
	tpl<typ _Ptr> struct pointer_traits
	{
	private:
		tpl<typ T> using element_type_aux = typ T::element_type;

		tpl<typ T> using differencetype_aux = typ T::differencetype;

		tpl<typ T, typ U, typ = void> struct rebind_aux
			: replace_first_arg<T, U>
		{};

		tpl<typ T, typ U> struct rebind_aux<T, U, void_t<typ T::tpl rebind<U>>>
		{
			using type = typ T::tpl rebind<U>;
		};

	public:
		/// The pointer type.
		using pointer = _Ptr;

		/// The type pointed to.
		using element_type
			= Faiz::detected_or_t<get_first_arg_t<_Ptr>, element_type_aux, _Ptr>;

		/// The type used to represent the difference between two pointers.
		using differencetype
			= Faiz::detected_or_t<ptrdiff_t, differencetype_aux, _Ptr>;

		/// A pointer to a different type.
		tpl<typ U> using rebind = typ rebind_aux<_Ptr, U>::type;

		static _Ptr
		pointer_to(make_not_void<element_type>& __e)
		{
			return _Ptr::pointer_to(__e);
		}

		static_assert(not_same_v<element_type, undefined>,
			"pointer type defines element_type or is like SomePointer<T, "
			"Args>");
	};

	/**
	 * @brief  Partial specialization for built-in pointers.
	 * @ingroup pointer_abstractions
	 */
	tpl<typ T> struct pointer_traits<T*>
	{
		/// The pointer type
		using pointer = T*;
		/// The type pointed to
		using element_type = T;
		/// Type used to represent the difference between two pointers
		using differencetype = ptrdiff_t;

		tpl<typ U> using rebind = U*;

		/**
		 *  @brief  Obtain a pointer to an object
		 *  @param  r  A reference to an object of type @c element_type
		 *  @return @c addressof(r)
		 */
		static pointer
		pointer_to(make_not_void<element_type>& r) noexcept
		{
			return Faiz::addressof(r);
		}
	};


	tpl<typ Ptr> cexp decltype(auto)
	to_address_aux(const Ptr& ptr) noexcept
	{
		return Faiz::pointer_traits<Ptr>::to_address(ptr);
	}

	tpl<typ Ptr, typ... None> cexp auto
	to_address_aux(const Ptr& ptr, None...) noexcept
	{
		return Faiz::to_address_aux(ptr.operator->());
	}

	tpl<typ Tp> cexp Tp*
	to_address(Tp* ptr) noexcept
	{
		return Faiz::to_address_aux(ptr);
	}

	tpl<typ Ptr> cexp auto
	to_address(const Ptr& ptr) noexcept
	{
		return Faiz::to_address_aux(ptr);
	}

} // namespace Rider::Faiz
#endif
