#include "card.h"

#include <iostream>
#include <tuple>

Card::Card(int newValue, Suit newSuit){
    this->value = newValue;
    suit = newSuit;
}

Card::Card(std::tuple<int, Suit> vals) {
    *this = Card(std::get<0>(vals), std::get<1>(vals));
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

auto Card::tied() const { return std::tie(value, suit); }

bool Card::operator==(const Card& rhs) const {
    return tied() == rhs.tied();
}

bool operator==(const Card& lhs, const std::tuple<int, Suit>& rhs) {
    return lhs.tied() == rhs;
}

bool operator==(const std::tuple<int, Suit>& lhs, const Card& rhs){
    return rhs == lhs; // related to above
}

bool operator!=(const Card& lhs, const std::tuple<int, Suit>& rhs) {
    return !(lhs.tied() == rhs);
}

bool operator!=(const std::tuple<int, Suit>& lhs, const Card& rhs){
    return rhs != lhs; // related to above
}

bool Card::compareCards::operator() (const Card a, const Card b) const {
    if (a.suit == b.suit)
        return a.value < b.value;
    return (int)a.suit < (int)b.suit;
};

bool Card::compareCards::operator() (const Card* a, const Card* b) const {
    if (a->suit == b->suit)
        return a->value < b->value;
    return (int)a->suit < (int)b->suit;
};

bool Card::compareCards::operator() (const Card a, const std::tuple<int,Suit> b) const {
    if (a.suit == std::get<1>(b))
        return a.value < std::get<0>(b);
    return (int)a.suit < (int)(std::get<1>(b));
};

bool Card::compareCards::operator() (const Card* a, const std::tuple<int,Suit> b) const {
    if (a->suit == std::get<1>(b))
        return a->value < std::get<0>(b);
    return (int)a->suit < (int)(std::get<1>(b));
};