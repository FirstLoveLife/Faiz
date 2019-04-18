#ifndef CASSERT
#define CASSERT
#include "rider/faiz/macros.hpp"
#include <cassert>
#include <iostream>

// Terminate the program if the precondition `e` evaluates
// to false.
#define Expects(e) assert(e)

// Terminates the program if the postcondition `e` evalautes
// to false.
#define Ensures(e) assert(e)

#endif
