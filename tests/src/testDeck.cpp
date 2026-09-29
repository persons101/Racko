#include "testDeck.h"

#include <set>
#include <vector>
#include <random>
#include "testPlayer.h"
#include "../../src/RackoCard.h"

TestDeck::TestDeck() {
    deck = {};
    for (int i = 11; i <= 15; i++) {
        Card* currCard = new RackoCard(i);
        deck.push_back(currCard);
    }

    std::vector<int>* testPlayerValsVect = &TestPlayer::GetTestCardVals();
    std::set<int> testPlayerVals = std::set<int>( testPlayerValsVect->begin(), testPlayerValsVect->end() );
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

TestDeck::TestDeck(const std::unordered_set<int>& cardsToInclude)
{
    // Set up a modern random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, cardsToInclude.size() - 1);

    // Advance the iterator to a random position
    // auto it = std::next(cardsToInclude.begin(), dis(gen));
    deck = {};
    for (std::list<int, std::allocator<int>>::const_iterator it = cardsToInclude.begin(); it != cardsToInclude.end(); std::next(cardsToInclude.begin(), dis(gen))) {
        AddCardToDeck(new RackoCard(*it));
    }
}
