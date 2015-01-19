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

void Game::handleRequest(shared_ptr<Socket> socket, ClientCommand command){
    shared_ptr<Player> player = getPlayer(socket, command);

    switch (mCurrentPhase){
        case SETUP:
            break;
        case STARTGAME:
            break;
        case STARTROUND:
            break;
        case SELECTCHARACTERS:
            selectCharactersPhase(player, command.get_cmd());
            break;
        case PLAYCHARACTERS:
            break;
        case ENDGAME:
            break;
        default:
            break;
    }
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
            socket->write("The game has started and it's your turn!\n");
        }
    }
}



void Game::pickCharacterCard(shared_ptr<Player> player){
    vector<shared_ptr<Card>> cards = mCharacterDeck->allCards();
    player->getSocket()->write("You may choose one card\n");
    for (size_t i = 0; i < cards.size(); i++){
        shared_ptr<CharacterCard> card = static_pointer_cast<CharacterCard>(cards.at(i));
        if (!card->isTaken()){
            string option = "[" + to_string(card->priority()) + "]" + card->getName() + "\n";
            player->getSocket()->write(option);
        }
    }

    // Opties om kaarten in te zien
    player->getSocket()->write("[See Hand] Display the card in your hands\n");
    player->getSocket()->write("[See Buildings] Display the buildings you have built\n");

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
    mBuildingDeck->shuffle();
	//shuffle buildingsdeck

	//Give players starting cards and gold
	for (size_t i = 0; i < mPlayers.size(); i++){
        shared_ptr<Player> player = mPlayers.at(i);
        player->addGold(2);

        for (int j = 0; j < 3; j++){
            player->addCardToHand(static_pointer_cast<BuildingCard>(mBuildingDeck->drawCard()));
        }
    }

	// Notify all players that the game has started
	sendStartMessage();
	changePhase(STARTROUND);
}

void Game::displayCardHand(shared_ptr<Player> player){
    vector<shared_ptr<BuildingCard>> cards = player->cardHand();
    player->getSocket()->write("You have the following cards in your hand\n");
	for (size_t i = 0; i < cards.size(); i++){
        shared_ptr<BuildingCard> card = cards.at(i);
            string option = "[" + card->getCardColorString() + "] " + card->getName() + "\n";
            player->getSocket()->write(option);
    }
}

void Game::displayBuiltCards(shared_ptr<Player> player){
    vector<shared_ptr<BuildingCard>> cards = player->builtCards();
    player->getSocket()->write("You have the following cards built.\n");
	for (size_t i = 0; i < cards.size(); i++){
        shared_ptr<BuildingCard> card = cards.at(i);
        string option = "[" + card->getCardColorString() + "] " + card->getName() + "\n";
        player->getSocket()->write(option);
    }
}

void Game::startRound(){
	// Shuffle characterdeck
	// Set first player.
	// if king is available that player is first. Else first in player vector.
	// Notify players who starts next phase.
	changePhase(SELECTCHARACTERS);
}

void Game::notifyOtherPlayers(shared_ptr<Player> player,  string message){
    for (size_t i = 0; i < mPlayers.size(); i++){
        shared_ptr<Player> tempPlayer = mPlayers.at(i);
        shared_ptr<Socket> socket = tempPlayer->getSocket();
        if (player != tempPlayer){
            socket->write(message);
        }
    }
}

