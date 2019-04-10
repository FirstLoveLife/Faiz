#ifndef TYPE_TRAITS
#define TYPE_TRAITS
#include "rider/faiz/cstddef.hpp" // for size_t
#include "rider/faiz/faiz_fwd.hpp"
#include <boost/predef.h>
#include <type_traits>
/*
Don't implement:
*/

namespace Rider::Faiz
{
	// Provides member typedef type, which is defined as T if B is true at
	// compile time, or as F if B is false.
	tpl<bool B, typ T, typ F> struct conditional : type_identity<T>
	{};

	// Provides member typedef type, which is defined as T if B is true at
	// compile time, or as F if B is false.
	tpl<typ T, typ F> struct conditional<false, T, F> : type_identity<F>
	{};

	//  Provides member typedef type, which is defined as T if B is true at
	//  compile time, or as F if B is false.
	tpl<bool B, typ T, typ F> using conditional_t = _t<conditional<B, T, F>>;

	// forward declaration
	// If the imaginary function definition To `test() { return
	// std::declval<From>(); }` is well-formed, (that is, either
	// `Faiz::declval<From>()` can be converted to To using implicit
	// conversions, or both From and To are possibly `cv-qualified void`),
	// provides the member constant value equal to true. Otherwise value is
	// false. For the purposes of this check, the use of `Faiz::declval` in the
	// return statement is not considered an odr-use. Access checks are
	// performed as if from a context unrelated to either type. Only the
	// validity of the immediate context of the expression in the return
	// statement (including conversions to the return type) is considered.
	tpl<typ From, typ To> struct is_convertible;

	namespace detail
	{

		tpl<typ Default,
			class AlwaysVoid,
			tpl<typ...> class Op,
			typ... Args> struct detector : type_identity<Default>
		{
			using value_t = false_;
		};

		tpl<typ Default, tpl<typ...> class Op, class... Args> struct detector<
			Default,
			void_t<Op<Args...>>,
			Op,
			Args...>
#if BOOST_COMP_GNUC
			: type_identity<Op<Args...>>
#endif
		{
			using value_t = true_;
#if BOOST_COMP_CLANG
			using type = Op<Args...>;
#endif
		};

	} // namespace detail

	// nonesuch cannot be constructed, destroyed, or copied in the usual way.
	// However, it is an aggregate and therefore can be constructed (presumably
	// unintentionally) via aggregate initialization in contexts where the
	// destructor's availability is not an issue, such as a new-expression: new
	// std::experimental::nonesuch{}.
	//
	// This type was inspired by, and patterned after, the internal type __nat
	// (which we believe is an acronym for “not a type”) found in libc++.
	struct nonesuch
	{
		~nonesuch() = delete;
		nonesuch(nonesuch const&) = delete;
		void
		operator=(nonesuch const&)
			= delete;
	};


	// The alias tpl `is_detected` is equivalent to typ
	// `detected_or<Faiz::nonesuch, Op, Args...>::value_t`. It is an
	// alias for `Faiz::true_type` if the `tpl-id Op<Args...>` denotes a
	// valid type; otherwise it is an alias for `Faiz::false_type`.
	tpl<tpl<typ...> class Op, typ... Args> using is_detected
		= typ detail::detector<nonesuch, void, Op, Args...>::value_t;


	// The alias tpl `detected_t` is equivalent to typ
	// `detected_or<Faiz::nonesuch, Op, Args...>::type`. It is an
	// alias for `Op<Args...>` if that tpl-id denotes a valid type;
	// otherwise it is an alias for the class `Faiz::nonesuch`.
	tpl<tpl<typ...> class Op, typ... Args> using detected_t
		= _t<detail::detector<nonesuch, void, Op, Args...>>;

	// The alias tpl `detected_or` is an alias for an unspecified class
	// type with two public member typedefs `value_t` and type, which are
	// defined as follows:
	//
	// - If the tpl-id `Op<Args...>` denotes a valid type,
	// then `value_t`
	//  is an alias for `Faiz::true_type`, and type is an alias for
	// `Op<Args...>`;
	// - Otherwise, `value_t` is an alias for `Faiz::false_type` and type is
	// an alias for `Default`.
	tpl<typ Default, tpl<typ...> class Op, typ... Args> using detected_or
		= detail::detector<Default, void, Op, Args...>;

	tpl<tpl<typ...> class Op, typ... Args> cexp bool is_detected_v
		= is_detected<Op, Args...>::value;

	tpl<typ Default, tpl<typ...> class Op, typ... Args> using detected_or_t
		= _t<detected_or<Default, Op, Args...>>;
	//
	// The alias tpl is_detected_exact checks whether `detected_t<Op,
	// Args...>` is Expected.
	tpl<typ Expected, tpl<typ...> class Op, typ... Args> using is_detected_exact
		= is_same<Expected, detected_t<Op, Args...>>;

