#ifndef TEST_PLAYER_H
#define TEST_PLAYER_H

#include "../../src/rackoPlayer.h"

#include <vector>

class TestPlayer : public RackoPlayer {
protected:
    void SetCards();
public:
    using RackoPlayer::center;
    TestPlayer();
    TestPlayer(std::string);
    void SetCards(const std::vector<Card*>&);
    void SetCards(const std::vector<int>&);
    void ClearCards();
    std::vector<int> static GetTestCardVals();
};

#endif