#include <iostream>

#include <device/dvd/dvd.h>
#include <disc/game_disc.h>

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

            DeviceWriteMsg msg{ .Address = 0xCC006008, .Count = 4, .Buffer = { { 0x00, 0x00, 0x00, 0xA8 } } };
            dvd.ConsumeWriteMessage(msg);

            msg.Address = 0xCC00600C;
            msg.Buffer  = { { 0x00, 0x20, 0x00, 0x00 } };
            dvd.ConsumeWriteMessage(msg);

            msg.Address = 0xCC006010;
            msg.Buffer  = { { 0x00, 0x01, 0x00, 0x00 } };
            dvd.ConsumeWriteMessage(msg);

            msg.Address = 0xCC006014;
            dvd.ConsumeWriteMessage(msg);

            msg.Address = 0xCC006018;
            dvd.ConsumeWriteMessage(msg);

            msg.Address = 0xCC00601C;
            msg.Buffer  = { { 0x03, 0x00, 0x00, 0x00 } };
            dvd.ConsumeWriteMessage(msg);
        }
        else
        {
            std::cout << "Couldn't load " << argv[1] << "\n";
        }
    }
    return 0;
}
