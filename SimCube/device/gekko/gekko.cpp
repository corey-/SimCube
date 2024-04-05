#include <device/gekko/gekko.h>
#include <util/endian.h>

void GekkoCpu::HardReset()
{
    for (auto& reg : mGenRegArr)
        reg = 0;
    for (auto& reg : mFloatRegArr)
        reg = 0;

    mFcsrReg.Val  = 0;
    mCondReg.Val  = 0;
    mXerReg.Val   = 0;
    mLinkReg.Val  = 0;
    mCountReg.Val = 0;
}

std::vector<PpcOpcode> GekkoCpu::InstructionFetch(addr_t pc)
{
    static constexpr auto INSTRUCTION_FETCH_COUNT = 8;

    std::vector<PpcOpcode> instBuffer(INSTRUCTION_FETCH_COUNT, -1);

    auto ptr = reinterpret_cast<uint8_t*>(instBuffer.data());

    DeviceReadMsg msg{ .Address = pc,
                       .Count   = 8,
                       .Buffer  = std::span{ ptr, INSTRUCTION_FETCH_COUNT * sizeof(PpcOpcode) } };

    for (auto* device : mDeviceConnections)
    {
        if (device->ConsumeReadMessage(msg))
        {
            for (auto& opcode : instBuffer)
            {
                opcode = ReadBigEndianU32(std::span<uint8_t, 4>(reinterpret_cast<uint8_t*>(&opcode), 4));
            }
            break;
        }
    }

    return instBuffer;
}
