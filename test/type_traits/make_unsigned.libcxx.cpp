 #include "../test_macros.h"
 #include "rider/faiz/type_traits.hpp"
 using Rider::Faiz::make_unsigned;
 using Rider::Faiz::make_unsigned_t;
 enum Enum
 {
     zero,
     one_
 };

 enum BigEnum : unsigned long long // MSVC's ABI doesn't follow the Standard
 {
     bigzero,
     big = 0xFFFFFFFFFFFFFFFFULL
 };

 // #if !defined(_LIBCPP_HAS_NO_INT128) && !defined(_LIBCPP_HAS_NO_STRONG_ENUMS)
 // enum HugeEnum : __int128_t
 // {
 // 	hugezero
 // };
 // #endif

 template<class T, class U>
 void
 test_make_unsigned()
 {
     static_assert(
         (Rider::Faiz::is_same<typename make_unsigned<T>::type, U>::value), "");
     static_assert((Rider::Faiz::is_same<make_unsigned_t<T>, U>::value), "");
 }

 int
 main()
 {
     test_make_unsigned<signed char, unsigned char>();
     test_make_unsigned<unsigned char, unsigned char>();
     test_make_unsigned<char, unsigned char>();
     test_make_unsigned<short, unsigned short>();
     test_make_unsigned<unsigned short, unsigned short>();
     test_make_unsigned<int, unsigned int>();
     test_make_unsigned<unsigned int, unsigned int>();
     test_make_unsigned<long, unsigned long>();
     test_make_unsigned<unsigned long, unsigned long>();
     test_make_unsigned<long long, unsigned long long>();
     test_make_unsigned<unsigned long long, unsigned long long>();
     test_make_unsigned<wchar_t,
         Rider::Faiz::conditional<sizeof(wchar_t) == 4,
             unsigned int,
             unsigned short>::type>();
     test_make_unsigned<const wchar_t,
         Rider::Faiz::conditional<sizeof(wchar_t) == 4,
             const unsigned int,
             const unsigned short>::type>();
     test_make_unsigned<const Enum,
         Rider::Faiz::conditional<sizeof(Enum) == sizeof(int),
             const unsigned int,
             const unsigned char>::type>();
     test_make_unsigned<BigEnum,
         Rider::Faiz::conditional<sizeof(long) == 4,
             unsigned long long,
             unsigned long>::type>();
 }
