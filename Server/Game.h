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

using namespace std;

class Game{
private:
    vector<unique_ptr<Player>> *mPlayers;
    unique_ptr<Player> getPlayer(shared_ptr<Socket> socket);
    void startGame();
    void sendStartMessage();
    void sendErrorMessage();
    bool loadDecks();
public:
    Game();
    ~Game();
    string handleRequest(shared_ptr<Socket> socket, ClientCommand command);
};

#endif /* defined(__Machiavelli__Game__) */
