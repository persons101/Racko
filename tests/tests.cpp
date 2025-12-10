#include "../src/card.h"
#include "../src/Deck.h"
#include "../src/racko.h"
#include "../src/player.h"

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
}