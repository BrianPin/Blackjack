/*
 *  game.cpp
 *  shbj
 *
 *  Created by SHAOHUANG PIN on 9/11/10.
 *  Copyright 2010. All rights reserved.
 *
 */
#include <iostream>
#include <sstream>
#include <vector>
#include "errno.h"
#include "player.h"
#include "deck.h"
#include "card.h"
#include "game.h"

using namespace std;

BlackjackGame::BlackjackGame()
{
    shuffle_counter_ = 0;
}

/* Deck should have all 52 cards in place, shuffled
 * player should clean his queue and ask for cash amount
 * dealer should clean his queue but dont ask for cash amount
 * init once per game
 */
void BlackjackGame::init()
{
    srand(time(NULL));
    deck_.init();
    player_.init();
    dealer_.init();
    shuffle_counter_ = 0;
    round_counter_ = 0;
}


/* When there is not enough cards in deck
 * When there has been over 6 rounds and need to reshuffle
 * All Queues should be put back to empty
 */
void BlackjackGame::deck_reset()
{
    player_.put_card_back(deck_);
    dealer_.put_card_back(deck_);
    deck_.init();
}

/* 0. Should ask for bet
 * 1. if more than 6 rounds, re-shuffle all cards
 * 2. Draw two cards for first dealer then player
 * 3. check blackjack
 */
int BlackjackGame::round_init()
{
    int bet;
    int status;
    bool is_player_blackjack;
    bool is_dealer_blackjack;
    string input;

    if (shuffle_counter_ == 6) {
        deck_reset();
        shuffle_counter_ = 0;
    }

    round_counter_ ++;
    cout << "---------------Round " << round_counter_ << "---------------" << endl;
    cout << "Enter bet(default 100), or (q)uit: ";
    getline(cin, input);
    if (input.size() > 0) {
        if ('q' == input[0]) {
            return PLAYER_QUIT;
        }
        
        stringstream ss(input);
        ss >> bet;
        if (0 < bet) {
            player_.bet(bet);
        } else {
            player_.bet(DEFAULT_BET);
        }
    } else {
        player_.bet(DEFAULT_BET);
    }

redraw:
    status = player_.draw_card(deck_);
    status |= dealer_.draw_card(deck_);
    status |= player_.draw_card(deck_);
    status |= dealer_.draw_card(deck_);
    if (SUCCESS != status) {
        cout << "The deck is running out of cards! Deck reset!" << endl;
        deck_reset();
        goto redraw;
    }

    is_player_blackjack = player_.check_blackjack();
    is_dealer_blackjack = dealer_.check_blackjack();
    
    if (is_player_blackjack && is_dealer_blackjack) {
        cout << "Blackjack PUSH!!" << endl;
        player_.hint();
        dealer_.Player::hint();
        return PUSH;
    } else if (is_player_blackjack && !is_dealer_blackjack) {
        cout << "Player Blackjack!" << endl;
        player_.hint();
        return PLAYER_BLACKJACK;
    } else if (!is_player_blackjack && is_dealer_blackjack) {
        cout << "Dealer Blackjack!" << endl;
        dealer_.Player::hint();
        return DEALER_BLACKJACK;
    } else {
        return SUCCESS;
    }
}

/*
 * 1. ask player to play and show (1) dealer 1 cards (2) player points
 * 2. ask dealer to play and return the results of this round
 * 3. round++ and player winning percentile accumulated and push card to used Q
 */
int BlackjackGame::round_play()
{
    int status;

    dealer_.hint();
    status = player_.play(deck_);
    if (PLAYER_STAND == status) {
        int points = player_.calc_points();
        status = dealer_.play(deck_, points);
    }

    return status;
}

/* Player win or lose the bet
 * Push the card in use to used Queue
 */
void BlackjackGame::round_end(int status)
{
    shuffle_counter_ ++;
    switch (status) {
        case PLAYER_BLACKJACK:
        case PLAYER_WIN:
        case DEALER_BUSTED:
        case PUSH:
            player_.win(status);
            break;
        case DEALER_BLACKJACK:
        case PLAYER_SURRENDER:
        case PLAYER_LOSE:
        case PLAYER_BUSTED:
            player_.lose(status);
            break;
        case PLAYER_QUIT:
            break;
        default:
            cout << "error round_end status " << status << endl;
            break;
    }
    player_.reset_round();
    dealer_.reset_round();
    cout << endl;
    player_.show_stats();
}
