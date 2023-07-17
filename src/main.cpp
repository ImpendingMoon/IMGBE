/**
 * @file main.cpp
 * @brief Handles program initialization
 * @author ImpendingMoon
 * @date 2023-07-15
 */

#include "main.hpp"
#include <stdexcept>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "logger.hpp"
#include "program.hpp"
#include "window.hpp"



int main(int argc, char** argv)
{
	try
	{
		mainInit();
	} catch(std::runtime_error& ex)
	{
		std::cerr
			<< "ERROR: Could not initialize program! "
			<< ex.what()
			<< std::endl;
		exit(1);
	}
	
	runMainLoop(argc, argv);
	mainExit();
	return 0;
}



/**
 * @brief Initializes SDL, Logger, and the Window.
 * @throws std::runtime_error on SDL failure.
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

	try
	{
		loggerInit(LOG_DEBUG, true, true);
	} catch(std::invalid_argument& ex)
	{
		std::cerr << "Couldn't initialize logger! " << ex.what() << std::endl;
		std::cerr << "Starting logger without logfile..." << std::endl;
		loggerInit(LOG_DEBUG, false, true);
	}

	windowInit("IMGBE", 160, 144);
}



/**
 * @brief Properly quits SDL and Logger.
 */
void mainExit(void) noexcept
{
	logMessage("Exiting IMGBE...", LOG_INFO);
	loggerExit();
	windowExit();
	SDL_Quit();
}
