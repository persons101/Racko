#include "player.h"

#include <random>
#include <iostream>

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

void Player::PrintCards() const {
    std::cout << name << "'s hand: ";

    std::unordered_set<Card*>::const_iterator it = myCards.begin();
    for (int i = 0; i < myCards.size(); i++){
        std::cout << (*it)->getValue() << (*it)->getSuitSymbol() << " ";
        it++;
    }
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
    auto nh = myCards.extract(*itCardBeingDiscarded);
    return nh.value();
}

Card* Player::DiscardRandomCard() {
    std::unordered_set<Card*>::iterator it = myCards.begin();
    for (int i = 0; i < (rand() % myCards.size()); i++){
        it++;
    }
    return *it;
}
