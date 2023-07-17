/**
 * @file main.hpp
 * @brief Handles program initialization
 * @author ImpendingMoon
 * @date 2023-07-16
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