	tpl<typ Expected, tpl<typ...> class Op, typ... Args> cexp bool
		is_detected_exact_v
		= is_detected_exact<Expected, Op, Args...>::value;
	//
	//  The alias tpl `is_detected_convertible` checks whether
	// `detected_t<Op, Args...>` is convertible to To.
	tpl<typ To, tpl<typ...> class Op, typ... Args> using is_detected_convertible
		= is_convertible<detected_t<Op, Args...>, To>;

	tpl<typ To, tpl<typ...> class Op, typ... Args> cexp bool
		is_detected_convertible_v
		= is_detected_convertible<To, Op, Args...>::value;

	tpl<typ T> using is_void = is_same<void, remove_cv_t<T>>;

	IS_NOT_ARE_ANY(void)

	tpl<typ T> using is_referenceable_aux = is_void<T&>;
	tpl<typ T> cexp bool is_referenceable_v
		= is_detected_v<is_referenceable_aux, T> and not_void_v<T>;
	tpl<typ T> struct is_referenceable : bool_<is_referenceable_v<T>>
	{};

	NOT(referenceable)

	tpl<typ T> struct is_rvalue_reference;

	IS_NOT_ARE_ANY(rvalue_reference)


} // namespace Rider::Faiz


namespace Rider::Faiz::detail
{
	tpl<typ T> struct is_member_pointer_helper : false_
	{};

	tpl<typ T, typ U> struct is_member_pointer_helper<T U::*> : true_
	{};

	tpl<typ Base> true_
	is_base_of_test_func(const volatile Base*);
	tpl<typ Base> Faiz::false_
	is_base_of_test_func(const volatile void*);
	tpl<typ Base, typ Derived> using pre_is_base_of
		= decltype(is_base_of_test_func<Base>(declval<Derived*>()));

	// with <experimental/type_traits>:
	// tpl <typ Base, typ Derived>
	// using pre_is_base_of2 =
	// detected_or_t<true_, pre_is_base_of,
	// Base, Derived>;
	tpl<typ Base, typ Derived, typ = void> struct pre_is_base_of2 : true_
	{};
	// note Faiz::void_t is a C++17 feature
	tpl<typ Base, typ Derived> struct pre_is_base_of2<Base,
		Derived,
		void_t<pre_is_base_of<Base, Derived>>> : pre_is_base_of<Base, Derived>
	{};

	tpl<typ T, typ> struct is_polymorphic_impl : false_
	{};
	tpl<typ T> struct is_polymorphic_impl<T,
		decltype(dynamic_cast<void*>(declval<remove_cv_t<T>*>()))> : true_
	{};


	// NOTE: !Rider::Faiz::is_assignable<bool &, std::nullptr_t>::value'
	// "Error", clang++ has a bug here.
	// TODO: use is_detected_v
	tpl<typ T, typ Arg, typ> struct is_assignable_imp : false_
	{};
	tpl<typ T, typ Arg> struct is_assignable_imp<T,
		Arg,
		void_t<decltype(declval<T>() = declval<Arg>())>> : not_void<Arg>
	{};

	tpl<bool IsArithmetic, typ T> struct is_signed_impl : bool_<T(-1) < T(0)>
	{};
	tpl<typ T> struct is_signed_impl<false, T> : false_
	{};

	tpl<bool IsArithmetic, typ T> struct is_unsigned_impl
		: bool_<(T(-1) > T(0))>
	{};
	tpl<typ T> struct is_unsigned_impl<false, T> : false_
	{};

	tpl<typ T, typ U = remove_cv_t<T>> struct is_floating_point_aux
		: is_any<U,
			  float,
			  double,
#if _GLIBCXX_USE_FLOAT128
			  __float128, // add gcc specific
#endif
			  long double>
	{};

	tpl<typ T> cexp bool not_enum_rvalue_reference_v
		= logic::and_v<not_enum<T>, not_rvalue_reference<T>>;

	// TODO: use is_detected_v
	tpl<typ T> using is_integral_arith
		= void_t<decltype(T{} * T{}), decltype(+T{})&, decltype(T{} % 1)>;
	tpl<typ T> cexp bool is_integral_impl
		= is_detected_v<is_integral_arith,
			  T> and not_enum_rvalue_reference_v<T>;

	tpl<typ T, bool = is_integral_impl<T>> struct is_integral_aux : false_
	{};
	tpl<typ T> struct is_integral_aux<T, true> : true_
	{};

	tpl<typ T> using is_arithmetic_arith
		= void_t<decltype(T{} * T{}), decltype(+T{})&>;

	// XXX: is_arithmetic_arith will allow std::complex, which is not arithmetic
	// type(If T is an arithmetic type (that is, an integral type or a
	// floating-point type) or a cv-qualified version thereof, provides the
	// member constant value equal true. For any other type, value is false.) ,
	// so add is_class to check.
	tpl<typ T> cexp bool is_arithmetic_impl
		= is_detected_v<is_arithmetic_arith,
			  T> and not_class_v<T> and not_enum_rvalue_reference_v<T>;

	tpl<typ T, bool = is_arithmetic_impl<T>> struct is_arithmetic_aux : false_
	{};

	tpl<typ T> struct is_arithmetic_aux<T, true> : true_
	{};

