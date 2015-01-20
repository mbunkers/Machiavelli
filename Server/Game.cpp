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
            if (player == mCurrentCharacter->owner()){
                playCharactersPhase(mCurrentCharacter, command.get_cmd());
            }
            else {
                player->getSocket()->write("Something went wrong, but it's not your turn!" + socketDefaults::endLine);
            }
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

    string returnValue = "Ok " + cmd + ", just wait for the other player to connect.." + socketDefaults::endLine;

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
    notifyPlayers("Something went terribly wrong therefor the game cannot be started. Are the resourcefiles available?" + socketDefaults::endLine);
    notifyPlayers("The game quits now!" + socketDefaults::endLine);
    notifyPlayers("Bye!" + socketDefaults::endLine);
}

void Game::sendStartMessage(){
    shared_ptr<Player> player = mPlayers.at(0);
    for (size_t i = 0; i < mPlayers.size(); i++){
        shared_ptr<Player> tempPlayer = mPlayers.at(i);
        shared_ptr<Socket> socket = tempPlayer->getSocket();
        socket->write("CLEAR\n");
        socket->write("TITLE\n");
        if (player != tempPlayer){
            socket->write("The game has started and it's " + player->getName() + "'s turn!" + socketDefaults::endLine);
        }
        else {
            socket->write("The game has started and it's your turn!" + socketDefaults::endLine);
        }
    }
}



void Game::pickCharacterCard(shared_ptr<Player> player){
    vector<shared_ptr<Card>> cards = mCharacterDeck->allCards();
    player->getSocket()->write("You may choose one card\n");

    for (size_t i = 0; i < mCharacterDeck->size(); i++){
        shared_ptr<CharacterCard> card = static_pointer_cast<CharacterCard>(mCharacterDeck->cardAtIndex(i));
        if (!card->isTaken()){
            string option = "[" + to_string(card->priority()) + "]" + card->getName()  + socketDefaults::endLine;
            player->getSocket()->write(option);
        }
    }

    // Opties om kaarten in te zien
    player->getSocket()->write("[See Hand] Display the card in your hands" + socketDefaults::endLine);
    player->getSocket()->write("[See Buildings] Display the buildings you have built" + socketDefaults::endLine);

    // Loop voor character kaarten typen
    // Alle character kaarten zonder owner in string bijhouden
}

void Game::startGame(){
	// Setup game
    try {
        if (!loadDecks()){
            sendErrorMessage();
        }
        else {
            //shuffle buildingsdeck
            mBuildingDeck->shuffle();

            //Give players starting cards and gold
            for (size_t i = 0; i < mPlayers.size(); i++){
                shared_ptr<Player> player = mPlayers.at(i);
                player->setGold(2);

                for (int j = 0; j < 3; j++){
                    player->addCardToHand(static_pointer_cast<BuildingCard>(mBuildingDeck->drawCard()));
                }
            }

            // Notify all players that the game has started
            sendStartMessage();
            changePhase(STARTROUND);
        }
    } catch (exception exception) {
        sendErrorMessage();
    }
}

void Game::displayCardHand(shared_ptr<Player> player){
    vector<shared_ptr<BuildingCard>> cards = player->cardHand();
    player->getSocket()->write("You have the following cards in your hand"  + socketDefaults::endLine);

	for (size_t i = 0; i < cards.size(); i++){
        shared_ptr<BuildingCard> card = cards.at(i);
        string option = card->getName() + "(" + card->getCardColorString() + ") cost: " + to_string(card->getBuildPrice()) + " value: " + to_string(card->getValue()) + socketDefaults::endLine;
        player->getSocket()->write(option);
    }
}

void Game::displayBuiltCards(shared_ptr<Player> player){
    vector<shared_ptr<BuildingCard>> cards = player->builtCards();
    player->getSocket()->write("You have the following cards built" + socketDefaults::endLine);
	for (size_t i = 0; i < cards.size(); i++){
        shared_ptr<BuildingCard> card = cards.at(i);
        string option = card->getName() + "(" + card->getCardColorString() + ") cost: " + to_string(card->getBuildPrice()) + " value: " + to_string(card->getValue()) + socketDefaults::endLine;
        player->getSocket()->write(option);
    }
}

