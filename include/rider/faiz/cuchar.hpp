#ifndef CUCHAR
#define CUCHAR
#include "rider/faiz/cstddef.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"

namespace rider::faiz
{
    struct mbstate_t
    {
        int __count;
        union
        {
            __WINT_TYPE__ __wch;
            char __wchb[4];
        } __value; /* Value so far.  */
    };

} // namespace rider::faiz
#endif
