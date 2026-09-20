#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include "../src/card.h"
#include "../src/Deck.h"
#include "../src/racko.h"
#include "../src/player.h"
#include "../src/rackoCard.h"
#include "../src/rackoDeck.h"
#include "src/testDeck.h"
#include "src/testPlayer.h"



TEST_CASE("Testing Suit to string", "[Suit],[suit_name]"){
    REQUIRE(Card::suit_name(Suit::SPADES) == "Spades");
    REQUIRE(Card::suit_name(Suit::HEARTS) == "Hearts");
    REQUIRE(Card::suit_name(Suit::DIAMONDS) == "Diamonds");
    REQUIRE(Card::suit_name(Suit::CLUBS) == "Clubs");
}

TEST_CASE("Testing Suit to int", "[Suit]"){
    REQUIRE(static_cast<int>(Suit::SPADES) == 1);
    REQUIRE(static_cast<int>(Suit::HEARTS) == 2);
    REQUIRE(static_cast<int>(Suit::DIAMONDS) == 3);
    REQUIRE(static_cast<int>(Suit::CLUBS) == 4);
}

TEST_CASE("Game class creation", "[Game],[constructor]"){
    Racko game;
}

TEST_CASE_METHOD(Player, "Player class creation", "[Player],[constructor]"){
    SECTION("Player score management"){
        REQUIRE(GetScore() == 0);
        REQUIRE(AddScore(25) == 25);
        REQUIRE(GetScore() == 25);
    }
    SECTION("Player card management"){
        REQUIRE(GetCards().empty());
        
        REQUIRE(DrawCard(new Card(1, Suit::HEARTS)));

        REQUIRE(GetCards().size() == 1);

        for (int i = 2; i <= 10; i++){
            REQUIRE(DrawCard(new Card(i, Suit::HEARTS)));
        }

        REQUIRE(GetCards().size() == 10);

        Card* discardedCard = DiscardCard(5, Suit::HEARTS);

        REQUIRE(discardedCard != nullptr);

        REQUIRE(GetCards().size() == 9);

        discardedCard = DiscardRandomCard();

        REQUIRE(discardedCard != nullptr);
        REQUIRE(discardedCard->getSuit() == HEARTS);
        REQUIRE(GetCards().size() == 8);
    }

    SECTION("Full deck card management"){
        Player player1("Steve");

        for (int suit = 1; suit <= 4; suit++)
            for (int cardVal = 1; cardVal <= 13; cardVal++)
                player1.DrawCard(new Card(cardVal, (Suit)suit));

        REQUIRE(player1.GetCards().size() == 13*4);

        for (int suit = 1; suit <= 4; suit++)
            for (int cardVal = 1; cardVal <= 13; cardVal++)
                player1.DiscardRandomCard();

        REQUIRE(player1.GetCards().size() == 0);
    }
}

TEST_CASE("Player drawing from deck", "[Player][Player::DrawCard][Deck][Deck::PopTopCard]"){
    Deck deck(60,1);
    Player player1("Steve");

    REQUIRE(deck.GetNumCards() == 60);
    REQUIRE(player1.GetCards().size() == 0);

    for (int suit = 1; suit <= 1; suit++)
        for (int cardVal = 1; cardVal <= 60; cardVal++)
            player1.DrawCard(deck.PopTopCard());

    REQUIRE(deck.GetNumCards() == 0);
    REQUIRE(player1.GetCards().size() == 60);
}

TEST_CASE("Trying RackoDeck", "[RackoDeck]"){
    RackoDeck deck;

    REQUIRE(deck.GetNumCards() == 60);

    Card* topCard = deck.PopTopCard();
    REQUIRE(topCard != nullptr);
    REQUIRE( ( (*topCard) == std::make_tuple(1, (Suit)0)) );
}

TEST_CASE("RackoCard vs Card", "[RackoCard][cout]")
{
    /// GOAL: Create RackoCards with values 1, 11, 12, and 13, (usually A, J, Q, K), and confirm they print numbers and not letters. This ensures the inheritance is working

    // Save original buffer
    auto old_buf = std::cout.rdbuf();

    // Redirect cout to a stringstream
    std::stringstream oss;
    std::cout.rdbuf(oss.rdbuf());

    // Execute code that writes to cout
    auto cardValue = GENERATE(1, 11, 12, 13);

    RackoCard rackoCard(cardValue);
    rackoCard.PrintCardShort();


    // Restore original buffer
    std::cout.rdbuf(old_buf);

    // Check the captured output
    REQUIRE(oss.str() == (std::to_string(rackoCard.getValue()) ) );
}

TEST_CASE_METHOD(Racko, "TestPlayer functions", "[TestPlayer][Racko]") {
    TestPlayer p1;
    std::vector<Card*> cards = p1.GetCards(); // 1,5,10,15,20,25,30,35,36,38,40,2,3,4,6 
    REQUIRE(CalculateRackScore(cards) == 11 * 5);
    REQUIRE(p1.AddScore(CalculateRackScore(cards)) == 55);
    REQUIRE(p1.GetScore() == 55);

}

