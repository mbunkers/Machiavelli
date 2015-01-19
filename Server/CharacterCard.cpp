//
//  CharacterCard.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 17/12/14.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#include "CharacterCard.h"

CharacterCard::CharacterCard(string name, int priority, CardColor color): Card(name, color){
    mPriority = priority;
    mIsTaken = false;
}

bool CharacterCard::hasOwner(){
    if (mCurrentOwner != nullptr){
        return true;
    }
    return false;
}

bool CharacterCard::isTaken(){
    return mIsTaken;
}

void CharacterCard::setIsTaken(bool taken){
    mIsTaken = taken;
}

void CharacterCard::setOwner(shared_ptr<Player> player){
    mCurrentOwner = player;
}

shared_ptr<Player> CharacterCard::owner(){
    return mCurrentOwner;
}