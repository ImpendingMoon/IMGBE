/**
 * @file main.cpp
 * @brief Handles program initialization
 * @author ImpendingMoon
 * @date 2023-07-15
 */

#include "main.hpp"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "logger.hpp"



int main(int argc, char** argv)
{
	mainInit();
	mainExit();
	return 0;
}



/**
 * @brief Initializes SDL, Logger, and the Window.
 */
void mainInit(void)
{
	int err;
	err = SDL_Init(
		0
		| SDL_INIT_VIDEO
		| SDL_INIT_EVENTS
	);

	if(err != 0)
	{
		std::string error_message = SDL_GetError();
		throw std::runtime_error("Couldn't initialize SDL2! "
								 + error_message
		);
	}

	loggerInit(LOG_DEBUG, true, true);

	logMessage("Successfully started IMGBE.", LOG_INFO);
}



/**
 * @brief Properly quits SDL and Logger.
 */
void mainExit(void)
{
	logMessage("Exiting IMGBE...", LOG_INFO);
	loggerExit();
	SDL_Quit();
}
