//
//  BuildingCard.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 17/12/14.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#include "BuildingCard.h"

BuildingCard::BuildingCard(string name, int value, CardColor color, string flavorText, int specialFunction) 
: Card(name, color){
	mPrice = value;
    mValue = value;
	mFlavorText = flavorText;
	setFunction(specialFunction);
}

int BuildingCard::getValue(){
	return mValue;
}

string BuildingCard::getText(){
	return mFlavorText;
}

bool BuildingCard::hasSpecialFunction(){
	bool hasFunction = false;
	if (mSpecialFunction > 0){
		hasFunction = true;
	}
	return hasFunction;
}

void BuildingCard::setFunction(int functionId){
	switch (functionId){
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		mSpecialFunction = functionId;
		break;
	case 9:
		mValue += 2;
		break;
	default:
		mSpecialFunction = 0;
		break;
	}
}

int BuildingCard::getBuildPrice(){
	return mPrice;
}