#ifndef UTILITY
#define UTILITY
#include "type_traits.hpp"
namespace rider
{
    namespace faiz
    {
        //  **faiz::move** is used to indicate that an object t may be "moved from",
        // i.e. allowing the efficient transfer of resources from t to another object.
        // In particular, **faiz::move** produces an xvalue expression that identifies its argument t.
        // It is exactly equivalent to a **static_cast** to an rvalue reference type.
        // // Simple move constructor
        // ```cpp
        // A(A&& arg) : member(std::move(arg.member)) // the expression "arg.member" is lvalue
        // {}
        // // Simple move assignment operator
        // A& operator=(A&& other) {
        //      member = std::move(other.member);
        //      return *this;
        // }
        // ```
        template<class T>
        inline typename faiz::remove_reference<T>::type&& move(T&& t) noexcept
        {
            typedef typename faiz::remove_reference<T>::type U;
            return static_cast<U&&>(t);
        }

        // syntactic sugar of *static_cast<T&&>(t)*
        //
        // **T&   &  -> T&**  // lvalue reference to cv TR -> lvalue reference to T
        // **T&   && -> T&**  // rvalue reference to cv TR -> TR (lvalue reference to T)
        // **T&&  &  -> T&**  // lvalue reference to cv TR -> lvalue reference to T
        // **T&&  && -> T&&** // rvalue reference to cv TR -> TR (rvalue reference to T)
        //
        // If it was an lvalue, we have an lvalue-reference parameter, otherwise we have an
        // rvalue-reference parameter. So, we never need multiple overload functions.
        //
        // ```cpp
        // template<class T>
        // void wrapper(T&& arg)
        // {
        //     // arg is always lvalue
        //     foo(faiz::forward<T>(arg)); // Forward as lvalue or as rvalue, depending on T
        // }
        // ```
        // * If a call to **wrapper()** passes an rvalue **faiz::string**, then **T** is deduced to
        // **faiz::string** (not **faiz::string&**, **const faiz::string&**, or **faiz::string&&**), and
        // **faiz::forward** ensures that an rvalue reference is passed to foo.
        //
        // * If a call to **wrapper()** passes a const lvalue **faiz::string**, then T is deduced to
        // **const faiz::string**&, and **faiz::forward** ensures that a const lvalue reference is passed to foo.
        //
        // * If a call to **wrapper()** passes a non-const lvalue **faiz::string**, then T is deduced to
        // **faiz::string**&, and **faiz::forward** ensures that a non-const lvalue reference is passed to foo.
        template<class T>
        inline constexpr T&& forward(typename faiz::remove_reference<T>::type& t) noexcept
        {
            return static_cast<T&&>(t);
        }

        // syntactic sugar of *static_cast<T&&>(t)*
        //
        // **T&   &  -> T&**  // lvalue reference to cv TR -> lvalue reference to T
        // **T&   && -> T&**  // rvalue reference to cv TR -> TR (lvalue reference to T)
        // **T&&  &  -> T&**  // lvalue reference to cv TR -> lvalue reference to T
        // **T&&  && -> T&&** // rvalue reference to cv TR -> TR (rvalue reference to T)
        //
        // If it was an lvalue, we have an lvalue-reference parameter, otherwise we have an
        // rvalue-reference parameter. So, we never need multiple overload functions.
        //
        // ```cpp
        // template<class T>
        // void wrapper(T&& arg)
        // {
        //     // arg is always lvalue
        //     foo(faiz::forward<T>(arg)); // Forward as lvalue or as rvalue, depending on T
        // }
        // ```
        // * If a call to **wrapper()** passes an rvalue **faiz::string**, then **T** is deduced to
        // **faiz::string** (not **faiz::string&**, **const faiz::string&**, or **faiz::string&&**), and
        // **faiz::forward** ensures that an rvalue reference is passed to foo.
        //
        // * If a call to **wrapper()** passes a const lvalue **faiz::string**, then T is deduced to
        // **const faiz::string**&, and **faiz::forward** ensures that a const lvalue reference is passed to foo.
        //
        // * If a call to **wrapper()** passes a non-const lvalue **faiz::string**, then T is deduced to
        // **faiz::string**&, and **faiz::forward** ensures that a non-const lvalue reference is passed to foo.
        template<class T>
        inline constexpr T&& forward(typename faiz::remove_reference<T>::type&& t) noexcept
        {
            static_assert(!faiz::is_lvalue_reference<T>::value, "can not forward an rvalue as an lvalue");
            return static_cast<T&&>(t);
        }
    } // namespace faiz
} // namespace rider
#endif
