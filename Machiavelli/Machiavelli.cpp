//
//  Machiavelli.cpp
//  Machiavelli
//
//  Created by Marc Bunkers and Leon van Tuijl.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#include "stdafx.h"
#include "Socket.h"
#include <chrono>
#include <thread>
#include "InputHandler.h"

#ifdef _WIN32
#include <cstdlib>
#else
#include <stdlib.h>
#endif

int main(int argc, const char * argv[]) {
    // insert code here...
	InputHandler inputHandler;
    string serverPort = "";
    string serverIp = "";
    
	//Draw title.
	inputHandler.handleInput("title");

	//Initial user input requests for server connection setup.
    cout << "Please insert the Ip of the server. or default to localhost\n";
    getline(cin, serverIp);
    if (serverIp.empty()){
        serverIp = "localhost";
        cout << serverIp << "\n";
    }

    cout << "Please insert the port of the server. or default to 1080\n";
    getline(cin, serverPort);
    if (serverPort.empty()){
        serverPort = "1080";
        cout << serverPort << "\n";
    }

	inputHandler.handleInput("clear");

    cout << "Trying to connect on " << serverIp << ":" << serverPort << "\n";

    try {
		//Start a socket with the ip and port
        unique_ptr<ClientSocket> socket(new ClientSocket(serverIp.c_str(), atoi(serverPort.c_str())));
		//Check if the socket is open and check for updates as long as the socket is open.
        if (socket){			
            while (socket){
                string input = "";
				string output = "";
				//Wait for input from the server
                while (input.empty()){
                    input = socket->readline();
                }
               
				//If the input equals "> " then the server requests output.
                if (input == "> "){
                    cout << "\n > ";
                    getline(cin, output);
                }
                else{
				//No output request.
					int handleCode = inputHandler.handleInput(input);
					//Standard delay time.
					chrono::seconds dura(5);
					//Input Commands
					switch (handleCode){
					case -1: //close application
						socket = nullptr;
						this_thread::sleep_for(dura);
						break;
					case 0: //do nothing
						break;
					case 1: //write output line
						cout << input << "\n";
						break;
					case 2: //write output
						cout << input;
						break;
					default:						
						break;
					}
                }	
				//If output was requested then send it back to the server.
				if (!output.empty()){
					socket->write(output + "\r\n");
				}
            }
        }
    } catch (exception exception) {
        cout << "Connection failed\n" << "Please restart\n";
        chrono::seconds dura(5);
        this_thread::sleep_for(dura);
    }
    return 0;
}

