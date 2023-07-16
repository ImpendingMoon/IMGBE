/**
 * @file logger.cpp
 * @brief Handles logging to cout, cerr, and a logfile.
 * @author ImpendingMoon
 * @date 2023-07-15
 */

#include "logger.hpp"
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <ctime>
#include <SDL2/SDL.h>
#include <fmt/core.h>



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
 * @throws std::runtime_error If log file cannot be opened.
 */
void loggerInit(
	LOG_LEVELS log_level,
	bool log_to_file,
	bool log_to_cout
)
{
	logLevel = log_level;
	logToCout = log_to_cout;
	logToFile = log_to_file;

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

		logFile.open(logFilePath, std::istream::out);

		if(!logFile.is_open())
		{
			throw std::runtime_error(
				"Cannot open file "
				+ logFilePath.string()
			);
		}
	}

	isInitialized = true;
}



/**
 * @brief Closes the logfile.
 */
void loggerExit(void) noexcept
{
	if(logFile.is_open()) { logFile.close(); }
	isInitialized = false;
}



/**
 * @brief Logs a message
 * @param msg
 * @param level default=LOG_INFO
 * @throws std::runtime_error If run before loggerInit.
 */
void logMessage(const std::string& msg, LOG_LEVELS level)
{
	if(!isInitialized)
	{
		throw std::runtime_error("Logger must be initialized!");
	}

	if(logLevel == LOG_NOTHING || level == LOG_NOTHING)
	{
		return;
	}

	std::string fmt_message = fmt::format(
		"[{}] {}\n",
		getTimestamp(),
		msg
		);

	if(logToCout)
	{
		if(level == LOG_ERRORS)
		{
			std::cerr << fmt_message;
		} else
		{
			std::cout << fmt_message;
		}
	}

	if(logToFile && logFile.is_open())
	{
		logFile << fmt_message;
	}
}



void setLogLevel(LOG_LEVELS level) noexcept
{
	logLevel = level;
}



void setLogToCout(bool value) noexcept
{
	logToCout = value;
}



void setLogToFile(bool value) noexcept
{
	logToCout = value;
}



/**
 * @brief Returns a timestamp formatted at HH:MM:SS
 */
std::string getTimestamp(void) noexcept
{
	std::time_t current_time = time(nullptr);
	std::tm local_time;
	localtime_s(&local_time, &current_time);

	return fmt::format(
		"{:02d}:{:02d}:{:02d}",
		local_time.tm_hour,
		local_time.tm_min,
		local_time.tm_sec
	);
}
