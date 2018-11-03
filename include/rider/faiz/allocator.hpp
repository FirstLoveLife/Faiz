#ifndef ALLOCATOR
#define ALLOCATOR
namespace rider::faiz
{

    // The **std::allocator** class template is the default Allocator used by
    // all standard library containers if no user-specified allocator is
    // provided. The default allocator is stateless, that is, all instances of
    // the given allocator are interchangeable, compare equal and can deallocate
    // memory allocated by any other instance of the same allocator type.
    class allocator
    {
    };
} // namespace rider::faiz
#endif
