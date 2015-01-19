//
//  BuildingCard.h
//  Machiavelli
//
//  Created by Marc Bunkers on 17/12/14.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#ifndef __Machiavelli__BuildingCard__
#define __Machiavelli__BuildingCard__

#include <stdio.h>
#include "Card.h"

class BuildingCard : public Card{
public:
    BuildingCard(string name, int value, CardColor color);
	int getValue();
	void setValue(int value);
//    ~BuildingCard();
private:
    int mValue;
};

#endif /* defined(__Machiavelli__BuildingCard__) */
