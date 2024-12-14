// https://github.com/rayliu0712/Snake_she
#include "game.hpp"
#include <memory>

// flags: "--fps=???" "--reward-frequency=???"
int main(int argc, char **argv) {
    static auto game = std::make_unique<Game>(argc, argv);
    game->welcome();
    game->adjustMapSize();
    game->playLoop();
}