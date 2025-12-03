#include "Suit.h"

const std::string suit_name(Suit suit) {
    switch (suit) {
        case SPADES: return "Spades";
        case HEARTS: return "Hearts";
        case DIAMONDS: return "Diamonds";
        case CLUBS: return "Clubs";
        default: return "Suit " + std::to_string(static_cast<int>(suit));
    }
}