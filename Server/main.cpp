//
//  main.cpp
//  socketexample
//
//  Created by Bob Polis on 16/09/14.
//  Copyright (c) 2014 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#include <thread>
#include <iostream>
#include <exception>
#include <memory>
using namespace std;

#include "Socket.h"
#include "Sync_queue.h"
#include "ClientCommand.h"
#include "Game.h"
#include <chrono>
#include <thread>

namespace socketexample {
    const std::string prompt {"> \n"};
}

static Sync_queue<ClientCommand> queue;
static shared_ptr<Game> mGame;

void consume_command() // runs in its own thread
{
    while (true) {
        ClientCommand command;
        queue.get(command); // will block here unless there still are command objects in the queue
        shared_ptr<Socket> client {command.get_client()};
        if (client) {
            try {
                string returnValue = mGame->handleRequest(client, command);
                //client->write(returnValue);
                //client->write("\n");
                
            } catch (const exception& ex) {
                client->write("Sorry, ");
                client->write(ex.what());
                client->write("\n");
                client->write(socketexample::prompt);
            } catch (...) {
                client->write("Sorry, something went wrong during handling of your request.\n");
                client->write(socketexample::prompt);
            }
        } else {
            cerr << "trying to handle command for client who has disappeared...\n";
        }
    }
}

void handle_client(Socket* socket) // this function runs in a separate thread
{
    shared_ptr<Socket> client {socket};
    client->write("Welcome to Machiavelli 1.0! What is your name?\n");
    client->write(socketexample::prompt);

    while (true) { // game loop
        try {
            // read first line of request
            string cmd = client->readline();
            cerr << "client (" << client->get() << ") said: " << cmd << '\n';
            
            if (cmd == "quit") {
                client->write("Bye!\n");
                break; // out of game loop, will end this thread and close connection
            }
            
            ClientCommand command {cmd, client};
            queue.put(command);

        } catch (const exception& ex) {
            client->write("ERROR: ");
            client->write(ex.what());
            client->write("\n");
            client->write(socketexample::prompt);
        } catch (...) {
            client->write("ERROR: something went wrong during handling of your request. Sorry!\n");
            client->write(socketexample::prompt);
        }
    }
}

int main(int argc, const char * argv[])
{
    // start command consumer thread
    thread consumer {consume_command};
    consumer.detach(); // detaching is usually ugly, but in this case the right thing to do

    // Aks for port to connect with, otherwise use default
    cout << "Enter the port the server should listen to or use as default 1080\n";
    string port = "";
    getline(cin, port);
    if (port.empty()){
        port = "1080";
        cout << port << "\n";
    }

    cout << "Trying to initialize on port " << port << "\n";
    
	// create a server socket
    try {
        ServerSocket server(atoi(port.c_str()));
        mGame = make_shared<Game>();

        while (true) {
            try {
                // wait for connection from client; will create new socket
                cerr << "Server is up and running" << '\n';
                Socket* client = nullptr;

                while ((client = server.accept()) != nullptr) {
                    // communicate with client over new socket in separate thread
                    thread handler {handle_client, client};
                    handler.detach(); // detaching is usually ugly, but in this case the right thing to do
                    cerr << "Server is listening for more incoming clients" << '\n';
                }
            } catch (const exception& ex) {
                cerr << ex.what() << ", resuming..." << '\n';
            }
        }
    } catch (exception exception) {
        cout << exception.what();
        chrono::seconds dura(5);
        this_thread::sleep_for(dura);
    }

    return 0;
}

