#ifndef MACROS_HPP
#define MACROS_HPP

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

#endif
