#ifndef PRUEFUNG_GAMECONTROLLER_H
#define PRUEFUNG_GAMECONTROLLER_H

#include "headers/player.h"
#include "headers/map.h"

/**
 * @class GameController
 * @brief Steuert den Hauptspielablauf und Benutzerinteraktionen.
 *
 * GameController initialisiert das Spiel, verarbeitet Benutzereingaben,
 * aktualisiert den Spielstatus und kontrolliert den Spielablauf.
 */
class GameController {
public:
    GameController();
    void processInput();
    bool exit() const;

private:
    bool gameOver, win, lose, endGame;

    void startMenu();
    void playerMoveLoop();
    void selectMapLoop();
    void gameReset();
    bool selectMap(int index);

    Map map;
    Player player;
};


#endif //PRUEFUNG_GAMECONTROLLER_H
