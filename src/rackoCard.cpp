#include "rackoCard.h"

#include <iostream>
#include <string>

RackoCard::RackoCard(int cardVal) : Card(cardVal, (Suit)0) { }

RackoCard::RackoCard(const Card& card) : Card(card.getValue(), (Suit)0) { }

Card *RackoCard::getCopy() const
{
    return new RackoCard(*this);
}

std::string RackoCard::getSuitSymbol() const { return ""; }

std::string RackoCard::PrintCard() const {
    return "Card Value: " + std::to_string(getValue());
}

std::string RackoCard::PrintCardShort() const {
    return std::to_string(this->getValue());
} 