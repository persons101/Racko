#pragma once

#include "player.h"

class RackoPlayer : public Player {
public:
    RackoPlayer(std::string);

	Card* DiscardCard(int cardPos, Suit suit = (Suit)0) override;
    virtual bool IsCompletedWithRack();
};
