#ifndef MACROS_HPP
#define MACROS_HPP

#define IS(name) \
	Tpl<Typ T> inline cexp bool is_##name##_v = is_##name<T>::value;

#define BI_IS(name) \
	Tpl<Typ From, Typ To> inline cexp bool is_##name##_v \
		= is_##name<From, To>::value;

#define NOT(name) \
	Tpl<Typ T> inline cexp bool not_##name##_v = not is_##name<T>::value; \
	Tpl<Typ T> struct not_##name : bool_<not_##name##_v<T>> \
	{};

#define BI_NOT(name) \
	Tpl<Typ From, Typ To> inline cexp bool not_##name##_v \
		= not is_##name<From, To>::value; \
	Tpl<Typ From, Typ To> struct not_##name : bool_<not_##name##_v<From, To>> \
	{};

#define ARE(name) \
	Tpl<Typ... T> inline cexp bool are_##name##_v = (is_##name##_v<T> && ...); \
\
	Tpl<Typ... T> struct are_##name : bool_<are_##name##_v<T...>> \
	{};

#define BI_ARE(name) \
	Tpl<Typ... T> inline cexp bool are_##name##_v \
		= detail::binaryTraitAre_impl<is_##name, T...>(); \
\
	Tpl<Typ... T> struct are_##name : bool_<are_##name##_v<T...>> \
	{};

#define BI_NOT_ALL(name) \
	Tpl<Typ... T> inline cexp bool not_all_##name##_v \
		= not detail::binaryTraitAre_impl<is_##name, T...>(); \
\
	Tpl<Typ... T> struct not_all_##name : bool_<not_all_##name##_v<T...>> \
	{};


#define ANY(name) \
	Tpl<Typ... T> inline cexp bool any_##name##_v = (is_##name##_v<T> || ...); \
\
	Tpl<Typ... T> struct any_##name : bool_<any_##name##_v<T...>> \
	{};

#define BI_ANY(name) \
	Tpl<Typ... T> inline cexp bool any_##name##_v \
		= detail::binaryTraitOr_impl<is_##name, T...>(); \
\
	Tpl<Typ... T> struct any_##name : bool_<any_##name##_v<T...>> \
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


#define Tpl template
#define Typ typename
#define SFINAE Typ
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
#define ceCTOR constexpr explicit
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

#define PACK_ARE(name) \
	namespace detail \
	{ \
		template<typename... Types> \
		struct name##Helper; \
		template<typename... Types> \
		struct name##Helper<Pack<Types...>> \
		{ \
			static constexpr bool value{(is_##name##_v<Types...>)}; \
		}; \
	}															\
	Tpl<Typ... TemplateTemplateTypes> inline cexp bool are_##name##_v		\
	= (detail::name##Helper<TemplateTemplateTypes>::value and ... and true);	\
																		\
	Tpl<Typ... TemplateTemplateTypes> struct are_##name : bool_<are_##name##_v<TemplateTemplateTypes...>> \
{};


#endif
