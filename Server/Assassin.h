//
//  Assassin.h
//  Machiavelli
//
//  Created by Marc Bunkers on 20/01/15.
//  Copyright (c) 2015 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#ifndef __Machiavelli__Assassin__
#define __Machiavelli__Assassin__

#include <stdio.h>
#include "CharacterCard.h"

class Assassin : public CharacterCard{
public:
    Assassin(string name, int priority, CardColor);
    //    ~CharacterCard();
    virtual void doSpecialAction();
private:
};

#endif /* defined(__Machiavelli__Assassin__) */
