//
//  Assassin.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 20/01/15.
//  Copyright (c) 2015 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#include "Assassin.h"

Assassin::Assassin(string name, int priority, CardColor color): CharacterCard(name, priority, color){

}

void Assassin::doSpecialAction(){
    
}

void Assassin::printOptions(){
	owner()->getSocket()->write("[Kill] Remove a character from the round\n");
}