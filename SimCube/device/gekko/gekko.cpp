#include <device/gekko/gekko.h>

void GekkoCpu::HardReset()
{
    for (auto& reg : mGenRegArr)
        reg = 0;
    for (auto& reg : mFloatRegArr)
        reg = 0;

    mFcsrReg.Val  = 0;
    mCondReg.Val  = 0;
    mXerReg.Val   = 0;
    mCountReg.Val = 0;
}
