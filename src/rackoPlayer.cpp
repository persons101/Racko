#include "rackoPlayer.h"

RackoPlayer::RackoPlayer(std::string newName) : Player(name) {}

Card* RackoPlayer::DiscardCard(int cardPos, Suit suit)
{
	Card* discardedCard = myCards.at(cardPos);
	myCards.erase(myCards.begin() + cardPos);
	return discardedCard;
}

bool RackoPlayer::IsCompletedWithRack()
{
    bool hasProblem = false;

    for (int i = 0; i < myCards.size() - 1 && !hasProblem; i++) {
        if (!(myCards.at(i + 1) > myCards.at(i)))
            hasProblem == true;
    }

    return !hasProblem;
}
