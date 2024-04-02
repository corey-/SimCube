#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <span>
#include <string>

struct GameDiscHeader
{
    uint32_t    GameCode;
    std::string GameName;

    uint32_t MainExeOffset;

    // TODO: several other fields
};

class IGameDisc
{
public:
    virtual ~IGameDisc() = default;

    virtual void Load() = 0;

    [[nodiscard]] virtual std::string GetGameName() const = 0;

    [[nodiscard]] virtual uint32_t GetGameExeOffset() const = 0;

    [[nodiscard]] virtual std::span<uint8_t> ReadData(size_t offset, size_t count) = 0;
};

[[nodiscard]] std::shared_ptr<IGameDisc> CreateDisc(const std::filesystem::path& discPath);