	tpl<typ T, bool = is_referenceable_v<T>> struct add_rvalue_reference_impl
		: type_identity<T&&>
	{};
	tpl<typ T> struct add_rvalue_reference_impl<T, false> : type_identity<T>
	{};

	tpl<typ T, bool = is_referenceable_v<T>> struct add_lvalue_reference_impl
		: type_identity<T&>
	{};
	tpl<typ T> struct add_lvalue_reference_impl<T, false> : type_identity<T>
	{};

	tpl<typ T> char
	test(int T::*);
	struct two
	{
		char c[2];
	};
	tpl<typ T> two
	test(...);

	tpl<typ T> struct is_pointer_helper : false_
	{};
	tpl<typ T> struct is_pointer_helper<T*> : true_
	{};


	tpl<typ T, bool is_function_type = false> struct add_pointer
		: type_identity<remove_reference_t<T>*>
	{};

	tpl<typ T> struct add_pointer<T, true> : type_identity<T>
	{};

	tpl<typ T, typ... Args> struct add_pointer<T(Args...), true>
		: type_identity<T (*)(Args...)>
	{};

	tpl<typ T, typ... Args> struct add_pointer<T(Args..., ...), true>
		: type_identity<T (*)(Args..., ...)>
	{};


} // namespace Rider::Faiz::detail

namespace Rider::Faiz
{

	tpl<typ... B> using conjunction = logic::and_<B...>;
	tpl<typ... B> inline cexp bool conjunction_v = conjunction<B...>::value;

	tpl<typ... B> using disjunction = logic::or_<B...>;
	tpl<typ... B> inline cexp bool disjunction_v = disjunction<B...>::value;

	tpl<typ B> using negation = logic::not_<B>;
	tpl<typ B> inline cexp bool negation_v = negation<B>::value;

	// clang-format on

	tpl<typ T> struct remove_reference : type_identity<T>
	{};
	tpl<typ T> struct remove_reference<T&> : type_identity<T>
	{};
	tpl<typ T> struct remove_reference<T&&> : type_identity<T>
	{};

	tpl<typ T> struct remove_const : type_identity<T>
	{};
	tpl<typ T> struct remove_const<const T> : type_identity<T>
	{};

	tpl<typ T> struct remove_volatile : type_identity<T>
	{};

	tpl<typ T> struct remove_volatile<volatile T> : type_identity<T>
	{};

	tpl<typ T> struct remove_extent : type_identity<T>
	{};

	tpl<typ T> struct remove_extent<T[]> : type_identity<T>
	{};

	tpl<typ T, size_t N> struct remove_extent<T[N]> : type_identity<T>
	{};
	tpl<typ T> using remove_extent_t = _t<remove_extent<T>>;

	tpl<typ T> struct remove_all_extents;
	tpl<typ T> using remove_all_extents_t = _t<remove_all_extents<T>>;

	tpl<typ T> struct remove_all_extents : type_identity<T>
	{};
	tpl<typ T> struct remove_all_extents<T[]>
		: type_identity<remove_all_extents_t<T>>
	{};
	tpl<typ T, size_t V> struct remove_all_extents<T[V]>
		: type_identity<remove_all_extents_t<T>>
	{};


	// Provides the member typedef type which is the type pointed to by T,
	// or, if T is not a pointer, then type is the same as T.
	// ```cpp
	// print_is_same<int, remove_pointer<int*>::type>();  // true
	// print_is_same<int, remove_pointer<int**>::type>(); // false
	// ```
	tpl<typ T> struct remove_pointer : type_identity<T>
	{};
	tpl<typ T> struct remove_pointer<T*> : type_identity<T>
	{};
	tpl<typ T> struct remove_pointer<T* const> : type_identity<T>
	{};
	tpl<typ T> struct remove_pointer<T* volatile> : type_identity<T>
	{};
	tpl<typ T> struct remove_pointer<T* const volatile> : type_identity<T>
	{};

	tpl<typ T> using remove_cv_t = _t<remove_cv<T>>;
	tpl<typ T> using remove_const_t = _t<remove_const<T>>;
	tpl<typ T> using remove_volatile_t = _t<remove_volatile<T>>;
	tpl<typ T> using remove_extent_t = _t<remove_extent<T>>;
	tpl<typ T> using remove_pointer_t = _t<remove_pointer<T>>;

	// If the type **T** is a reference type, provides the member typedef
	// type which is the type referred to by
	// **T** with its topmost cv-qualifiers removed. Otherwise type is **T**
	// with its topmost cv-qualifiers removed.
	//
	// Removing cvref does most of what decay does, but doesn't convert
	// function types and array types to pointers.

	tpl<typ T> struct remove_cvref : remove_cv<remove_reference_t<T>>
	{};

	// tpl<typ T> using remove_cvref_t = typename remove_cvref<T>::type;

	/******************** is *********************/

	// If T and U name the same type (including const/volatile
	// qualifications), provides the member constant value equal to true.
	// Otherwise value is false.
	tpl<typ T, typ U> struct is_same : false_
	{};

