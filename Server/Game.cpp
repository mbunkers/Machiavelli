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



void Game::pickCharacterCard(){
    // Loop voor character kaarten typen
    // Alle character kaarten zonder owner in string bijhouden
}

void Game::doTurn(){
    // Doorloop characterdeck
    // Voor elke character "ingame" vraag de speler op
    // laat hem zijn beurt doen.
}


void Game::startGame(){
	// Setup game
	if (!loadDecks()){
		sendErrorMessage();
	}
	//shuffle buildingsdeck

	//Give players starting cards and gold

	// Notify all players that the game has started
	sendStartMessage();
	changePhase(STARTROUND);
}

void Game::startRound(){
	// Shuffle characterdeck
	// Set first player.
	// if king is avialable that player is first. Else first in player vector.
	// Notify players who starts next phase.
	changePhase(SELECTCHARACTERS);
}

void Game::selectCharactersPhase(){
	//(LOOP)

	//First player picks his character and removes another.
	//first player notifies second player to pick from remaining characters
	//second player notifies first player to pick from remaining characters.
	//first player notifies second player again.
	pickCharacterCard();

	//if remaining cards = 0 -> next phase
	changePhase(STARTROUND);
	
}

void Game::playCharactersPhase(){
	//(LOOP)

	//Get current card from characters deck.
		//if ingame is false -> next card
		//else get owner
			// notify owner to play his turn with "character" 
			// wait for turn to finish.
			// next card
	//if all cards played -> finish

	changePhase()

}

void Game::endRound(){

}

void Game::countScoresPhase(){

}

void Game::endGame(){

}


void Game::changePhase(phases nextPhase){

	switch (nextPhase){
	case STARTGAME:
		break;
	case STARTROUND:
		break;
	case SELECTCHARACTERS:
		break;
	case PLAYCHARACTERS:
		break;
	case ENDGAME:
		break;
	default:
		break;
	}
}