#include "racko.h"

#include <iostream>
#include <string>

int Racko::CalculateRackScore(std::vector<Card *>& cards) const
{
    int score = 0;
    const int RACKO_BONUS_REQ = 75;
    const int RACKO_BONUS = 25;

    for (int i = 0; i < cards.size() - 1; i++) {
        if (cards.at(1 + i) > cards.at(i))
            score += 5;
        else 
            break;
    }

    if (score >= RACKO_BONUS_REQ)
        score += RACKO_BONUS;

    return score;
}

int Racko::PlayTurnForPlayerByIdx(int playerIdx)
{

}

int Racko::PlayTurnForPlayerByName(std::string playerName)
{
    std::vector<Player*>::iterator it = playerVector.begin();
    int i = 0;
    while (it != playerVector.end() && (*it)->GetName() != playerName) {
        i++;
        it++;
    }

    if (it == playerVector.end()) {
        std::cerr << ">>> Error: Player name not found.";
        return -1;
    }

    PlayTurnForPlayerByIdx(i);
    return 0;
}

void Racko::ResetPlayers()
{
    for (Player* player : playerVector) {
        delete player;
    }
    playerVector = {};
}

void Racko::AddPlayer(Player * newPlayer)
{
    playerVector.push_back(newPlayer);
}

Racko::Racko()
{
    int restartVal = 1;
    int gameNumber = 0;
    do {
        gameNumber++;
        SetupGame();
        // Game turns
        // Check for winner
        restartVal = FinishGame();
        if (restartVal == 1) {
            ResetGame();
        }
    } while (restartVal == 1);
    
}

int Racko::GetNumTurns() const
{
    return numTurns;
}

int Racko::GetPlayerCount() const
{
    return playerCnt;
}

int Racko::GetDeckCardCount() const
{
    return deck->GetNumCards();
}

int Racko::GetDiscardCardCount() const
{
    return discardPile.size();
}

void Racko::PlayTurn()
{
    for (int i = 0; i < playerVector.size(); i++) {
        PlayTurnForPlayerByIdx(i);
    }
}

int Racko::FinishGame()
{
    Player* pFirst = nullptr;
    Player* pSecond = nullptr;
    Player* pThird = nullptr;
    Player* pLast = nullptr;
    char playAgainChar;

    for (Player* player : playerVector) {
        int currPlayerScore = player->GetScore();
        if (pFirst == nullptr) {
            player = pFirst;
        }
        else if (pFirst->GetScore() < currPlayerScore) {
            if (pSecond == nullptr) {
                pSecond = pFirst;
                pFirst = player;
            }
            else {
                if (pSecond->GetScore() < currPlayerScore) {
                    pThird = pSecond;
                    pSecond = player;
                }
                else if (pThird->GetScore() < currPlayerScore) {
                    pThird = player;
                }
            }
        }

        if (pLast->GetScore() > currPlayerScore || pLast == nullptr) {
            pLast = player;
        }
    }

    std::cout << "--------------------------------------\n";
    std::cout << "First Place: " << pFirst->GetName() << "\n";
    if (playerCnt > 1) {
        std::cout << "Second Place: " << pSecond->GetName() << "\n";
        if (playerCnt > 2) {
            std::cout << "Third Place: " << pThird->GetName() << "\n";
            if (playerCnt > 3) {
                std::cout << "Last Place: " << pLast->GetName() << "\n";
            }
        }
    }

    std::cout << "\n\nWould you like to play again? (Y/N) ";
    while (playAgainChar != 'y' && playAgainChar != 'n') {
        std::cin >> playAgainChar;
        if (playAgainChar < 91) {
            playAgainChar += 32;
        }
    }
    if (playAgainChar == 'y') {
        return 1;
    }
    return 0; // Game Over
}
