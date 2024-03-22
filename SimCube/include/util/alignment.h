#pragma once

#include <cstdint>

template <typename T>
constexpr bool IsAligned(T value, unsigned alignmentBytes)
{
    return (value % alignmentBytes) == 0;
}

inline bool IsAligned(void* value, unsigned alignmentBytes)
{
    return IsAligned(reinterpret_cast<uintptr_t>(value), alignmentBytes);
}
