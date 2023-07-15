/**
 * @file logger.cpp
 * @brief Handles logging to cout, cerr, and a logfile.
 * @author ImpendingMoon
 * @date 2023-07-15
 */

#include "logger.hpp"



 /**
  * @brief Initializes the logger with given settings, or defaults
  * @param log_level default=LOG_ERRORS
  * @param log_to_file default=true, Does not attempt to open a file when false.
  * @param log_to_cout default=true
  * @return 0 on success, 1 on file failure
  */
int loggerInit(
	LOG_LEVELS log_level = LOG_ERRORS,
	bool log_to_file = true,
	bool log_to_cout = true
)
{
	return 0;
}



/**
 * @brief Closes the logfile.
 */
void loggerExit(void)
{

}



/**
 * @brief Logs a message
 * @param msg
 * @param level default=LOG_INFO
 */
void logMessage(const std::string& msg, LOG_LEVELS level = LOG_INFO)
{

}