	// If T and U name the same type (including const/volatile
	// qualifications), provides the member constant value equal to true.
	// Otherwise value is false.
	tpl<typ T> struct is_same<T, T> : true_
	{};


	tpl<typ T> struct is_null_pointer : is_same<nullptr_t, remove_cv_t<T>>
	{};
	IS(null_pointer)

	tpl<typ T> struct is_const : false_
	{};
	tpl<typ T> struct is_const<const T> : true_
	{};

	IS_NOT_ARE_ANY(const)

	tpl<typ T> struct is_reference : false_
	{};
	tpl<typ T> struct is_reference<T&> : true_
	{};
	tpl<typ T> struct is_reference<T&&> : true_
	{};

	IS_NOT_ARE_ANY(reference)

	tpl<typ T> struct is_function
		: bool_<not_const_v<T const> and not_reference_v<T>>
	{};

	tpl<typ T> struct is_volatile : false_
	{};
	tpl<typ T> struct is_volatile<T volatile> : true_
	{};
	tpl<typ T, size_t N> struct is_volatile<T volatile[N]> : true_
	{};
	tpl<typ T> struct is_volatile<T volatile[]> : true_
	{};

	IS_NOT_ARE_ANY(volatile)

	IS_NOT_ARE_ANY(function)

	tpl<typ T> struct is_floating_point : detail::is_floating_point_aux<T>
	{};


	IS_NOT_ARE_ANY(floating_point);

	// Checks whether T is an integral type. Provides the member constant
	// value which is equal to true, if T is the type bool, char, char16_t,
	// char32_t, wchar_t, short, int, long, long long, or any
	// implementation-defined extended integer types, including any signed,
	// unsigned, and cv-qualified variants. Otherwise, value is equal to
	// false.

	tpl<typ T> struct is_integral : detail::is_integral_aux<T>
	{};

	IS_NOT_ARE_ANY(integral)

	//  If T is an arithmetic type (that is, an integral type or a
	// floating-point type) or a cv-qualified version thereof, provides the
	// member constant value equal true. For any other type, value is false.
	//
	// gcc may complains, latest msvc/clang compiles happily
	tpl<typ T> struct is_arithmetic : detail::is_arithmetic_aux<T>
	{};

	IS_NOT_ARE_ANY(arithmetic)


	// Checks whether T is **a pointer to object** or **a pointer to
	// function** (but not a pointer to member/member function). Provides
	// the member constant value which is equal to true, if T is a
	// object/function pointer type. Otherwise, value is equal to false.
	tpl<typ T> struct is_pointer : detail::is_pointer_helper<remove_cv_t<T>>
	{};

	IS_NOT_ARE_ANY(pointer)

	tpl<typ T> struct is_lvalue_reference : false_
	{};
	tpl<typ T> struct is_lvalue_reference<T&> : true_
	{};

	tpl<typ T> struct is_rvalue_reference : false_
	{};
	tpl<typ T> struct is_rvalue_reference<T&&> : true_
	{};


	tpl<typ T> struct is_object
		: bool_<not_reference_v<T> and not_void_v<T> and not_function_v<T>>
	{};


	tpl<typ T, unsigned N = 0> struct extent : size_t_<0>
	{};

	tpl<typ T> struct extent<T[], 0> : size_t_<0>
	{};

	tpl<typ T, unsigned N> struct extent<T[], N> : extent<T, N - 1>
	{};

	tpl<typ T, Faiz::size_t I> struct extent<T[I], 0> : size_t_<I>
	{};

	tpl<typ T, Faiz::size_t I, unsigned N> struct extent<T[I], N>
		: extent<T, N - 1>
	{};
	tpl<typ T, unsigned N = 0> inline cexp Faiz::size_t extent_v
		= extent<T, N>::value;

	//  If T is an object type or a function type that has no cv- or ref-
	//  qualifier (since C++17), provides a member typedef type which is T&. If
	//  T is an rvalue reference to some type U, then type is U&. Otherwise,
	//  type is T.


	// FIXME: add_rvalue_reference_impl is a workaround here, otherwise forward
	// declaration can also not solve type dependency.
	tpl<typ T> struct add_rvalue_reference
		: detail::add_rvalue_reference_impl<T>
	{};

	tpl<typ T> struct add_lvalue_reference
		: detail::add_lvalue_reference_impl<T>
	{};


	// Converts any type **T** to a reference type, making it possible to
	// use member functions in *decltype* expressions without the need to go
	// through constructors.
	//
	// *declval* is commonly used in tpls where acceptable tpl
	// parameters may have no constructor in common, but have the same
	// member function whose return type is needed.
	//
	// Note that because no definition exists for *declval*, it can only be
	// used in unevaluated contexts; it is an error to evaluate an
	// expression that contains this function. Formally, the program is
	// *ill-formed* if this function is *odr-used*.
	//
	// Cannot be called and thus never returns a value. The return type is
	// T&& unless **T** is (possibly cv-qualified) void, in which case the
	// return type is **T**.
	//
	// return `add_rvalue_reference_t<T>` is for reference collapsing

