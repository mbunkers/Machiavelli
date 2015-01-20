//
//  King.h
//  Machiavelli
//
//  Created by Marc Bunkers on 20/01/15.
//  Copyright (c) 2015 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#ifndef __Machiavelli__King__
#define __Machiavelli__King__

#include <stdio.h>
#include "CharacterCard.h"

class King : public CharacterCard{
public:
    King(string name, int priority, CardColor);
    //    ~CharacterCard();
    
    virtual void doSpecialAction();
private:
};

#endif /* defined(__Machiavelli__King__) */
