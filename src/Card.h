// Card.h
// Implementation of a Card class for use in a card game
#ifndef CARD_H
#define CARD_H

#include "Suit.h"

class Card {
private:
    int value;
    Suit suit;

public:
    Card(int, Suit);
    int getValue() const;
    Suit getSuit() const;
    bool isBlack() { return (int)suit % 2 == 1; }
};

#endif