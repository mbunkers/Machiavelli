//
//  Thief.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 20/01/15.
//  Copyright (c) 2015 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#include "Thief.h"

Thief::Thief(string name, int priority, CardColor color): CharacterCard(name, priority, color){

}

void Thief::doSpecialAction(){

}

void Thief::printOptions(){
	owner()->getSocket()->write("[Steal] target a character and steal from him when it's his turn\n");
}