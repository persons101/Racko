#include "testDeck.h"

#include <set>
#include "testPlayer.h"
#include "../../src/RackoCard.h"

TestDeck::TestDeck() {
    deck = {};
    for (int i = 11; i <= 15; i++) {
        Card* currCard = new RackoCard(i);
        deck.push_back(currCard);
    }

    std::set<int> testPlayerVals = TestPlayer::GetTestCardVals();
    for (int i = 1; i <= 60; i++) {
        if (i >= 11 && i <= 15) {
            continue;
        }
        
        if (testPlayerVals.count(i) > 0) {
            continue;
        }

        Card* currCard = new RackoCard(i);
        deck.push_back(currCard);
    }
}