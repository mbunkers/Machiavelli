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
    BuildingCard(string name, CardColor color);
    ~BuildingCard();
private:
};

#endif /* defined(__Machiavelli__BuildingCard__) */
