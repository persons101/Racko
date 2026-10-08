#ifndef TEST_DECK_H
#define TEST_DECK_H

#include "../../src/rackoDeck.h"
#include <unordered_set>

class TestDeck : public RackoDeck {
public:
    TestDeck();
    TestDeck(const std::unordered_set<int>& cardsToInclude);
    TestDeck(const std::vector<int>& cardsToInclude);
};

class InspectableDeck : public Deck {
public:
    using Deck::Deck;
    using Deck::MakeNewCard;
};

class InspectableRackoDeck : public RackoDeck {
public:
    using RackoDeck::MakeNewCard;
};

#endif