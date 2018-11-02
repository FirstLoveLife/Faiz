#ifndef TUPLEE
#define TUPLEE
namespace rider::faiz
{
    struct ignore_t
    {
        ignore_t()
        {
        }

        template<typename T>
        const ignore_t& operator=(const T&) const
        {
            return *this;
        }
    };

} // namespace rider::faiz

#endif
