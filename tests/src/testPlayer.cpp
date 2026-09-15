#include "testPlayer.h"

#include "../../src/rackoCard.h"

void TestPlayer::SetCards() {
    std::set<int> myCardVals = GetTestCardVals();
    myCards = {};
    for (auto val : myCardVals) {
        Card* currCard = new RackoCard(val);
        myCards.push_back(currCard);
    }
}

TestPlayer::TestPlayer() : RackoPlayer("Test") {
    SetCards();
}

TestPlayer::TestPlayer(std::string newName) : RackoPlayer(newName) {
    SetCards();
}

std::set<int> TestPlayer::GetTestCardVals()
{
    return std::set<int>({1,5,10,18,20,25,30,35,36,38,40,2,3,4,6 });
}
