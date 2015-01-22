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

    if (player != nullptr){
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
                chooseColorForEndGame(player, command.get_cmd());
                break;
            default:
                break;
        }
    }
    else {
        socket->write("The game has already started... You can't enter!");
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

    if (mPlayers.size() == 2){
        return nullptr;
    }


    auto p = make_shared<Player>(command.get_cmd(), socket);

    mPlayers.push_back(p);
    string cmd = command.get_cmd();

    if (mPlayers.size() == 2){
        changePhase(STARTGAME);
    }
    else {
        socket->write("Ok " + cmd + ", just wait for the other player to connect.." + socketDefaults::endLine);
    }

    return p;
}

bool Game::loadDecks(){
    mBuildingDeck = make_shared<CardDeck<shared_ptr<BuildingCard>>>("Bouwkaarten.csv");
    mCharacterDeck = make_shared<CardDeck<shared_ptr<CharacterCard>>>("Karakterkaarten.csv");
    return (mBuildingDeck->isLoaded() && mCharacterDeck->isLoaded());
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
        cleanScreen(tempPlayer);
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
    // See other players data
    for (size_t i = 0; i < mPlayers.size(); i++){
        if (mPlayers.at(i) != player){
            player->getSocket()->write("[Check " + mPlayers.at(i)->getName() + "]" + socketDefaults::endLine);
        }
    }

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
                player->setGold(100);

                for (int j = 0; j < 3; j++){
                    shared_ptr<BuildingCard> card = static_pointer_cast<BuildingCard>(mBuildingDeck->drawCard());
                    if (card != nullptr){
                        player->addCardToHand(static_pointer_cast<BuildingCard>(mBuildingDeck->drawCard()));
                    }
                    else {
                        player->getSocket()->write("The buildingsdeck is empty" + socketDefaults::endLine);
                    }
                }
            }

            // Notify all players that the game has started
            sendStartMessage();
            changePhase(STARTROUND);
        }
    } catch (...) {
        sendErrorMessage();
    }
}

void Game::displayCardHand(shared_ptr<Player> player){
    vector<shared_ptr<BuildingCard>> cards = player->cardHand();
    player->getSocket()->write("You have the following cards in your hand"  + socketDefaults::endLine);

    for (size_t i = 0; i < cards.size(); i++){
        shared_ptr<BuildingCard> card = cards.at(i);
        player->getSocket()->write(card->formattedString());
    }
}

void Game::displayBuiltCards(shared_ptr<Player> player){
    vector<shared_ptr<BuildingCard>> cards = player->builtCards();
    player->getSocket()->write("This is your current town:" + socketDefaults::endLine);
    for (size_t i = 0; i < cards.size(); i++){
        shared_ptr<BuildingCard> card = cards.at(i);
        player->getSocket()->write(card->formattedString());
    }
    if (cards.size() == 0){
        player->getSocket()->write(">EMPTY<" + socketDefaults::endLine);
    }
}