	tpl<typ T> struct add_cv : type_identity<const volatile T>
	{};

	tpl<typ T> struct add_const : type_identity<const T>
	{};

	tpl<typ T> struct add_volatile : type_identity<volatile T>
	{};

	tpl<typ T> using add_cv_t = _t<add_cv<T>>;
	tpl<typ T> using add_const_t = _t<add_const<T>>;
	tpl<typ T> using add_volatile_t = _t<add_volatile<T>>;

	// p0007r0
	// https://stackoverflow.com/questions/34566063/why-is-the-const-overload-of-as-const-deleted
	// Forms lvalue reference to const type of t
	tpl<typ T> cexp add_const_t<T>&
	as_const(T& t) noexcept;

	//  const rvalue reference overload is deleted to disallow rvalue arguments
	tpl<typ T> void
	as_const(const T&&)
		= delete;

	// TODO: check whether standard has as_cref_t
	tpl<typ T, typ R = remove_reference_t<T>> using as_cref_t
		= add_lvalue_reference_t<add_const_t<R>>;

	tpl<typ T> struct is_signed : detail::is_signed_impl<is_arithmetic_v<T>, T>
	{};


	// using std::is_signed_v;
	// IS_NOT_ARE_ANY(signed)
	IS_NOT_ARE_ANY(signed)

	tpl<typ T> cexp bool is_unsigned_v = not_signed_v<T> and is_arithmetic_v<T>;
	tpl<typ T> struct is_unsigned : bool_<is_unsigned_v<T>>
	{};

	// TODO: add support for compiler extension larger signed.
	// XXX: I don't want support non-standard extension now.
	tpl<typ T> struct make_signed
	{
	private:
		static_assert((is_integral_v<T> or is_enum_v<T>),
			"The tpl argument to make_signed must be an integer or enum "
			"type.");
		static_assert(not_same_v<remove_cv_t<T>, bool>,
			"The tpl argument to make_signed must not be the type bool.");

		using t_no_cv = remove_cv_t<T>;

		// clang-format off
		static cfn
		base_integer_type_impl()
		{
			cIf(is_signed_v<T> and
						 is_integral_v<T> and
						 not_any_v<t_no_cv, char, wchar_t, bool>)
			{
				return type_identity<T>{};
			}
			cElseIf  (is_integral_v<T> and
							   not_any_v<t_no_cv, char, wchar_t, bool>)
			{
				cIf (is_same_v<t_no_cv, unsigned char>)
				{
					return type_identity<signed char>{};
				}
				cElseIf (is_same_v<t_no_cv, unsigned short>)
				{
					return  type_identity<signed short>{};
				}
				 cElseIf (is_same_v<t_no_cv, unsigned int>)
				{
					return  type_identity<int>{};
				}
				cElseIf (is_same_v<t_no_cv, unsigned long>)
				{
					return  type_identity<long>{};
				}
				else
				{
					return type_identity<long long> {};
				}
			}
			else
			{
				cIf (sizeof(t_no_cv) == sizeof(unsigned char))
				{
					return type_identity<signed char>{};
				}
				cElseIf (sizeof(t_no_cv) == sizeof(unsigned short))
				{
					return type_identity<signed short>{};
				}
				cElseIf (sizeof(t_no_cv) == sizeof(unsigned int))
				{
					return type_identity<int>{};
				}
				cElseIf (sizeof(t_no_cv) == sizeof(unsigned long))
				{
					return type_identity<long>{};
				}
				else
				{
					return type_identity<long long>{};
				}
			}
		}
		using base_integer_type = _t<decltype(base_integer_type_impl())>;

		// clang-format on

		// Add back any const qualifier:
		using const_base_integer_type = meta::
			if_<is_const<T>, add_const_t<base_integer_type>, base_integer_type>;

	public:
		// Add back any volatile qualifier:
		using type = meta::if_<is_volatile<T>,
			add_volatile_t<const_base_integer_type>,
			const_base_integer_type>;
	};

