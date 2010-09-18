/*
 *  deck.cpp
 *  shbj
 *
 *  Created by SHAOHUANG PIN on 9/10/10.
 *  Copyright 2010. All rights reserved.
 *
 */
#include <algorithm>
#include "deck.h"

using namespace std;

ptrdiff_t rng(ptrdiff_t i) {
    return rand()%i;
}

ptrdiff_t (*deck_rng)(ptrdiff_t) = rng;

Deck::Deck()
{
    cards_ = new Card[TOTAL_CARDS];
    if (!cards_)
        return;

    for (int i = 0; i < MAX_TYPE_SUIT; i++) {
        for (int j = 0; j < MAX_NUMBER_RANK; j++) {
            cards_[i*MAX_NUMBER_RANK+j].set(i, j+1);
        }
    }
}

Deck::~Deck()
{
    if (cards_) {
        delete[] cards_;
        cards_ = NULL;
    }
}

void Deck::init()
{
    in_deck_cards_.clear();

    for (int i = 0; i < TOTAL_CARDS; i++) {
        Card *c = &cards_[i];
        in_deck_cards_.push_back(c);
    }
    
    shuffle();
}

void Deck::shuffle()
{
    random_shuffle(in_deck_cards_.begin(), in_deck_cards_.end(), deck_rng);
}

/* return the Card of the next draw from the deck
 * but 
 */
Card* Deck::next()
{
    Card *c;

    if (!in_deck_cards_.empty()) {
        c = in_deck_cards_.back();
        in_deck_cards_.pop_back();
    } else {
        c = NULL;
    }

    return c;
}

/*
 * There should be someway for users to return cards
 */
void Deck::card_back(Card *c)
{
    if (c) {
        in_deck_cards_.push_back(c);
    }
}
