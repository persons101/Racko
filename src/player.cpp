#include "player.h"

#include <algorithm>
#include <iostream>
#include <random>

Player::Player() { *this = Player("Test"); }

Player::Player(std::string inName) : name(inName), score(0), numCards(0) {}

std::string Player::GetName() const {
    return name;
}

int Player::GetScore() const {
    return score;
}

int Player::AddScore(int points) {
    score += points;
    return points;
}

Player::~Player()
{
    for (auto card : myCards) {
        delete card;
    }
}

int Player::GetNumCards() const
{
    return numCards; 
}

std::vector<Card *> Player::GetCards() const
{
    return myCards;
}

std::vector<int> Player::GetCardVals() const
{
    std::vector<int> myCardVals;
    for (Card* card : myCards) {
        myCardVals.push_back(card->getValue());
    }
    return myCardVals;
}

std::string Player::PrintCards() const {
    std::string returnVal = "";
    returnVal += name + "'s hand: ";

    for (Card* card : myCards) {
        returnVal += card->PrintCardShort() + " ";
    }

    return returnVal.substr(0, returnVal.size() - 1);
}

bool Player::DrawCard(Card* drawCard) {
    if (drawCard == nullptr) {
        return false;
    }

    myCards.push_back(drawCard);
    numCards++;
    return true;
}

Card* Player::DiscardCard(int discardVal, Suit discardSuit) {
    Card cardToDiscard(discardVal, discardSuit);
    Card::compareCards comparator;

    auto it = std::find_if(
        myCards.begin(),
        myCards.end(),
        [&](Card* card) {
            return !comparator(card, &cardToDiscard) &&
                   !comparator(&cardToDiscard, card);
        });

    if (it == myCards.end()) {
        return nullptr;
    }

    Card* discardedCard = *it;
    myCards.erase(it);
    numCards--;

    return discardedCard;
}

Card* Player::DiscardRandomCard() {
    if (myCards.empty()) {
        return nullptr;
    }

    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());

    std::uniform_int_distribution<std::size_t> distribution(
        0,
        myCards.size() - 1
    );

    auto it = myCards.begin() + distribution(generator);
    Card* discardedCard = *it;

    myCards.erase(it);
    numCards--;

    return discardedCard;
}