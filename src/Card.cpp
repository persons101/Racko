#include "Card.h"

Card::Card(int newValue, Suit newSuit){
    this->value = newValue;
    suit = newSuit;
}

int Card::getValue() const {
    return value;
}

Suit Card::getSuit() const {
    return suit;
}