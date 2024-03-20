#include <iostream>

#include <disc/game_disc.h>

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        if (auto game = CreateDisc({ argv[1] }))
        {
            game->Load();
            std::cout << "Loaded \"" << game->GetGameName() << "\"\n";
        }
        else
        {
            std::cout << "Couldn't load " << argv[1] << "\n";
        }
    }
    return 0;
}