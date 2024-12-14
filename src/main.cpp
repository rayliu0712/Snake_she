#include "game.hpp"
#include <memory>
using namespace std;

// flags: "--fps" "--freward"
int main(int argc, char **argv) {
    int fps = -1;
    int fReward = -1;

    for (int i = 1; i < argc - 1; i++) {
        string arg = argv[i];

        if (arg == "--fps") {
            i++;
            fps = stoi(arg);
        } else if (arg == "--freward") {
            i++;
            fReward = stoi(arg);
        }
    }

    static auto game = make_unique<Game>(fps, fReward);
    game->welcome();
    game->adjustMapSize();
    game->playLoop();
}