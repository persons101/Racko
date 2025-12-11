#include "deck.h"

#include <random>
#include <chrono>
#include <algorithm>
#include <iostream>

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

    AddCardsToDeck(numCardsPerSuit, numSuits);
    AddJokersToDeck(numJokers);
}

Card* Deck::MakeNewCard(int cardValue, Suit cardSuit){
    return new Card(cardValue, cardSuit);
}

void Deck::AddCardsToDeck(int numCardsPerSuit, int numSuits){
    for (int i = 1; i <= numSuits; i++){
        for (int j = 1; j <= numCardsPerSuit; j++){
            deck.push_back(this->MakeNewCard(j,(Suit)i));
        }
    }
    numCards += numSuits * numCardsPerSuit;
}

void Deck::AddJokersToDeck(int numJokers){
    for (int i = 1; i <= numJokers; i++){
        deck.push_back(MakeNewCard(0, (Suit)i));
    }
    numCards += numJokers;
}

Card* Deck::GetTopCard() {
    if (deck.size() == 0)
        return nullptr;

    Card* topCard = new Card(*deck.at(0));
    deck.erase(deck.begin());
    return topCard;
}

Card* Deck::GetBottomCard() {
    if (deck.size() == 0)
        return nullptr;

    Card* bottomCard = new Card(*(deck.at(deck.size() - 1)));
    deck.pop_back();
    return bottomCard;
}

void Deck::Shuffle() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    shuffle (deck.begin(), deck.end(), std::default_random_engine(seed));
}

void Deck::PrintDeck() const {
    std::cout << "---Start Deck---\n";
    for (const auto& card : deck) {
        card->PrintCardShort();
    }
    std::cout << "\n";
    std::cout << "--- End Deck ---\n";
}