#include "deck.h"

#include <random>
#include <chrono>
#include <algorithm>

#include "card.h"

// JOKER IS 0-value card


Deck::Deck() {
    *this = Deck(13, 4, 0);
}

Deck::Deck(int numCardsPerSuit, int numSuits){
    *this = Deck(numCardsPerSuit, numSuits, 0);
}


Deck::Deck(int numCardsPerSuit, int numSuits, int numJokers){
    this->numSuits = numSuits;
    numCards = numSuits * numCardsPerSuit + numJokers;

    for (int i = 1; i <= numSuits; i++){
        for (int j = 1; j <= numCardsPerSuit; j++){
            deck.push_back(new Card(j, (Suit)i));
        }
    }
    for (int i = 1; i <= numJokers; i++){
        deck.push_back(new Card(0, (Suit)i));
    }
}

Card* Deck::GetTopCard() {
    Card* topCard = deck.at(0);
    deck.erase(deck.begin());
    return topCard;
    }

Card* Deck::GetBottomCard() {
    Card* bottomCard = deck.at(deck.size() - 1);
    deck.pop_back();
    return bottomCard;
}

void Deck::Shuffle() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    shuffle (deck.begin(), deck.end(), std::default_random_engine(seed));
}

void Deck::PrintDeck() const {
    for (const auto& card : deck) {
        std::cout << "Card Value: " << card->getValue() << ", Suit: " << Card::suit_name(card->getSuit()) << std::endl;
    }
}