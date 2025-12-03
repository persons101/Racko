#ifndef SUIT_H
#define SUIT_H

#include <string>

enum Suit {
    SPADES = 1,
    HEARTS = 2,
    DIAMONDS = 3,
    CLUBS = 4,
};

const std::string suit_name(Suit suit) {
    switch (suit) {
        case Suit::SPADES: return "Spades";
        case Suit::HEARTS: return "Hearts";
        case Suit::DIAMONDS: return "Diamonds";
        case Suit::CLUBS: return "Clubs";
        default: return "Suit " + std::to_string(static_cast<int>(suit));
    }
}

#endif