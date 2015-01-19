//
//  Player.h
//  Machiavelli
//
//  Created by Marc Bunkers on 14/12/14.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#ifndef __Machiavelli__Player__
#define __Machiavelli__Player__

#include <stdio.h>
#include <string>
#include <vector>
#include "Socket.h"
#include <memory>
#include "BuildingCard.h"

using namespace std;

class Player{
private:
    string mName;
    int mGold;
    vector<shared_ptr<BuildingCard>> mBuiltCards;
    vector<shared_ptr<BuildingCard>> mCardHand;
    shared_ptr<Socket> mSocket;
public:
    Player(string name, shared_ptr<Socket> socket);
    ~Player();

    bool isPlayer(shared_ptr<Socket> socket);
    shared_ptr<Socket> getSocket();
    string getName();
    int calculateScore();
    void addGold(int goldToAdd);
    void setGold(int newGold);
    int gold();
    void addCardToHand(shared_ptr<BuildingCard> card);
    vector<shared_ptr<BuildingCard>> cardHand();
    vector<shared_ptr<BuildingCard>> builtCards();
};

#endif /* defined(__Machiavelli__Player__) */
