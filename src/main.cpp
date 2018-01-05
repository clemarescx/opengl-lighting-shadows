#include "GameManager.h"
#include <iostream>
#include <memory>

#ifdef _WIN32
#include <Windows.h>
#endif

/**
 * Simple blinn_phong_program that starts our game manager
 */
int main(int argc, char *argv[]) {
	std::shared_ptr<GameManager> game;
	game.reset(new GameManager());
	game->init();
	game->play();
	game.reset();
	return 0;
}
