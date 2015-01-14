//
//  Player.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 14/12/14.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#include "Player.h"

Player::Player(string name, shared_ptr<Socket> socket){
    mName = name;
    mSocket = socket;
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
    return 0;
}
