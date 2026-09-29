#include "testRacko.h"

#include <set> // std::set is an ORDERED SET
#include "testPlayer.h"

void TestRacko::SetupGame()
{
    Player* p1 = new TestPlayer("Jessie");
    Player* p2 = new TestPlayer("Benny");
    Player* p3 = new TestPlayer("John");


    std::set<int> usedCardVals = {};
    const std::vector<int> p1CardVals = {31,32,33,34,35,6,7,8,9,10};
    const std::vector<int> p2CardVals = {21,22,23,24,25,16,17,18,19,20};
    const std::vector<int> p3CardVals = {1,2,3,4,5,56,57,58,59,30};

    usedCardVals.insert(p1CardVals.begin(), p1CardVals.end());
    usedCardVals.insert(p2CardVals.begin(), p2CardVals.end());
    usedCardVals.insert(p3CardVals.begin(), p3CardVals.end());

    dynamic_cast<TestPlayer*>(p1)->SetCards(p1CardVals);
    dynamic_cast<TestPlayer*>(p2)->SetCards(p2CardVals);
    dynamic_cast<TestPlayer*>(p3)->SetCards(p3CardVals);

    AddPlayer(p1);
    AddPlayer(p2);
    AddPlayer(p3);
}
