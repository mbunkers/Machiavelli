#pragma once
#include "stdafx.h"
#include <unordered_map>
class InputHandler
{
private:
	enum commands{
		BYE = 1,
		CLEAR = 2,
		TABLE = 3,
		TITLE = 4,
	};
	unordered_map<string, commands> mEnums;

	/* Standard error macro for reporting API errors */
#define PERR(bSuccess, api){if(!(bSuccess)) printf("%s:Error %d from %s \ on line %d\n", __FILE__, GetLastError(), api, __LINE__);}

	void clear();
public:
	InputHandler();
	virtual ~InputHandler();
	int handleInput(string input);
};

