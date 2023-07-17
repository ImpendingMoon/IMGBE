/**
 * @file window.hpp
 * @brief Handles the main window
 * @author ImpendingMoon
 * @date 2023-07-16
 */

#pragma once

#include <iostream>

constexpr int IMGBE_WIN_MIN_WIDTH = 160;
constexpr int IMGBE_WIN_MIN_HEIGHT = 144;

/**
 * @brief Creates the window and renderer.
 * @param title Window title
 * @param width Window width. Must be >= IMGBE_WIN_MIN_WIDTH
 * @param height Window height. Must be >= IMGBE_WIN_MIN_HEIGHT
 * @throws std::runtime_error on SDL failure.
 */
void windowInit(
	const std::string& title,
	int width,
	int height
);

/**
 * @brief Destroys the window and renderer.
 */
void windowExit(void) noexcept;


/**
 * @brief Clears the window to black.
 */
void windowClear(void) noexcept;

/**
 * @brief Updates the window with any changes.
 */
void windowUpdate(void) noexcept;
