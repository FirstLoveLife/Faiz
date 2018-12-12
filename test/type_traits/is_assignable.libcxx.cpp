 //===----------------------------------------------------------------------===//
 //
 //                     The LLVM Compiler Infrastructure
 //
 // This file is dual licensed under the MIT and the University of Illinois Open
 // Source Licenses. See LICENSE.TXT for details.
 //
 //===----------------------------------------------------------------------===//

 // type_traits

 // is_assignable

 #include "../test_macros.h"
 #include "rider/faiz/type_traits.hpp"

 struct A
 {};

 struct B
 {
     void operator=(A);
 };

 template<class T, class U>
 void
 test_is_assignable()
 {
     static_assert((Rider::Faiz::is_assignable<T, U>::value), "");
     static_assert(Rider::Faiz::is_assignable_v<T, U>, "");
 }

 template<class T, class U>
 void
 test_is_not_assignable()
 {
     static_assert((!Rider::Faiz::is_assignable<T, U>::value), "");
     static_assert(!Rider::Faiz::is_assignable_v<T, U>, "");
 }

 struct D;

 struct C
 {
     template <class U>
    D operator,(U&&);
 };

 struct E
 {
     C
     operator=(int);
 };

 template<typename T>
 struct X
 {
     T t;
 };

 int
 main()
 {
     test_is_assignable<int&, int&>();
     test_is_assignable<int&, int>();
     test_is_assignable<int&, double>();
     test_is_assignable<B, A>();
     test_is_assignable<void*&, void*>();

     test_is_assignable<E, int>();

     test_is_not_assignable<int, int&>();
     test_is_not_assignable<int, int>();
     test_is_not_assignable<A, B>();
     test_is_not_assignable<void, const void>();
     test_is_not_assignable<const void, const void>();
     test_is_not_assignable<int(), int>();

     //  pointer to incomplete template type
     test_is_assignable<X<D>*&, X<D>*>();
 }
