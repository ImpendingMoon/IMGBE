/**
 * @file logger.cpp
 * @brief Handles logging to cout, cerr, and a logfile.
 * @author ImpendingMoon
 * @date 2023-07-15
 */

#include "logger.hpp"
#include <fstream>
#include <exception>
#include <SDL2/SDL.h>
#include <filesystem>



bool isInitialized = false;
LOG_LEVELS logLevel;
bool logToFile;
bool logToCout;
std::filesystem::path logFilePath = "";
std::ofstream logFile;



/**
 * @brief Initializes the logger with given settings, or defaults
 * @param log_level default=LOG_ERRORS
 * @param log_to_file default=true, Does not attempt to open a file when false.
 * @param log_to_cout default=true
 */
void loggerInit(
	LOG_LEVELS log_level,
	bool log_to_file,
	bool log_to_cout
)
{
	logLevel = log_level;
	logToCout = log_to_cout;

	if(log_to_file)
	{
		// SDL handles creating the directory
		logFilePath = SDL_GetPrefPath("ImpendingMoon", "IMGBE");
		// Returns NULL if failed, so try using current path.
		if(logFilePath.empty())
		{
			logFilePath = std::filesystem::current_path();
		}

		logFilePath += "imgbe.log";

		logFile.open(logFilePath, std::istream::app);

		if(!logFile.is_open())
		{
			throw new std::runtime_error("Cannot open file " + logFilePath.string());
		}
	}

	isInitialized = true;

	logMessage("Logger successfully initialized.", LOG_INFO);
}



/**
 * @brief Closes the logfile.
 */
void loggerExit(void)
{
	if(logFile.is_open()) { logFile.close(); }
	isInitialized = false;
}



/**
 * @brief Logs a message
 * @param msg
 * @param level default=LOG_INFO
 */
void logMessage(const std::string& msg, LOG_LEVELS level)
{
	if(!isInitialized)
	{
		throw new std::runtime_error("Logger must be initialized!");
	}

	if(logLevel == LOG_NOTHING || level == LOG_NOTHING)
	{
		return;
	}

	if(logToCout)
	{
		if(level == LOG_ERRORS)
		{
			std::cerr << msg << std::endl;
		} else
		{
			std::cout << msg << std::endl;
		}
	}

	if(logToFile && logFile.is_open())
	{
		logFile << msg << std::endl;
	}
}
