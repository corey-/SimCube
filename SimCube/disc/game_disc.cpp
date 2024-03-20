#include <disc/ciso_disc.h>
#include <disc/game_disc.h>

std::unique_ptr<IGameDisc> CreateDisc(const std::filesystem::path& discPath)
{
    if (discPath.extension() == ".ciso")
    {
        return std::make_unique<CisoGameDisc>(discPath);
    }

    // Other formats here

    return nullptr;
}
