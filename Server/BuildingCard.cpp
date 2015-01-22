//
//  BuildingCard.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 17/12/14.
//  Copyright (c) 2014 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#include "BuildingCard.h"

BuildingCard::BuildingCard(string name, int value, CardColor color, string flavorText, int specialFunction) 
: Card(name, color){
	mPrice = value;
    mValue = value;
	mFlavorText = flavorText;
	setFunction(specialFunction);
	mIndestructable = false;
}

int BuildingCard::getValue(){
	return mValue;
}

string BuildingCard::getText(){
	return mFlavorText;
}

string BuildingCard::formattedString(){
    return getName() + "(" + getCardColorString() + ") cost: " + to_string(getBuildPrice()) + " value: " + to_string(getValue()) + "\n";
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
		mIndestructable = true;
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

bool BuildingCard::isIndestructable(){
	return mIndestructable;
}