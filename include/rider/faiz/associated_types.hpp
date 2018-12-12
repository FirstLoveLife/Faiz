#ifndef ASSOCIATED_TYPES
#define ASSOCIATED_TYPES

#include "rider/faiz/cstddef.hpp"
#include "rider/faiz/faiz_fwd.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"
#include <iosfwd>

namespace Rider::Faiz
{
	namespace detail
	{
		template<typename T, typename Enable = void>
		struct difference_type2
		{};

		template<typename T>
		struct difference_type2<T,
			meta::if_<
				is_integral<decltype(declval<const T>() - declval<const T>())>>>
			: make_signed<decltype(declval<const T>() - declval<const T>())>
		{};

		template<typename T, typename Enable = void>
		struct difference_type1 : detail::difference_type2<T>
		{};

		template<typename T>
		struct difference_type1<T*> : meta::lazy::if_<is_object<T>, ptrdiff_t>
		{};

		template<typename T>
		struct difference_type1<T, void_t<typename T::difference_type>>
		{
			using type = typename T::difference_type;
		};
	} // namespace detail
	/// \endcond

	template<typename T>
	struct difference_type : detail::difference_type1<T>
	{};

	template<typename T>
	struct difference_type<T const> : difference_type<T>
	{};

	////////////////////////////////////////////////////////////////////////////////////////
	/// \cond
	namespace detail
	{
		template<typename I,
			typename R = decltype(*declval<I&>()),
			typename = R&>
		using reference_t_ = R;
	} // namespace detail
	/// \endcond

	template<typename R>
	using reference_t = detail::reference_t_<R>;

	////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	struct size_type
		: meta::lazy::let<make_unsigned<meta::lazy::_t<difference_type<T>>>>
	{};

	/// \cond
	namespace detail
	{
#if !defined(__GNUC__) || defined(__clang__)
		// GCC does not implement CWG393
		// per https://gcc.gnu.org/bugzilla/show_bug.cgi?id=69316
		template<typename T>
		remove_cv<T> value_type_helper(T (*)[]);
#endif
		template<typename T, size_t N>
		remove_cv<T> value_type_helper(T (*)[N]);

		template<typename T>
		using object_remove_cv = meta::if_<is_object<T>, remove_cv<T>>;

		template<typename T>
		object_remove_cv<T>
		value_type_helper(T**);

		template<typename T>
		object_remove_cv<typename T::value_type>
		value_type_helper(T*);

		template<typename T>
		object_remove_cv<typename T::element_type>
		value_type_helper(T*);

		template<typename T>
		meta::if_<is_base_of<std::ios_base, T>,
			remove_cv<typename T::char_type>>
		value_type_helper(T*);

		template<typename T>
		using value_type_ = _t<decltype(
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
	template<typename T>
	struct value_type : meta::defer<detail::value_type_, T>
	{};

	template<typename T>
	struct value_type<T const> : value_type<T>
	{};

	template<typename S, typename I>
	struct disable_sized_sentinel : false_
	{};
	/// @}
} // namespace Rider::Faiz

#endif
