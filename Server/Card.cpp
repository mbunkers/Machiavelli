//
//  Card.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 17/12/14.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#include "Card.h"

Card::Card(string name, CardColor color){
    mName = name;
    mColor = color;
}

string Card::getName(){
    return mName;
}

CardColor Card::getCardColor(){
    return mColor;
}