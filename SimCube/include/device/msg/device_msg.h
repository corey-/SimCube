#pragma once

#include <util/types.h>

#include <cstdint>
#include <span>

struct DeviceReadMsg
{
    vaddr_t            Address;
    size_t             Count;
    std::span<uint8_t> Buffer;
};

struct DeviceWriteMsg
{
    vaddr_t                  Address;
    size_t                   Count;
    std::span<const uint8_t> Buffer;
};
