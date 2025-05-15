#define memcpy_s memcpy_shim

// shim to fix compiler bug
// See https://developercommunity.visualstudio.com/t/Modules-exporting-CWinRT-objects-enco/10527223
// https://developercommunity.visualstudio.com/t/Visual-Studio-cant-find-time-function/1126857
int memcpy_shim(
    void* const _Destination,
    size_t     const _DestinationSize,
    void const* const _Source,
    size_t     const _SourceSize
);
