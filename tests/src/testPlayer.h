#ifndef TEST_PLAYER_H
#define TEST_PLAYER_H

#include "../../src/rackoPlayer.h"

#include <set>

class TestPlayer : public RackoPlayer {
protected:
    void SetCards();
public:
    using RackoPlayer::center;
    TestPlayer();
    TestPlayer(std::string);
    std::set<int> static GetTestCardVals();
};

#endif