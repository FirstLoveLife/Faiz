#ifndef MACROS_HPP
#define MACROS_HPP

#define IS(name) \
	tpl<typ T> inline cexp bool is_##name##_v = is_##name<T>::value;

#define BI_IS(name) \
	tpl<typ From, typ To> inline cexp bool is_##name##_v \
		= is_##name<From, To>::value;

#define NOT(name) \
	tpl<typ T> inline cexp bool not_##name##_v = not is_##name<T>::value; \
	tpl<typ T> struct not_##name : bool_<not_##name##_v<T>> \
	{};

#define BI_NOT(name) \
	tpl<typ From, typ To> inline cexp bool not_##name##_v \
		= not is_##name<From, To>::value; \
	tpl<typ From, typ To> struct not_##name : bool_<not_##name##_v<From, To>> \
	{};

#define ARE(name) \
	tpl<typ... T> inline cexp bool are_##name##_v = (is_##name##_v<T> && ...); \
\
	tpl<typ... T> struct are_##name : bool_<are_##name##_v<T...>> \
	{};

#define BI_ARE(name) \
	tpl<typ... T> inline cexp bool are_##name##_v \
		= detail::binaryTraitAre_impl<is_##name, T...>(); \
\
	tpl<typ... T> struct are_##name : bool_<are_##name##_v<T...>> \
	{};

#define ANY(name) \
	tpl<typ... T> inline cexp bool any_##name##_v = (is_##name##_v<T> || ...); \
\
	tpl<typ... T> struct any_##name : bool_<any_##name##_v<T...>> \
	{};

#define BI_ANY(name) \
	tpl<typ... T> inline cexp bool any_##name##_v \
		= detail::binaryTraitOr_impl<is_##name, T...>(); \
\
	tpl<typ... T> struct any_##name : bool_<any_##name##_v<T...>> \
	{};


#define IS_NOT_ARE_ANY(name) \
	IS(name) \
	NOT(name) \
	ARE(name) \
	ANY(name)

#define NOT_ARE_ANY(name) \
	NOT(name) \
	ARE(name) \
	ANY(name)

#define BI_IS_NOT_ARE_ANY(name) \
	BI_IS(name) \
	BI_NOT(name) \
	BI_ARE(name) \
	BI_ANY(name)


#define tpl template
#define typ typename
#define SFINAE typ
#define cElseIf else if constexpr
#define cIf if constexpr
#define cElse else
#define cexp constexpr
#define PAIR(A, B) A, B
#define INLINE_VARIABLE(type, name) inline cexp type name{};
#define DEPRECATED(MSG) __attribute__((deprecated(MSG)))
#define IMPL(...) __VA_ARGS__
#define fn auto
#define FN auto
#define cval constexpr auto
#define cfn constexpr auto
#define cdfn constexpr decltype(auto)
#define cFN constexpr auto
#define CNV
#define cCNV constexpr
#define cCTOR constexpr
#define SEMICOLON ;
#define COMMA ,

#define DECLTYPE_AUTO_RETURN(...) \
	->decltype(__VA_ARGS__) \
	{ \
		return (__VA_ARGS__); \
	} \
	/**/

#define DECLTYPE_AUTO_RETURN_NOEXCEPT(...) \
	noexcept(noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))) \
		->decltype(__VA_ARGS__) \
	{ \
		return (__VA_ARGS__); \
	} \
	/**/

#define AUTO_RETURN_NOEXCEPT(...) \
	noexcept(noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))) \
	{ \
		return (__VA_ARGS__); \
	} \
	/**/

#define DECLTYPE_NOEXCEPT(...) \
	noexcept(noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))) \
		->decltype(__VA_ARGS__) /**/

#define PAIR(A, B) A, B

#define INLINE_VARIABLE(type, name) inline cexp type name{};


#define DEPRECATED(MSG) __attribute__((deprecated(MSG)))
#define IMPL(...) __VA_ARGS__
#define SEMICOLON ;
#define COMMA ,

#define DECLTYPE_AUTO_RETURN(...) \
	->decltype(__VA_ARGS__) \
	{ \
		return (__VA_ARGS__); \
	} \
	/**/

#define DECLTYPE_AUTO_RETURN_NOEXCEPT(...) \
	noexcept(noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))) \
		->decltype(__VA_ARGS__) \
	{ \
		return (__VA_ARGS__); \
	} \
	/**/

#define AUTO_RETURN_NOEXCEPT(...) \
	noexcept(noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))) \
	{ \
		return (__VA_ARGS__); \
	} \
	/**/

#define DECLTYPE_NOEXCEPT(...) \
	noexcept(noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))) \
		->decltype(__VA_ARGS__) /**/



#endif
