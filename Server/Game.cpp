//
//  Game.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 14/12/14.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#include "Game.h"

Game::Game(){
    mPlayers = vector<shared_ptr<Player>>();
}

Game::~Game(){
    
}

string Game::handleRequest(shared_ptr<Socket> socket, ClientCommand command){
    shared_ptr<Player> player = getPlayer(socket, command);

    return "Not yet implemented\n";
}

shared_ptr<Player> Game::getPlayer(shared_ptr<Socket> socket, ClientCommand command){
    if (!mPlayers.empty()){
        for (size_t i = 0; i < mPlayers.size(); i++){
            if (mPlayers.at(i)->isPlayer(socket)){
                return mPlayers.at(i);
            }
        }
    }
    auto p = make_shared<Player>(command.get_cmd(), socket);

    //make_shared<Player>(new Player{ command.get_cmd(), socket});
    //weak_ptr<Player> p{ new Player{ command.get_cmd(), socket}};
    //player = new Player(command.get_cmd(), socket);
    
    mPlayers.push_back(p);
    string cmd = command.get_cmd();

    string returnValue = "Ok " + cmd + ", just wait for the other player to connect";

    socket->write(returnValue);

    if (mPlayers.size() == 2){
        startGame();
    }
    return p;
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

}

void Game::sendStartMessage(){
    for (size_t i = 0; i < mPlayers.size(); i++){
        shared_ptr<Player> tempPlayer = mPlayers.at(i);
        shared_ptr<Socket> socket = tempPlayer->getSocket();
        socket->write("CLEAR\n");
        socket->write("TITLE\n");
        socket->write("The game has started and it's " + tempPlayer->getName() + "'s turn!\n");
    }

    shared_ptr<Player> player = mPlayers.at(0);
    shared_ptr<Socket> socket = player->getSocket();
    socket->write("It's your turn, what do you want to do?\n");
    socket->write("< \n");
}

void Game::startRound(){
    // Shuffle characterdeck
    // Koning mag beginnen
    pickCharacterCard();
}

void Game::pickCharacterCard(){
    // Loop voor character kaarten typen
    // Alle character kaarten zonder owner in string bijhouden
}

void Game::doTurn(){
    // Doorloop characterdeck
    // Voor elke character "ingame" vraag de speler op
    // laat hem zijn beurt doen.
}
