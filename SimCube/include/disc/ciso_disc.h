#pragma once

#include <disc/game_disc.h>

#include <span>

class CisoGameDisc : public IGameDisc
{
public:
    explicit CisoGameDisc(std::filesystem::path gamePath) : mGamePath{ std::move(gamePath) } {}

    void Load() override;

    [[nodiscard]] std::string GetGameName() const override;

    [[nodiscard]] uint32_t GetGameExeOffset() const override;

    [[nodiscard]] std::span<uint8_t> ReadData(size_t offset, size_t count) override;

private:
    std::filesystem::path mGamePath;
    std::vector<uint8_t>  mData;
    std::span<uint8_t>    mDataSpan;
};
