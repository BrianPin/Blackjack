/*
 *  player.h
 *  shbj
 *
 *  Created by SHAOHUANG PIN on 9/10/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <vector>
#include "card.h"
#include "deck.h"

const int MIN_BET = 50;
const int DEFAULT_BET = 100;
const int MAX_BET = 5000;
const int DEFAULT_CASH = 10000;
const unsigned INIT_CASH = 500;

/* Player class - Do a real player should do, calculate the cash, make the bet
 * Do the decision
 */
class Card;
class Deck;
class Player {
private:    
    Player(const Player& p);
    Player& operator=(const Player& p);
protected:
    std::vector<Card*> cards_in_use_;
    std::vector<Card*> cards_used_;

    int  played_rounds_;
    int  winning_rounds_;
    int  pushed_rounds_;
    int  blackjack_rounds_;
    int  cash_;
    int  bet_;
public:
    Player() {cash_ = INIT_CASH;}
    virtual ~Player() {}

    virtual void init();
    virtual void reset_round();

    //draw or put back card
    virtual int draw_card(Deck& deck);
    virtual void put_card_back(Deck& deck);

    //points related
    virtual int calc_points();
    virtual int check_blackjack();

    // play action related
    virtual void bet(int amount);
    virtual int play(Deck& deck);
    virtual int play_hit(Deck& deck);
    virtual int play_double(Deck& deck);

    // win/lose and statistics and bets related
    virtual void lose(int result);
    virtual void win(int result);
    virtual void show_stats();

    // more like a debugging function
    virtual void hint();
};

/* Dealer although in some places very different than a player
 * But he is also a player
 */
class Dealer : public Player {
    Dealer(const Dealer&);
    Dealer& operator=(const Dealer&);
public:
    Dealer() {}
    ~Dealer() {}
    void  init();
    int   play(Deck& deck, int player_points);

    // Player has to know one card of dealer's
    void  hint();
};
#endif
