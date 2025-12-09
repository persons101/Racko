#include "player.h"

Player::Player() { *this = Player("Test"); }

Player::Player(std::string inName) : name(inName), score(0) {}

std::string Player::GetName() const { return name; }


int Player::GetScore() const { return score; }

int Player::AddScore(int points) {
    score += points;
    return points;
}


std::vector<Card*> Player::GetCards() const {
    return myCards;
}