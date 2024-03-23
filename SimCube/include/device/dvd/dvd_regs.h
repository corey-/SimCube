#pragma once

#include <cstdint>

struct DvdCmd0Reg
{
    uint16_t Subcommand2;
    uint8_t  Subcommand1;
    uint8_t  Command;
};

using DvdCmd1Reg = uint32_t;
using DvdCmd2Reg = uint32_t;

using DvdDmaAddrReg   = uint32_t; // TODO: do we care about reserved bits?
using DvdDmaLengthReg = uint32_t; // TODO: do we care about reserved bits?

struct DvdCtrlReg
{
    uint8_t Control;
    uint8_t Padding[3];
};

enum class DvdCtrlRegMode
{
    DMA,
    IMMEDIATE
};

[[nodiscard]] inline DvdCtrlRegMode GetDvdCtrlRegMode(const DvdCtrlReg& reg)
{
    if (reg.Control & 0x2)
        return DvdCtrlRegMode::DMA;

    return DvdCtrlRegMode::IMMEDIATE;
}

[[nodiscard]] inline bool GetDvdCtrlRegTStart(const DvdCtrlReg& reg)
{
    return (reg.Control & 0x1);
}

inline void ClearDvdCtrlRegTStart(DvdCtrlReg& reg)
{
    reg.Control &= ~0x1;
}