#ifndef CALL_TRAITS
#define CALL_TRAITS
#include "rider/faiz/type_traits.hpp"
namespace rider::faiz
{
	// use pandoc convert doc from
	// https://www.boost.org/doc/libs/1_51_0/libs/utility/call_traits.htm
	// The template class call_traits<T> encapsulates the "best" method to
	// pass a parameter of some type T to or from a function, and consists
	// of a collection of typedefs defined as in the table below. The
	// purpose of call_traits is to ensure that problems like "[references
	// to references](#refs)" never occur, and that parameters are passed in
	// the most efficient manner possible (see [examples](#examples)). In
	// each case if your existing practice is to use the type defined on the
	// left, then replace it with the call_traits defined type on the right.
	// Note that for compilers that do not support either partial
	// specialization or member templates, no benefit will occur from using
	// call_traits: the call_traits defined types will always be the same as
	// the existing practice in this case. In addition if only member
	// templates and not partial template specialisation is support by the
	// compiler (for example Visual C++ 6) then call_traits can not be used
	// with array types (although it can be used to solve the reference to
	// reference problem).
	//
	// +-----------------+-----------------+-----------------+-----------------+
	// | Existing        | call_traits     | Description     | Notes | |
	// practice        | equivalent      |                 | |
	// +-----------------+-----------------+-----------------+-----------------+
	// | T               | `call_traits<T>  | Defines a type  | 2 | |
	// (return by      | ::value_type`    | that represents | | | value) |
	// | the "value"     |                 | |                 | | of type
	// T. Use  |                 | |                 |                 |
	// this for        |                 | |                 | | functions
	// that  |                 | |                 |                 |
	// return by       |                 | |                 | | value, or
	// |                 | |                 |                 | possibly
	// for    |                 | |                 |                 |
	// stored values   |                 | |                 | | of type T.
	// |                 |
	// +-----------------+-----------------+-----------------+-----------------+
	// | T&              | `call_traits<T>  | Defines a type  | 1 | |
	// (return value)  | ::reference`     | that represents | | | | | a
	// reference to  |                 | |                 | | type T. Use
	// for |                 | |                 |                 |
	// functions that  |                 | |                 | | would
	// normally  |                 | |                 |                 |
	// return a T&.    |                 |
	// +-----------------+-----------------+-----------------+-----------------+
	// | const T&        | `call_traits<T>  | Defines a type  | 1 | |
	// (return value)  | ::const_referen | that represents | | | | ce` | a
	// constant      |                 | |                 | | reference to
	// |                 | |                 |                 | type T. Use
	// for |                 | |                 |                 |
	// functions that  |                 | |                 | | would
	// normally  |                 | |                 |                 |
	// return a const  |                 | |                 | | T&. | |
	// +-----------------+-----------------+-----------------+-----------------+
	// | const T&        | `call_traits<T>  | Defines a type  | 1,3 | |
	// (function       | ::param_type`    | that represents | | | parameter)
	// |                 | the "best"      |                 | | | | way to
	// pass a   |                 | |                 |                 |
	// parameter of    |                 | |                 | | type T to a
	// |                 | |                 |                 | function.
	// |                 |
	// +-----------------+-----------------+-----------------+-----------------+
	// Notes:
	// 1.  If T is already reference type, then call_traits is defined such
	//     that [references to references](#refs) do not occur (requires
	//     partial specialization).
	// 2.  If T is an array type, then call_traits defines `value_type` as a
	//     "constant pointer to type" rather than an "array of type"
	//     (requires partial specialization). Note that if you are using
	//     value_type as a stored value then this will result in storing a
	//     "constant pointer to an array" rather than the array itself. This
	//     may or may not be a good thing depending upon what you actually
	//     need (in other words take care!).
	// 3.  If T is a small built in type or a pointer, then `param_type` is
	//     defined as `T const`, instead of `T         const&`. This can
	//     improve the ability of the compiler to optimize loops in the body
	//     of the function if they depend upon the passed parameter, the
	//     semantics of the passed parameter is otherwise unchanged
	//     (requires partial specialization).
	template<typename T>
	struct call_traits
	{
	public:
		using value_type = T;
		using reference = T&;
		using const_reference = const T&;
		// using param_type = typename ct_imp<T,
		// 	faiz::is_pointer_v<T>,
		// 	faiz::is_arithmetic_v<T>>::param_type;
		using param_type = meta::if_<is_pointer<T>,
			T const,
			meta::if_<std::is_arithmetic<T>,
				meta::if_c<
					detected_or_t<size_t_<sizeof(void*) + 1>, sizeof_able, T>{}
						// defer sizeof on incomplete type
						<= sizeof(void*),
					T,
					const T&>,
				const T&>>;
	};

	template<typename T>
	struct call_traits<T&>
	{
		using value_type = T&;
		using reference = T&;
		using const_reference = const T&;
		using param_type = T&;
	};

	template<typename T, size_t N>
	struct call_traits<T[N]>
	{
	private:
		using array_type = T[N];

	public:
		using value_type = const T*;
		using reference = array_type&;
		using const_reference = const array_type&;
		using param_type = const T* const;
	};

	template<typename T, size_t N>
	struct call_traits<const T[N]>
	{
	private:
		using array_type = const T[N];

	public:
		using value_type = const T*;
		using reference = array_type&;
		using const_reference = const array_type&;
		using param_type = const T* const;
	};

} // namespace rider::faiz
#endif
