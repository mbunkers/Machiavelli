//
//  Condottiere.h
//  Machiavelli
//
//  Created by Marc Bunkers on 20/01/15.
//  Copyright (c) 2015 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#ifndef __Machiavelli__Condottiere__
#define __Machiavelli__Condottiere__

#include <stdio.h>
#include "CharacterCard.h"

class Condottiere : public CharacterCard{
public:
    Condottiere(string name, int priority, CardColor);
    //    ~CharacterCard();
    virtual void doSpecialAction();
	virtual void printOptions();
private:
};

#endif /* defined(__Machiavelli__Condottiere__) */
