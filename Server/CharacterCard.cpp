//
//  CharacterCard.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 17/12/14.
//  Copyright (c) 2014 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#include "CharacterCard.h"


CharacterCard::CharacterCard(string name, int priority, CardColor color): Card(name, color){
    mPriority = priority;
    mIsTaken = false;
    mIsUsingAction = false;
    mHasUsedAction = false;
    mIsBeingRobbed = false;
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

int CharacterCard::priority(){
    return mPriority;
}

void CharacterCard::doSpecialAction(){
    owner()->getSocket()->write("This character doesn't have an special ability\n");
}

void CharacterCard::printOptions(){
	//keep empty
}

void CharacterCard::setHasUsedAction(bool hasUsedAction){
    mHasUsedAction = hasUsedAction;
    if (mHasUsedAction){
        mIsUsingAction = false;
    }
}

bool CharacterCard::hasUsedAction(){
    return mHasUsedAction;
}

void CharacterCard::setIsUsingAction(bool isUsingAction){
    mIsUsingAction = isUsingAction;
}

bool CharacterCard::isUsingAction(){
    return mIsUsingAction;
}

void CharacterCard::setIsBeingRobbed(bool isBeingRobbed){
    mIsBeingRobbed = isBeingRobbed;
}

bool CharacterCard::isBeingRobbed(){
    return mIsBeingRobbed;
}
