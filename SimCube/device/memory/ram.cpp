#include <device/memory/ram.h>

bool Ram::ConsumeReadMessage(const DeviceReadMsg& msg)
{
    if (msg.Address < mBaseAddr)
        return false;

    const auto relAddr = (msg.Address & mAddrMask) - mBaseAddr;
    if (relAddr < mRam.size())
    {
        memcpy_s(msg.Buffer.data(), msg.Buffer.size(), &mRam[relAddr], msg.Count);
        return true;
    }

    return false;
}

bool Ram::ConsumeWriteMessage(const DeviceWriteMsg& msg)
{
    if (msg.Address < mBaseAddr)
        return false;

    const auto relAddr = (msg.Address & mAddrMask) - mBaseAddr;
    if (relAddr < mRam.size())
    {
        memcpy_s(&mRam[relAddr], mRam.size() - relAddr, msg.Buffer.data(), msg.Count);
        return true;
    }

    return false;
}
