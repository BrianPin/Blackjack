/*
 *  card.h
 *  shbj
 *
 *  Created by SHAOHUANG PIN on 9/10/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef __CARD_H__
#define __CARD_H__
#include "player.h"

typedef enum Rank_e {
    ACE = 1,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NIGH,
    TEN,
    JACK,
    QUEEN,
    KING,
} Rank;
const int MAX_NUMBER_RANK = 13;

typedef enum Suit_e {
    SPADES,
    HEARTS,
    DIAMONDS,
    CLUBS,
} Suit;
const int MAX_TYPE_SUIT = 4;

class Player;

class Card {
private:
    int rank_;
    int suit_;
    Player *player_;
    Card(const Card& c) {}
    Card& operator=(const Card& c);
    void set(int suit, int rank) {suit_ = suit; rank_ = rank;}

public:
    Card () {rank_ = -1; suit_ = -1; player_ = NULL;}
    Card (int suit, int rank) {suit_ = suit; rank_ = rank; player_ = NULL;}
    virtual ~Card() {}

    int get_rank() {return rank_;}
    int get_suit() {return suit_;}

    friend class Deck;

    void set_owner(Player &player) {player_ = &player;}
    void remove_owner() {player_ = NULL;}

    void show();
};

#endif