TEST_CASE_METHOD(Racko, "Racko game initialization", "[Racko]") {
    REQUIRE(GetNumTurns() == 0);
    REQUIRE(GetDeckCardCount() == 60);
    REQUIRE(GetDiscardCardCount() == 0);
    REQUIRE(GetPlayerCount() == 0);
}

TEST_CASE_METHOD(Racko, "Racko game score calculation", "[Racko][RackoCard][Score][Player]") {
    // https://www.hasbro.com/common/instruct/Racko%281987%29.PDF
    RackoCard* card1 = new RackoCard(1);
    RackoCard* card2 = new RackoCard(2);
    RackoCard* card3 = new RackoCard(3);
    RackoCard* card4 = new RackoCard(4);
    RackoCard* card5 = new RackoCard(5);
    RackoCard* card6 = new RackoCard(6);

    SECTION("Check Calculations") {
        std::vector<Card*> oneCard = {card6, card2, card3, card5, card4, card1};
        std::vector<Card*> fourCards = {card1, card2, card3, card5, card4, card6};
        std::vector<Card*> fiveCards = {card1, card2, card3, card4, card6, card5};

        REQUIRE(CalculateRackScore(fourCards) == (4 * 5)); 
        REQUIRE(CalculateRackScore(fiveCards) == (5 * 5));

    }

    SECTION("Add Player by name") {
        Player p1("Steve");
        AddPlayer("Steve");
        REQUIRE(GetPlayerCount() == 1);
        REQUIRE(p1.GetName() == GetPlayerByIdx(0)->GetName());
    }

    SECTION("Add Player by reference") {
        Player p1("Steve");
        TestPlayer* p2 = new TestPlayer("Steve");
        AddPlayer(&p1);
        AddPlayer(p2);
        REQUIRE(GetPlayerCount() == 2);

        REQUIRE(p1.GetName() == GetPlayerByIdx(1)->GetName());
    }

    SECTION("One player turn") {
        TestPlayer* p1 = new TestPlayer("Steve"); // {1,5,10,18,20,25,30,35,36,38,40,2,3,4,6 }
        AddPlayer(p1);
        REQUIRE(GetPlayerCount() == 1);

        REQUIRE(p1->GetName() == GetPlayerByIdx(0)->GetName());

        TestDeck* testDeck = new TestDeck(); // top on left: 11,12,13,14,15
        deck = testDeck;
        PlayTurnForPlayerByIdx(0);

        REQUIRE(GetPlayerByIdx(0)->GetCardVals() == std::vector({1,5,10,11,20,25,30,35,36,38,40,2,3,4,6 }));
    }

}


//  Racko::SelectDrawCard() -> 
//  - test when discardPile is empty
//  - check newest card drawn is correct card
//  - test garbage characters
//  - test long strings
//  = test expected behaviors

TEST_CASE_METHOD(Racko, "SelectDrawCard returns error for invalid player",
                 "[Racko][SelectDrawCard]") {
    REQUIRE(SelectDrawCard(-1) == 'e');
}

TEST_CASE_METHOD(Racko, "SelectDrawCard chooses the deck",
                 "[Racko][SelectDrawCard]") {
    AddPlayer("Steve");

    std::istringstream input("0\n");
    std::ostringstream output;

    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());

    char result = SelectDrawCard(0);

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    REQUIRE(result == 'r');
    REQUIRE(output.str().find("Choose a card") != std::string::npos);
}

TEST_CASE_METHOD(Racko, "SelectDrawCard chooses the discard pile",
                 "[Racko][SelectDrawCard]") {
    AddPlayer("Steve");
    discardPile.push(new Card(42, Suit::HEARTS));

    std::istringstream input("1\n");
    std::ostringstream output;

    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());

    char result = SelectDrawCard(0);

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    REQUIRE(result == 'i');
    REQUIRE(output.str().find("Discard") != std::string::npos);
}

TEST_CASE_METHOD(Racko, "SelectDrawCard rejects invalid input before deck choice",
                 "[Racko][SelectDrawCard]") {
    AddPlayer("Steve");

    std::istringstream input("x\n0\n");
    std::ostringstream output;

    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());

    char result = SelectDrawCard(0);

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    REQUIRE(result == 'r');
}

TEST_CASE_METHOD(Racko, "SelectDrawCard rejects discard choice when pile is empty",
                 "[Racko][SelectDrawCard]") {
    AddPlayer("Steve");

    std::istringstream input("1\n0\n");
    std::ostringstream output;

    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());

    char result = SelectDrawCard(0);

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    REQUIRE(result == 'r');
}

TEST_CASE_METHOD(Racko, "SelectDrawCard accepts discard choice when pile is not empty",
                 "[Racko][SelectDrawCard]") {
    AddPlayer("Steve");
    discardPile.push(new Card(25, Suit::CLUBS));

    std::istringstream input("1\n");
    std::ostringstream output;

    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());

    char result = SelectDrawCard(0);

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    REQUIRE(result == 'i');
    REQUIRE(output.str().find("25") != std::string::npos);
}

TEST_CASE_METHOD(Racko, "SelectDrawCard rejects multi-character choices",
                 "[Racko][SelectDrawCard]") {
    AddPlayer("Steve");

    std::istringstream input("10\n0\n");
    std::ostringstream output;

    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());

    char result = SelectDrawCard(0);

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    REQUIRE(result == 'r');
}