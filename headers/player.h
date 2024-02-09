#ifndef PRUEFUNG_PLAYER_H
#define PRUEFUNG_PLAYER_H

#include "headers/map.h"

/**
 * @class Player
 * @brief Repräsentiert den Spieler im Spiel.
 *
 * Verwaltet die Position und Zustand des Spielers
 * und prüft, ob der Spieler gewonnen oder verloren hat.
 */
class Player {

public:
    Player(Map& MapObject);

    void reset();
    bool updatePosition(char input);

    bool isDead() const;
    bool hasWon() const;

private:
    enum class Direction { LEFT, RIGHT, UP, DOWN};

    size_t x, y, goalX, goalY, width, height;
    std::vector<std::vector<char>> gameMap;

    bool dead;
    Map& map;

    const size_t SAFE_FALL;

    bool deathFall(size_t row, size_t col);
    unsigned short checkLadder() const;

    bool move(Direction direction);
    void climb(Direction direction);
    bool handleClimbing();
};


#endif
