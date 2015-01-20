//
//  Merchant.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 20/01/15.
//  Copyright (c) 2015 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#include "Merchant.h"

Merchant::Merchant(string name, int priority, CardColor color): CharacterCard(name, priority, color){

}

void Merchant::doSpecialAction(){
    owner()->addGold(2);
}
