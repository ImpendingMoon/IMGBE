/**
 * @file logger.hpp
 * @brief Handles logging to cout, cerr, and a logfile.
 * @author ImpendingMoon
 * @date 2023-07-15
 */

#pragma once

#include <iostream>

// Logged messages must be >= log level to be printed.
enum LOG_LEVELS
{
	LOG_NOTHING,
	LOG_ERRORS,
	LOG_INFO,
	LOG_DEBUG,
};

/**
 * @brief Initializes the logger with given settings, or defaults
 * @param log_level default=LOG_ERRORS
 * @param log_to_file default=true, Does not attempt to open a file when false.
 * @param log_to_cout default=true
 * @throws std::runtime_error If log file cannot be opened.
 */
void loggerInit(
	LOG_LEVELS log_level = LOG_ERRORS,
	bool log_to_file = true,
	bool log_to_cout = true
);

/**
 * @brief Closes the logfile.
 */
void loggerExit(void) noexcept;

/**
 * @brief Logs a message
 * @param msg
 * @param level
 * @throws std::runtime_error if run before loggerInit.
 */
void logMessage(const std::string& msg, LOG_LEVELS level);

void setLogLevel(LOG_LEVELS level) noexcept;

void setLogToCout(bool value) noexcept;

void setLogToFile(bool value) noexcept;

/**
 * @brief Returns a timestamp formatted at HH:MM:SS
 */
std::string getTimestamp(void) noexcept;
