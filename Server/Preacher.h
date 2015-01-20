//
//  Preacher.h
//  Machiavelli
//
//  Created by Marc Bunkers on 20/01/15.
//  Copyright (c) 2015 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#ifndef __Machiavelli__Preacher__
#define __Machiavelli__Preacher__

#include <stdio.h>
#include "CharacterCard.h"

class Preacher : public CharacterCard{
public:
    Preacher(string name, int priority, CardColor);
    //    ~CharacterCard();
private:
};

#endif /* defined(__Machiavelli__Preacher__) */
