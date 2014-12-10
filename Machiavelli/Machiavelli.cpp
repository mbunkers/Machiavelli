// Machiavelli.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Socket.h"
#include "ASCIIgen.h"

#ifdef _WIN32
#include <cstdlib>
#else
#include <stdlib.h>
#endif

int main(int argc, const char * argv[]) {
	// insert code here...
	string serverPort = "";
	string serverIp = "";
	ASCIIgen artGen;

	cout << artGen.getTitle();
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
	

	cout << "Trying to connect on " << serverIp << ":" << serverPort << "\n";

	//TODO smartpointer
	unique_ptr<ClientSocket> socket(new ClientSocket(serverIp.c_str(), atoi(serverPort.c_str())));
	if (socket){
        for (int n = 0; n < 10; n++){
            printf( "\n\n\n\n\n\n\n\n\n\n" );
        }

		cout << "You've succeeded in connecting with the server. \n" << "Please wait for the game to start\n";
		while (socket){
			
		}
	}
	return 0;
}

