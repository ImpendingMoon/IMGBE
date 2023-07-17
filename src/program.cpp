/**
 * @file program.cpp
 * @brief Handles the main loop
 * @author ImpendingMoon
 * @date 2023-07-16
 */

#include "program.hpp"
#include <stdexcept>
#include <filesystem>
#include <fmt/core.h>
#include <SDL2/SDL.h>
#include "logger.hpp"
#include "window.hpp"
#include "main.hpp"
#include "emu/emusys.hpp"

bool exitRequested = false;
double frameRate = 60;

void handleArguments(int argc, char** argv);
void handleEvents(void) noexcept;
void throwInvalidArgument(const std::string& argument);
std::string getKey(const std::string& pair, char delimiter);
std::string getValue(const std::string& pair, char delimiter);
void createEmuSystem(void) noexcept;

EmuSys* emuSystem = nullptr;

/**
 * @brief Runs the main program loop until an exit is requested
 * @param argc
 * @param argv
 */
void runMainLoop(int argc, char** argv) noexcept
{
	try
	{
		handleArguments(argc, argv);
	} catch(std::invalid_argument& ex)
	{
		std::cerr << ex.what();
		mainExit();
		exit(1);
	}

	createEmuSystem();

	logMessage("Starting main loop...", LOG_INFO);

	while(!exitRequested)
	{
		handleEvents();
		windowClear();
		windowUpdate();
	}

	emuSystem->dumpSystem();
	delete emuSystem;

	logMessage("Exited main loop.", LOG_INFO);
}



/**
 * @brief Requests to exit the program at the end of the current loop
 */
void requestExit(void) noexcept
{
	logMessage("Main loop exit requested...", LOG_INFO);
	exitRequested = true;
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
			setLogLevel(LOG_NOTHING);
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
			mainExit();
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
			try { level = std::atoi(value.c_str()); }
			catch(std::invalid_argument&) { throwInvalidArgument(argument); }

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
			if(!std::filesystem::exists(file_path))
			{
				throw std::invalid_argument(
					"Cannot open file "
					+ file_path.string()
				);
			}

			try
			{
				createEmuSystem();
				emuSystem->stop();
				emuSystem->loadROM(file_path);
				emuSystem->start();
			} catch(std::exception& ex)
			{
				logMessage(fmt::format(
					"Couldn't load file {}. Error: {}",
					file_path.string(), ex.what()
				));
			}
		}
		}
	}
}



/**
 * @brief Polls and handles all SDL_Events in the SDL Queue
 */
void handleEvents(void) noexcept
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
		{
			requestExit();
			break;
		}

		case SDL_DROPFILE:
		{
			std::filesystem::path file_path = event.drop.file;

			try
			{
				createEmuSystem();
				emuSystem->stop();
				emuSystem->loadROM(file_path);
				emuSystem->start();
			} catch(std::exception& ex)
			{
				logMessage(fmt::format(
					"Couldn't load file {}. Error: {}",
					file_path.string(), ex.what()
				));
			}
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



/**
 * @brief Gets the key in a key/value pair
 * @param pair Key/Value pair
 * @param delimiter Key/Value separator
 * @throws std::invalid_argument if delimiter not found.
 */
std::string getKey(const std::string& pair, char delimiter)
{
	size_t delim_index = pair.find_first_of(delimiter);

	if(delim_index == std::string::npos)
	{
		std::string error_msg = fmt::format(
			"Cannot find delimiter {} in string {}!",
			delimiter,
			pair
		);

		throw std::invalid_argument(error_msg);
	}

	return pair.substr(0, delim_index);
}



/**
 * @brief Gets the value in a key/value pair
 * @param pair Key/Value pair
 * @param delimiter Key/Value separator
 * @throws std::invalid_argument if delimiter not found.
 */
std::string getValue(const std::string& pair, char delimiter)
{
	size_t delim_index = pair.find_first_of(delimiter);

	if(delim_index == std::string::npos)
	{
		std::string error_msg = fmt::format(
			"Cannot find delimiter {} in string {}!",
			delimiter,
			pair
		);

		throw std::invalid_argument(error_msg);
	}

	return pair.substr(delim_index + 1, pair.length());
}



/**
 * @brief Creates the emuSystem object, if it doesn't already exist.
 */
void createEmuSystem(void) noexcept
{
	if(emuSystem == nullptr) { emuSystem = new EmuSys(); }
}
