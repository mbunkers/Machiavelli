//
//  Thief.h
//  Machiavelli
//
//  Created by Marc Bunkers on 20/01/15.
//  Copyright (c) 2015 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#ifndef __Machiavelli__Thief__
#define __Machiavelli__Thief__

#include <stdio.h>
#include "CharacterCard.h"

class Thief : public CharacterCard{
public:
    Thief(string name, int priority, CardColor);
    //    ~CharacterCard();
    virtual void doSpecialAction();
private:
};

#endif /* defined(__Machiavelli__Thief__) */
