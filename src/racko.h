#ifndef RACKO_H
#define RACKO_H

#include <stack>
#include <vector>

#include "card.h"
#include "deck.h"
#include "player.h"
#include "rackoCard.h"

class Racko {
private:
    int numTurns = 0;
    int playerCnt = 0;
    int cardsInDeck = 60;
    int cardsInDiscard = 0;
protected:
    Deck* deck;
    std::stack<Card*> discardPile;
    std::vector<Player*> playerVector;

    int CalculateRackScore(std::vector<Card*>&) const;

    void PlayTurnForPlayerByIdx(int playerIdx);
    void PlayTurnForPlayerByName(std::string playerName);
    void ResetPlayers();
    void AddPlayer(std::string);
    void AddPlayer(Player*);


public:
    Racko();

    int GetNumTurns() const;
    int GetPlayerCount() const;
    int GetDeckCardCount() const;
    int GetDiscardCardCount() const;

    Card* GetTopCardFromDeck();
    Card* GetTopCardFromDiscard();
    std::vector<Card*> GetPlayerCardsByIdx(int playerIdx);
    std::vector<Card*> GetPlayerCardsByName(std::string playerName);
    Player* GetPlayerByIdx(int playerIdx);
    Player* GetPlayerByName(std::string playerName);
    
    void ScoreRackByIdx(int playerIdx);
    void ScoreRackByName(std::string playerName);
    
    void SetupGame();
    void PlayTurn();
    void FinishGame();
    void ResetGame();
    
};


#endif