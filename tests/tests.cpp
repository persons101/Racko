#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include "../src/Suit.h"
#include "../src/Card.h"
#include "../src/Deck.h"


TEST_CASE("Testing Suit to string", "[Suit],[suit_name]"){
    REQUIRE(suit_name(Suit::SPADES) == "Spades");
    REQUIRE(suit_name(Suit::HEARTS) == "Hearts");
    REQUIRE(suit_name(Suit::DIAMONDS) == "Diamonds");
    REQUIRE(suit_name(Suit::CLUBS) == "Clubs");
}

TEST_CASE("Testing Suit to int", "[Suit],[(int)Suit]"){
    REQUIRE(static_cast<int>(Suit::SPADES) == 1);
    REQUIRE(static_cast<int>(Suit::HEARTS) == 2);
    REQUIRE(static_cast<int>(Suit::DIAMONDS) == 3);
    REQUIRE(static_cast<int>(Suit::CLUBS) == 4);
}

