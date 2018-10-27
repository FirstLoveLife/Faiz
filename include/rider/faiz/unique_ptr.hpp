#ifndef UNIQUE_PTR
#define UNIQUE_PTR
#include "type_traits.hpp"
namespace rider
{
    // faiz is a standard library like library
    namespace faiz
    {
        // default_delete is the default destruction policy used by
        // unique_ptr when no deleter is specified.
        // 1) The non-specialized default_delete uses delete to deallocate
        // memory for a single object.
        // 2) A partial specialization for array
        // types that uses delete[] is also provided.
        template<class T>
        struct default_delete
        {
            static_assert(!is_function<T>::value,
                "Oops, default_delete cannot be instantiated for function "
                "types, please give me a new type");

            // Constructs a default_delete object.
            constexpr default_delete() noexcept = default;

            // Constructs a default_delete object from another
            // default_delete object. This constructor will only
            // participate in overload resolution if U* is implicitly
            // convertible to T*.
            template<class U>
            default_delete(
                const default_delete<U>&, typename enable_if<is_convertible<U*, T*>::value>::type* = 0) noexcept;

            // At the point in the code the operator() is called, the type must
            // be complete. In some implementations a static_assert is used to
            // make sure this is the case. The reason for this requirement is
            // that calling delete on an incomplete type is undefined behavior
            // in C++ if the complete class type has a nontrivial destructor or
            // a deallocation function, as the compiler has no way of knowing
            // whether such functions exist and must be invoked.
            void operator()(T* ptr) const noexcept
            {
                static_assert(sizeof(T) > 0, "Oops, default_delete can not delete incomplete type");
                static_assert(!is_void<T>::value, "Oops, default_delete can not delete incomplete type");
                delete ptr;
            }
        };

        // default_delete is the default destruction policy used by
        // unique_ptr when no deleter is specified.
        // 1) The non-specialized default_delete uses delete to deallocate
        // memory for a single object.
        // 2) A partial specialization for array
        // types that uses delete[] is also provided.
        template<class T>
        struct default_delete<T[]>
        {
        private:
            // helper struct for default_delete<T[]>
            template<class U>
            struct EnableIfConvertible : enable_if<is_convertible<U (*)[], T (*)[]>::value>
            {
            };

        public:
            // Constructs a default_delete object.
            constexpr default_delete() noexcept = default;

            // Constructs a default_delete<U[]> object from another
            // default_delete<U[]> object. This constructor will only
            // participate in overload resolution if U(*)[] is implicitly
            // convertible to T(*)[].
            template<class U>
            default_delete(const default_delete<U[]>&, typename EnableIfConvertible<U>::type* = 0) noexcept
            {
            }

            //  At the point in the code the operator() is called, the type must
            // be complete. In some implementations a static_assert is used to
            // make sure this is the case. The reason for this requirement is
            // that calling delete on an incomplete type is undefined behavior
            // in C++ if the complete class type has a nontrivial destructor or
            // a deallocation function, as the compiler has no way of knowing
            // whether such functions exist and must be invoked.
            template<class U>
            typename EnableIfConvertible<U>::type operator()(U* ptr) const noexcept
            {
                static_assert(sizeof(T) > 0, "default_delete can not delete incomplete type");
                static_assert(!is_void<T>::value, "default_delete can not delete void type");
                delete[] ptr;
            }
        };

        // "I'm returning a managed resource to you. If you don't explicitly
        // capture the return value, then the resource will be cleaned up. If
        // you do, then you now have exclusive ownership of that resource."
        //
        // unique_ptr is a smart pointer that owns and manages another
        // object through a pointer and disposes of that object when the
        // unique_ptr goes out of scope.
        //```c++
        // unique_ptr<T> myPtr(new T);       // Okay
        // unique_ptr<T> myOtherPtr = myPtr; // Error: Can't copy unique_ptr
        //
        // unique_ptr<T> myPtr(new T);                  // Okay
        // unique_ptr<T> myOtherPtr = move(myPtr); // Okay, resource now
        // stored in myOtherPtr
        //```
        // Similarly, you can do something like this:
        // ```c++
        // unique_ptr<T> MyFunction() {
        //     unique_ptr<T> myPtr(/* ... */);
        //     /* ... */
        //     return myPtr;
        // }
        //```
        //
        // for custom deleter, just With:
        //```c++
        // unique_ptr<Bar, decltype(&destroy)> ptr_;
        //```
        /* https://stackoverflow.com/a/6089065/6949852:
        Complete type requirements for
        //                           unique_ptr and shared_ptr
        +------------------------+---------------+---------------+
        |          P()           |      I        |      I        |
        |  default constructor   |               |               |
        +------------------------+---------------+---------------+
        |      P(const P&)       |     N/A       |      I        |
        |    copy constructor    |               |               |
        +------------------------+---------------+---------------+
        |         P(P&&)         |      I        |      I        |
        |    move constructor    |               |               |
        +------------------------+---------------+---------------+
        |         ~P()           |      C        |      I        |
        |       destructor       |               |               |
        +------------------------+---------------+---------------+
        |         P(A*)          |      I        |      C        |
        +------------------------+---------------+---------------+
        |  operator=(const P&)   |     N/A       |      I        |
        |    copy assignment     |               |               |
        +------------------------+---------------+---------------+
        |    operator=(P&&)      |      C        |      I        |
        |    move assignment     |               |               |
        +------------------------+---------------+---------------+
        |        reset()         |      C        |      I        |
        +------------------------+---------------+---------------+
        |       reset(A*)        |      C        |      C        |
        +------------------------+---------------+---------------+
        */

        template<typename T, class Deleter = default_delete<T>>
        class unique_ptr
        {

        private:
            template<typename T_, typename Deleter_>
            class unique_ptr_helper
            {
                template<typename U>
                static typename U::pointer test(typename U::pointer*);

                template<typename U>
                static T_* test(...);

            public:
                using type = decltype(test<typename remove_reference<Deleter_>::type>(0));
            };

        public:
            // Deleter, the function object or lvalue reference to function or
            // to function object, to be called from the destructor
            using deleter_type = Deleter;
            // A synonym for the template parameter T.
            using element_type = T;
            // A synonym for `Delete::pointer` if defined, otherwise `T *`.
            // IOW, `std::remove_reference<Deleter>::type::pointer` if that type
            // exists, otherwise `T*`. Must satisfy *NullablePointer*
            using pointer = typename unique_ptr_helper<element_type, deleter_type>::type;

            static_assert(!is_rvalue_reference<deleter_type>::value, "Come on, rvalue_reference is not what I want");

        protected:
            pointer ptr;

        public:
            constexpr unique_ptr() noexcept : ptr{pointer{}}
            {
                static_assert(!is_pointer<deleter_type>::value, "WTF, you give me a pointer type? Please not!");
            }

            constexpr unique_ptr(std::nullptr_t) noexcept : ptr{}
            {
                static_assert(!is_pointer<deleter_type>::value, "WTF, you give me a pointer type? Please not!");
            }

            explicit unique_ptr(pointer ptr) noexcept : ptr{ptr}
            {
                static_assert(!is_pointer<deleter_type>::value, "WTF, you give me a pointer type? Please not!");
            }
            template<class U, class E>
            unique_ptr(unique_ptr<U, E>&& u) noexcept
            {
            }
            Deleter& get_deleter() noexcept;
            const Deleter& get_deleter() const noexcept
            {
                return Deleter();
            }

        }; // class unique_ptr
    } // namespace faiz
} // namespace rider
#endif
