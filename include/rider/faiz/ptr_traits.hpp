#ifndef PTR_TRAITS
#define PTR_TRAITS
#include "rider/faiz/cstddef.hpp"
#include "rider/faiz/macros.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"
namespace Rider::Faiz
{
	class undefined;

	Tpl<Typ T> struct get_first_arg
	{
		using type = undefined;
	};
	Tpl<Tpl<Typ, Typ...> class TT, Typ T, Typ... types> struct get_first_arg<
		TT<T, types...>>
	{
		using type = T;
	};

	Tpl<Typ T> using get_first_arg_t = Typ get_first_arg<T>::type;

	// Given TT<T, ...> and U return TT<U, ...>, otherwise invalid.
	Tpl<Typ T, Typ U> struct replace_first_arg
	{};

	Tpl<Tpl<Typ, Typ...> class TT,
		Typ U,
		Typ T,
		Typ... types> struct replace_first_arg<TT<T, types...>, U>
	{
		using type = TT<U, types...>;
	};

	Tpl<Typ T, Typ U> using replace_first_arg_t
		= Typ replace_first_arg<T, U>::type;

	Tpl<Typ T> using make_not_void
		= Typ Faiz::conditional_t<is_void_v<T>, undefined, T>;

	/**
	 * @brief  Uniform interface to all pointer-like types
	 * @ingroup pointer_abstractions
	 */
	Tpl<Typ _Ptr> struct pointer_traits
	{
	private:
		Tpl<Typ T> using element_type_aux = Typ T::element_type;

		Tpl<Typ T> using differencetype_aux = Typ T::differencetype;

		Tpl<Typ T, Typ U, Typ = void> struct rebind_aux
			: replace_first_arg<T, U>
		{};

		Tpl<Typ T, Typ U> struct rebind_aux<T, U, void_t<Typ T::Tpl rebind<U>>>
		{
			using type = Typ T::Tpl rebind<U>;
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
		Tpl<Typ U> using rebind = Typ rebind_aux<_Ptr, U>::type;

		static fn
		pointer_to(make_not_void<element_type>& __e)->_Ptr
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
	Tpl<Typ T> struct pointer_traits<T*>
	{
		/// The pointer type
		using pointer = T*;
		/// The type pointed to
		using element_type = T;
		/// type used to represent the difference between two pointers
		using differencetype = ptrdiff_t;

		Tpl<Typ U> using rebind = U*;

		/**
		 *  @brief  Obtain a pointer to an object
		 *  @param  r  A reference to an object of type @c element_type
		 *  @return @c addressof(r)
		 */
		static fn
		pointer_to(make_not_void<element_type>& r) noexcept->pointer
		{
			return Faiz::addressof(r);
		}
	};


	Tpl<Typ Ptr> cfn
	to_address_aux(const Ptr& ptr) noexcept->decltype(auto)
	{
		return Faiz::pointer_traits<Ptr>::to_address(ptr);
	}

	Tpl<Typ Ptr, Typ... None> cfn
	to_address_aux(const Ptr& ptr, None...) noexcept
	{
		return Faiz::to_address_aux(ptr.operator->());
	}

	Tpl<Typ Tp> cfn
	to_address(Tp* ptr) noexcept->Tp*
	{
		return Faiz::to_address_aux(ptr);
	}

	Tpl<Typ Ptr> cfn
	to_address(const Ptr& ptr) noexcept
	{
		return Faiz::to_address_aux(ptr);
	}

} // namespace Rider::Faiz
#endif
