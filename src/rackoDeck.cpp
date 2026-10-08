#include "rackoDeck.h"

#include <iostream>
#include "card.h"
#include "helper.hpp"

RackoDeck::RackoDeck() : Deck(0, 0, 0) { 
    AddCardsToDeck(60, 1);
    Shuffle();
}

std::string RackoDeck::PrintDeck() const
{
    std::string returnStr = "";
    returnStr += "---Start Deck---\n";
    for (const auto& card : GetCards()) {
        returnStr += card->PrintCardShort() + " ";
    }
    Helper::rtrim(returnStr);
    returnStr += "\n";
    returnStr += "--- End Deck ---\n";

    return returnStr;
}

Card* RackoDeck::MakeNewCard(int cardVal, Suit cardSuit) {
    return new RackoCard(cardVal);
}