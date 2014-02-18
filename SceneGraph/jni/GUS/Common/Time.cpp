//
//  Time.cpp
//  helloGL
//
//  Created by tomato on 10/9/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Time.h"

tp_hd Time::time;
double Time::dT;
tp_hd Time::game_epoch;
const tp_hd Time::END_OF_TIME = tp_hd() + ms(600000000);
const ms Time::UNTIL_END =  ms(600000000);

