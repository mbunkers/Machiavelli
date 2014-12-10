#include "stdafx.h"
#include "ASCIIgen.h"



ASCIIgen::ASCIIgen()
{
}


ASCIIgen::~ASCIIgen()
{
}

string ASCIIgen::getTitle(){
	vector<string> stringBuilder;

	stringBuilder.push_back(" _______  _______  _______          _________ _______           _______  _        _       _________");
	stringBuilder.push_back("(       )(  ___  )(  ____ \\|\\     /|\\__   __/(  ___  )|\\     /|(  ____ \\( \\      ( \\      \\__   __/");
	stringBuilder.push_back("| () () || (   ) || (    \\/| )   ( |   ) (   | (   ) || )   ( || (    \\/| (      | (         ) (   ");
	stringBuilder.push_back("| || || || (___) || |      | (___) |   | |   | (___) || |   | || (__    | |      | |         | |   ");
	stringBuilder.push_back("| |(_)| ||  ___  || |      |  ___  |   | |   |  ___  |( (   ) )|  __)   | |      | |         | |   ");
	stringBuilder.push_back("| |   | || (   ) || |      | (   ) |   | |   | (   ) | \\ \\_/ / | (      | |      | |         | |   ");
	stringBuilder.push_back("| )   ( || )   ( || (____/\\| )   ( |___) (___| )   ( |  \\   /  | (____/\\| (____/\\| (____/\\___) (___");
	stringBuilder.push_back("|/     \\||/     \\|(_______/|/     \\|\\_______/|/     \\|   \\_/   (_______/(_______/(_______/\\_______/");
	stringBuilder.push_back("                                                                                                   ");

	return genOutputString(stringBuilder);
}

string ASCIIgen::genOutputString(vector<string> input){
	string output = "";
	for (size_t i = 0; i < input.size(); i++){
		output += input[i] + "\n";
	}
	return output;
}