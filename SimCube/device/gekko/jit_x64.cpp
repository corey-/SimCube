#include <device/gekko/gekko.h>
#include <device/gekko/jit_x64.h>
#include <device/gekko/power_pc_tables.h>

size_t X64JitEngine::Execute(addr_t startingPc, size_t targetInstrCount)
{
    for (const auto opcode : mCpu->InstructionFetch(startingPc))
    {
        auto decodeResultOpt = PpcDecode(opcode);
        if (decodeResultOpt.has_value())
            decodeResultOpt->Handler(*this);
    }

    return -1;
}
