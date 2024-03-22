#pragma once

#include <device/msg/device_msg.h>

class IDevice
{
public:
    virtual ~IDevice() = default;

    virtual bool ConsumeReadMessage(const DeviceReadMsg& msg)   = 0;
    virtual bool ConsumeWriteMessage(const DeviceWriteMsg& msg) = 0;
};
