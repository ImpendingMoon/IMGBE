/**
 * @file main.hpp
 * @brief Handles program initialization
 * @author ImpendingMoon
 * @date 2023-07-19
 */

#pragma once

 /**
  * @brief Initializes SDL, Logger, and the Window.
  * @throws std::runtime_error on SDL failure.
  */
void mainInit(void);

/**
 * @brief Properly quits SDL and Logger.
 */
void mainExit(void) noexcept;

/**
 * @brief Handles provided arguments. Throws exceptions for invalid arguments.
 * @param argc
 * @param argv
 * @throws std::invalid_argument if program argument is invalid.
 */
void handleArguments(int argc, char** argv);
