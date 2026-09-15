#include "rackoPlayer.h"

RackoPlayer::RackoPlayer(std::string newName) : Player(name) {}

Card* RackoPlayer::DiscardCard(int cardPos, Suit suit)
{
	Card* discardedCard = myCards.at(cardPos);
	myCards.erase(myCards.begin() + cardPos);
	return discardedCard;
}

