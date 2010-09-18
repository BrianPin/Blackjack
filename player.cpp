/*
 *  player.cpp
 *  shbj
 *
 *  Created by SHAOHUANG PIN on 9/10/10.
 *  Copyright 2010. All rights reserved.
 *
 */
#include <iostream>
#include <iomanip>
#include <sstream>
#include <typeinfo>
#include "errno.h"
#include "player.h"

using namespace std;

/* Return 0 for success,
 * Return <0 if error happens
 */
int Player::draw_card(Deck& deck)
{
    Card* c = deck.next();
    
    if (!c) {
        return -EDECK_RUN_OUT_CARD;
    }

    cards_in_use_.push_back(c);
    return SUCCESS;
}

/* We dont rendomly return cards, we return in a whole.
 * Return all cards either in use or used
 * Time is when Deck run out of cards or needs to reset/reshuffle
 */
void Player::put_card_back(Deck& deck)
{
    Card *c;

    if (!cards_in_use_.empty()) {
        vector<Card*>::iterator i = cards_in_use_.begin();
        while (i != cards_in_use_.end()) {
            c = *i;
            c->remove_owner();
            deck.card_back(c);
            i = cards_in_use_.erase(i);
        }
    }

    if (!cards_used_.empty()) {
        vector<Card*>::iterator i = cards_used_.begin();
        while (i != cards_used_.end()) {
            c = *i;
            c->remove_owner();
            deck.card_back(c);
            i = cards_used_.erase(i);
        }
    }
}

void Player::reset_round()
{
    if (!cards_in_use_.empty()) {
        vector<Card*>::iterator i = cards_in_use_.begin();
        while (i != cards_in_use_.end()) {
            Card *c = *i;
            i = cards_in_use_.erase(i);
            cards_used_.push_back(c);
        }
    }
}

/*
 * For some of the debugging and inspecting purposes
 * This hint() shows all the card that player holds on hand
 */
void Player::hint()
{
    vector<Card*>::const_iterator ci = cards_in_use_.begin();
    if (typeid(*this) == typeid(Player)) {
        cout << "Player has: ";
    } else {
        cout << "Dealer has: ";
    }
    
    while (ci != cards_in_use_.end()) {
        Card *c = *ci;
        c->show();
        ci++;
    }
    cout << "points: " << calc_points();
    cout << endl;
}

/* Return the points, 0 if no cards in hand
 */
int Player::calc_points()
{
    int ace_count = 0;
    int points = 0;

    if (cards_in_use_.empty()) {
        return 0;
    }

    vector<Card*>::const_iterator ci = cards_in_use_.begin();
    
    while (ci != cards_in_use_.end()) {
        int r = (*ci)->get_rank();
        switch (r) {
            case TWO:
            case THREE:
            case FOUR:
            case FIVE:
            case SIX:
            case SEVEN:
            case EIGHT:
            case NIGH:
            case TEN:
                points += r;
                break;
            case JACK:
            case QUEEN:
            case KING:
                points += 10;
                break;
            case ACE:
                ace_count ++;
                break;
            default:
                // throw exception
                break;
        }
        ci++;
    }
    
    switch (ace_count) {
        case 0:
            break;
        case 1:
            if (points > 10)
                points += 1;
            else
                points += 11;
            break;
        case 2:
        case 3:
        case 4:
            points += ace_count;
            break;
        default:
            // throw exceptions
            break;
    }

    return points;
}

/*
 * Hit(Draw) the next card from deck
 * Return status code, <0 for unusual case
 */
int Player::play_hit(Deck& deck)
{
    int result;
    int points;

    result = draw_card(deck);
    if (SUCCESS != result) {
        return result;
    }
    
    points = calc_points();
    if (21 < points) {
        result = PLAYER_BUSTED;
        Card *card = cards_in_use_.back();
        card->show();
        cout << "Player busted!!!" << endl;
    } else {
        result = SAFE_HIT;
    }

    return result;
}


/*
 * Double and make one hit(Draw) the next card from deck
 * Return status code, <0 for unusual case
 * and double the bet
 */
int Player::play_double(Deck& deck)
{
    int ret;
    
    bet_ *= 2;
    ret = play_hit(deck);
    if (SAFE_HIT == ret) {
        Card *card = cards_in_use_.back();
        card->show();
        cout << " Points:" << calc_points() << endl;
        ret = PLAYER_STAND;
    }
    
    return ret;
}

/*
 * play - asking for players decision
 * return <0 when error
 * usually return status code: BUSTED, SAFE_HIT, ...
 */
int Player::play(Deck& deck)
{
    string input;
    int play_result;

    do {
        hint();
        cout << "select: (h)it, ((s))tand, (d)ouble, su(r)render or (q)uit: ";
        getline(cin, input);
        switch (input[0]) {
            case 'h':
                play_result = play_hit(deck);
                break;
            case 'd':
                play_result = play_double(deck);
                break;
            case 'r':
                play_result = PLAYER_SURRENDER;
                break;
            case 'q':
                play_result = PLAYER_QUIT;
                break;
            default:
            case 's':
                play_result = PLAYER_STAND;
                break;
        }
    } while (SAFE_HIT == play_result);
    
    return play_result;
}

