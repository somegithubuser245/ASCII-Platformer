#include "headers/map.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

/**
 * @brief Konstruktor, der die Map initialisiert und versucht, die verfügbaren Karten zu laden.
 *
 * Setzt die Start- und Zielpositionen sowie die Dimensionen der Map auf 0 und
 * ruft loadMaps() auf, um verfügbare Karten zu laden.
 *
 * MAX_SPACE und PLAYER_HEIGHT sollen eigentlich gleich sein, da im Muster der Spieler 3 Zeilen Hoch ist.
 * Bei mir ist aber der Spieler als einzelne Symbol dargestellt.
 *
 * @post Die Map ist initialisiert und bereit zur Auswahl einer Karte.
 */
Map::Map():MAX_SPACE(3), PLAYER_HEIGHT(1), MAP_DIRECTORY("maps/")
{
    goalPos = {0, 0};
    startPos = {0, 0};

    width = 0;
    height = 0;

    mapOK = false;

    if(!loadMaps())
    {
        std::cout << "\nKeine Karten im Ordner!\n";
    } else {
        std::cout << "\nHochladen erfolgreich\n";
    }
    std::cout << "Um Ihre eigene Karte zu verwenden, laden Sie bitte die .txt-Datei in den Root-Ordner des Projekts.\n"
                 "Bitte entfernen Sie alle .txt-Dateien, die keine Karten sind, aus dem Ordner. Starten Sie nach dem Hochladen das Programm neu.";
}

/**
 * @brief Lädt alle verfügbaren Karten.
 *
 * BITTE COMPILE.TXT UND TEST.TXT aus dem Ordner entfernen.
 *
 * Geht alle Dateien durch und fügt Karten zur Liste der verfügbaren Karten hinzu.
 * @return Gibt zurück, ob mindestens eine Karte gefunden wurde.
 * @post Die Liste der verfügbaren Karten ist aktualisiert.
 */
bool Map::loadMaps()
{
    bool mapFound = false;
    for(const auto& entry : fs::directory_iterator(MAP_DIRECTORY)) //durch alle Dateien gehen
    {
        if(entry.is_regular_file() && entry.path().extension() == ".txt")
        {
            addNew(entry.path().filename().string());
            mapFound = true;
        }
    }
    return mapFound;
}

/**
 * @brief Fügt eine neue Karte zu den verfügbaren Karten hinzu.
 *
 * Diese Methode nimmt den Dateinamen einer Karte und fügt ihn zur Liste der verfügbaren Karten hinzu.
 * @param mapFileName Der Dateiname der hinzuzufügenden Karte.
 */
void Map::addNew(const std::string& mapFileName)
{
    availableMaps.push_back(mapFileName);
}

/**
 * @brief Wählt eine Karte aus der Liste der verfügbaren Karten und lädt sie.
 *
 * Diese Methode lädt die Karte, die durch den Index spezifiziert wird, und überprüft ihre Gültigkeit.
 * @param index Der Index der auszuwählenden Karte in der Liste der verfügbaren Karten.
 * @return Gibt zurück, ob die Karte erfolgreich ausgewählt und geladen wurde.
 * @pre Der Index muss innerhalb der Grenzen der verfügbaren Kartenliste liegen.
 * @post Die Karte ist geladen, falls sie gültig ist.
 * @see setDimension()
 * @see checkCurrentMap()
 */
bool Map::selectMap(const size_t index)
{
    std::ifstream map (MAP_DIRECTORY + availableMaps.at(index));

    if (map.is_open())
    {
        std::string line, sizes;

        std::getline(map, sizes);
        size_t separator = sizes.find(' ');

        height = setDimension(sizes.substr(0, separator));
        width = setDimension(sizes.substr(separator + 1));

        if(width == 0 || height == 0) return false;

        renderMap2D.clear(); //wenn eine neue Karte gewählt wird, müssen alle alte Speicherzellen leer sein
        renderMap2D.resize(height, std::vector<char>(width, ' '));

        size_t row = 0;

        while ( std::getline (map,line) )
        {
            for (size_t col = 0; col < width; ++col) {
                char input = (col < line.length()) ? line[col] : ' '; // wenn es nicht genug leerzeichen im line gibt
                renderMap2D.at(row).at(col) = input;
            }
            ++row;
        }

        map.close();

        mapOK = checkCurrentMap();

        if(!mapOK)
        {
            std::cout << "Die hochgeladene Karte ist ungültig\n";
            return false;
        }

        return true;

    } else {
        std::cout << "Datei konnte nicht geöffnet werden\n";
        return false;
    }
}

///@brief Zeichnet die Karte
///@param bufferMap ist die Karte mit Player drin
void Map::renderMap(const std::vector<std::vector<char>>& bufferMap) const
{
    for (auto & row : bufferMap) {
        for (char col : row) {
            std::cout << col;
        }
        std::cout << "\n";
    }
}

/**
 * @brief Erstellt buffer map und zeichnet die Karte mit P-symbol (player)
 * @param x  x-position von Player
 * @param y  y-position von Player
 */
void Map::renderPlayer(size_t x, size_t y) const
{
    std::vector<std::vector<char>> bufferMap = renderMap2D;

    bufferMap[y][x] = 'P';

    renderMap(bufferMap);
}

/**
 * @brief Überprüft die aktuell geladene Karte auf Gültigkeit basierend auf den Spielregeln.
 *
 * Geht alle Zellen der Karte durch und stellt sicher, dass die Karte den Spielregeln entspricht, wie z.B.
 * die korrekte Platzierung von Plattformen und Leitern sowie die Start- und Zielsymbole. Zuweist auch
 * die Start- und Zielposition
 * @return Gibt zurück, ob die aktuelle Karte gültig ist.
 * @pre renderMap2D darf nicht leer sein
 * @post Das Attribut mapOK ist gesetzt, um anzuzeigen, ob die Karte gültig ist.
 * @see isLadderValid()
 * @see isPlatformValid()
 * @see symbolCheck()
 */
