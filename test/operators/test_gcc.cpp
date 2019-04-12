// #include "rider/faiz/math/RelationAlgebra.hpp"
// template< typename T >
// struct foo : private Rider::Math::AbstractAlgebra::RelationAlgebra::totally_ordered< foo< T > >
// {
//    T x, y;
//    explicit constexpr foo( T x, T y )
//       : x{ x }, y{ y } {}
//    friend constexpr bool operator==( const foo& f1, const foo& f2 )
//    {
//       return f1.x == f2.x and f1.y == f2.y;
//    }
//    friend constexpr bool operator<( const foo& f1, const foo& f2 )
//    {
//       return f1.x < f2.x;
//    }
// };
// namespace detail
// {
//    template< size_t N, typename T >
//    struct tight_pair_element
//    {
//    private:
//       T value;
//    };
//    template< typename T1, typename T2 >
//    struct tight_pair_storage
//    {
//       template< typename U1, typename U2 >
//       constexpr tight_pair_storage( U1&& first, U2&& second )
//          : tight_pair_element< 0, T1 >( std::forward< U1 >( first ) ),
//            tight_pair_element< 1, T2 >( std::forward< U2 >( second ) )
//       {
//       }
//    };
// }  // namespace detail
// template< typename T1, typename T2 >
// struct tight_pair
//    : private detail::tight_pair_storage< T1, T2 >,
//      private Rider::Math::AbstractAlgebra::RelationAlgebra::totally_ordered<
//         tight_pair< T1, T2 > >
// {
//    explicit constexpr tight_pair( T1 x, T1 y )
//       : detail::tight_pair_storage< T1, T2 >( x, y ) {}
//    friend constexpr bool operator==( const tight_pair& f1, const tight_pair& f2 )
//    {
//       return f1.elements[ 0 ] == f2.elements[ 0 ] and f1.elements[ 1 ] == f2.elements[ 1 ];
//    }
//    friend constexpr bool operator<( const tight_pair& f1, const tight_pair& f2 )
//    {
//       if( f1.elements[ 0 ] < f2.elements[ 0 ] )
//          return true;
//       else
//          return f1.elements[ 1 ] < f2.elements[ 1 ];
//    }
// };
// int main()
// {
//    constexpr tight_pair< int, int > a( 1, 2 );
//    constexpr tight_pair< int, int > b( 2, 3 );
//    static_assert( a < b );
//    static_assert( b > a );
// }



#include <cstddef>
struct A
{
    constexpr A(std::nullptr_t) {}
};

constexpr bool operator ==(A a1, A a2) noexcept
{
    return true;
}

constexpr bool operator ==(A a, std::nullptr_t) noexcept
{
    return true;
}

constexpr bool operator ==(std::nullptr_t, A a) noexcept
{
    return true;
}

int main()
{}