/* return true if cuurently in blackjack condition
 * otherwise false
 */
int Player::check_blackjack()
{
    int points;
    
    points = calc_points();
    if (points == 21 && cards_in_use_.size() == 2)
        return true;
    else
        return false;
}


/*
 * The Game should ask for player's bet in the beginning of each round
 * the bet is having a limit of MAX_BET
 */
void Player::bet(int newbet)
{
    if (newbet > cash_ && cash_ > MAX_BET) {
        newbet = DEFAULT_BET;
    }

    if (newbet < MIN_BET) {
        newbet = MIN_BET;
    }

    if (newbet > cash_) {
        cout << "Player play with borrowed money: " << (newbet - cash_) << endl;
        cash_ = newbet;
    }
    
    bet_ = newbet;
    cout << "Playing with bet: " << bet_ << endl;
}

void Player::win(int result)
{
    switch (result) {
        case PLAYER_BLACKJACK:
            cash_ += (bet_ * 3)/2;
            blackjack_rounds_ ++;
            winning_rounds_ ++;
            break;

        case PLAYER_WIN:
        case DEALER_BUSTED:
            cash_ += bet_;
            winning_rounds_ ++;
            break;

        case PUSH:
            pushed_rounds_ ++;
            break;
        default:
            break;
    }
    played_rounds_ ++;
}

void Player::lose(int result)
{
    switch (result) {
        case DEALER_BLACKJACK:
            cash_ -= (bet_ * 3)/2;
            break;
        case PLAYER_LOSE:
        case PLAYER_BUSTED:
            cash_ -= bet_;
            break;
        case PLAYER_SURRENDER:
            cash_ -= bet_ / 2;
            break;

        default:
            break;
    }
    played_rounds_ ++;
}

void Player::show_stats()
{
    cout << "Played("      << played_rounds_ << "),";
    cout << "Won("         << winning_rounds_ << "),";
    cout << "Pushed("      << pushed_rounds_ << "),";
    cout << "Blackjack-ed("   << blackjack_rounds_ << ")";
    cout << endl;
    cout << "Cash left("   << cash_ << ") ";
    if (played_rounds_ > 0) {
        float pct = static_cast<float>(winning_rounds_) / played_rounds_;
        cout << "Winning ratio(" << setiosflags(ios::fixed) << setprecision(2) << pct << ")";
    }
    cout << endl;
}

void Player::init()
{
    string input;

    cards_in_use_.clear();
    cards_used_.clear();
    played_rounds_ = 0;
    winning_rounds_ = 0;
    pushed_rounds_ = 0;
    blackjack_rounds_ = 0;
    cash_ = 0;
    bet_ = 0;
    cout << "Input player cash amount (default cash will give 10000):";
    getline(cin, input);
    istringstream ss(input);
    ss >> cash_;
    if (cash_ <= 0) {
        cash_ = DEFAULT_CASH;
    }
    cout << "You have " << cash_ << " amount of cash" << endl;
}

void Dealer::init()
{
    cards_in_use_.clear();
    cards_used_.clear();
    played_rounds_ = 0;
    winning_rounds_ = 0;
    pushed_rounds_ = 0;
    blackjack_rounds_ = 0;
    cash_ = 0;
    bet_ = 0;
}
/*
 * Dealer must obey soft-17
 * Dealer must know the player points
 * the return value is one among: BUSTED, PUSH, WIN
 */
int Dealer::play(Deck& deck, int player_points)
{
    int ret;
    int i = 2;
    Card *card;
    int points = calc_points();

    cout << "Dealer's turn:" << endl;
    card = cards_in_use_[0];
    card->show();
    card = cards_in_use_[1];
    card->show();
    while (17 > points) {
        if (SUCCESS != (ret = draw_card(deck))) {
            break;
        }
        card = cards_in_use_[i++];
        card->show();
        points = calc_points();
    }
    cout << "Dealer points: " << calc_points();
    
    if (21 < points) {
        ret = DEALER_BUSTED;
        cout << " Busted!";
    } else if (player_points < points) {
        ret = PLAYER_LOSE;
        cout << " Dealer Win!";
    } else if (player_points == points) {
        ret = PUSH;
        cout << " PUSH!";
    } else if (player_points > points) {
        ret = PLAYER_WIN;
        cout << " Dealer lose!";
    }
    cout << endl;

    return ret;
}

/* This Dealer hint() only hints the first card in dealer's hand
 *
 */
void Dealer::hint()
{
    Card *c;

    if (cards_in_use_.size() != 0) {
        c = cards_in_use_[0];
        cout << "Dealer hint: ";
        c->show();
        cout << endl;
    }
}
