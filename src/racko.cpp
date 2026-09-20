#include "racko.h"

#include <iostream>
#include <string>
#include <limits>
#include "rackoPlayer.h"

int Racko::CalculateRackScore(const std::vector<Card *>& cards) const
{
    int score = 0;

    for (std::size_t i = 0; i < cards.size() - 1; i++) {
        if (cards.at(1 + i) > cards.at(i))
            score += 5;
        else 
            break;
    }

    if (score >= (RACKO_BONUS_REQ + racko_bonus_req_handicap))
        score += (RACKO_BONUS + racko_bonus_handicap);

    return score;
}

char Racko::SelectDrawCard(int playerIdx) const
{
    /// Shows the player at @param{playerIdx} the top of the draw and discard piles
    /// @return D'r'aw pile, D'i'scard, or 'e'rror
    Player* player = playerVector.at(playerIdx);
    if (player == nullptr) {
        return 'e';
    }

    std::string input = "";

    std::cout << player->PrintCards() << "\n";
    std::cout << "Deck (0): " << deck->GetTopCard()->PrintCardShort() << ", Discard (1): " << ((!discardPile.empty()) ? discardPile.top()->PrintCardShort() : "-") << "\n";
    std::cout << "Choose a card: Deck='0', Discard='1'";
    
    // input validation
    while (input != "0" && input != "1") {
        std::cin >> input;
        if (input.size() > 1) {
            input = "-";
        }
        else if (discardPile.empty() && input == "1"){
            input = "-";
        }

        if ( !(input == "0" || input == "1")) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } 
    }

    if (input == "1") {
        return 'i';
    } 

    return 'r';
}

int Racko::DrawCard(int playerIdx, bool isDiscardPileChosen)
{
    Player* player = GetPlayerByIdx(playerIdx);

    if (player == nullptr){
        return -1;
    }

    Card* card = nullptr;
    try {
        switch (isDiscardPileChosen) {
            case false:
                // draw from deck
                card = deck->PopTopCard();
            case true:
                // draw from discard
                // TODO create discardPile.h/.cpp to handle stack manipulation
                card = discardPile.top();
                discardPile.pop();
            default:
                // should never run
                throw std::out_of_range("Switch-case failure");
        }
        if (card == nullptr) {
            throw std::out_of_range("No card found in stack!");
        }
    }
    catch (const std::out_of_range& e) {
        std::cerr << e.what();
        return -2;
    }

    bool playerDrawStatus = player->DrawCard(card);

    return 0;
}

int Racko::GetPlayerIdxByName(std::string playerName) const
{
    int idx = -1;

    Player* currPlayer = nullptr;

    std::vector<Player*>::iterator it;

    int i = 0;
    for (i; i < playerCnt; i++) {
        currPlayer = playerVector.at(i);
        if (currPlayer->GetName() == playerName)
            break;
    }

    idx = i;
    return idx;
}

int Racko::PlayTurnForPlayerByIdx(int playerIdx)
{
    Player* player_raw = playerVector.at(playerIdx);
    if (player_raw == nullptr) {
        return -1;
    }

    RackoPlayer* player = dynamic_cast<RackoPlayer*>(player_raw);
    if (!player) {
        return -2;
    }

    // show cards
    // select card to pickup
    // pickup card
    // select card index to replace
    // put card in discard pile

    char drawSelectionResult = SelectDrawCard(playerIdx);
    
    bool isDiscardPileChosen;
    switch (drawSelectionResult) {
        case 'r':
            isDiscardPileChosen = false;
        case 'i':
            isDiscardPileChosen = true;
        default:
            return -3;
    }

    int drawStatus = DrawCard(playerIdx, isDiscardPileChosen);

    if (drawStatus != 0) {
        return -4;
    }

    int score = CalculateRackScore(player->GetCards());
    if (player->IsCompletedWithRack()) {
        return score;
    }
    
    return 0;
}

int Racko::PlayTurnForPlayerByName(std::string playerName)
{
    int idx = GetPlayerIdxByName(playerName);

    PlayTurnForPlayerByIdx(idx);
    return 0;
}

void Racko::ResetPlayers()
{
    for (Player* player : playerVector) {
        delete player;
    }
    playerVector = {};
}

void Racko::AddPlayer(std::string playerName)
{
    Player* newPlayer = new RackoPlayer(playerName);
    playerVector.push_back(newPlayer);
    playerCnt++;
}

void Racko::AddPlayer(Player * newPlayer)
{
    playerVector.push_back(newPlayer);
    playerCnt++;
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

std::vector<Card *> Racko::GetPlayerCardsByIdx(int playerIdx) const
{
    Player* player = playerVector.at(playerIdx);
    if (player == nullptr) {
        return std::vector<Card*>();
    }

    std::vector<Card *> cards = player->GetCards();
    return cards;
}

std::vector<Card *> Racko::GetPlayerCardsByName(std::string playerName) const
{
    int idx = GetPlayerIdxByName(playerName);

    if (idx == -1) {
        return std::vector<Card *>();
    }

    Player* player = playerVector.at(idx);

    return player->GetCards();
}

void Racko::PlayTurn()
{
    bool isRoundOver = false;
    for (int i = 0; i < playerVector.size() && !isRoundOver; i++) {
        // score is returned for possible subclasses/accessors, but value is not used in this implementation.
        int playerScore = 0;
        playerScore = PlayTurnForPlayerByIdx(i); 

        if (playerScore != 0) {
            isRoundOver = true;
            for (int j = 0; j < playerVector.size(); j++) {
                ScoreRackByIdx(j);
            }
        }
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

        if ( pLast == nullptr || pLast->GetScore() > currPlayerScore) {
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
