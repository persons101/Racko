#include "testDeck.h"

#include <set>
#include <algorithm>
#include <vector>
#include <random>
#include "testPlayer.h"
#include "../../src/RackoCard.h"

TestDeck::TestDeck() {
    deck = {}; // acceptable because numCards starts and ends at 60
    for (int i = 11; i <= 15; i++) {
        Card* currCard = new RackoCard(i);
        deck.push_back(currCard);
    }

    std::vector<int> testPlayerValsVect = TestPlayer::GetTestCardVals();
    std::set<int> testPlayerVals = std::set<int>( testPlayerValsVect.begin(), testPlayerValsVect.end() );
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
    while (!deck.empty()) {
        this->PopTopCard();
    }

    std::vector<int> shuffledCards = std::vector<int>(cardsToInclude.begin(), cardsToInclude.end());
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(shuffledCards.begin(), shuffledCards.end(), gen);

    for (int cardValue : shuffledCards) {
        AddCardToDeck(new RackoCard(cardValue));
    }
}

TestDeck::TestDeck(const std::vector<int> &cardsToInclude)
{
    while (!deck.empty()) {
        this->PopTopCard();
    }

    for (int cardValue : cardsToInclude) {
        AddCardToDeck(new RackoCard(cardValue));
    }
}
