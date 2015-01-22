//
//  Card.h
//  Machiavelli
//
//  Created by Marc Bunkers on 17/12/14.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#ifndef __Machiavelli__Card__
#define __Machiavelli__Card__

#include <stdio.h>
#include <string>
using namespace std;

enum CardColor{
    YELLOW = 0,
    BLUE = 1,
    GREEN = 2,
    RED = 3,
    LILA = 4,
    UNKNOWN = 5
};

class Card{
public:
    Card(string name, CardColor color);
//    ~Card();

    CardColor getCardColor();
    string getName();

    string getCardColorString();
    void changeColor(CardColor color);

private:
    string mName;
    CardColor mColor;
};

#endif /* defined(__Machiavelli__Card__) */
