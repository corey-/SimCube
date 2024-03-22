#include <disc/ciso_disc.h>
#include <disc/game_disc.h>

std::shared_ptr<IGameDisc> CreateDisc(const std::filesystem::path& discPath)
{
    if (discPath.extension() == ".ciso")
    {
        return std::make_shared<CisoGameDisc>(discPath);
    }

    // Other formats here

    return nullptr;
}