void Game::startRound(){
	// Notify players who starts next phase.	
    vector<shared_ptr<Card>> cards = mCharacterDeck->allCards();

    for (size_t i = 0; i < mCharacterDeck->size(); i++){
        shared_ptr<CharacterCard> card = static_pointer_cast<CharacterCard>(mCharacterDeck->cardAtIndex(i));
        card->setIsTaken(false);
        card->setOwner(nullptr);
    }
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

void Game::notifyPlayers(string message){
    for (size_t i = 0; i < mPlayers.size(); i++){
        shared_ptr<Player> tempPlayer = mPlayers.at(i);
        shared_ptr<Socket> socket = tempPlayer->getSocket();
        socket->write(message);
    }
}

void Game::pickCard(shared_ptr<Player> player, int command){
    if (attachPlayerToCard(player, command)){
        notifyOtherPlayers(player, player->getName() + " picked a card"  + socketDefaults::endLine);
        player->setState(Player::DISCARDCARD);
        player->getSocket()->write("You must remove a card from the characterdeck" + socketDefaults::endLine);
        pickCharacterCard(player);
        player->getSocket()->write(socketDefaults::prompt);
    }
    else {
        player->getSocket()->write("Option not found, try again..."  + socketDefaults::endLine);
        player->getSocket()->write(socketDefaults::prompt);
    }
}

void Game::removeCard(shared_ptr<Player> player, int command){
    if (attachPlayerToCard(nullptr, command)){
        notifyOtherPlayers(player, player->getName() + " removed a card from the characterdeck"  + socketDefaults::endLine);
        // Volgende speler aanwijzen
        player->setState(Player::IDLE);
        player->getSocket()->write("Your turn has ended"  + socketDefaults::endLine);
        if (mCharacterDeck->allCardsTaken()){
            mCurrentCharacter = static_pointer_cast<CharacterCard>(mCharacterDeck->cardAtIndex(0));
            changePhase(PLAYCHARACTERS);
        }
        else {
            assignNextPlayerCardChoosing(player);
        }
    }
    else {
        player->getSocket()->write("Option not found, try again..." + socketDefaults::endLine);
        player->getSocket()->write(socketDefaults::prompt);
    }
}

void Game::selectCharactersPhase(shared_ptr<Player> player, string command){
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
                                pickCard(player, digit);
                            }
                            else {
                                if (player->mState == Player::DISCARDCARD){
                                    removeCard(player, digit);
                                }
                            }
                        }
                        else {
                            player->getSocket()->write("Option not found, try again..." + socketDefaults::endLine);
                            player->getSocket()->write(socketDefaults::prompt);
                        }
                    }
                    else {
                        if (command != player->getName()){
                            player->getSocket()->write("Option not found, try again..." + socketDefaults::endLine);
                            player->getSocket()->write(socketDefaults::prompt);
                        }
                    }
                }
            }
        }
    }
}

bool Game::attachPlayerToCard(shared_ptr<Player> player, int characterIndex){
    vector<shared_ptr<Card>> cards = mCharacterDeck->allCards();

    for (size_t i = 0; i < mCharacterDeck->size(); i++){
        shared_ptr<CharacterCard> card = static_pointer_cast<CharacterCard>(mCharacterDeck->cardAtIndex(i));
        if (card->priority() == characterIndex){
            if (!card->isTaken()){
                card->setOwner(player);
                card->setIsTaken(true);
                return true;
            }
            return false;
        }
    }
    return false;
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
    notifyOtherPlayers(nextPlayer, "Now it's " + nextPlayer->getName() + "'s turn" + socketDefaults::endLine);
    nextPlayer->setState(Player::CHOOSECARD);
    nextPlayer->getSocket()->write("It's your turn now" + socketDefaults::endLine);
    pickCharacterCard(nextPlayer);
    nextPlayer->getSocket()->write(socketDefaults::prompt);
}

bool Game::is_number(const string& s){
    return !s.empty() && find_if(s.begin(), s.end(), [](char c){
                                          return !isdigit(c);
                                      }) == s.end();
}

shared_ptr<CharacterCard> Game::whoIsNextCharacter(shared_ptr<CharacterCard> card){
    size_t newPriority = card->priority() + 1;
    shared_ptr<CharacterCard> newCard = nullptr;
    if (newPriority <= mCharacterDeck->size()){
        for (size_t i = 0; i < mCharacterDeck->size(); i++){
            shared_ptr<CharacterCard> tempCard = static_pointer_cast<CharacterCard>(mCharacterDeck->cardAtIndex(i));
            if (newPriority == tempCard->priority()){
                newCard = tempCard;
                break;
            }
        }
    }

    if (newCard != nullptr){
        return newCard;
    }
    else {
        return nullptr;
    }
}

