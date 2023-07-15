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
 * @param log_to_file Attempt to open a logfile and print logs, default=true
 * @param log_to_cout Print logs to cout/cerr, default=true
 */
void loggerInit(
	LOG_LEVELS log_level = LOG_ERRORS,
	bool log_to_file = true,
	bool log_to_cout = true
);

/**
 * @brief Closes the logfile.
 */
void loggerExit(void);

/**
 * @brief Logs a message
 * @param msg
 * @param level default=LOG_INFO
 */
void logMessage(const std::string& msg, LOG_LEVELS level = LOG_INFO);
