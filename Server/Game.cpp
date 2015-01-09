//
//  Game.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 14/12/14.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#include "Game.h"

Game::Game(){
    mPlayers = new vector<unique_ptr<Player>>();
}

Game::~Game(){
    
}

string Game::handleRequest(shared_ptr<Socket> socket, ClientCommand command){
    weak_ptr<Player> player = getPlayer(socket);
    if (player == nullptr){
        player = new Player(command.get_cmd(), socket);
        mPlayers->push_back(player);
        string cmd = command.get_cmd();

        string returnValue = "Ok " + cmd + ", just wait for the other player to connect";

        if (mPlayers->size() == 2){
            startGame();
            weak_ptr<Player> tempPlayer = mPlayers->at(0);
            return "The game has started and it's " + tempPlayer->getName() + "'s turn!\n";
        }

        return returnValue;
    }
    else {
        return "Not yet implemented\n";
    }
    return "Not yet implemented\n";
}

weak_ptr<Player> Game::getPlayer(shared_ptr<Socket> socket){
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
    if (!loadDecks()){
        sendErrorMessage();
    }

    // Notifiy all players that the game has started
    sendStartMessage();
}

bool Game::loadDecks(){
    return true;
}

void Game::sendErrorMessage(){
    for (size_t i = 0; i < mPlayers->size(); i++){
        Player *tempPlayer = mPlayers->at(i);
        shared_ptr<Socket> socket = tempPlayer->getSocket();
		socket->write("CLEAR\n");
		socket->write("TITLE\n");
        socket->write("The game has started and it's " + player->getName() + "'s turn!\n");
        unique_ptr<Player> player = mPlayers->at(i);

    }
}

void Game::sendStartMessage(){
    unique_ptr<Player> player = mPlayers->at(0);
    shared_ptr<Socket> socket = player->getSocket();
    socket->write("It's your turn, what do you want to do?\n");
    socket->write("< \n");
}