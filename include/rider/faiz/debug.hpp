#ifndef CASSERT
#define CASSERT
#include <iostream>
namespace rider
{
// Terminate the program if the precondition `e` evaluates
// to false.
#define Expects(e) assert(e)

// Terminates the program if the postcondition `e` evalautes
// to false.
#define Ensures(e) assert(e)


} // namespace rider
#endif
