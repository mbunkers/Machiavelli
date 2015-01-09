//
//  CharacterCard.h
//  Machiavelli
//
//  Created by Marc Bunkers on 17/12/14.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#ifndef __Machiavelli__CharacterCard__
#define __Machiavelli__CharacterCard__

#include <stdio.h>
#include "Card.h"

class CharacterCard : public Card{
public:
    CharacterCard(string name, int priority, CardColor);
    ~CharacterCard();
private:
    int mPriority;
};

#endif /* defined(__Machiavelli__CharacterCard__) */
