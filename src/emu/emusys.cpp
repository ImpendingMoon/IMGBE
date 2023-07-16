/**
 * @file emu/emusys.cpp
 * @brief Implements the main emulated system
 * @author ImpendingMoon
 * @date 2023-07-15
 */

#include "emusys.hpp"



EmuSys::EmuSys()
{}



EmuSys::~EmuSys()
{}


/**
 * @brief Attempts to load a ROM file into the emulator
 * @param file_path Path to ROM file
 * @throws std::invalid_argument on non-existant file or bad ROM.
 */
void EmuSys::loadROM(std::filesystem::path file_path)
{
}



/**
 * @brief Steps the system by one CPU instruction
 * @throws std::runtime_error on system not running.
 */
void EmuSys::step(void)
{
}



/**
 * @brief Starts the system with an opened ROM.
 * @throws std::runtime_error on ROM not loaded.
 */
void EmuSys::start(void)
{
}



/**
 * @brief Pauses the system if it is running.
 */
void EmuSys::pause(void)
{
}



/**
 * @brief Resumes the system if it is paused.
 */
void EmuSys::resume(void)
{
}



/**
 * @brief Stops the system if it is running.
 */
void EmuSys::stop(void)
{
}



/**
 * @brief Stops and re-starts the system.
 * @throws std::runtime_error on ROM not loaded.
 */
void EmuSys::reset(void)
{
}



bool EmuSys::isLoaded(void) const
{
	return loaded;
}



bool EmuSys::isRunning(void) const
{
	return running;
}



bool EmuSys::isPaused(void) const
{
	return paused;
}



/**
 * @brief Dumps information of the current system state to LOG_DEBUG
 */
void EmuSys::dumpSystem(void) const
{
}