void Game::startRound(){
    // Notify players who starts next phase.
    vector<shared_ptr<Card>> cards = mCharacterDeck->allCards();

    for (size_t i = 0; i < mCharacterDeck->size(); i++){
        shared_ptr<CharacterCard> card = static_pointer_cast<CharacterCard>(mCharacterDeck->cardAtIndex(i));
        card->setIsTaken(false);
        card->setOwner(nullptr);
        card->setHasUsedAction(false);
        card->setIsBeingRobbed(false);
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
        prompt(player);
    }
    else {
        player->getSocket()->write("Option not found, try again..."  + socketDefaults::endLine);
        prompt(player);
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
        prompt(player);
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
            prompt(player);
        }
        else{
            if (command == "See Buildings"){
                displayBuiltCards(player);
                prompt(player);
            }
            else {
                if (command == ""){
                    pickCharacterCard(player);
                    prompt(player);
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
                                    cleanScreen(player);
                                    removeCard(player, digit);
                                }
                            }
                        }
                        else {
                            player->getSocket()->write("Option not found, try again..." + socketDefaults::endLine);
                            prompt(player);
                        }
                    }
                    else {
                        if (command != player->getName()){
                            vector<string> splittedCommand = splittedString(command, char(32));

                            if (splittedCommand.size() == 2){
                                if (splittedCommand.at(0) == "Check"){
                                    checkPlayer(player, splittedCommand);
                                }
                                else {
                                    player->getSocket()->write("Option not found, try again..." + socketDefaults::endLine);
                                    player->getSocket()->write(socketDefaults::prompt);
                                }
                            }
                            else {
                                player->getSocket()->write("Option not found, try again..." + socketDefaults::endLine);
                                player->getSocket()->write(socketDefaults::prompt);
                            }
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
    prompt(nextPlayer);
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
    //cleanScreen(card->owner());
    notifyPlayers("It's " + card->getName() + " turn!" + socketDefaults::endLine);
    if (card->hasOwner()){
        mCurrentCharacter = card;

        // If king, remove from old king and assign new king
        if (card == dynamic_pointer_cast<King>(card)){
            for (size_t i = 0; i < mPlayers.size(); i++){
                mPlayers.at(i)->setKing(false);
            }

            if (!card->owner()->isKing()){
                notifyOtherPlayers(card->owner(), "From now on, " + card->owner()->getName() + " is the king!" + socketDefaults::endLine);
                card->owner()->getSocket()->write("You are the king now!");
            }
            else {
                notifyPlayers(card->owner()->getName() + " is the " + card->getName() + "!" + socketDefaults::endLine);
            }

            card->owner()->setKing(true);
        }
        //If merchant add an extra coin to yer poket
        else if (card == dynamic_pointer_cast<Merchant>(card)){
            notifyPlayers(card->owner()->getName() + " is the " + card->getName() + "!" + socketDefaults::endLine);
            card->owner()->addGold(1);
            card->owner()->getSocket()->write("You've received 1 gold coin because of your trading skills" + socketDefaults::endLine);
            notifyOtherPlayers(card->owner(), card->owner()->getName() + " got 1 golden coin because he is a greedy bastard" + socketDefaults::endLine);
        }
        //If architect ye shall receive 2 building cards
        else if (card == dynamic_pointer_cast<Architect>(card)){
            notifyPlayers(card->owner()->getName() + " is the " + card->getName() + "!" + socketDefaults::endLine);

            shared_ptr<BuildingCard> card1 = static_pointer_cast<BuildingCard>(mBuildingDeck->drawCard());
            shared_ptr<BuildingCard> card2 = static_pointer_cast<BuildingCard>(mBuildingDeck->drawCard());
            if (card1 != nullptr){
                card->owner()->addCardToHand(card1);
            }
            else {
                card->owner()->getSocket()->write("The buildingsdeck is empty" + socketDefaults::endLine);
            }
            if (card2 != nullptr){
                card->owner()->addCardToHand(card2);
            }
            else {
                card->owner()->getSocket()->write("The buildingsdeck is empty" + socketDefaults::endLine);
            }

            card->owner()->getSocket()->write("You've designed 2 new buildings and put them into your hand" + socketDefaults::endLine);
            notifyOtherPlayers(card->owner(), card->owner()->getName() + " received 2 building cards because of his character" + socketDefaults::endLine);
        }
        else {
            notifyPlayers(card->owner()->getName() + " is the " + card->getName() + "!" + socketDefaults::endLine);
        }

        if (card->isBeingRobbed()){
            int gold = card->owner()->gold();

            shared_ptr<CharacterCard> thief = nullptr;

            for (size_t i = 0; i < mCharacterDeck->size(); i++){
                shared_ptr<CharacterCard> tempCard = static_pointer_cast<CharacterCard>(mCharacterDeck->cardAtIndex(i));
                if (tempCard == dynamic_pointer_cast<Thief>(tempCard)){
                    thief = static_pointer_cast<CharacterCard>(mCharacterDeck->cardAtIndex(i));
                }
            }

            thief->owner()->addGold(gold);
            card->owner()->removeGold(gold);

            notifyOtherPlayers(card->owner(), card->owner()->getName() + " is being robbed and lost " + to_string(gold) + " golden coins" + socketDefaults::endLine);
            card->owner()->getSocket()->write("You lost " + to_string(gold) + " golden coins" + socketDefaults::endLine);
            thief->owner()->getSocket()->write("You've robbed " + to_string(gold) + "golden coins from " + card->owner()->getName() + socketDefaults::endLine);
        }

        // Gold for buildings and let all the players know it
        int addedGold = card->owner()->goldForCardColor(card->getCardColor());
        card->owner()->getSocket()->write("You've got " + to_string(addedGold) + " golden coins for your buildings" + socketDefaults::endLine);
        notifyOtherPlayers(card->owner(), card->owner()->getName() + " got " + to_string(addedGold) + " golden coins for his buildings" + socketDefaults::endLine);

        // Start turn
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

void Game::printPossibleActions(shared_ptr<CharacterCard> card){
    cleanScreen(card->owner());

    //standard info
    card->owner()->getSocket()->write("You are the " + card->getName() + socketDefaults::endLine);
    card->owner()->getSocket()->write("Coinpurse: " + to_string(card->owner()->gold()) + " pieces of gold" + socketDefaults::endLine);
    displayBuiltCards(card->owner());
    card->owner()->getSocket()->write(" " + socketDefaults::endLine);


    if (!card->owner()->hasBuild()){
        // Show buildings to build
        card->owner()->getSocket()->write("Options:" + socketDefaults::endLine);
        for (size_t i = 0; i < card->owner()->cardHand().size(); i++){
            shared_ptr<BuildingCard> buildingCard = card->owner()->cardHand().at(i);
            card->owner()->getSocket()->write("[Build " + to_string(i) + "] " + buildingCard->formattedString());
        }
    }
    else {
        // Show buildings you have in your hand
        card->owner()->getSocket()->write("Buildings in hand:" + socketDefaults::endLine);
        for (size_t i = 0; i < card->owner()->cardHand().size(); i++){
            shared_ptr<BuildingCard> buildingCard = card->owner()->cardHand().at(i);
            card->owner()->getSocket()->write(buildingCard->formattedString());
        }
        card->owner()->getSocket()->write(" " + socketDefaults::endLine);
        card->owner()->getSocket()->write("Options:" + socketDefaults::endLine);
    }

    // Print starndard options
    if (!card->owner()->hasDoneTurnAction()){
        card->owner()->getSocket()->write("[Take gold]" + socketDefaults::endLine);
        card->owner()->getSocket()->write("[Draw cards]" + socketDefaults::endLine);
    }

    // print character options
    if (!card->hasUsedAction()){
        card->printOptions();
    }

    // print special building options

    // See other players data
    for (size_t i = 0; i < mPlayers.size(); i++){
        if (mPlayers.at(i) != card->owner()){
            card->owner()->getSocket()->write("[Check " + mPlayers.at(i)->getName() + "]" + socketDefaults::endLine);
        }
    }

    card->owner()->getSocket()->write("[End turn]" + socketDefaults::endLine);
}

void Game::playCharactersPhase(shared_ptr<CharacterCard> card, string command){
    if (command.empty()){
        printPossibleActions(card);
        prompt(card->owner());
    }
    else {
        if (card->owner()->hasDrawnCards()){ //After cards have been drawn
            removeDrawnCards(card, command);
        }
        else {
            if (!card->isUsingAction()){
                doTurn(card, command);
            }
            else {
                if (card == dynamic_pointer_cast<Assassin>(card)){
                    if (is_number(command)){
                        int digit = atoi(command.c_str());
                        if (digit > 0 && digit < 9){
                            assassinAction(card, digit);
                            card->setHasUsedAction(true);
                            prompt(card->owner());
                        }
                        else {
                            card->owner()->getSocket()->write("Option not found, try again..." + socketDefaults::endLine);
                            prompt(card->owner());
                        }
                    }
                }
                else {
                    if (card == dynamic_pointer_cast<Thief>(card)){
                        if (is_number(command)){
                            int digit = atoi(command.c_str());
                            if (digit > 0 && digit < 9){
                                thiefAction(card, digit);
                                card->setHasUsedAction(true);
                                prompt(card->owner());
                            }
                            else {
                                card->owner()->getSocket()->write("Option not found, try again..." + socketDefaults::endLine);
                                prompt(card->owner());
                            }
                        }
                    }
                }
            }
        }
    }
}

void Game::assassinAction(shared_ptr<CharacterCard> card, int command){
    for (size_t i = 1; i < mCharacterDeck->size(); i++){
        shared_ptr<CharacterCard> characterToKill = static_pointer_cast<CharacterCard>(mCharacterDeck->cardAtIndex(i));
        if (characterToKill->priority() == command){
            characterToKill->setOwner(nullptr);
            notifyOtherPlayers(card->owner(), "The " + characterToKill->getName() + " has been killed this round" + socketDefaults::endLine);
            card->owner()->getSocket()->write("You've killed the " + characterToKill->getName() + socketDefaults::endLine);
            break;
        }
    }
}

void Game::thiefAction(shared_ptr<CharacterCard> card, int command){
    for (size_t i = 1; i < mCharacterDeck->size(); i++){
        shared_ptr<CharacterCard> characterToRob = static_pointer_cast<CharacterCard>(mCharacterDeck->cardAtIndex(i));
        if (characterToRob->priority() == command){
            characterToRob->setIsBeingRobbed(true);
            notifyOtherPlayers(card->owner(), "The " + characterToRob->getName() + " will be robbed this round" + socketDefaults::endLine);
            card->owner()->getSocket()->write("You will robbed the " + characterToRob->getName() + socketDefaults::endLine);
            break;
        }
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
    prompt(player);
}

void Game::takeGold(shared_ptr<CharacterCard> card){
    if (!card->owner()->hasDoneTurnAction()){
        int goldToAdd = 2;
        card->owner()->addGold(goldToAdd);
        card->owner()->setHasDoneTurnAction(true);
        printPossibleActions(card);
        card->owner()->getSocket()->write("You got " + to_string(goldToAdd) + " golden coins from the bank" + socketDefaults::endLine);
        card->owner()->getSocket()->write("You have a total of " + to_string(card->owner()->gold()) + " golden coins" + socketDefaults::endLine);
        notifyOtherPlayers(card->owner(), card->owner()->getName() + " took " + to_string(goldToAdd) + " golden coins" + socketDefaults::endLine);
        prompt(card->owner());
    }
    else {
        card->owner()->getSocket()->write("You already did one of your turn actions.." + socketDefaults::endLine);
        prompt(card->owner());
    }
}

void Game::drawCards(shared_ptr<CharacterCard> card){
    if (!card->owner()->hasDoneTurnAction()){
        shared_ptr<BuildingCard> building1 = static_pointer_cast<BuildingCard>(mBuildingDeck->drawCard());
        shared_ptr<BuildingCard> building2 = static_pointer_cast<BuildingCard>(mBuildingDeck->drawCard());

        if (building1 != nullptr){
            card->owner()->addCardToHand(building1);
        }
        else {
            card->owner()->getSocket()->write("The buildingsdeck is empty" + socketDefaults::endLine);
        }
        if (building2 != nullptr){
            card->owner()->addCardToHand(building2);
        }
        else {
            card->owner()->getSocket()->write("The buildingsdeck is empty" + socketDefaults::endLine);
        }

        if (card->owner()->hasCardBuilt("Bibliotheek")){
            printPossibleActions(card);

            notifyOtherPlayers(card->owner(), card->owner()->getName() + " drew 2 cards because he has a Bibliotheek" + socketDefaults::endLine);
            card->owner()->getSocket()->write("You drew these cards:\n");
            if (building1){
                card->owner()->getSocket()->write(building1->formattedString());
            }
            if (building2){
                card->owner()->getSocket()->write(building2->formattedString());
            }

            card->owner()->setHasDoneTurnAction(true);
            card->owner()->setHasDrawnCards(false);

            prompt(card->owner());
        }
        else {
            card->owner()->getSocket()->write("You drew these cards:\n");
            if (building1){
                card->owner()->getSocket()->write("[1] " + building1->formattedString());
            }
            if (building2) {
                card->owner()->getSocket()->write("[2] " + building2->formattedString());
            }

            card->owner()->setHasDoneTurnAction(true);
            card->owner()->setHasDrawnCards(true);
            card->owner()->getSocket()->write("Which card do you want to have ?\n");
            prompt(card->owner());
        }
    }
    else {
        card->owner()->getSocket()->write("You already did one of your turn actions.." + socketDefaults::endLine);
        prompt(card->owner());
    }
}

void Game::removeDrawnCards(shared_ptr<CharacterCard> card, string command){
    if (command == "1" || command == "2"){
        if (command == "1"){
            card->owner()->removeCard(1);
        }
        else{
            card->owner()->removeCard(2);
        }
        card->owner()->setHasDrawnCards(false);
        notifyPlayers(card->owner()->getName() + " draws a building card\n");
        printPossibleActions(card);
        prompt(card->owner());
    }
    else {
        card->owner()->getSocket()->write("Option not found, try again..." + socketDefaults::endLine);
        prompt(card->owner());
    }

}

void Game::build(shared_ptr<CharacterCard> card, vector<string> commands){
    if (!card->owner()->hasBuild()){
        if (is_number(commands.at(1))){
            size_t number = atoi(commands.at(1).c_str());
            if (number < card->owner()->cardHand().size()){
                shared_ptr<BuildingCard> buildingCard = card->owner()->cardHand().at(number);
                if (card->owner()->buildCard(buildingCard)){
                    if (card == dynamic_pointer_cast<Architect>(card)){
                        static_pointer_cast<Architect>(card)->doSpecialAction();
                        if (static_pointer_cast<Architect>(card)->buildLimitReached()){
                            card->owner()->setHasBuild(true);
                        }
                    }
                    else{
                        card->owner()->setHasBuild(true);
                    }

                    printPossibleActions(card);
                    card->owner()->getSocket()->write("You have build your building" + socketDefaults::endLine);
                    notifyOtherPlayers(card->owner(), card->owner()->getName() + " has built " + buildingCard->getName() + "(" + buildingCard->getCardColorString() + ") cost: " + to_string(buildingCard->getBuildPrice()) + " value: " + to_string(buildingCard->getValue()) + socketDefaults::endLine);
                    prompt(card->owner());
                }
                else {
                    card->owner()->getSocket()->write("You don't have a sufficient amount of gold..." + socketDefaults::endLine);
                    prompt(card->owner());
                }
            }
            else {
                card->owner()->getSocket()->write("That is not a card, try again..." + socketDefaults::endLine);
                prompt(card->owner());
            }
        }
        else {
            card->owner()->getSocket()->write("Option not found, try again..." + socketDefaults::endLine);
            prompt(card->owner());
        }
    }
    else {
        card->owner()->getSocket()->write("You have already built a building..." + socketDefaults::endLine);
        prompt(card->owner());
    }
}

void Game::checkPlayer(shared_ptr<Player> player, vector<string> commands){
    shared_ptr<Player> otherPlayer = nullptr;
    for (size_t i = 0; i < mPlayers.size(); i++){
        if (mPlayers.at(i)->getName() == commands.at(1)){
            otherPlayer = mPlayers.at(i);
        }
    }

    if (otherPlayer == nullptr){
        player->getSocket()->write("Player not found, try again..." + socketDefaults::endLine);
        player->getSocket()->write(socketDefaults::prompt);
    }
    else {
        if (player == otherPlayer){
            player->getSocket()->write("Check your own options, try again..." + socketDefaults::endLine);
            player->getSocket()->write(socketDefaults::prompt);
        }
        else {
            // List the other players stats
            player->getSocket()->write(otherPlayer->getName() + " has " + to_string(otherPlayer->gold()) + " golden coins" + socketDefaults::endLine);
            player->getSocket()->write(otherPlayer->getName() + " has " + to_string(otherPlayer->cardHand().size()) + " buildings in his hand" + socketDefaults::endLine);
            player->getSocket()->write(otherPlayer->getName() + " has the following buildings in his town:" + socketDefaults::endLine);
            for (size_t i = 0; i < otherPlayer->builtCards().size(); i++){
                player->getSocket()->write(otherPlayer->builtCards().at(i)->formattedString());
            }
            if (otherPlayer->builtCards().empty()){
                player->getSocket()->write("> EMPTY <" + socketDefaults::endLine);
            }
            player->getSocket()->write(socketDefaults::prompt);
        }
    }
}

void Game::checkPlayer(shared_ptr<CharacterCard> card, vector<string> commands){
    checkPlayer(card->owner(), commands);
}

void Game::listCharactersForAssassin(shared_ptr<CharacterCard> card){
    for (size_t i = 1; i < mCharacterDeck->size(); i++){
        shared_ptr<CharacterCard> tempCard = static_pointer_cast<CharacterCard>(mCharacterDeck->cardAtIndex(i));
        string option = "[" + to_string(tempCard->priority()) + "]" + tempCard->getName() + socketDefaults::endLine;
        card->owner()->getSocket()->write(option);
    }
}

void Game::listCharactersForThief(shared_ptr<CharacterCard> card){
    for (size_t i = 2; i < mCharacterDeck->size(); i++){
        shared_ptr<CharacterCard> tempCard = static_pointer_cast<CharacterCard>(mCharacterDeck->cardAtIndex(i));
        string option = "[" + to_string(tempCard->priority()) + "]" + tempCard->getName() + socketDefaults::endLine;
        card->owner()->getSocket()->write(option);
    }
}

void Game::swapHand(shared_ptr<CharacterCard> card){
    shared_ptr<Player> otherPlayer = nullptr;
    for (size_t i = 0; i < mPlayers.size(); i++){
        if (mPlayers.at(i) != card->owner()){
            otherPlayer = mPlayers.at(i);
        }
    }

    vector<shared_ptr<BuildingCard>> otherPlayerCardDeck = otherPlayer->cardHand();
    vector<shared_ptr<BuildingCard>> playerCardDeck = card->owner()->cardHand();

    otherPlayer->clearHand();
    otherPlayer->addMultipleCardsToHand(playerCardDeck);
    card->owner()->clearHand();
    card->owner()->addMultipleCardsToHand(otherPlayerCardDeck);

    notifyOtherPlayers(card->owner(), card->owner()->getName() + " swapped it's cardhand with " + otherPlayer->getName() + socketDefaults::endLine);
    card->setHasUsedAction(true);
    printPossibleActions(card);
    card->owner()->getSocket()->write("You've swapped your cardhand with " + otherPlayer->getName() + socketDefaults::endLine);
}

void Game::swapDeck(shared_ptr<CharacterCard> card){
    if (card->owner()->cardHand().size() > 0){
        size_t numberOfCards = card->owner()->cardHand().size();
        card->owner()->clearHand();
        bool isEmpty = false;
        for (size_t i = 0; i < numberOfCards; i++){
            shared_ptr<BuildingCard> buildingCard = static_pointer_cast<BuildingCard>(mBuildingDeck->drawCard());
            if (buildingCard != nullptr){
                card->owner()->addCardToHand(buildingCard);
            }
            else {
                isEmpty = true;
            }
        }

        if (isEmpty){
            card->owner()->getSocket()->write("You got less cards because the buildingsdeck is empty" + socketDefaults::endLine);
        }

        notifyOtherPlayers(card->owner(), card->owner()->getName() + " swapped it's cardhand with the buildingsdeck" + socketDefaults::endLine);
        card->setHasUsedAction(true);
        printPossibleActions(card);
        card->owner()->getSocket()->write("You've swapped your cardhand with the buidingsdeck" + socketDefaults::endLine);
    }
    else {
        card->owner()->getSocket()->write("You might consider swapping hand, because your carddeck is empty" + socketDefaults::endLine);
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
                    prompt(card->owner());
                }
                else {
                    if (command == "See buildings"){
                        displayBuiltCards(card->owner());
                        prompt(card->owner());
                    }
                    else {
                        if (command == "Kill" && card == dynamic_pointer_cast<Assassin>(card) && !card->hasUsedAction()){
                            card->setIsUsingAction(true);
                            listCharactersForAssassin(card);
                            prompt(card->owner());
                        }
                        else {
                            if (command == "Steal" && card == dynamic_pointer_cast<Thief>(card) && !card->hasUsedAction()){
                                card->setIsUsingAction(true);
                                listCharactersForThief(card);
                                prompt(card->owner());
                            }
                            else {
                                if (command == "Destroy" && card == dynamic_pointer_cast<Condottiere>(card) && !card->hasUsedAction()){
                                    notYetImplementedMessage(card->owner());
                                }
                                else {
                                    if (command == "Swap Hand" && card == dynamic_pointer_cast<Magician>(card) && !card->hasUsedAction()){
                                        swapHand(card);
                                        prompt(card->owner());
                                    }
                                    else {
                                        if (command == "Swap Deck" && card == dynamic_pointer_cast<Magician>(card) && !card->hasUsedAction()){
                                            swapDeck(card);
                                            prompt(card->owner());
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
                                                        prompt(card->owner());
                                                    }
                                                }
                                            }
                                            else {
                                                card->owner()->getSocket()->write("Option not found, try again..." + socketDefaults::endLine);
                                                prompt(card->owner());
                                            }
                                        }
                                    }
                                }
                            }
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
        changePhase(ENDGAME);
    }
    else{
        for (size_t i = 0; i < mPlayers.size(); i++){
            cleanScreen(mPlayers.at(i));
        }

        notifyPlayers("A new round has started!" + socketDefaults::endLine);
        changePhase(STARTROUND);
    }
}

void Game::chooseColorForEndGame(shared_ptr<Player> player, string colorString){
    CardColor color = colorForString(colorString);
    if (color == UNKNOWN){
        player->getSocket()->write("This isn't a valid color. Choose one from above" + socketDefaults::endLine);
        prompt(player);
    }
    else {
        endGame(color);
    }
}

CardColor Game::colorForString(string stringColor){
    if (stringColor == "geel"){
        return YELLOW;
    }
    if (stringColor == "blauw"){
        return BLUE;
    }
    if (stringColor == "lila"){
        return LILA;
    }
    if (stringColor == "groen"){
        return GREEN;
    }
    if (stringColor == "rood"){
        return RED;
    }
    return UNKNOWN;
}

void Game::endGame(CardColor color){
    //Notify players that the game has finished they should restart the program to start a new game.
    notifyPlayers("The game has been ended!" + socketDefaults::endLine);

    shared_ptr<Player> player = nullptr;
    shared_ptr<BuildingCard> buildingCard = nullptr;
    for (size_t i = 0; i < mPlayers.size(); i++){
        if (mPlayers.at(i)->hasCardBuilt("Hof der Wonderen")){
            player = mPlayers.at(i);
            buildingCard = mPlayers.at(i)->buildcard("Hof der Wonderen");
        }
    }

    // When the card is there the players must pick a color
    if (buildingCard != nullptr && color == UNKNOWN){
        player->getSocket()->write("You've got the " + buildingCard->getName() + " build. Which color should it be?" + socketDefaults::endLine);
        player->getSocket()->write("[geel]" + socketDefaults::endLine);
        player->getSocket()->write("[blauw]" + socketDefaults::endLine);
        player->getSocket()->write("[groen]" + socketDefaults::endLine);
        player->getSocket()->write("[geel]" + socketDefaults::endLine);
        player->getSocket()->write("[lila]" + socketDefaults::endLine);
        prompt(player);
        notifyOtherPlayers(player, player->getName() + " has the " + buildingCard->getName() + " and may change it's color" + socketDefaults::endLine);
    }
    else {
        if (buildingCard != nullptr && color != UNKNOWN){
            buildingCard->changeColor(color);
        }

        if (mPlayers.at(0)->calculateScore() == mPlayers.at(1)->calculateScore()){
            notifyPlayers("It's a draw! Congratulations!" + socketDefaults::endLine);
        }
        else {
            if (mPlayers.at(0)->calculateScore() > mPlayers.at(1)->calculateScore()){
                notifyPlayers("The winner is " + mPlayers.at(0)->getName() + socketDefaults::endLine);
            }
            else {
                notifyPlayers("The winner is " + mPlayers.at(1)->getName() + socketDefaults::endLine);
            }
        }

        notifyPlayers("Here are the results per player:" + socketDefaults::endLine);
        for (size_t i = 0; i < mPlayers.size(); i++){
            notifyPlayers(mPlayers.at(i)->getName() + " has a score of " + to_string(mPlayers.at(i)->calculateScore()) + " points" + socketDefaults::endLine);
        }
        
        changePhase(FINISHED);
    }
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
            endGame(UNKNOWN);
            break;
        default:
            break;
    }
}

bool Game::hasFinished(){
    return mCurrentPhase == FINISHED;
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

void Game::cleanScreen(shared_ptr<Player> player){
    player->getSocket()->write("CLEAR" + socketDefaults::endLine);
    player->getSocket()->write("TITLE" + socketDefaults::endLine);
}


void Game::prompt(shared_ptr<Player> player){
    player->getSocket()->write(socketDefaults::prompt);
}
