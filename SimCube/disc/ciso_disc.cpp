#include <disc/ciso_disc.h>

#include <fstream>

#include <logging/logger.h>
#include <util/endian.h>

static const LoggerInfo LOGGER{ .SourceName = "CisoGameDisc" };

void CisoGameDisc::Load()
{
    const auto fileSize = file_size(mGamePath);
    mData.resize(fileSize);

    DevLog(LOGGER, LogLevel::INFO, "Loading {} bytes from {}", fileSize, mGamePath.string());

    std::ifstream ifs(mGamePath, std::ifstream::binary);
    ifs.read(reinterpret_cast<char*>(mData.data()), mData.size());
    mDataSpan = std::span(mData);
}

std::string CisoGameDisc::GetGameName() const
{
    const auto name = mDataSpan.subspan<0x8020, 0x3e0>();
    return std::string(name.begin(), name.end());
}

uint32_t CisoGameDisc::GetGameExeOffset() const
{
    const auto offset = mDataSpan.subspan<0x420, 4>();
    return ReadBigEndianU32(offset);
}
