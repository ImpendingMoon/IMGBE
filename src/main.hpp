/**
 * @file main.hpp
 * @brief Handles program initialization
 * @author ImpendingMoon
 * @date 2023-07-15
 */

#pragma once

/**
 * @brief Initializes SDL, Logger, and the Window.
 * @return 0 on success, 1 on SDL error
 */
int mainInit(void);

/**
 * @brief Properly quits SDL and Logger.
 */
void mainExit(void);
