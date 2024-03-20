#pragma once

#include <cstdint>
#include <span>

inline uint16_t ReadBigEndianU16(std::span<const uint8_t, 2> data)
{
    return (data[0] << 8) | data[1];
}

inline uint32_t ReadBigEndianU32(std::span<const uint8_t, 4> data)
{
    return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
}
