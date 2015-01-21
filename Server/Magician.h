//
//  Magician.h
//  Machiavelli
//
//  Created by Marc Bunkers on 20/01/15.
//  Copyright (c) 2015 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#ifndef __Machiavelli__Magician__
#define __Machiavelli__Magician__

#include <stdio.h>
#include "CharacterCard.h"

class Magician : public CharacterCard{
public:
    Magician(string name, int priority, CardColor);
	void printOptions();
    //    ~CharacterCard();
private:
};

#endif /* defined(__Machiavelli__Magician__) */
