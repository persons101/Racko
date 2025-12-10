#include "player.h"

#include <random>
#include <iostream>

Player::Player() { *this = Player("Test"); }

Player::Player(std::string inName) : name(inName), score(0), numCards(0) {}

std::string Player::GetName() const { return name; }


int Player::GetScore() const { return score; }

int Player::AddScore(int points) {
    score += points;
    return points;
}


std::set<Card*, Card::compareCards> Player::GetCards() const {
    return myCards;
}

void Player::PrintCards() const {
    std::cout << name << "'s hand: ";

    for (Card* card : myCards){
        card->PrintCardShort();
    }
    std::cout << "\n";
}

bool Player::DrawCard(Card* drawCard){
    myCards.insert(drawCard);
    numCards++;
    return true;
}

Card* Player::DiscardCard(int discardVal, Suit discardSuit){
    Card cardToDiscard(discardVal, discardSuit);
    std::set<Card*>::const_iterator itCardBeingDiscarded = myCards.find(&cardToDiscard);
    if (itCardBeingDiscarded == myCards.end())
        return nullptr;

    auto nh = myCards.extract(*itCardBeingDiscarded);
    numCards--;
    
    return nh.value();
}

Card* Player::DiscardRandomCard() {
    std::set<Card*>::iterator it = myCards.begin();
    for (int i = 0; i < (rand() % myCards.size()); i++){
        it++;
    }

    auto nh = myCards.extract(it);
    numCards--;
    
    return nh.value();
}
