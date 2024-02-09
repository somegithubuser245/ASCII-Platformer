#include "headers/gameController.h"

int main () {
    GameController game = GameController();

    while(!game.exit())
    {
        game.processInput();
    }

    return 0;
}
