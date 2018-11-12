#ifndef CASSERT
#define CASSERT
#include <iostream>
namespace rider::faiz
{
    // https://stackoverflow.com/questions/7182972/how-can-i-implement-assert-macro-as-a-method
    class Debug
    {
    public:
        static void assert(bool condition,
            const char* assert,
            const char* message,
            const char* file,
            long line)
        {
            if (condition == false)
            {
                std::cerr << "Assertion `" << assert << "` failed in "
                          << __FILE__ << " line " << __LINE__ << ": " << message
                          << std::endl;
                std::terminate();
            }
        }
#ifdef NDEBUG
#    define myassert(x, message) assert(true, "", "", 0)
#else
#    define assertWithLog(x, message) \
        my_assert(x, #x, message, __FILE__, __LINE__)
#endif
    };
} // namespace rider::faiz
#endif
