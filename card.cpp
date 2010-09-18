/*
 *  card.cpp
 *  shbj
 *
 *  Created by SHAOHUANG PIN on 9/10/10.
 *  Copyright 2010. All rights reserved.
 *
 */
#include <iostream>
#include <iomanip>
#include "card.h"

using namespace std;

static string rank_name[] = {
    "DUMMY",
    "ACE",
    "2", "3", "4", "5", "6", "7", "8", "9", "10",
    "JACK", "QUEEN", "KING",
};

static string suit_name[] = {
    "SPADES",
    "HEARTS",
    "DIAMONDS",
    "CLUBS",
};

void Card::show()
{
    cout << suit_name[suit_] << "(" << rank_name[rank_] << "), ";
}