	tpl<typ T> struct make_unsigned
	{
	private:
		static_assert(is_integral_v<T> or is_enum_v<T>,
			"The tpl argument to make_unsigned must be an integer or enum "
			"type.");
		static_assert(not_same_v<remove_cv_t<T>, bool>,
			"The tpl argument to make_unsigned must not be the type "
			"bool");

		using t_no_cv = remove_cv_t<T>;

		// clang-format off
		static cfn
		base_integer_type_impl()
		{
			cIf(is_unsigned_v<T> and
						 is_integral_v<T> and
						 not_any_v<t_no_cv, char, wchar_t, bool>)
			{
				return type_identity<T>{};
			}
			cElseIf(is_integral_v<T> and
							  not_any_v<t_no_cv, char, wchar_t, bool>)
			{
				cIf(is_same_v<t_no_cv, signed char>)
				{
					return type_identity<unsigned char>{};
				}
				cElseIf(is_same_v<t_no_cv, short>)
				{
					return type_identity<unsigned short>{};
				}
				cElseIf(is_same_v<t_no_cv, int>)
				{
					return type_identity<unsigned int>{};
				}
				cElseIf(is_same_v<t_no_cv, long>)
				{
					return type_identity<unsigned long>{};
				}
				else
				{
					return type_identity<unsigned long long>{};
				}
			}
			else
			{
				cIf(sizeof(t_no_cv) == sizeof(unsigned char))
				{
					return type_identity<unsigned char>{};
				}
				cElseIf(sizeof(t_no_cv) == sizeof(unsigned short))
				{
					return type_identity<unsigned short>{};
				}
				cElseIf(sizeof(t_no_cv) == sizeof(unsigned int))
				{
					return type_identity<unsigned int>{};
				}
				cElseIf(sizeof(t_no_cv) == sizeof(unsigned long))
				{
					return type_identity<unsigned long>{};
				}
				else
				{
					return type_identity<unsigned long long>{};
				}
			}
		}

		// clang-format on

		using base_integer_type = _t<decltype(base_integer_type_impl())>;

		using const_base_integer_type = meta::
			if_<is_const<T>, add_const_t<base_integer_type>, base_integer_type>;

	public:
		// Add back any volatile qualifier:
		using type = meta::if_<is_volatile<T>,
			add_volatile_t<const_base_integer_type>,
			const_base_integer_type>;
	};

	tpl<typ T> using make_unsigned_t = _t<make_unsigned<T>>;

	tpl<typ T> using make_signed_t = _t<make_signed<T>>;

	tpl<bool B, typ T> struct enable_if
	{};

	tpl<typ T> struct enable_if<true, T> : type_identity<T>
	{};

	tpl<typ T> inline cexp bool is_array_v = false;
	tpl<typ T> inline cexp bool is_array_v<T[]> = true;
	tpl<typ T, size_t N> inline cexp bool is_array_v<T[N]> = true;

	tpl<typ T> struct is_array : bool_<is_array_v<T>>
	{};

	NOT_ARE_ANY(array)

	// If the imaginary function definition To test() { return
	// std::declval<From>(); } is well-formed, (that is, either
	// std::declval<From>() can be converted to To using implicit conversions,
	// or both From and To are possibly cv-qualified void), provides the member
	// constant value equal to true. Otherwise value is false. For the purposes
	// of this check, the use of std::declval in the return statement is not
	// considered an odr-use.
	//
	//  Access checks are performed as if from a context unrelated to either
	// type. Only the validity of the immediate context of the expression in the
	// return statement (including conversions to the return type) is
	// considered.
	// // using namespace logic;
	// tpl<typ From, typ To>
	// cexp bool is_convertible_v = or_<
	// 	and_<or_<is_void<From>, is_function<To>, is_array<To>>, is_void<To>>,
	// 	is_detected<is_convertible_helper, From, To>>::value;
	//
	tpl<typ To1> static void test_aux(To1) noexcept;

	tpl<typ From, typ To> using is_convertible_helper
		= decltype(test_aux<To>(declval<From>()));

	tpl<typ From, typ To> cfn
	my_is_convertible()
	{
		cIf(disjunction_v<is_void<From>, is_function<To>, is_array<To>>)
		{
			return is_void<To>();
		}
		else
		{
			return is_detected<is_convertible_helper, From, To>();
		}
	}

	tpl<typ From, typ To> struct is_convertible
		: decltype(my_is_convertible<From, To>())
	{};

	BI_IS_NOT_ARE_ANY(convertible)

	// TODO: use detection idiom implement is_convertible
	// If T is an object or reference type and the variable definition T
	// obj(std::declval<Args>()...); is well-formed, provides the member
	// constant value equal to true. In all other cases, value is false.

	// FIXME: the codes below cannot pass tests somehow
	// tpl<typ T, class, class...>
	// struct is_constructible_impl : false_
	// {};
	// tpl<typ T, class... Us>
	// struct is_constructible_impl<T,
	// 	decltype(::new(declval<void*>()) T(declval<Us&>()...)),
	// 	Us...> : true_
	// {};
	// tpl<typ T, class... Us>
	// struct is_constructible : is_constructible_impl<T, void, Us...>
	// {};

	tpl<typ Tp> struct is_copy_constructible
		: is_constructible<Tp, as_cref_t<Tp>>
	{};

	IS_NOT_ARE_ANY(copy_constructible)

	tpl<typ T> inline cexp size_t rank_v = 0;
	tpl<typ T> inline cexp size_t rank_v<T[]> = 1u + rank_v<T>;
	tpl<typ T, size_t N> inline cexp size_t rank_v<T[N]> = 1u + rank_v<T>;

	tpl<typ T> struct rank : size_t_<rank_v<T>>
	{};

