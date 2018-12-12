#ifndef TUPLEE
#define TUPLEE
namespace Rider::Faiz
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

} // namespace Rider::Faiz

#endif
