#include <memory>

int memcpy_shim(
    void* const destination,
    size_t const destinationSize,
    void const* const source,
    size_t const sourceSize
)
{
    return memcpy_s(destination, destinationSize, source, sourceSize);
}
