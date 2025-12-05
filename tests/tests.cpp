#include "../src/Card.cpp"
#include "../src/Deck.cpp"
#include "../src/Game.cpp"
#include "../src/Player.cpp"

#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"


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
    Game game;
}

TEST_CASE_METHOD(Player, "Player class creation", "[Player],[constructor]"){
    SECTION("Player score management"){
        REQUIRE(GetScore() == 0);
        REQUIRE(AddScore(25) == 25);
        REQUIRE(GetScore() == 25);
    }
    SECTION("Player card management"){
        REQUIRE(GetCards().empty());
    }
}