void Game::nextPlayerTurn(shared_ptr<CharacterCard> card){
	notifyPlayers("CLEAR\n");
	notifyPlayers("TITLE\n");
    notifyPlayers("It's " + card->getName() + " turn!" + socketDefaults::endLine);
    if (card->hasOwner()){
        mCurrentCharacter = card;
        notifyPlayers(card->owner()->getName() + " is the " + card->getName() + "!" + socketDefaults::endLine);
        card->owner()->getSocket()->write("It's your turn, what do you want to do?" + socketDefaults::endLine);
        playCharactersPhase(mCurrentCharacter, "");
    }
    else {
        notifyPlayers("It appears the " + card->getName() + " is not here this round!" + socketDefaults::endLine);

        shared_ptr<CharacterCard> nextCard = nullptr;
        nextCard = whoIsNextCharacter(card);

        if (nextCard != nullptr){
            nextPlayerTurn(nextCard);
        }
        else {
            endRound();
        }
    }
}

void Game::playCharactersPhase(shared_ptr<CharacterCard> card, string command){
    if (command.empty()){
        // Print mogelijkheden
        if (!card->owner()->hasDoneTurnAction()){
            card->owner()->getSocket()->write("[Take gold]" + socketDefaults::endLine);
            card->owner()->getSocket()->write("[Draw cards]" + socketDefaults::endLine);
            card->owner()->getSocket()->write("[See Hand] Display the card in your hands" + socketDefaults::endLine);
            card->owner()->getSocket()->write("[See Buildings] Display the buildings you have built" + socketDefaults::endLine);
        }
        if (!card->owner()->hasBuild()){
            // Show buildings to build
            for (size_t i = 0; i < card->owner()->cardHand().size(); i++){
                shared_ptr<BuildingCard> characterCard = card->owner()->cardHand().at(i);
                card->owner()->getSocket()->write("[Build " + to_string(i) + "] " + characterCard->getName() + "(" + characterCard->getCardColorString() + ") cost: " + to_string(characterCard->getBuildPrice()) + " value: " + to_string(characterCard->getValue()) + socketDefaults::endLine);
            }
        }

        // See other players data
        for (size_t i = 0; i < mPlayers.size(); i++){
            if (mPlayers.at(i) != card->owner()){
                card->owner()->getSocket()->write("[Check " + mPlayers.at(i)->getName() + "]" + socketDefaults::endLine);
            }
        }

        card->owner()->getSocket()->write("[End turn]" + socketDefaults::endLine);
        card->owner()->getSocket()->write(socketDefaults::prompt);
    }
    else {
        doTurn(card, command);
    }
}

void Game::endTurn(shared_ptr<CharacterCard> card){
    // Reset values
    card->owner()->setHasDoneTurnAction(false);
    card->owner()->setHasBuild(false);

    shared_ptr<CharacterCard> newCard = nullptr;
    newCard = whoIsNextCharacter(card);
    if (newCard != nullptr){
        nextPlayerTurn(newCard);
    }
    else {
        endRound();
    }
}

void Game::notYetImplementedMessage(shared_ptr<Player> player){
    player->getSocket()->write("Not yet implemented, try again..." + socketDefaults::endLine);
    player->getSocket()->write(socketDefaults::prompt);
}

void Game::takeGold(shared_ptr<CharacterCard> card){
    if (!card->owner()->hasDoneTurnAction()){
        int goldToAdd = 2;
        card->owner()->addGold(goldToAdd);
        card->owner()->setHasDoneTurnAction(true);

        card->owner()->getSocket()->write("You got " + to_string(goldToAdd) + " golden coins from the bank" + socketDefaults::endLine);
        card->owner()->getSocket()->write("You have a total of " + to_string(card->owner()->gold()) + " golden coins" + socketDefaults::endLine);
        notifyOtherPlayers(card->owner(), card->owner()->getName() + " took " + to_string(goldToAdd) + " golden coins" + socketDefaults::endLine);
        card->owner()->getSocket()->write(socketDefaults::prompt);
    }
    else {
        card->owner()->getSocket()->write("You already did one of your turn actions.." + socketDefaults::endLine);
        card->owner()->getSocket()->write(socketDefaults::prompt);
    }
}

void Game::drawCards(shared_ptr<CharacterCard> card){
    if (!card->owner()->hasDoneTurnAction()){
        notYetImplementedMessage(card->owner());
        card->owner()->setHasDoneTurnAction(true);
    }
    else {
        card->owner()->getSocket()->write("You already did one of your turn actions.." + socketDefaults::endLine);
        card->owner()->getSocket()->write(socketDefaults::prompt);
    }
}

