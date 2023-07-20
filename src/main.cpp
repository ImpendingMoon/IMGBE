/**
 * @file main.cpp
 * @brief Handles program initialization
 * @author ImpendingMoon
 * @date 2023-07-19
 */

#include "main.hpp"
#include <stdexcept>
#include <filesystem>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <fmt/core.h>
#include "logger.hpp"
#include "program.hpp"
#include "window.hpp"

void throwInvalidArgument(const std::string& argument);

bool isMainInitialized = false;

int main(int argc, char** argv)
{
	try
	{
		handleArguments(argc, argv);
		mainInit();
	} catch(std::runtime_error& ex)
	{
		std::cerr
			<< "Could not initialize program! "
			<< ex.what()
			<< std::endl;
		exit(1);
	} catch(std::invalid_argument& ex)
	{
		std::cerr << ex.what();
		exit(1);
	}
	
	runMainLoop();
	mainExit();
	return 0;
}



/**
 * @brief Initializes SDL, Logger, and the Window.
 * @throws std::runtime_error on SDL failure.
 */
void mainInit(void)
{
	if(isMainInitialized) { return; }

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

	isMainInitialized = true;
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



/**
 * @brief Handles provided arguments. Throws exceptions for invalid arguments.
 * @param argc
 * @param argv
 * @throws std::invalid_argument if program argument is invalid.
 */
void handleArguments(int argc, char** argv)
{
	if(argc <= 1) { return; }

	for(int i = 1; i < argc; i++)
	{
		std::string argument = argv[i];

		// All valid arguments start with '-'.
		if(argument[0] != '-')
		{
			throwInvalidArgument(argument);
		}

		switch(argument[1]) // Originally supposed to be strings, didn't work.
		{
		case 'v':
		{
			std::cout
				<< "IMGBE Version "
				<< IMGBE_VERSION_STRING
				<< std::endl
				<< "Compiled on "
				<< __DATE__
				<< ", "
				<< __TIME__
				<< "."
				<< std::endl;
			exit(0);
			break;
		}

		case 'l': // Log level
		{
			if(argument.find('=') == std::string::npos)
			{
				throwInvalidArgument(argument);
			}

			std::string value = getValue(argument, '=');
			int level;
			try
			{
				level = std::atoi(value.c_str());
				mainInit();
			} catch(std::invalid_argument&)
			{
				throwInvalidArgument(argument);
			}

			if(level >= LOG_NOTHING && level <= LOG_DEBUG)
			{
				setLogLevel(static_cast<LOG_LEVELS>(level));
			} else
			{
				throwInvalidArgument(argument);
			}

			break;
		}

		case 'f': // File
		{
			if(argument.find('=') == std::string::npos)
			{
				throwInvalidArgument(argument);
			}

			std::filesystem::path file_path = getValue(argument, '=');
			
			mainInit();
			loadEmuSystem(file_path);
		}
		}
	}
}



/**
 * @brief Throws an invalid_argument exception with a formatted message
 * @param argument Argument string.
 * @throws std::invalid_argument always.
*/
void throwInvalidArgument(const std::string& argument)
{
	std::string error_msg = fmt::format(
	"Invalid program argument: {}",
	argument
	);
	throw std::invalid_argument(error_msg);
}
