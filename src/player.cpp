#include "player.h"

#include <random>

Player::Player() { *this = Player("Test"); }

Player::Player(std::string inName) : name(inName), score(0) {}

std::string Player::GetName() const { return name; }


int Player::GetScore() const { return score; }

int Player::AddScore(int points) {
    score += points;
    return points;
}


std::unordered_set<Card*> Player::GetCards() const {
    return myCards;
}

bool Player::DrawCard(Card* drawCard){
    myCards.insert(drawCard);
    return true;
}

Card* Player::DiscardCard(int discardVal, Suit discardSuit){
    Card cardToDiscard(discardVal, discardSuit);
    std::unordered_set<Card*>::const_iterator itCardBeingDiscarded = myCards.find(&cardToDiscard);
    if (itCardBeingDiscarded == myCards.end())
        return nullptr;
    myCards.erase(*itCardBeingDiscarded);
    return *itCardBeingDiscarded;
}

Card* Player::DiscardRandomCard() {
    std::unordered_set<Card*>::iterator it = myCards.begin();
    for (int i = 0; i < (rand() % myCards.size()); i++){
        it++;
    }
    return *it;
}