void Game::build(shared_ptr<CharacterCard> card, vector<string> commands){
    if (!card->owner()->hasBuild()){
        if (is_number(commands.at(1))){
            size_t number = atoi(commands.at(1).c_str());
            if (number < card->owner()->cardHand().size()){
                shared_ptr<BuildingCard> buildingCard = card->owner()->cardHand().at(number);
                if (card->owner()->buildCard(buildingCard)){
                    card->owner()->getSocket()->write("You have build your building" + socketDefaults::endLine);
                    card->owner()->getSocket()->write(socketDefaults::prompt);

                    notifyOtherPlayers(card->owner(), card->owner()->getName() + " has built " + buildingCard->getName() + "(" + buildingCard->getCardColorString() + ") cost: " + to_string(buildingCard->getBuildPrice()) + " value: " + to_string(buildingCard->getValue()) + socketDefaults::endLine);
                }
                else {
                    card->owner()->getSocket()->write("You don't have a sufficient amount of gold..." + socketDefaults::endLine);
                    card->owner()->getSocket()->write(socketDefaults::prompt);
                }
            }
            else {
                card->owner()->getSocket()->write("That is not a card, try again..." + socketDefaults::endLine);
                card->owner()->getSocket()->write(socketDefaults::prompt);
            }
        }
        else {
            card->owner()->getSocket()->write("Option not found, try again..." + socketDefaults::endLine);
            card->owner()->getSocket()->write(socketDefaults::prompt);
        }
    }
    else {
        card->owner()->getSocket()->write("You have already built a building..." + socketDefaults::endLine);
        card->owner()->getSocket()->write(socketDefaults::prompt);
    }
}

void Game::checkPlayer(shared_ptr<CharacterCard> card, vector<string> commands){
    shared_ptr<Player> player = nullptr;
    for (size_t i = 0; i < mPlayers.size(); i++){
        if (mPlayers.at(i)->getName() == commands.at(1)){
            player = mPlayers.at(i);
        }
    }

    if (player == nullptr){
        card->owner()->getSocket()->write("Player not found, try again..." + socketDefaults::endLine);
        card->owner()->getSocket()->write(socketDefaults::prompt);
    }
    else {
        if (card->owner() == player){
            card->owner()->getSocket()->write("Check your own options, try again..." + socketDefaults::endLine);
            card->owner()->getSocket()->write(socketDefaults::prompt);
        }
        else {
            // List the other players stats
            notYetImplementedMessage(card->owner());
        }
    }
}

void Game::doTurn(shared_ptr<CharacterCard> card, string command){
	if (command == "End turn"){
		endTurn(card);
	}
	else {
		if (command == "Draw cards"){
			drawCards(card);
		}
		else {
			if (command == "Take gold"){
				takeGold(card);
			}
			else {
				if (command == "See hand"){
					displayCardHand(card->owner());
					card->owner()->getSocket()->write(socketDefaults::prompt);
				}
				else {
					if (command == "See buildings"){
						displayBuiltCards(card->owner());
						card->owner()->getSocket()->write(socketDefaults::prompt);
					}
					else {
						vector<string> splittedCommand = splittedString(command, char(32));

                        if (splittedCommand.size() == 2){
                            if (splittedCommand.at(0) == "Build"){
                                build(card, splittedCommand);
                            }
                            else {
                                if (splittedCommand.at(0) == "Check"){
                                    checkPlayer(card, splittedCommand);
                                }
                                else {
                                    card->owner()->getSocket()->write("Option not found, try again..." + socketDefaults::endLine);
                                    card->owner()->getSocket()->write(socketDefaults::prompt);
                                }
                            }
                        }
                        else {
                            card->owner()->getSocket()->write("Option not found, try again..." + socketDefaults::endLine);
                            card->owner()->getSocket()->write(socketDefaults::prompt);
                        }
					}
				}
			}
		}
	}
}

void Game::endRound(){
	//check if a player has 8 or more building
	bool finishGame = false;
	for (size_t i = 0; i < mPlayers.size(); i++){
		if (mPlayers[i]->getNumberOfBuildings() > 7){
			finishGame = true;
		}
	}

	if (finishGame){
		endGame();
	}
	else{
		changePhase(STARTROUND);
	}
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
	for (size_t i = 0; i < mPlayers.size(); i++){
		if (mPlayers[i]->isKing()){
			return mPlayers[i];
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

            nextPlayerTurn(mCurrentCharacter);
		break;
	case ENDGAME:
		break;
	default:
		break;
	}
}

vector<string> Game::splittedString(const string line, char delim){
    vector<string> elems;
    stringstream ss(line);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
