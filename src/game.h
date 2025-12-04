#ifndef GAME_H
#define GAME_H

#include <vector>

#include "card.h"
#include "deck.h"

class Game {
private: 
    Deck deck;
    std::vector<Card*> discardPile;

public:
    Card* GetUpCard();
    
};


#endif