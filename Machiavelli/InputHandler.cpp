#include "stdafx.h"
#include "InputHandler.h"
#include "ASCIIgen.h"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <Windows.h>
#endif

InputHandler::InputHandler()
{
	mEnums.emplace("BYE!", BYE);
	mEnums.emplace("BYE", BYE);
	mEnums.emplace("TITLE", TITLE);
	mEnums.emplace("CLEAR", CLEAR);
	mEnums.emplace("TABLE", TABLE);
}


InputHandler::~InputHandler()
{
}

int InputHandler::handleInput(string input){
	ASCIIgen artGen;
	int handleMessage = 1;

	transform(input.begin(), input.end(), input.begin(), ::toupper);
	commands enumCode = mEnums[input];
	switch (enumCode){
	case BYE:
		handleMessage = -1;
		cout << "\n";
		break;
	case CLEAR:
		handleMessage = 0;
		clear();
		break;
	case TITLE:	
		handleMessage = 0;
		setTextColor(YELLOW);
		cout << artGen.getTitle().c_str();
		setTextColor(WHITE);
		break;
	case TABLE:
		handleMessage = 0;
		break;
	default:
		handleMessage = 1;
		break;
	}
	return handleMessage;
}


void InputHandler::clear()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); 	
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
		);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
		);
	SetConsoleCursorPosition(console, topLeft);
#endif
#if defined(__APPLE__) || defined(__linux__)
	std::cout << "\x1B[2J\x1B[H";
#endif

}

void InputHandler::setTextColor(int colorCode){
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, colorCode);
#endif
#if defined(__APPLE__) || defined(__linux__)
	if(colorCode < 10){
		cout << "\x1B[" << colorCode << "m";
	}
#endif
}