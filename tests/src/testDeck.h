#ifndef TEST_DECK_H
#define TEST_DECK_H

#include "../../src/rackoDeck.h"
#include <unordered_set>

class TestDeck : public RackoDeck {
public:
    TestDeck();
    TestDeck(const std::unordered_set<int>& cardsToInclude);
};

#endif