void Game::selectCharactersPhase(shared_ptr<Player> player, string command){
	//(LOOP)

	//First player picks his character and removes another.
	//first player notifies second player to pick from remaining characters
	//second player notifies first player to pick from remaining characters.
	//first player notifies second player again.
    if (player != nullptr){
        if (command == "See Hand"){
            displayCardHand(player);
            player->getSocket()->write(socketDefaults::prompt);
        }
        else{
            if (command == "See Buildings"){
                displayBuiltCards(player);
                player->getSocket()->write(socketDefaults::prompt);
            }
            else {
                if (command == ""){
                    pickCharacterCard(player);
                    player->getSocket()->write(socketDefaults::prompt);
                }
                else {
                    if (is_number(command)){
                        int digit = atoi(command.c_str());
                        if (digit > -1 && digit < 9){
                            // Actie op basis van staat
                            if (player->mState == Player::CHOOSECARD){
                                attachPlayerToCard(player, digit);
                                notifyOtherPlayers(player, player->getName() + " picked a card\n");
                                player->setState(Player::DISCARDCARD);
                                player->getSocket()->write("You must remove a card from the characterdeck");
                                pickCharacterCard(player);
                                player->getSocket()->write(socketDefaults::prompt);
                            }
                            else {
                                if (player->mState == Player::DISCARDCARD){
                                    attachPlayerToCard(nullptr, digit);
                                    notifyOtherPlayers(player, player->getName() + " removed a card from the characterdeck\n");
                                    // Volgende speler aanwijzen
                                    player->setState(Player::IDLE);
                                    player->getSocket()->write("Your turn has ended\n");
                                    if (mCharacterDeck->allCardsTaken()){
                                        changePhase(PLAYCHARACTERS);
                                    }
                                    else {
                                        assignNextPlayerCardChoosing(player);
                                    }
                                }
                            }
                        }
                        else {
                            player->getSocket()->write("That is not a card, try again...");
                            player->getSocket()->write(socketDefaults::prompt);
                        }
                    }
                    else {
                        if (command != player->getName()){
                            player->getSocket()->write("Option not found, try again...");
                            player->getSocket()->write(socketDefaults::prompt);
                        }
                    }
                }
            }
        }
    }
}

void Game::attachPlayerToCard(shared_ptr<Player> player, int characterIndex){
    vector<shared_ptr<Card>> cards = mCharacterDeck->allCards();

    for (size_t i = 0; i < cards.size(); i++){
        shared_ptr<CharacterCard> card = static_pointer_cast<CharacterCard>(cards.at(i));
        if (card->priority() == characterIndex){
            card->setOwner(player);
            card->setIsTaken(true);
            break;
        }
    }
}

void Game::assignNextPlayerCardChoosing(shared_ptr<Player> player){
    shared_ptr<Player> nextPlayer = nullptr;
    for (size_t i = 0; i < mPlayers.size(); i++){
        shared_ptr<Player> tempPlayer = mPlayers.at(i);
        if (tempPlayer == player){
            if ((i + 1) == mPlayers.size()){
                nextPlayer = mPlayers.at(0);
            }
            else {
                nextPlayer = mPlayers.at((i + 1));
            }
        }
    }
    notifyOtherPlayers(nextPlayer, "Now it's " + nextPlayer->getName() + "'s turn\n");
    nextPlayer->setState(Player::CHOOSECARD);
    nextPlayer->getSocket()->write("It's your turn now");
    pickCharacterCard(nextPlayer);
    nextPlayer->getSocket()->write(socketDefaults::prompt);
}

bool Game::is_number(const string& s){
    return !s.empty() && find_if(s.begin(), s.end(), [](char c){
                                          return !isdigit(c);
                                      }) == s.end();
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

shared_ptr<Player> Game::getKing(){
    vector<shared_ptr<Card>> cards = mCharacterDeck->allCards();
	for (size_t i = 0; i < cards.size(); i++){
        shared_ptr<CharacterCard> card = static_pointer_cast<CharacterCard>(cards.at(i));
        if (card->getName() == "Koning"){
            if (card->hasOwner()){
                return card->owner();
            }
            else {
                break;
            }
        }
    }
    return mPlayers.at(0);
}

void Game::changePhase(phases nextPhase){
    mCurrentPhase = nextPhase;
    // Find king or first player
    shared_ptr<Player> king = nullptr;
	switch (nextPhase){
	case STARTGAME:
            startGame();
		break;
	case STARTROUND:
            startRound();
		break;
	case SELECTCHARACTERS:
            king = getKing();
            king->setState(Player::CHOOSECARD);
            selectCharactersPhase(king, "");
		break;
	case PLAYCHARACTERS:
            king = getKing();
            playCharactersPhase();
		break;
	case ENDGAME:
		break;
	default:
		break;
	}
}
