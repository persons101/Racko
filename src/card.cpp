#include "card.h"

#include <iostream>

Card::Card(int newValue, Suit newSuit){
    this->value = newValue;
    suit = newSuit;
}

int Card::getValue() const {
    return value;
}

char Card::getValueChar() const {
    switch (value) {
        case 0: return 'X';
        case 1: return 'A';
        case 10: return 'T';
        case 11: return 'J';
        case 12: return 'Q';
        case 13: return 'K';
        default:
            if (value > 13)
                return '^';
            else if (value < 0)
                return '_';
            return std::to_string(value)[0];
    }
}

std::string Card::getValueString() const {
    switch (value) {
        case 0: return "X";
        case 1: return "A";
        case 10: return "T";
        case 11: return "J";
        case 12: return "Q";
        case 13: return "K";
        default:
            return std::to_string(value);
    }
}

Suit Card::getSuit() const {
    return suit;
}

std::string Card::getSuitName() const {
    return suit_name(suit);
}

std::string Card::getSuitSymbol() const {
    switch (suit) {
        case SPADES: return "\xe2\x99\xA0";
        case HEARTS: return "\xe2\x99\xA5";
        case DIAMONDS: return "\xe2\x99\xA6";
        case CLUBS: return "\xe2\x99\xA3";
        default: return std::to_string(static_cast<int>(suit));
    }
}

void Card::PrintCard() const {
    std::cout << "Card Value: " << value << ", Suit: " << suit_name(suit) << std::endl;
}

void Card::PrintCardShort() const {
    std::cout << this->getValueString() << this->getSuitSymbol() << " ";
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

bool Card::compareCards::operator() (Card a, Card b) const {
    if (a.suit == b.suit)
        return a.value < b.value;
    return (int)a.suit < (int)b.suit;
};

bool Card::compareCards::operator() (Card* a, Card* b) const {
    if (a->suit == b->suit)
        return a->value < b->value;
    return (int)a->suit < (int)b->suit;
};
