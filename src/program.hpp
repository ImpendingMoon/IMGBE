/**
 * @file program.cpp
 * @brief Handles the main loop
 * @author ImpendingMoon
 * @date 2023-07-23
 */

#pragma once

constexpr const char* IMGBE_VERSION_STRING = "0.1.8-devel";

#include <iostream>
#include <filesystem>

/**
 * @brief Runs the main program loop until an exit is requested
 */
void runMainLoop(void) noexcept;

/**
 * @brief Requests to exit the program at the end of the current loop
 */
void requestExit(void) noexcept;

/**
 * @brief Gets the key in a key/value pair
 * @param pair Key/Value pair
 * @param delimiter Key/Value separator
 * @throws std::invalid_argument if delimiter not found.
 */
std::string getKey(const std::string& pair, char delimiter);

/**
 * @brief Gets the value in a key/value pair
 * @param pair Key/Value pair
 * @param delimiter Key/Value separator
 * @throws std::invalid_argument if delimiter not found.
 */
std::string getValue(const std::string& pair, char delimiter);

/**
 * @brief Creates the emulated system, if it doesn't already exist.
 */
void createEmuSystem(void) noexcept;

/**
 * @brief Attempts to open a ROM in the emulated system.
 * @param file_path
 */
void loadEmuSystem(const std::filesystem::path file_path) noexcept;
