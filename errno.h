/*
 *  errno.h
 *  shbj
 *
 *  Created by SHAOHUANG PIN on 9/11/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef __ERRNO_H__
#define __ERRNO_H__

/* Status code
 */
enum {
    SUCCESS = 0,
    ENOMEM = 1,
    EDECK_RUN_OUT_CARD = 2,
    ENO_CARD_IN_HAND = 3,
    EUNKNOWN_INPUT = 4,

    PLAYER_BLACKJACK,
    DEALER_BLACKJACK,
    SAFE_HIT,
    PLAYER_BUSTED,
    DEALER_BUSTED,
    PUSH,
    PLAYER_WIN,
    PLAYER_WIN_21,
    PLAYER_LOSE,
    PLAYER_STAND,
    PLAYER_SURRENDER,
    PLAYER_QUIT,
};

#endif
