#pragma once

#include <array>

#include <device/gekko/gekko_regs.h>

class GekkoCpu
{
public:
    void HardReset();

private:
    std::array<GekkoGprReg, 32> mGenRegArr{};
    std::array<GekkoFprReg, 32> mFloatRegArr{};
    GekkoFcsrReg mFcsrReg{};
    GekkoConditionReg mCondReg{};
    GekkoExceptionReg mXerReg{};
    GekkoCountReg mCountReg{};
};
