//
//  Architect.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 20/01/15.
//  Copyright (c) 2015 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#include "Architect.h"

Architect::Architect(string name, int priority, CardColor color): CharacterCard(name, priority, color){
	mBuildCount = 0;
	mBuildlimit = 3;
}

void Architect::doSpecialAction(){
	mBuildCount++;
}

bool Architect::buildLimitReached(){
	if (mBuildCount == mBuildlimit){
		mBuildCount = 0;
		return true;
	}
	return false;
}