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
	bool mFirstFinished;
	bool mIsKing;
    bool mHasDoneTurnAction;
    bool mHasBuild;
	bool mHasDrawnCards;

    vector<shared_ptr<BuildingCard>> mBuiltCards;
    vector<shared_ptr<BuildingCard>> mCardHand;
    shared_ptr<Socket> mSocket;


public:
    enum states{
        CHOOSECARD = 0,
        DISCARDCARD = 1,
        IDLE = 2,
    };

    enum states mState = IDLE;

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
    bool buildCard(shared_ptr<BuildingCard> card);
    vector<shared_ptr<BuildingCard>> cardHand();
    vector<shared_ptr<BuildingCard>> builtCards();

	void setFirstFinished();
    void setState(enum states newState);
    void setHasDoneTurnAction(bool state);
    bool hasDoneTurnAction();
    void setHasBuild(bool hasBuilt);
    bool hasBuild();
	void setHasDrawnCards(bool hasDrawn);
	bool hasDrawnCards();
	void removeCard(int index);

	size_t getNumberOfBuildings();
	bool isKing();
	void setKing(bool isKing);

    void removeGold(int gold);

    int goldForCardColor(CardColor color);
};

#endif /* defined(__Machiavelli__Player__) */
