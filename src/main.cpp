#include "game.hpp"
#include <memory>
using namespace std;

// flags: "--fps" "--reward-frequency"
int main(int argc, char **argv) {
    static auto game = make_unique<Game>(argc, argv);
    game->welcome();
    game->adjustMapSize();
    game->playLoop();
}