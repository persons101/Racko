#include "rackoCard.h"

#include <iostream>

RackoCard::RackoCard(int cardVal) : Card(cardVal, (Suit)0) { }

RackoCard::RackoCard(Card card) : Card(card.getValue(), (Suit)0) { }

std::string RackoCard::getSuitSymbol() const { return ""; }