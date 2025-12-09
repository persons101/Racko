#include "player.h"

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

Card* Player::DiscardCard(Card* cardToDiscard){
    int count = myCards.count(cardToDiscard);
    myCards.erase(cardToDiscard);
    if (myCards.size() != (count - 1))
        return nullptr;
    return cardToDiscard;
}
