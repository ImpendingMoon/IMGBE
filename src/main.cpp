/**
 * @file main.cpp
 * @brief Handles program initialization
 * @author ImpendingMoon
 * @date 2023-07-15
 */

#include "main.hpp"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

int main(int argc, char** argv)
{
	mainInit();
	mainExit();
	return 0;
}



/**
 * @brief Initializes SDL, Logger, and the Window.
 * @return 0 on success, 1 on SDL error
 */
int mainInit(void)
{
	int err;
	err = SDL_Init(
		0
		| SDL_INIT_VIDEO
		| SDL_INIT_EVENTS
	);

	if(err != 0) { return 1; }
	return 0;
}



/**
 * @brief Properly quits SDL and Logger.
 */
void mainExit(void)
{
	SDL_Quit();
}
