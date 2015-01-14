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

    vector<shared_ptr<Player>> mPlayers;
    shared_ptr<Player> getPlayer(shared_ptr<Socket> socket, ClientCommand command);
    unique_ptr<CardDeck<shared_ptr<CharacterCard>>> characterDeck;
    unique_ptr<CardDeck<shared_ptr<BuildingCard>>> buildingDeck;
    void sendStartMessage();
    void sendErrorMessage();
    bool loadDecks();

    // Phases
    void startGame();
    void startRound();
    void selectCharactersPhase();
    void playCharactersPhase();
    void endRound();
    void countScoresPhase();
    void endGame();

	void changePhase(phases);
public:
    

    Game();
    ~Game();
    string handleRequest(shared_ptr<Socket> socket, ClientCommand command);

    void pickCharacterCard();
    void doTurn();
};

#endif /* defined(__Machiavelli__Game__) */
