#pragma once

#include <vector>

#include <device/device.h>

class Ram : public IDevice
{
public:
    Ram(addr_t baseAddr, size_t memSizeBytes, addr_t addrMask = 0xFFFFFFF) :
        mBaseAddr{ baseAddr },
        mAddrMask{ addrMask },
        mRam(memSizeBytes)
    {
    }

    bool ConsumeReadMessage(const DeviceReadMsg& msg) override;
    bool ConsumeWriteMessage(const DeviceWriteMsg& msg) override;

private:
    addr_t               mBaseAddr;
    addr_t               mAddrMask;
    std::vector<uint8_t> mRam;
};
