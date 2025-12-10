#ifndef RACKODECK_H
#define RACKODECK_H

#include "rackoCard.h"
#include "deck.h"

class RackoDeck : public Deck {
protected:
    virtual Card* MakeNewCard(int, Suit) override;
public:
    RackoDeck();
};

#endif