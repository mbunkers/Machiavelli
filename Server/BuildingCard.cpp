//
//  BuildingCard.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 17/12/14.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#include "BuildingCard.h"

BuildingCard::BuildingCard(string name, int value, CardColor color): Card(name, color){
    mValue = value;
}

