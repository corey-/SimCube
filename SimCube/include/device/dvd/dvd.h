#pragma once

#include <device/device.h>
#include <device/dvd/dvd_regs.h>
#include <disc/game_disc.h>

#include <memory>

class DvdReader : public IDevice
{
public:
    void InsertDisc(std::shared_ptr<IGameDisc> disc) { mDisc = std::move(disc); }

    void ConnectDevice(std::weak_ptr<IDevice> device) override { mDeviceConnections.push_back(std::move(device)); }
    bool ConsumeReadMessage(const DeviceReadMsg& msg) override;
    bool ConsumeWriteMessage(const DeviceWriteMsg& msg) override;

private:
    void PostWriteControlReg();

    void DmaReadSector();

private:
    std::shared_ptr<IGameDisc> mDisc;

    DvdCmd0Reg      mCmd0Reg{};
    DvdCmd1Reg      mCmd1Reg{};
    DvdCmd2Reg      mCmd2Reg{};
    DvdDmaAddrReg   mDmaAddrReg{};
    DvdDmaLengthReg mDmaLengthReg{};
    DvdCtrlReg      mCtrlReg{};

    std::vector<std::weak_ptr<IDevice>> mDeviceConnections{};
};
