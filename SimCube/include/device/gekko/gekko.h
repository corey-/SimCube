#pragma once

#include <array>
#include <vector>

#include <device/device.h>
#include <device/gekko/execution.h>
#include <device/gekko/gekko_regs.h>
#include <device/gekko/jit_x64.h>
#include <device/gekko/power_pc_tables.h>


class GekkoCpu : public IDevice
{
public:
    explicit GekkoCpu() : mExecEngine{ std::make_unique<X64JitEngine>(*this) } {}
    void HardReset();

    void SetPc(addr_t address) { mInstructionPointer = address; }

    void Execute() { mExecEngine->Execute(mInstructionPointer, -1); }

    // IDevice functions
    void ConnectDevice(IDevice& device) override { mDeviceConnections.push_back(&device); }
    bool ConsumeReadMessage(const DeviceReadMsg& msg) override { return false; }
    bool ConsumeWriteMessage(const DeviceWriteMsg& msg) override { return false; }

    std::vector<PpcOpcode> InstructionFetch(addr_t pc);

private:
    addr_t                      mInstructionPointer{};
    std::array<GekkoGprReg, 32> mGenRegArr{};   // GPRn
    std::array<GekkoFprReg, 32> mFloatRegArr{}; // FPRn
    GekkoFcsrReg                mFcsrReg{};     // FPCSR
    GekkoConditionReg           mCondReg{};     // CR
    GekkoExceptionReg           mXerReg{};      // SPR 1
    GekkoLinkReg                mLinkReg{};     // SPR 8
    GekkoCountReg               mCountReg{};    // SPR 9

    std::vector<IDevice*>             mDeviceConnections{};
    std::unique_ptr<IExecutionEngine> mExecEngine;
};
