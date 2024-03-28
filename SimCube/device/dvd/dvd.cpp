#include <device/dvd/dvd.h>
#include <logging/logger.h>
#include <util/alignment.h>

constexpr addr_t DVD_REG_STATUS     = 0xCC006000;
constexpr addr_t DVD_REG_STATUS2    = 0xCC006004;
constexpr addr_t DVD_REG_CMD0       = 0xCC006008;
constexpr addr_t DVD_REG_CMD1       = 0xCC00600C;
constexpr addr_t DVD_REG_CMD2       = 0xCC006010;
constexpr addr_t DVD_REG_DMA_ADDR   = 0xCC006014;
constexpr addr_t DVD_REG_DMA_LENGTH = 0xCC006018;
constexpr addr_t DVD_REG_CTRL       = 0xCC00601C;

static const LoggerInfo LOGGER{ .SourceName = "DvdReader" };

bool DvdReader::ConsumeReadMessage(const DeviceReadMsg& msg)
{
    // All DVD regs 4 bytes
    const auto bytesToRead = std::min(msg.Count, 4ull);

    switch (msg.Address)
    {
    case DVD_REG_CMD0:
        memcpy_s(msg.Buffer.data(), msg.Buffer.size_bytes(), &mCmd0Reg, bytesToRead);
        return true;
    case DVD_REG_CMD1:
        memcpy_s(msg.Buffer.data(), msg.Buffer.size_bytes(), &mCmd1Reg, bytesToRead);
        return true;
    case DVD_REG_CMD2:
        memcpy_s(msg.Buffer.data(), msg.Buffer.size_bytes(), &mCmd2Reg, bytesToRead);
        return true;
    case DVD_REG_DMA_ADDR:
        memcpy_s(msg.Buffer.data(), msg.Buffer.size_bytes(), &mDmaAddrReg, bytesToRead);
        return true;
    case DVD_REG_DMA_LENGTH:
        memcpy_s(msg.Buffer.data(), msg.Buffer.size_bytes(), &mDmaLengthReg, bytesToRead);
        return true;
    case DVD_REG_CTRL:
        memcpy_s(msg.Buffer.data(), msg.Buffer.size_bytes(), &mCtrlReg, bytesToRead);
        return true;
    default:
        return false;
    }
}

bool DvdReader::ConsumeWriteMessage(const DeviceWriteMsg& msg)
{
    // All DVD regs 4 bytes
    const auto bytesToWrite = std::min(msg.Count, 4ull);

    switch (msg.Address)
    {
    case DVD_REG_CMD0:
        memcpy_s(&mCmd0Reg, sizeof(mCmd0Reg), msg.Buffer.data(), bytesToWrite);
        return true;
    case DVD_REG_CMD1:
        memcpy_s(&mCmd1Reg, sizeof(mCmd1Reg), msg.Buffer.data(), bytesToWrite);
        return true;
    case DVD_REG_CMD2:
        memcpy_s(&mCmd2Reg, sizeof(mCmd2Reg), msg.Buffer.data(), bytesToWrite);
        return true;
    case DVD_REG_DMA_ADDR:
        memcpy_s(&mDmaAddrReg, sizeof(mDmaAddrReg), msg.Buffer.data(), bytesToWrite);
        return true;
    case DVD_REG_DMA_LENGTH:
        memcpy_s(&mDmaLengthReg, sizeof(mDmaLengthReg), msg.Buffer.data(), bytesToWrite);
        return true;
    case DVD_REG_CTRL:
        memcpy_s(&mCtrlReg, sizeof(mCtrlReg), msg.Buffer.data(), bytesToWrite);
        PostWriteControlReg();
        return true;
    default:
        return false;
    }
}

void DvdReader::PostWriteControlReg()
{
    if (GetDvdCtrlRegTStart(mCtrlReg))
    {
        ClearDvdCtrlRegTStart(mCtrlReg);

        // TODO: fill in other commands. Just testing DMA read sector for now
        if ((mCmd0Reg.Command == 0xA8) && (mCmd0Reg.Subcommand2 == 0x00))
        {
            DmaReadSector();
            return;
        }
#ifndef NDEBUG
        Log(LOGGER, LogLevel::ERROR, "Unimplemented command {}/{}/{}", mCmd0Reg.Command, mCmd0Reg.Subcommand1,
            mCmd0Reg.Subcommand2);
        std::exit(-1);
#endif
    }
}

void DvdReader::DmaReadSector()
{
    const auto discPos = (mCmd1Reg << 2);
    const auto discLen = mCmd2Reg;

    DeviceWriteMsg wrMsg{ .Address = mDmaAddrReg,   //
                          .Count   = mDmaLengthReg, //
                          .Buffer  = mDisc->ReadData(discPos, discLen) };

    // TODO: send message, for now print
    DevLog(LOGGER, LogLevel::INFO, "DMA Read Sector: {}:{} -> {}:{}", discPos, discLen, mDmaAddrReg, mDmaLengthReg);
}
