//
//  Condottiere.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 20/01/15.
//  Copyright (c) 2015 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#include "Condottiere.h"

Condottiere::Condottiere(string name, int priority, CardColor color): CharacterCard(name, priority, color){
	mHasTarget = false;

}

void Condottiere::doSpecialAction(){
	mHasTarget = true;
}

void Condottiere::printOptions(){
	owner()->getSocket()->write("[Destroy] Destroy your opponent's building for it's build cost - 1\n");
}

bool Condottiere::hasTarget(){
	bool temp = mHasTarget;
	mHasTarget = false;
	return temp;
}