bool Map::checkCurrentMap()
{
    for (size_t row = 0; row < height; ++row)
    {
        for (size_t col = 0; col < width; ++col)
        {
            char cell = renderMap2D[row][col];
            switch (cell)
            {
                case '-':
                    if(!isPlatformValid(row, col))
                    {
                        return false;
                    }
                    break;
                case 'H':
                    if(!isLadderValid(row, col))
                    {
                        return false;
                    }
                    break;
                case 'S':
                    if(!symbolCheck(row, col))
                    {
                        return false;
                    }

                    startPos = {row + 1, col};
                    renderMap2D[row][col] = ' ';
                    break;
                case 'O':
                    if(!symbolCheck(row, col))
                    {
                        return false;
                    }

                    goalPos = {row + 1, col};
                    break;
                default:
                {
                    if (cell != ' ')
                    {
                        renderMap2D[row][col] = ' ';
                    }
                    break;
                }
            }
        }
    }
    return !(goalPos.at(0) == 0 || startPos.at(0) == 0); // ist O und S dabei?
}

/**
 * @brief Überprüft, ob das Symbol 'S' oder 'O' korrekt über einer Plattform platziert ist.
 *
 * Das Symbol ist korrekt platziert, wenn es genau eine Position über einer Plattform steht.
 *
 * @param row Die Zeilenposition des Symbols, die überprüft werden soll.
 * @param col Die Spaltenposition des Symbols, die überprüft werden soll.
 * @return True, wenn das Symbol korrekt platziert ist, sonst False.
 * @pre Die Position sollte innerhalb der Grenzen der Karte sein.
 */
bool Map::symbolCheck(size_t row, size_t col) const
{
    size_t check_row = row + 2; //2 Eintraege runter
    if (check_row + 1 > height || renderMap2D[check_row][col] != '-')
    {
        return false;
    }
    return true;
}

/**
 * @brief Überprüft, ob eine Leiter an der gegebenen Position korrekt platziert ist.
 *
 * Eine Leiter ist dann korrekt platziert, wenn sie nicht im leeren Raum endet
 *
 * @param row Die Zeilenposition der Leiter, die überprüft werden soll.
 * @param col Die Spaltenposition der Leiter, die überprüft werden soll.
 * @return True, wenn die Leiter korrekt platziert ist, sonst False.
 * @pre Die Position sollte innerhalb der Grenzen der Karte sein.
 */
bool Map::isLadderValid(size_t row, size_t col) const
{
    if (row == 0 || renderMap2D[row-1][col] == ' ')
    {
        std::cout << "falsche Leiter " << row + 2 << " x " << col + 1 << "\n";
        return false;
    }

    size_t check_row = row + 1;

    while (renderMap2D[check_row][col] != '-')
    {
        if (renderMap2D[check_row][col] == ' ' || check_row + 1 == height) //out of range oder leeres Zeichen
        {
            return false;
        }

        ++check_row; //naechste Leiter
    }

    return true;
}

/**
 * @brief Überprüft, ob eine Plattform an der gegebenen Position korrekt platziert ist.
 *
 * Eine Plattform ist dann korrekt platziert, wenn genügend freier Raum über ihr ist
 * und wenn sie korrekt in der Kartenstruktur positioniert ist.
 *
 * @param row Die Zeilenposition der Plattform, die überprüft werden soll.
 * @param col Die Spaltenposition der Plattform, die überprüft werden soll.
 * @return True, wenn die Plattform korrekt platziert ist, sonst False.
 * @pre Die Position sollte innerhalb der Grenzen der Karte sein
 */
bool Map::isPlatformValid(size_t row, size_t col) const
{
    if (row < PLAYER_HEIGHT) return false;

    size_t freeSpace = height - row - 1; // berechnet, wie hoch man von unten steht;
    size_t availableCheckRoom = MAX_SPACE;

    if (freeSpace < MAX_SPACE)  //out of range exception handling
    {
        availableCheckRoom = freeSpace;
    }

    //prüft MAX_SPACE untenstehende zellen
    for(size_t check_row = row + 1; check_row <= row + availableCheckRoom; ++check_row)
    {
        if (renderMap2D[check_row][col] == '-')
        {
            std::cout << "falsche ebene: " << check_row + 1 << "x" << col + 1 << std::endl;
            return false;
        }
    }
    return true;
}

/**
 * @brief Prüft, ob Höhe und Breite richtig angegeben wurden
 * @param input input string
 * @pre input muss int sein
 * @return size_t dimension (width or height)
 */
size_t Map::setDimension(const std::string& input)
{
    try
    {
        int dimension = std::stoi(input);
        if(dimension < 1)
        {
            std::cerr << "Breite/Höhe muss größer als 0 sein\n";
            return 0;
        }

        return static_cast<size_t>(dimension);
    }
    catch (const std::invalid_argument&)
    {
        std::cerr << "Falsche Breite/Höhe\n";
    }
    catch (const std::out_of_range&)
    {
        std::cerr << "Breite/Höhe ist zu groß\n";
    }

    return 0;
}

///@brief Map getter
const std::vector<std::vector<char>>& Map::getMap() const
{
    return renderMap2D;
}

///@brief Anfangsposition getter
const std::array<size_t, 2>& Map::getStartPos() const
{
    return startPos;
}

///@brief Zielposition getter
const std::array<size_t, 2>& Map::getGoalPos() const
{
    return goalPos;
}

///@brief Map Liste getter
const std::vector<std::string>& Map::getMapsNames() const
{
    return availableMaps;
}
