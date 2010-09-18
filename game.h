/*
 *  game.h
 *  shbj
 *
 *  Created by SHAOHUANG PIN on 9/11/10.
 *  Copyright 2010. All rights reserved.
 *
 */
#ifndef __GAME_H__
#define __GAME_H__

#include "card.h"
#include "deck.h"
#include "player.h"

class BlackjackGame {
private:
    int     round_counter_;
    int     shuffle_counter_;
    Deck    deck_;
    Dealer  dealer_;
    Player  player_;

    BlackjackGame(const BlackjackGame&);
    BlackjackGame& operator=(const BlackjackGame&);
    void  deck_reset();
public:
    BlackjackGame();
    ~BlackjackGame() {}

    void  init();
    int   round_init();
    int   round_play();
    void  round_end(int status);
};
#endif
