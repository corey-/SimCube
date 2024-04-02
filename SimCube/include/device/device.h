#pragma once

#include <memory>

#include <device/msg/device_msg.h>

class IDevice
{
public:
    virtual ~IDevice() = default;

    virtual void ConnectDevice(std::weak_ptr<IDevice> device) = 0;

    virtual bool ConsumeReadMessage(const DeviceReadMsg& msg)   = 0;
    virtual bool ConsumeWriteMessage(const DeviceWriteMsg& msg) = 0;
};
