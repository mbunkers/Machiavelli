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
}
