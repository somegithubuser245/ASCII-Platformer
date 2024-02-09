#ifndef PRUEFUNG_MAP_H
#define PRUEFUNG_MAP_H

#include <iostream>
#include <array>
#include <vector>

/**
 * @class Map
 * @brief Verwaltet die Karte und deren Zustände im Spiel.
 *
 * Lädt und speichert die Karten, überprüft Karten und
 * stellt Informationen über die Umgebung zur Verfügung.
 */
class Map {
public:
    Map();

    bool selectMap(size_t index);
    void renderPlayer(size_t x, size_t y) const;

    const std::vector<std::vector<char>>& getMap() const;
    const std::array<size_t, 2>& getStartPos() const;
    const std::array<size_t, 2>& getGoalPos() const;
    const std::vector<std::string>& getMapsNames() const;

private:
    bool mapOK;

    std::vector<std::vector<char>> renderMap2D;
    std::vector<std::string> availableMaps;

    std::array<size_t, 2> startPos, goalPos;

    size_t width, height;

    bool isPlatformValid(size_t row, size_t col) const;
    bool isLadderValid(size_t row, size_t col) const;
    bool symbolCheck(size_t row, size_t col) const;
    bool checkCurrentMap();
    bool loadMaps();
    size_t setDimension(const std::string& input);

    void renderMap(const std::vector<std::vector<char>>& bufferMap) const;
    void addNew(const std::string& mapFileName);

    const size_t MAX_SPACE, PLAYER_HEIGHT;
    const std::string MAP_DIRECTORY;
};


#endif //PRUEFUNG_MAP_H
