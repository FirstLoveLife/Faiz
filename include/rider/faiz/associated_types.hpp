#ifndef ASSOCIATED_TYPES
#define ASSOCIATED_TYPES

#include "rider/faiz/cstddef.hpp"
#include "rider/faiz/faiz_fwd.hpp"
#include "rider/faiz/macros.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"
#include <iosfwd>

namespace Rider::Faiz
{
	namespace detail
	{
		Tpl<Typ T, Typ Enable = void> struct difference_type2
		{};

		Tpl<Typ T> struct difference_type2<T,
			meta::if_<
				is_integral<decltype(declval<const T>() - declval<const T>())>>>
			: make_signed<decltype(declval<const T>() - declval<const T>())>
		{};

		Tpl<Typ T, Typ Enable = void> struct difference_type1
			: detail::difference_type2<T>
		{};

		Tpl<Typ T> struct difference_type1<T*>
			: meta::lazy::if_<is_object<T>, ptrdiff_t>
		{};

		Tpl<Typ T> struct difference_type1<T, void_t<Typ T::difference_type>>
		{
			using type = Typ T::difference_type;
		};
	} // namespace detail
	/// \endcond

	Tpl<Typ T> struct difference_type : detail::difference_type1<T>
	{};

	Tpl<Typ T> struct difference_type<T const> : difference_type<T>
	{};

	////////////////////////////////////////////////////////////////////////////////////////
	/// \cond
	namespace detail
	{
		Tpl<Typ I, Typ R = decltype(*declval<I&>()), Typ = R&> using reference_t_
			= R;
	} // namespace detail
	/// \endcond

	Tpl<Typ R> using reference_t = detail::reference_t_<R>;

	////////////////////////////////////////////////////////////////////////////////////////
	Tpl<Typ T> struct size_type
		: meta::lazy::let<make_unsigned<meta::lazy::_t<difference_type<T>>>>
	{};

	/// \cond
	namespace detail
	{
#if !defined(__GNUC__) || defined(__clang__)
		// GCC does not implement CWG393
		// per https://gcc.gnu.org/bugzilla/show_bug.cgi?id=69316
		Tpl<Typ T> remove_cv<T> value_type_helper(T (*)[]);
#endif
		Tpl<Typ T, size_t N> remove_cv<T> value_type_helper(T (*)[N]);

		Tpl<Typ T> using object_remove_cv
			= meta::if_<is_object<T>, remove_cv<T>>;

		Tpl<Typ T> object_remove_cv<T>
		value_type_helper(T**);

		Tpl<Typ T> object_remove_cv<Typ T::value_type>
		value_type_helper(T*);

		Tpl<Typ T> object_remove_cv<Typ T::element_type>
		value_type_helper(T*);

		Tpl<Typ T>
			meta::if_<is_base_of<std::ios_base, T>, remove_cv<Typ T::char_type>>
			value_type_helper(T*);

		Tpl<Typ T> using value_type_ = _t<decltype(
			detail::value_type_helper(meta::detail::_nullptr_v<T>()))>;
	} // namespace detail
	/// \endcond

	////////////////////////////////////////////////////////////////////////////////////////
	// Not to spec:
	// * arrays of unknown bound have no value type on compilers that do not
	// implement
	//   CWG 393
	//   (http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#393).
	// * For class types with member type element_type, value type is
	// element_type with any
	//   cv-qualifiers stripped (See ericniebler/stl2#299).
	// * using member "char_type" as the value type of class types derived
	// from
	//   std::ios_base is an extension.
	Tpl<Typ T> struct value_type : meta::defer<detail::value_type_, T>
	{};

	Tpl<Typ T> struct value_type<T const> : value_type<T>
	{};

	Tpl<Typ S, Typ I> struct disable_sized_sentinel : false_
	{};
	/// @}
} // namespace Rider::Faiz

#endif
