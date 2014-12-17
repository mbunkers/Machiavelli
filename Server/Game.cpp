//
//  Game.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 14/12/14.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#include "Game.h"

Game::Game(){
    mPlayers = new vector<Player *>();
}

Game::~Game(){
    
}

string Game::handleRequest(shared_ptr<Socket> socket, ClientCommand command){
    Player *player = getPlayer(socket);
    if (player == nullptr){
        player = new Player(command.get_cmd(), socket);
        mPlayers->push_back(player);
        string cmd = command.get_cmd();

        string returnValue = "Ok " + cmd + ", just wait for the other player to connect";

        if (mPlayers->size() == 2){
            startGame();
            return "The game will be started soon, when the game is ready";
        }

        return returnValue;
    }
    else {
        return "Not yet implemented\n";
    }
    return "Not yet implemented\n";
}

Player* Game::getPlayer(shared_ptr<Socket> socket){
    if (!mPlayers->empty()){
        for (size_t i = 0; i < mPlayers->size(); i++){
            if (mPlayers->at(i)->isPlayer(socket)){
                return mPlayers->at(i);
            }
        }
    }
    return nullptr;
}

void Game::startGame(){
    // Setup game

    // Notifiy all players that the game has started
    sendStartMessage();
}

void Game::sendStartMessage(){
    Player *player = mPlayers->at(0);

    for (size_t i = 0; i < mPlayers->size(); i++){
        Player *tempPlayer = mPlayers->at(i);
        shared_ptr<Socket> socket = tempPlayer->getSocket();
        socket->write("The game has started and it's " + player->getName() + "'s turn!\n");
    }

    shared_ptr<Socket> socket = player->getSocket();
    socket->write("It's your turn, what do you want to do?\n");
    socket->write("< \n");
}