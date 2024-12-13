#include "game.hpp"
#include <cstdlib>
#include <memory>

// flags: "--fps" "--freward"
int main(int argc, char** argv)
{
    int fps = -1;
    int fReward = -1;

    for (int i = 1; i < argc - 1; i++) {
        std::string arg = argv[i];
        if (arg == "--fps") {
            i++;
            fps = atoi(argv[i]);
        } else if (arg == "--freward") {
            i++;
            fReward = atoi(argv[i]);
        }
    }

    static auto game = std::make_unique<Game>(fps, fReward);
    game->welcome();
    game->adjustMapSize();
    game->playLoop();
}