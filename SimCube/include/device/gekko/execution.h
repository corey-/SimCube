#pragma once

#include <device/gekko/power_pc_tables.h>
#include <util/types.h>

class IExecutionEngine : public IPpcInstructionDecodeHandler
{
public:
    virtual ~IExecutionEngine() = default;

    // Returns number of instructions executed
    virtual size_t Execute(addr_t startingPc, size_t targetInstrCount) = 0;
};
