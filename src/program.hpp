/**
 * @file program.cpp
 * @brief Handles the main loop
 * @author ImpendingMoon
 * @date 2023-07-15
 */

#pragma once

/**
 * @brief Runs the main program loop until an exit is requested
 * @param argc
 * @param argv
 */
void runMainLoop(int argc, char** argv) noexcept;

/**
 * @brief Requests to exit the program at the end of the current loop
 */
void requestExit(void) noexcept;
