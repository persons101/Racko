#ifndef RACKO_H
#define RACKO_H

#include <vector>

#include "card.h"
#include "deck.h"

class Racko {
private: 
    Deck deck;
    std::vector<Card*> discardPile;

public:
    Card* GetUpCard();
    
};




#endif