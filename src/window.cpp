/**
 * @file window.cpp
 * @brief Handles the main window
 * @author ImpendingMoon
 * @date 2023-07-16
 */

#include "window.hpp"
#include <SDL2/SDL.h>
#include "fmt/core.h"
#include "logger.hpp"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

/**
 * @brief Creates the window and renderer.
 * @param title Window title
 * @param width Window width. Must be >= IMGBE_WINDOW_MIN_WIDTH
 * @param height Window height. Must be >= IMGBE_WINDOW_MIN_HEIGHT
 * @throws std::runtime_error on SDL failure.
 */
void windowInit(
	const std::string& title,
	int width,
	int height
)
{
	window = SDL_CreateWindow(
		"IMGBE",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		(width >= IMGBE_WIN_MIN_WIDTH) ? width : 160,
		(height >= IMGBE_WIN_MIN_HEIGHT) ? height : 144,
		0
		| SDL_WINDOW_SHOWN
		| SDL_WINDOW_RESIZABLE
		| SDL_WINDOW_ALLOW_HIGHDPI
	);

	if(window == nullptr)
	{
		throw std::runtime_error(fmt::format(
			"Cannot create window! Error: {}", SDL_GetError()
		));
	}

	renderer = SDL_CreateRenderer(
		window,
		-1,
		0
		| SDL_RENDERER_ACCELERATED
	);

	if(renderer == nullptr)
	{
		throw std::runtime_error(fmt::format(
			"Cannot create renderer! Error: {}", SDL_GetError()
		));
	}

	SDL_SetWindowMinimumSize(
		window,
		IMGBE_WIN_MIN_WIDTH,
		IMGBE_WIN_MIN_HEIGHT
	);
}



/**
 * @brief Destroys the window and renderer.
 */
void windowExit(void) noexcept
{
	if(window != nullptr) { SDL_DestroyWindow(window); }
	if(renderer != nullptr) { SDL_DestroyRenderer(renderer); }
}



/**
 * @brief Clears the window to black.
 */
void windowClear(void) noexcept
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}



/**
 * @brief Updates the window with any changes.
 */
void windowUpdate(void) noexcept
{
	SDL_RenderPresent(renderer);
}
