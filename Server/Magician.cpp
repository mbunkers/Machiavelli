//
//  Magician.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 20/01/15.
//  Copyright (c) 2015 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#include "Magician.h"

Magician::Magician(string name, int priority, CardColor color): CharacterCard(name, priority, color){

}

void Magician::printOptions(){
	owner()->getSocket()->write("[Swap Hand] Swap your buildingcards with your opponent\n");
	owner()->getSocket()->write("[Swap Deck] Swap your buildingcards with an equal amount from the deck\n");
}