#include <type_traits>
#include "rider/faiz/type_traits.hpp"
#include "../test_macros.h"

template <class T>
void test_is_nothrow_constructible()
{
    static_assert(( Rider::Faiz::is_nothrow_constructible<T>::value), "");
    static_assert(( Rider::Faiz::is_nothrow_constructible_v<T>), "");
}

template <class T, class A0>
void test_is_nothrow_constructible()
{
    static_assert(( Rider::Faiz::is_nothrow_constructible<T, A0>::value), "");
    static_assert(( Rider::Faiz::is_nothrow_constructible_v<T, A0>), "");
}

template <class T>
void test_is_not_nothrow_constructible()
{
    static_assert((!Rider::Faiz::is_nothrow_constructible<T>::value), "");
    static_assert((!Rider::Faiz::is_nothrow_constructible_v<T>), "");
}

template <class T, class A0>
void test_is_not_nothrow_constructible()
{
    static_assert((!Rider::Faiz::is_nothrow_constructible<T, A0>::value), "");
    static_assert((!Rider::Faiz::is_nothrow_constructible_v<T, A0>), "");
}

template <class T, class A0, class A1>
void test_is_not_nothrow_constructible()
{
    static_assert((!Rider::Faiz::is_nothrow_constructible<T, A0, A1>::value), "");
    static_assert((!Rider::Faiz::is_nothrow_constructible_v<T, A0, A1>), "");
}

class Empty
{
};

class NotEmpty
{
    virtual ~NotEmpty();
};

union Union {};

struct bit_zero
{
    int :  0;
};

class Abstract
{
    virtual ~Abstract() = 0;
};

struct A
{
    A(const A&);
};

struct C
{
    C(C&);  // not const
    void operator=(C&);  // not const
};

struct Tuple {
    Tuple(Empty&&) noexcept {}
};

int main()
{
    test_is_nothrow_constructible<int> ();
    test_is_nothrow_constructible<int, const int&> ();
    test_is_nothrow_constructible<Empty> ();
    test_is_nothrow_constructible<Empty, const Empty&> ();

    test_is_not_nothrow_constructible<A, int> ();
    test_is_not_nothrow_constructible<A, int, double> ();
    test_is_not_nothrow_constructible<A> ();
    test_is_not_nothrow_constructible<C> ();
    test_is_nothrow_constructible<Tuple &&, Empty> (); // See bug #19616.

    static_assert(!Rider::Faiz::is_constructible<Tuple&, Empty>::value, "");
    test_is_not_nothrow_constructible<Tuple &, Empty> (); // See bug #19616.
}
