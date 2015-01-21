//
//  Game.h
//  Machiavelli
//
//  Created by Marc Bunkers on 14/12/14.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#ifndef __Machiavelli__Game__
#define __Machiavelli__Game__

#include <stdio.h>
#include <vector>
#include "Player.h"
#include "Socket.h"
#include "ClientCommand.h"
#include "CardDeck.h"
#include "CharacterCard.h"
#include "BuildingCard.h"

using namespace std;

namespace socketDefaults {
    const string prompt {"> \n"};
    const string endLine {"\n"};
}

class Game{
private:
	enum phases{
		SETUP = 0,
		STARTGAME = 1,
		STARTROUND = 2,
		SELECTCHARACTERS = 3,
		PLAYCHARACTERS = 4,
		ENDGAME = 5,
	};

    enum phases mCurrentPhase = SETUP;

    shared_ptr<CharacterCard> mCurrentCharacter;

    vector<shared_ptr<Player>> mPlayers;
    shared_ptr<Player> getPlayer(shared_ptr<Socket> socket, ClientCommand command);
    shared_ptr<CardDeck<shared_ptr<CharacterCard>>> mCharacterDeck;
    shared_ptr<CardDeck<shared_ptr<BuildingCard>>> mBuildingDeck;
    void sendStartMessage();
    void sendErrorMessage();
    bool loadDecks();

    // Phases
    void startGame();
    void startRound();
    void selectCharactersPhase(shared_ptr<Player> player, string command);
    void playCharactersPhase(shared_ptr<CharacterCard> card, string command);
    void endRound();
    void countScoresPhase();
    void endGame();
	void changePhase(phases);

    // Actions
    void endTurn(shared_ptr<CharacterCard> card);
    void displayCardHand(shared_ptr<Player> player);
    void displayBuiltCards(shared_ptr<Player> player);
    void pickCharacterCard(shared_ptr<Player> player);
    void doTurn(shared_ptr<CharacterCard> card, string command);
    void notifyOtherPlayers(shared_ptr<Player> player,  string message);
    void assignNextPlayerCardChoosing(shared_ptr<Player> player);
    bool attachPlayerToCard(shared_ptr<Player> player, int characterIndex);
    void nextPlayerTurn(shared_ptr<CharacterCard> card);
    shared_ptr<Player> getKing();
    void notifyPlayers(string message);
    shared_ptr<CharacterCard> whoIsNextCharacter(shared_ptr<CharacterCard> card);
	void removeDrawnCards(shared_ptr<CharacterCard> card, string command);

    bool is_number(const string& s);
    vector<string> splittedString(const string line, char delim);

    void notYetImplementedMessage(shared_ptr<Player> player);

    void takeGold(shared_ptr<CharacterCard> card);
    void drawCards(shared_ptr<CharacterCard> card);
    void build(shared_ptr<CharacterCard> card, vector<string> commands);
    void checkPlayer(shared_ptr<CharacterCard> card, vector<string> commands);

    void pickCard(shared_ptr<Player> player, int command);
    void removeCard(shared_ptr<Player> player, int command);
    void printPossibleActions(shared_ptr<CharacterCard> card);
	void cleanScreen(shared_ptr<Player> player);
	void prompt(shared_ptr<Player> player);

public:
    

    Game();
    ~Game();
    void handleRequest(shared_ptr<Socket> socket, ClientCommand command);
};

#endif /* defined(__Machiavelli__Game__) */
