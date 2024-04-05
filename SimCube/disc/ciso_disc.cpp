#include <disc/ciso_disc.h>

#include <fstream>

#include <logging/logger.h>
#include <util/endian.h>

static const LoggerInfo LOGGER{ .SourceName = "CisoGameDisc" };

static constexpr size_t CISO_HEADER_SIZE = 0x8000;

void CisoGameDisc::Load()
{
    if (!exists(mGamePath))
        throw std::runtime_error("File not found");

    if (file_size(mGamePath) <= CISO_HEADER_SIZE)
        throw std::runtime_error("Invalid disc file");

    const auto fileSize = file_size(mGamePath) - CISO_HEADER_SIZE;
    mData.resize(fileSize);

    DevLog(LOGGER, LogLevel::INFO, "Loading {} bytes from {}", fileSize, mGamePath.string());

    std::ifstream ifs(mGamePath, std::ifstream::binary);
    ifs.seekg(CISO_HEADER_SIZE);
    ifs.read(reinterpret_cast<char*>(mData.data()), mData.size());

    mDataSpan = std::span(mData);
}

std::string CisoGameDisc::GetGameName() const
{
    const auto name = mDataSpan.subspan<0x20, 0x3e0>();
    return std::string(name.begin(), name.end());
}

uint32_t CisoGameDisc::GetGameExeOffset() const
{
    const auto offset = mDataSpan.subspan<0x420, 4>();
    return ReadBigEndianU32(offset);
}

std::span<uint8_t> CisoGameDisc::ReadData(size_t offset, size_t count)
{
    return mDataSpan.subspan(offset, count);
}
