//
//  ClientCommand.h
//  socketexample
//
//  Created by Bob Polis on 27/11/14.
//  Copyright (c) 2014 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#ifndef __socketexample__ClientCommand__
#define __socketexample__ClientCommand__

#include <string>
#include <memory>
#include "Socket.h"

class ClientCommand {
public:
    ClientCommand() {}
    ClientCommand(const string& cmd, shared_ptr<Socket> client) : cmd(cmd), client(client) {}
    
    string get_cmd() const { return cmd; }
    void set_cmd(const string& command) { cmd = command; }
    
    shared_ptr<Socket> get_client() const { return client; }
    void set_client(shared_ptr<Socket> socket) { client = socket; }
    
private:
    string cmd;
    shared_ptr<Socket> client;
};

#endif /* defined(__socketexample__ClientCommand__) */
