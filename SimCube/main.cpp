#include <iostream>

#include <device/dvd/dvd.h>
#include <device/gekko/gekko.h>
#include <device/memory/ram.h>
#include <disc/game_disc.h>
#include <util/endian.h>

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        if (auto game = CreateDisc({ argv[1] }))
        {
            game->Load();
            std::cout << "Loaded \"" << game->GetGameName() << "\"\n";

            DvdReader dvd;
            dvd.InsertDisc(game);

            Ram ram(0, 24ull * 1024ull * 1024ull);
            ram.InitializeRam(0x1200000, game->ReadData(0x2460, 0x2000));

            const auto entryPoint = ReadBigEndianU32(game->ReadData(0x2450, 4).subspan<0, 4>());

            GekkoCpu cpu;
            cpu.SetPc(entryPoint);

        }
        else
        {
            std::cout << "Couldn't load " << argv[1] << "\n";
        }
    }
    return 0;
}
