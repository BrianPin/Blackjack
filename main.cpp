/*
 * Project name: blackjack command line game
 * Author: Shao Huang Pin (Brian)
 * Date: 2010-09-10
 */
#include <iostream>
#include "errno.h"
#include "game.h"

static BlackjackGame game;

int main (int argc, char * const argv[]) {
    int status;

    game.init();
    do {
        status = game.round_init();
        // When status is not SUCCESS
        // it means either someone has blackjack or both have blackjack
        // SUCCESS means continue to play
        if (SUCCESS != status) {
            game.round_end(status);
            if (PLAYER_QUIT == status) {
                break;
            }
            continue;
        }

        status = game.round_play();
        game.round_end(status);
        if (PLAYER_QUIT == status)
            break;
    } while (1);

    return 0;
}
