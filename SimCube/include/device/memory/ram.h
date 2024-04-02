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

    void InitializeRam(addr_t offset, std::span<uint8_t> data);

    void ConnectDevice(std::weak_ptr<IDevice> device) override { mDeviceConnections.push_back(std::move(device)); }
    bool ConsumeReadMessage(const DeviceReadMsg& msg) override;
    bool ConsumeWriteMessage(const DeviceWriteMsg& msg) override;

private:
    addr_t               mBaseAddr;
    addr_t               mAddrMask;
    std::vector<uint8_t> mRam;

    std::vector<std::weak_ptr<IDevice>> mDeviceConnections{};
};
