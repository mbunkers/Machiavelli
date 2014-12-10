#pragma once
#include <vector>
class ASCIIgen
{
public:
	ASCIIgen();
	string getTitle();
	string genOutputString(vector<string> input);
	virtual ~ASCIIgen();
};

