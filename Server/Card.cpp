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

string Card::getCardColorString(){
    switch (mColor) {
        case YELLOW:
            return "Yellow";
            break;
        case BLUE:
            return "Blue";
        case GREEN:
            return "Green";
        case RED:
            return "Red";
            case LILA:
            return "Lila";
        default:
            return "Unknown";
            break;
    }
}