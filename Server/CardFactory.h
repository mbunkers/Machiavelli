//
//  CardFactory.h
//  Machiavelli
//
//  Created by Marc Bunkers on 20/01/15.
//  Copyright (c) 2015 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#ifndef __Machiavelli__CardFactory__
#define __Machiavelli__CardFactory__

#include <stdio.h>
#include <sstream>
#include "Assassin.h"
#include "Thief.h"
#include "Magician.h"
#include "King.h"
#include "Preacher.h"
#include "Merchant.h"
#include "Architect.h"
#include "Condottiere.h"
#include "BuildingCard.h"

class CardFactory{
private:
public:
    CardFactory();

    shared_ptr<Card> createCard(string line);
    shared_ptr<Card> createBuildingCard(vector<string> data);
    shared_ptr<Card> createCharacterCard(vector<string> data);
    shared_ptr<Card> createAssassinCard(string name, int priority, CardColor color);
    shared_ptr<Card> createThiefCard(string name, int priority, CardColor color);
    shared_ptr<Card> createMagicianCard(string name, int priority, CardColor color);
    shared_ptr<Card> createKingCard(string name, int priority, CardColor color);
    shared_ptr<Card> createPreacherCard(string name, int priority, CardColor color);
    shared_ptr<Card> createMerchantCard(string name, int priority, CardColor color);
    shared_ptr<Card> createArchitectCard(string name, int priority, CardColor color);
    shared_ptr<Card> createCondottiereCard(string name, int priority, CardColor color);

    vector<string> splittedString(const string line, char delim);
    CardColor colorForString(string stringColor);
};

#endif /* defined(__Machiavelli__CardFactory__) */
