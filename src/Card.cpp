#include "card.h"

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

std::string Card::getSuitName() const {
    return suit_name(suit);
}

const std::string Card::suit_name(Suit suit)  {
    switch (suit) {
        case SPADES: return "Spades";
        case HEARTS: return "Hearts";
        case DIAMONDS: return "Diamonds";
        case CLUBS: return "Clubs";
        default: return "Suit " + std::to_string(static_cast<int>(suit));
    }
}