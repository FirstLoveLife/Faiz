#ifndef UTILITY
#define UTILITY
#include "rider/faiz/faiz_fwd.hpp"
#include "rider/faiz/macros.hpp"
#include "rider/faiz/type_traits.hpp"
namespace Rider::Faiz
{
	//  **Faiz::move** is used to indicate that an object t may be "moved
	//  from",
	// i.e. allowing the efficient transfer of resources from t to another
	// object. In particular, **Faiz::move** produces an xvalue expression
	// that identifies its argument t. It is exactly equivalent to a
	// **static_cast** to an rvalue reference type.
	// // Simple move constructor
	// ```cpp
	// A(A&& arg) : member(Faiz::move(arg.member)) // the expression
	// "arg.member" is lvalue
	// {}
	// // Simple move assignment operator
	// A& operator=(A&& other) {
	//      member = Faiz::move(other.member);
	//      return *this;
	// }
	// ```
	Tpl<class T> cfn
	move(T&& t) noexcept->remove_reference_t<T>&&
	{
		using U = remove_reference_t<T>;
		return static_cast<U&&>(t);
	}

	// syntactic sugar of *static_cast<T&&>(t)*
	//
	// **T&   &  -> T&**  // lvalue reference to cv TR -> lvalue reference
	// to T
	// **T&   && -> T&**  // rvalue reference to cv TR -> TR (lvalue
	// reference to T)
	// **T&&  &  -> T&**  // lvalue reference to cv TR -> lvalue reference
	// to T
	// **T&&  && -> T&&** // rvalue reference to cv TR -> TR (rvalue
	// reference to T)
	//
	// If it was an lvalue, we have an lvalue-reference parameter, otherwise
	// we have an rvalue-reference parameter. So, we never need multiple
	// overload functions.
	//
	// ```cpp
	// Tpl<class T>
	// void wrapper(T&& arg)
	// {
	//     // arg is always lvalue
	//     foo(Faiz::forward<T>(arg)); // Forward as lvalue or as rvalue,
	//     depending on T
	// }
	// ```
	// * If a call to **wrapper()** passes an rvalue **Faiz::string**, then
	// **T** is deduced to
	// **Faiz::string** (not **Faiz::string&**, **const Faiz::string&**, or
	// **Faiz::string&&**), and
	// **Faiz::forward** ensures that an rvalue reference is passed to foo.
	//
	// * If a call to **wrapper()** passes a const lvalue **Faiz::string**,
	// then T is deduced to
	// **const Faiz::string**&, and **Faiz::forward** ensures that a const
	// lvalue reference is passed to foo.
	//
	// * If a call to **wrapper()** passes a non-const lvalue
	// **Faiz::string**, then T is deduced to
	// **Faiz::string**&, and **Faiz::forward** ensures that a non-const
	// lvalue reference is passed to foo.
	Tpl<class T> cfn
	forward(remove_reference_t<T>& t) noexcept->T&&
	{
		return static_cast<T&&>(t);
	}

	// syntactic sugar of *static_cast<T&&>(t)*
	//
	// **T&   &  -> T&**  // lvalue reference to cv TR -> lvalue reference
	// to T
	// **T&   && -> T&**  // rvalue reference to cv TR -> TR (lvalue
	// reference to T)
	// **T&&  &  -> T&**  // lvalue reference to cv TR -> lvalue reference
	// to T
	// **T&&  && -> T&&** // rvalue reference to cv TR -> TR (rvalue
	// reference to T)
	//
	// If it was an lvalue, we have an lvalue-reference parameter, otherwise
	// we have an rvalue-reference parameter. So, we never need multiple
	// overload functions.
	//
	// ```cpp
	// Tpl<class T>
	// void wrapper(T&& arg)
	// {
	//     // arg is always lvalue
	//     foo(Faiz::forward<T>(arg)); // Forward as lvalue or as rvalue,
	//     depending on T
	// }
	// ```
	// * If a call to **wrapper()** passes an rvalue **Faiz::string**, then
	// **T** is deduced to
	// **Faiz::string** (not **Faiz::string&**, **const Faiz::string&**, or
	// **Faiz::string&&**), and
	// **Faiz::forward** ensures that an rvalue reference is passed to foo.
	//
	// * If a call to **wrapper()** passes a const lvalue **Faiz::string**,
	// then T is deduced to
	// **const Faiz::string**&, and **Faiz::forward** ensures that a const
	// lvalue reference is passed to foo.
	//
	// * If a call to **wrapper()** passes a non-const lvalue
	// **Faiz::string**, then T is deduced to
	// **Faiz::string**&, and **Faiz::forward** ensures that a non-const
	// lvalue reference is passed to foo.
	Tpl<class T> cfn
	forward(remove_reference_t<T>&& t) noexcept->T&&
	{
		static_assert(not_lvalue_reference_v<T>,
			"can not forward an rvalue as an lvalue");
		return static_cast<T&&>(t);
	}

	// Obtains the actual address of the object or function arg, even in
	// presence of overloaded operator&
	Tpl<Typ T> cfn
	AddressOf(T& v) noexcept->T*
	{
		return reinterpret_cast<T*>(
			const_cast<char*>(reinterpret_cast<const volatile char(&)[1]>(v)));
	}

	// Rvalue overload is deleted to prevent taking the address of const
	// rvalues.
	Tpl<class T> const T*
	addressof(const T&&)
		= delete;

} // namespace Rider::Faiz
#endif
