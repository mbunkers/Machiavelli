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
#include "Player.h"

class CharacterCard : public Card{
public:
    CharacterCard(string name, int priority, CardColor);
//    ~CharacterCard();
    bool hasOwner();
    bool isTaken();
    void setIsTaken(bool taken);
    void setOwner(shared_ptr<Player> player);
    shared_ptr<Player> owner();

private:
    int mPriority;
    bool mIsTaken;
    shared_ptr<Player> mCurrentOwner;
};

#endif /* defined(__Machiavelli__CharacterCard__) */
