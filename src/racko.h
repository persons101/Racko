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

    virtual int CalculateRackScore(std::vector<Card*>&) const;

    virtual int PlayTurnForPlayerByIdx(int playerIdx);
    virtual int PlayTurnForPlayerByName(std::string playerName);
    void ResetPlayers();
    virtual void AddPlayer(std::string);
    virtual void AddPlayer(Player*);


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
    
    virtual void SetupGame();
    virtual void PlayTurn();
    virtual int FinishGame();
    virtual void ResetGame();
    
};


#endif