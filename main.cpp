// main.cpp:

#include <iostream>

#include "GameEngine.h"
//#include "Constants.h"

using namespace unrailed_game;

int main()
{
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	std::locale::global(std::locale(""));

	GameEngine game;
	game.Run();

	return 0;
}