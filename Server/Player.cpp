//
//  Player.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 14/12/14.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#include "Player.h"
#include <algorithm>

Player::Player(string name, shared_ptr<Socket> socket){
    mName = name;
    mSocket = socket;
    mGold = 0;
	mHasBuild = false;
	mFirstFinished = false;
    mIsKing = false;
    mHasDoneTurnAction = false;
	mHasDrawnCards = false;
}

Player::~Player(){

}

bool Player::isPlayer(shared_ptr<Socket> socket){
    return mSocket == socket;
}

shared_ptr<Socket> Player::getSocket(){
    return mSocket;
}

string Player::getName(){
    return mName;
}

int Player::calculateScore(){
	int score = 0;
	vector<CardColor> colors;
	colors.push_back(LILA);
	colors.push_back(BLUE);
	colors.push_back(GREEN);
	colors.push_back(YELLOW);
	colors.push_back(RED);

	if (mBuiltCards.size() > 7){
		//2 points if player have 8 or more buildings.
		score += 2;
		if (mFirstFinished){
			//an extra 2 points if player finished first
			score += 2;
		}
	}
	
	vector<CardColor> foundColors;
	for (size_t i = 0; i < mBuiltCards.size(); i++){
		//for each building it's value as extra points.
		score += mBuiltCards[i]->getValue();
		CardColor color = mBuiltCards[i]->getCardColor();
		//Check for color combo bonus
		//adds each new color to a vector foundcolors and if that vector contains al possible colors the player wil get a bonus
		for (size_t c = 0; c < colors.size(); c++){ 
			if (find(colors.begin(), colors.end(), color) != colors.end()) {
				if (find(foundColors.begin(), foundColors.end(), color) == foundColors.end()) {
					foundColors.push_back(color);
				}
			}
		}
	}

	if(foundColors.size() > 4){
		//3 points for color bonus
		score += 3;
	}
    return score;
}

void Player::addGold(int goldToAdd){
    mGold += goldToAdd;
}

void Player::removeGold(int gold){
    mGold -= gold;
}

void Player::setGold(int newGold){
    mGold = newGold;
}

int Player::gold(){
    return mGold;
}

void Player::addCardToHand(shared_ptr<BuildingCard> card){
    mCardHand.push_back(card);
}

vector<shared_ptr<BuildingCard>> Player::cardHand(){
    return mCardHand;
}
vector<shared_ptr<BuildingCard>> Player::builtCards(){
    return mBuiltCards;
}

void Player::setFirstFinished(){
	mFirstFinished = true;
}

void Player::setState(enum states newState){
    mState = newState;
}

void Player::setKing(bool isKing){
	mIsKing = isKing;
}

bool Player::isKing(){
	return mIsKing;
}

void Player::setHasDoneTurnAction(bool state){
    mHasDoneTurnAction = state;
}

bool Player::hasDoneTurnAction(){
    return mHasDoneTurnAction;
}

void Player::setHasBuild(bool hasBuilt){
    mHasBuild = hasBuilt;
}

bool Player::hasBuild(){
    return mHasBuild;
}

bool Player::buildCard(shared_ptr<BuildingCard> card){
    if (card->getBuildPrice() > mGold){
        return false;
    }

    mGold -= card->getBuildPrice();
    mBuiltCards.push_back(card);
	
    mCardHand.erase(remove(mCardHand.begin(), mCardHand.end(), card), mCardHand.end());

    return true;
}

size_t Player::getNumberOfBuildings(){
	return mBuiltCards.size();
}

void Player::setHasDrawnCards(bool hasDrawn){
	mHasDrawnCards = hasDrawn;
}

bool Player::hasDrawnCards(){
	return mHasDrawnCards;
}

void Player::removeCard(int i){
	mCardHand.erase(mCardHand.end() - i);
}

int Player::destroyBuilding(int i){
	int price = mBuiltCards[i]->getBuildPrice() - 1;
	mBuiltCards.erase(mBuiltCards.begin() + i);
	return price;
}

int Player::goldForCardColor(CardColor color){
    int addedGold = 0;
    for (size_t i = 0; i < mBuiltCards.size(); i ++){
        if (color == mBuiltCards.at(i)->getCardColor() && color != UNKNOWN){
            addGold(1);
            addedGold++;
        }
    }
    return addedGold;
}

void Player::clearHand(){
    while (!mCardHand.empty()){
        mCardHand.pop_back();
    }
}

void Player::addMultipleCardsToHand(vector<shared_ptr<BuildingCard>> cards){
    for (size_t i = 0; i < cards.size(); i++){
        addCardToHand(cards.at(i));
    }
}

bool Player::hasCardBuilt(string name){
    for (size_t i = 0; i < mBuiltCards.size(); i++){
        if (mBuiltCards.at(i)->getName() == name){
            return true;
        }
    }
    return false;
}

shared_ptr<BuildingCard> Player::buildcard(string name){
    for (size_t i = 0; i < mBuiltCards.size(); i++){
        if (mBuiltCards.at(i)->getName() == name){
            return mBuiltCards.at(i);
        }
    }
    return nullptr;
}
