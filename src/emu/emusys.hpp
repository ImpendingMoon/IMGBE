/**
 * @file emu/emusys.hpp
 * @brief Implements the main emulated system
 * @author ImpendingMoon
 * @date 2023-08-21
 */

#pragma once

#include <filesystem>
#include "emumemory.hpp"
#include "emucartridge.hpp"
#include "emucpu.hpp"
#include "emuppu.hpp"

class EmuSys
{
public:
	EmuSys();
	~EmuSys();

	/**
	 * @brief Attempts to load a ROM file into the emulator
	 * @param file_path Path to ROM file
	 * @throws std::ios_base::failure on file error.
	 * @throws std::runtime_error on invalid ROM.
	 */
	void loadROM(std::filesystem::path file_path);

	/**
	 * @brief Runs through one frame of emulation if not paused
	 * @throws std::runtime_error on system not running.
	 */
	void runFrame(void);

	/**
	 * @brief Steps the system by one CPU instruction
	 * @throws std::runtime_error on system not running.
	 * @param log_instruction Whether to log internal operations (slow!)
	 * @returns Number of cycles used
	 */
	int step(bool log_instruction);

	/**
	 * @brief Starts the system with an opened ROM.
	 * @throws std::runtime_error on ROM not loaded.
	 */
	void start(void);

	/**
	 * @brief Toggles pause if the system is running.
	 */
	void togglePause(void) noexcept;

	/**
	 * @brief Pauses the system if it is running.
	 */
	void pause(void) noexcept;

	/**
	 * @brief Resumes the system if it is paused. 
	 */
	void resume(void) noexcept;

	/**
	 * @brief Stops the system if it is running.
	 */
	void stop(void) noexcept;

	/**
	 * @brief Stops and re-starts the system.
	 * @throws std::runtime_error on ROM not loaded.
	 */
	void reset(void);

	bool isLoaded(void) const noexcept;
	bool isRunning(void) const noexcept;
	bool isPaused(void) const noexcept;

	/**
	 * @brief Dumps information of the current system state to LOG_DEBUG
	 */
	void dumpSystem(void) const;

private:
	bool loaded = false;
	bool running = false;
	bool paused = false;
	std::filesystem::path romFilePath = "";

	EmuMemory mem;
	EmuCartridge cart;
	EmuCPU cpu;
	EmuPPU ppu;

	int cpu_speed = 4194304;
};
