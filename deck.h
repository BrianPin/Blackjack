/*
 *  deck.h
 *  shbj
 *
 *  Created by SHAOHUANG PIN on 9/10/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef __DECK_H__
#define __DECK_H__

#include <vector>
#include "card.h"

const int TOTAL_CARDS = 52;
class Card;
class Deck {
private:
    Card *cards_;
    std::vector<Card*>  in_deck_cards_;

    Deck(const Deck& d);
    Deck& operator=(const Deck& d);
public:
    Deck();
    ~Deck();

    void  init();
    void  shuffle();
    Card* next();
    void  card_back(Card *c);
};
#endif
