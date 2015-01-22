//
//  BuildingCard.h
//  Machiavelli
//
//  Created by Marc Bunkers on 17/12/14.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#ifndef __Machiavelli__BuildingCard__
#define __Machiavelli__BuildingCard__

#include <stdio.h>
#include "Card.h"

class BuildingCard : public Card{
public:
    BuildingCard(string name, int value, CardColor color, string flavorText, int specialFunction);
	int getValue();
	int getFunctionId();
	int getBuildPrice();
	void setValue(int value);
	bool hasSpecialFunction();
	bool isIndestructable();
	//void doSpecialFunction();
	//int getFunctionId();
	string getText();
//    ~BuildingCard();
    int getPrice();

    string formattedString();
private:
	void setFunction(int functionId);
	string mFlavorText;
	int mSpecialFunction;
    int mValue;
	int mPrice;
	bool mIndestructable;
};

#endif /* defined(__Machiavelli__BuildingCard__) */
