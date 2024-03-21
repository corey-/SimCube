#pragma once

#include <cstdint>

using GekkoGprReg = uint32_t;
using GekkoFprReg = uint64_t;

struct GekkoConditionReg
{
    uint32_t Val;
};

struct GekkoFcsrReg
{
    uint32_t Val;
};

struct GekkoExceptionReg
{
    uint32_t Val;
};

struct GekkoLinkReg
{
    uint32_t Val;
};

struct GekkoCountReg
{
    uint32_t Val;
};
