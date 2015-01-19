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

    switch (mCurrentPhase){
        case SETUP:
            break;
        case STARTGAME:
            break;
        case STARTROUND:
            selectCharactersPhase(player);
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

    return "";
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
    
    mPlayers.push_back(p);
    string cmd = command.get_cmd();

    string returnValue = "Ok " + cmd + ", just wait for the other player to connect\n";

    socket->write(returnValue);

    if (mPlayers.size() == 2){
        changePhase(STARTGAME);
    }
    return p;
}

bool Game::loadDecks(){
    mBuildingDeck = make_shared<CardDeck<shared_ptr<BuildingCard>>>("Bouwkaarten.csv");
    mCharacterDeck = make_shared<CardDeck<shared_ptr<CharacterCard>>>("Karakterkaarten.csv");
    return true;
}

void Game::sendErrorMessage(){

}

void Game::sendStartMessage(){
    shared_ptr<Player> player = mPlayers.at(0);
    for (size_t i = 0; i < mPlayers.size(); i++){
        shared_ptr<Player> tempPlayer = mPlayers.at(i);
        shared_ptr<Socket> socket = tempPlayer->getSocket();
        socket->write("CLEAR\n");
        socket->write("TITLE\n");
        if (player != tempPlayer){
            socket->write("The game has started and it's " + player->getName() + "'s turn!\n");
        }
        else {
            socket->write("The game is started and it's your turn.\n");
        }
    }
}



void Game::pickCharacterCard(shared_ptr<Player> player){

    vector<shared_ptr<Card>> cards = mCharacterDeck->allCards();
    player->getSocket()->write("You may choose one card\n");
    for (int i = 0; i < cards.size(); i++){
        shared_ptr<CharacterCard> card = static_pointer_cast<CharacterCard>(cards.at(i));
        if (!card->hasOwner()){
            string option = "[" + to_string(i) + "]" + card->getName();
            player->getSocket()->write(option);
        }
    }

    player->getSocket()->write("> \n");

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

void Game::selectCharactersPhase(shared_ptr<Player> player){
	//(LOOP)

	//First player picks his character and removes another.
	//first player notifies second player to pick from remaining characters
	//second player notifies first player to pick from remaining characters.
	//first player notifies second player again.
	pickCharacterCard(player);

    if (mCharacterDeck->allCardsTaken()){
        changePhase(STARTROUND);
    }
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

    changePhase(STARTROUND);
}

void Game::endRound(){

}


void Game::countScoresPhase(){
		// (loop) for each player getscore()
	// put into map  "player" "score" 
	//sort based on scores 
	//notify all players the scores in order from high to low and to who the scores belongs.
	
		
}

void Game::endGame(){
	//Notify players that the game has finished they should restart the program to start a new game.
}


void Game::changePhase(phases nextPhase){

	switch (nextPhase){
	case STARTGAME:
            startGame();
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