	// If **T** is a reference type, then provides the member typedef type
	// which is a pointer to the referred type. Otherwise, if **T** names an
	// object type, a function type that is not **cv-** or **ref-qualified**
	// (since C++17), or a (possibly **cv-qualified**) void type, provides
	// the member typedef type which is the type T*. Otherwise (if T is a
	// **cv-** or **ref-qualified** function type), provides the member
	// typedef type which is the type **T**
	//
	// * type:	pointer to T or to the type referenced by T
	tpl<typ T> struct add_pointer : detail::add_pointer<T, is_function_v<T>>
	{};
	tpl<typ T> using add_pointer_t = _t<add_pointer<T>>;

	tpl<typ T, typ U = remove_reference_t<T>, typ E = remove_cv_t<U>> fn
	decay_impl()
	{
		cIf(not_referenceable_v<T>)
		{
			return type_identity<E>();
		}
		cElseIf(is_array_v<U>)
		{
			return type_identity<add_pointer_t<remove_extent_t<U>>>();
		}
		cElseIf(is_function_v<U>)
		{
			return type_identity<add_pointer_t<U>>();
		}
		else
		{
			return type_identity<E>();
		}
	}

	tpl<typ T> struct decay : decltype(decay_impl<T>())
	{};

	tpl<typ T> using decay_t = _t<decay<T>>;

	tpl<typ T, typ Arg> struct is_assignable
		: detail::is_assignable_imp<T, Arg, void>
	{};

	// If Derived is derived from Base or if both are the same non-union class
	// (in both cases ignoring cv-qualification), provides the member constant
	// value equal to true. Otherwise value is false.
	//
	//  If both Base and Derived are non-union class types, and they are not the
	// same type (ignoring cv-qualification), Derived shall be a complete type;
	// otherwise the behavior is undefined.
	//
	// Faiz::is_base_of<A, B>::value is true even if A is a private, protected,
	// or ambiguous base class of B. In many situations,
	// Faiz::is_convertible<B*, A*> is the more appropriate test.
	//
	// Although no class is its own base, Faiz::is_base_of<T, T>::value is true
	// because the intent of the trait is to model the "is-a" relationship, and
	// T is a T. Despite that, Faiz::is_base_of<int, int>::value is false
	// because only classes participate in the relationship that this trait
	// models.
	tpl<typ Base, typ Derived> struct is_base_of
		: meta::if_<logic::and_<is_class<Base>, is_class<Derived>>,
			  detail::pre_is_base_of2<Base, Derived>,
			  false_>
	{};
	tpl<typ Base, typ Derived> inline cexp bool is_base_of_v
		= is_base_of<Base, Derived>::value;


	tpl<typ T> struct is_member_pointer
		: detail::is_member_pointer_helper<remove_cv_t<T>>
	{};

	IS(member_pointer)
	// If `T` is a scalar type (that is a possibly **cv-qualified** arithmetic,
	// pointer, pointer to member, enumeration, or `Faiz::nullptr_t` type),
	// provides the member constant value equal `true`. For any other type,
	// value is `false`.
	//
	// Each individual memory location in the C++ memory model, including the
	// hidden memory locations used by language features (e.g virtual table
	// pointer), has scalar type (or is a sequence of adjacent bit-fields of
	// non-zero length). Sequencing of side-effects in expression evaluation,
	// interthread synchronization, and dependency ordering are all defined in
	// terms of individual scalar objects.
	//
	tpl<typ T> struct is_scalar : disjunction<is_arithmetic<T>,
									  is_enum<T>,
									  is_pointer<T>,
									  is_member_pointer<T>,
									  is_null_pointer<T>>
	{};

	IS_NOT_ARE_ANY(scalar)

	tpl<typ T> struct is_member_function_pointer_helper : false_
	{};

	tpl<typ T, typ U> struct is_member_function_pointer_helper<T U::*>
		: is_function<T>
	{};
	//
	//  Checks whether T is a non-static member function pointer. Provides the
	// member constant value which is equal to true, if T is a non-static member
	// function pointer type. Otherwise, value is equal to false.
	tpl<typ T> struct is_member_function_pointer
		: is_member_function_pointer_helper<remove_cv_t<T>>
	{};

	IS_NOT_ARE_ANY(member_function_pointer)

	// Checks whether T is a non-static member object. Provides the member
	// constant value which is equal to true, if T is a non-static member object
	// type. Otherwise, value is equal to false.
	tpl<typ T> struct is_member_object_pointer
		: bool_<is_member_pointer_v<T> and not_member_function_pointer_v<T>>
	{};

	IS_NOT_ARE_ANY(member_object_pointer)

	// Given two (possibly identical) types Base and Derived,
	// is_base_of<Base, Derived>::value == true if and only if Base is a
	// direct or indirect base class of Derived. This is like
	// is_base_of<Base, Derived> but returns false if Derived is the same as
	// Base. So is_derived is true only if Derived is actually a subclass of
	// Base and not Base itself.
	//
	// is_derived may only be applied to complete types.
	//
	// Example usage:
	//     is_derived<int, int>::value             => false
	//     is_derived<int, bool>::value            => false
	//     is_derived<Parent, Child>::value        => true
	//     is_derived<Child, Parent>::value        => false
	tpl<typ Base, typ Derived> struct is_derived
		: bool_<
			  is_base_of_v<Base,
				  Derived> and not_same_v<remove_cv_t<Base>, remove_cv_t<Derived>>>
	{};

