//
//  King.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 20/01/15.
//  Copyright (c) 2015 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#include "King.h"

King::King(string name, int priority, CardColor color): CharacterCard(name, priority, color){

}

void King::doSpecialAction(){
    owner()->setKing(true);
}
