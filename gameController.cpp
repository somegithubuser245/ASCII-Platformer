#include "headers/gameController.h"

/**
 * @brief Konstruktor, initialisiert das Spiel mit dem Startzustand.
 */
GameController::GameController():
                map(), player(map)
{
    endGame = false;
    gameOver = true;
    win = false;
    lose = false;
}

/**
 * @brief Verarbeitet die Eingabe des Spielers und aktualisiert den Spielzustand.
 */
void GameController::processInput()
{
    if(gameOver)
    {
        gameReset();
    } else
    {
        player.reset();

        playerMoveLoop();

        if(win)
        {
            std::cout << "\nYOU WON!!!\n\n";
        } else {
            std::cout << "\nGAME OVER\n\n";
        }

        gameOver = true;
    }
}

/**
 * @brief Setzt das Spiel zurück und zeigt das Startmenü.
 */
void GameController::gameReset()
{
    win = false;
    lose = false;

    startMenu();
    selectMapLoop();

    gameOver = false;
}

/**
 * @brief Zeigt das Startmenü und ermöglicht die Auswahl einer Karte.
 */
void GameController::startMenu() {
    auto mapList = map.getMapsNames();

    std::cout << "\nWählen Sie die Karte. Um Spiel zu beenden, geben Sie -1 ein\n\n";

    for (size_t i = 0; i < mapList.size(); ++i) {
        std::cout << "[" << i+1 << "] " << mapList[i] << "\n";
    }
}

/**
 * @brief Wählt eine Karte basierend auf dem Index aus.
 * @param index Der Index der auszuwählenden Karte.
 * @return True, wenn die Auswahl erfolgreich war, sonst false.
 */
bool GameController::selectMap(int index) {

    int mapListSize = static_cast<int>(map.getMapsNames().size());

    if(index >= 0 && index < mapListSize)
    {
        return map.selectMap(static_cast<size_t>(index));
    } else if(index == -2)
    {
        endGame = true;
        return true;
    }
    else
    {
        std::cerr << "Ungültige Auswahl. Bitte wählen Sie eine gültige Kartennummer.\n";
        return false;
    }
}

/**
 * @brief Schleife zur Auswahl der Karte durch den Spieler.
 */
void GameController::selectMapLoop()
{
    std::string input;
    int index;

    do {
        std::cin >> input;
        try
        {
            index = std::stoi(input);
        }
        catch (std::invalid_argument&)
        {
            std::cerr << "Falshe Eingabe. Bitte Nummer eingeben\n";
            continue;
        }
        catch (std::out_of_range&)
        {
            std::cerr << "Zahl ist zu groß\n";
            continue;
        }
    } while (!selectMap(index - 1));
}

/**
 * @brief Führt die Bewegungsschleife des Spielers aus, bis das Spiel gewonnen oder verloren ist.
 */
void GameController::playerMoveLoop()
{
    while(!(win || lose))
    {
        char input;
        std::cout << "D - rechts, A - links, F zu klettern. E zum Beenden\n";
        std::cin >> input;

        player.updatePosition(input);

        win = player.hasWon();
        lose = player.isDead();
    }
}

/**
 * @brief Überprüft, ob das Spiel beendet werden soll.
 * @return True, wenn das Spiel beendet werden soll, sonst false.
 */
bool GameController::exit() const
{
    return endGame;
}