	BI_IS_NOT_ARE_ANY(derived)

	tpl<typ T> struct is_polymorphic : detail::is_polymorphic_impl<T, void*>
	{};

	IS(polymorphic)

	tpl<typ T> struct is_copy_assignable
		: is_assignable<add_lvalue_reference_t<T>, as_cref_t<T>>
	{};

	IS_NOT_ARE_ANY(copy_assignable)

	// tpl<typ... T >
	// inline cexp bool are_copy_assignable_v = (is_copy_assignable_v<T> &&
	// ...);

	// tpl<typ... T>
	// struct are_copy_assignable : bool_<are_copy_assignable_v<T...>>
	// {
	// };

	tpl<typ T> struct is_trivially_copy_assignable
		: is_trivially_assignable<add_lvalue_reference_t<T>, as_cref_t<T>>
	{};

	IS_NOT_ARE_ANY(trivially_copy_assignable)

	tpl<bool, typ T, typ A> struct is_nothrow_assignable_aux;

	tpl<typ T, typ A> struct is_nothrow_assignable_aux<false, T, A> : false_
	{};

	tpl<typ T> using move_assignment_t
		= decltype(declval<T&>() = declval<T&&>());

	tpl<typ T> inline cexp bool is_move_assignable_v
		= is_detected_v<move_assignment_t, T>;

	tpl<typ T> struct is_move_assignable : bool_<is_move_assignable_v<T>>
	{};

	NOT_ARE_ANY(move_assignable)

	// // clang-format off
	// tpl<typ T>
	// inline cexp bool is_nothrow_move_assignable_v
	// 	= is_move_assignable_v<T> and noexcept(declval<T&>() = declval<T&&>());
	// // clang-format on

	// tpl<typ T>
	// struct is_nothrow_move_assignable :
	// bool_<is_nothrow_move_assignable_v<T>>
	// {};

	tpl<typ T> struct is_nothrow_move_assignable
		: is_nothrow_assignable<add_lvalue_reference_t<T>,
			  add_rvalue_reference_t<T>>
	{};

	IS_NOT_ARE_ANY(nothrow_move_assignable)

	tpl<typ T, typ A> struct is_nothrow_assignable_aux<true, T, A>
		: bool_<noexcept(declval<T>() = declval<A>())>
	{};

	tpl<typ T, typ A> struct is_nothrow_assignable
		: is_nothrow_assignable_aux<is_assignable_v<T, A>, T, A>
	{};

	tpl<typ T> struct is_nothrow_copy_assignable
		: is_nothrow_assignable<add_lvalue_reference_t<T>, as_cref_t<T>>
	{};


	IS_NOT_ARE_ANY(nothrow_copy_assignable);

	tpl<typ T> struct is_unknown_bound_array : false_
	{};
	tpl<typ T> struct is_unknown_bound_array<T[]> : true_
	{};

	IS_NOT_ARE_ANY(unknown_bound_array)

	tpl<typ T, typ U = remove_all_extents_t<T>> using has_dtor
		= decltype(declval<U>().~U());

	// clang-format off
    // FIXME \\\ fix compiler.....  gcc has bug here, I post a thread here: https://stackoverflow.com/questions/53456848/implement-is-destructible-with-detected-idiomhttps://stackoverflow.com/questions/53456848/implement-is-destructible-with-detected-idiom
    // tpl<typ T>
    // cexp bool is_destructible_v =
    // 	is_reference_v<T> || (!(is_void_v<T> || is_function_v<T> || is_unknown_bound_array_v<T>) and is_object_v<T> and is_detected_v<has_dtor, T>);

 #if BOOST_COMP_CLANG
    tpl<typ T>
    cexp bool is_destructible_v =
        (is_detected_v<has_dtor,
           T>
       or is_reference_v<T>)
    and not_unknown_bound_array_v<T>
        and not_function_v<T>;

 #elif BOOST_COMP_GNUC
    tpl<typ T>
    cexp bool
    my_is_destructible()
    {
        cIf(is_reference_v<T>)
        {
            return true;
        }
        cElseIf(
            is_same_v<remove_cv_t<T>,
                void> || is_function_v<T> || is_unknown_bound_array<T>::value)
        {
            return false;
        }
        cElseIf(is_object_v<T>)
        {
            return is_detected_v<has_dtor, T>;
        }
        else
        {
            return false;
        }
    }
    tpl<typ T>
    cexp bool is_destructible_v = my_is_destructible<T>();
 #endif
	// clang-format on
	tpl<typ T> struct is_destructible : bool_<is_destructible_v<T>>
	{};

	tpl<typ T> inline cexp bool is_nothrow_destructible_v
		= is_destructible_v<T> and noexcept(is_destructible_v<T>);
	tpl<typ T> struct is_nothrow_destructible
		: bool_<is_nothrow_destructible_v<T>>
	{};
} // namespace Rider::Faiz

#endif
