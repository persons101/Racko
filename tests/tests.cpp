#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include "../src/card.h"
#include "../src/Deck.h"
#include "../src/racko.h"
#include "../src/player.h"
#include "../src/rackoCard.h"
#include "../src/rackoDeck.h"



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

TEST_CASE("Player drawing from deck", "[Player][Player::DrawCard][Deck][Deck::GetTopCard]"){
    Deck deck(60,1);
    Player player1("Steve");

    REQUIRE(deck.GetNumCards() == 60);
    REQUIRE(player1.GetCards().size() == 0);

    for (int suit = 1; suit <= 1; suit++)
        for (int cardVal = 1; cardVal <= 60; cardVal++)
            player1.DrawCard(deck.GetTopCard());

    REQUIRE(deck.GetNumCards() == 0);
    REQUIRE(player1.GetCards().size() == 60);
}

TEST_CASE("Trying RackoDeck", "[RackoDeck]"){
    RackoDeck deck;

    REQUIRE(deck.GetNumCards() == 60);
    REQUIRE(deck.GetTopCard());
    //REQUIRE(deck.GetTopCard()/*== SPECIFIC CARD*/)
}