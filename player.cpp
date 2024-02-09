#include "headers/player.h"

/**
 * @brief Konstruktor, initialisiert den Spieler mit der übergebenen Karte.
 *
 * Setzt die Anfangsposition des Spielers und initialisiert die Spielzustände.
 *
 * @param MapObject Referenz auf das Map-Objekt, das die Spielkarte enthält.
 */
Player::Player(Map& MapObject)
        : x(0), y(0), goalX(0), goalY(0), width(0), height(0), dead(false), map(MapObject), SAFE_FALL(5)
{
}



/**
 * @brief Lädt die Karte neu und aktualisiert die Position des Spielers.
 *
 * Setzt den Spieler auf die Startposition zurück und lädt die aktuelle Karte aus dem Map-Objekt.
 * Neue Zielkoordinaten.
 *
 * @pre Das Map-Objekt sollte eine geladene und gültige Karte enthalten.
 * @post Die Position des Spielers ist auf die Startposition der Karte gesetzt.
 */
void Player::reset()
{
    std::array<size_t, 2> startPos = map.getStartPos();
    std::array<size_t, 2> goalPos = map.getGoalPos();

    gameMap = map.getMap();

    y = startPos[0];
    x = startPos[1];

    goalY = goalPos[0];
    goalX = goalPos[1];

    width = gameMap.at(0).size();
    height = gameMap.size();

    dead = false;

    map.renderPlayer(x, y);
}

/**
 * @brief Aktualisiert die Position des Spielers basierend auf dem Eingabebefehl.
 *
 * Verarbeitet die Eingabe und bewegt den Spieler entsprechend auf der Karte, falls möglich.
 *
 * @param input Das Zeichen, das die Richtung der Bewegung bestimmt ('A', 'D', 'F', 'E').
 * @return True, wenn die Bewegung erfolgreich war, False, wenn die Bewegung ungültig ist oder das Spiel beendet wird.
 * @pre input muss char sein
 * @post Die Position des Spielers ist aktualisiert, oder das Spiel endet, wenn 'E' eingegeben wurde.
 */
bool Player::updatePosition(char input)
{
    bool success;

    switch (input)
    {
        case 'A':
            success = move(Direction::LEFT);
            break;
        case 'D':
            success = move(Direction::RIGHT);
            break;
        case 'F':
            success = handleClimbing();
            break;
        case 'E':
            dead = true;
            return false;
        default:
            success = false;
            break;
    }

    if (success)
    {
        map.renderPlayer(x,y);
    }

    return success;
}

/**
 * @brief Bewegt den Spieler in die angegebene Richtung.
 *
 * Berechnet die neue Position basierend auf der aktuellen Position und der Eingaberichtung.
 *
 * @param direction Die Richtung, in die sich der Spieler bewegen soll.
 * @return True, wenn die Bewegung innerhalb der Grenzen der Karte möglich ist, sonst False.
 * @pre Die Richtung muss eine gültige Richtung sein (LEFT oder RIGHT).
 * @post Die Position des Spielers ist aktualisiert, wenn die Bewegung gültig ist.
 */
bool Player::move(Direction direction) {
    int xINT = static_cast<int>(x);
    int delta = (direction == Direction::RIGHT) ? 1 : -1;

    if (delta < 0 && xINT == 0)
    {
        return false;
    }

    size_t newX = static_cast<size_t>(xINT + delta); //update x-Position

    if (newX >= width)
    {
        return false;
    }

    if (deathFall(y, newX))
    {
        dead = true;
        return false;
    }

    x = newX;
    return true;
}

/**
 * @brief Prüft, ob der Spieler überlebt, falls er fällt
 *
 * Diese funktion prüft, ob der Spieler fällt und ändert ihr Y-Position falls er überlebt
 *
 * @param row Zeile
 * @param col Spalte
 * @pre Die Position sollte innerhalb der Grenzen der Karte sein.
 * @post Die Y-Position des Spielers wird aktualisiert
 * @return True, wenn Player gestorben ist
 */
bool Player::deathFall(size_t row, size_t col)
{
    size_t check_row = row + 1; //ist jetzt was unten steht

    while(gameMap[check_row][col] != '-')
    {
        if (check_row - row > SAFE_FALL || check_row == height - 1) //mehr als 5 Felder, oder Karte endet
        {
            return true;
        }
        ++check_row;
    }

    y = check_row - 1;

    return false;
}

/**
 * @brief Versucht, den Spieler auf einer Leiter auf- oder absteigen zu lassen.
 *
 * Diese Funktion prüft, ob sich der Spieler auf einer Leiter befindet und führt dann die entsprechende
 * Bewegung aus, entweder nach oben oder unten.
 *
 * @return True, wenn der Spieler auf einer Leiter bewegt wurde, sonst False.
 * @pre Der Spieler muss sich auf oder direkt unter einer Leiter befinden, um sie benutzen zu können.
 * @post Die Y-Position des Spielers wird aktualisiert, wenn das Klettern erfolgreich war.
 */
bool Player::handleClimbing()
{
    switch(checkLadder())
    {
        case 0:
            std::cout << "Es gibt hier kein Leiter\n";
            return false;
        case 1:
            climb(Direction::DOWN);
            return true;
        case 2:
            climb(Direction::UP);
            return true;
        default:
            return false;
    }
}

/**
 * @brief Bewegt den Spieler auf der Leiter nach oben oder unten.
 *
 * Die Methode berechnet die neue Position des Spielers beim Klettern auf der Leiter,
 * abhängig von der gewählten Richtung.
 *
 * @param direction Die Richtung des Kletterns (nach oben oder unten).
 * @pre Es wird angenommen, dass sich der Spieler bereits auf einer Leiter befindet.
 * @post Die Y-Position des Spielers ist aktualisiert, nachdem er auf der Leiter geklettert ist.
 */
void Player::climb(Direction direction)
{
    int yINT = static_cast<int>(y); //nur damit Compiler keine Warnungen ausgibt
    int climbingPosition = yINT - 3; //default fuer direction::UP. 3 einträge höher
    int step = -1;

    if (direction == Direction::DOWN)
    {
        climbingPosition = yINT + 5; // 3 + 2 x Ebene runter
        step = 1;
    }

    while(gameMap[static_cast<size_t>(climbingPosition)][x] != '-')
    {
        climbingPosition += step;
    }

    y = static_cast<size_t>(climbingPosition - 1);
}

/**
 * @brief Überprüft die Leiter an der aktuellen Position des Spielers.
 *
 * Ermittelt, ob der Spieler sich auf einer Leiter befindet und welche Aktion möglich ist:
 * aufsteigen, absteigen oder keine Aktion.
 *
 * @return Eine Zahl, die den Status der Leiter anzeigt: 0 für keine Aktion, 1 für absteigen, 2 für aufsteigen.
 * @pre Der Spieler muss sich an einer potenziellen Leiterposition befinden.
 */
unsigned short Player::checkLadder() const
{
    if (gameMap[y][x] == 'H')
    {
        return 2;
    }
    else if (gameMap[y + 2][x] == 'H' && y < height - 2)
    {
        return 1;
    }

    return 0;
}

///@brief getter fuer bool dead
bool Player::isDead() const
{
    return dead;
}

///@brief getter fuer bool win
bool Player::hasWon() const
{
    return goalX == x && goalY == y;
}
