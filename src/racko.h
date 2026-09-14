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
    std::stack<Card*> discardPile;
    std::vector<Player*> playerVector;

protected:
    Deck deck;

public:
    Card* GetTopCardFromDeck();
    Card* GetTopCardFromDiscard();
    std::vector<Card*> GetPlayerCards(int playerIdx);
    std::vector<Card*> GetPlayerCards(std::string playerName);
    
};


#endif