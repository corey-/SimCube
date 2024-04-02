#pragma once

#include <array>
#include <vector>

#include <device/device.h>
#include <device/gekko/gekko_regs.h>
#include <device/gekko/power_pc_tables.h>

class GekkoCpu : public IDevice, IPpcInstructionDecodeHandler
{
public:
    void HardReset();

    void SetPc(addr_t address) { mInstructionPointer = address; }

    // IDevice functions
    void ConnectDevice(std::weak_ptr<IDevice> device) override { mDeviceConnections.push_back(std::move(device)); }
    bool ConsumeReadMessage(const DeviceReadMsg& msg) override { return false; }
    bool ConsumeWriteMessage(const DeviceWriteMsg& msg) override { return false; }

private:
    addr_t                      mInstructionPointer{};
    std::array<GekkoGprReg, 32> mGenRegArr{};   // GPRn
    std::array<GekkoFprReg, 32> mFloatRegArr{}; // FPRn
    GekkoFcsrReg                mFcsrReg{};     // FPCSR
    GekkoConditionReg           mCondReg{};     // CR
    GekkoExceptionReg           mXerReg{};      // SPR 1
    GekkoLinkReg                mLinkReg{};     // SPR 8
    GekkoCountReg               mCountReg{};    // SPR 9

    std::vector<std::weak_ptr<IDevice>> mDeviceConnections{};
};
