/**
 * @file emu/emusys.cpp
 * @brief Implements the main emulated system
 * @author ImpendingMoon
 * @date 2023-08-21
 */

#include "emusys.hpp"
#include <fmt/core.h>
#include "../logger.hpp"



EmuSys::EmuSys() :
	mem(),
	cart(&mem),
	cpu(&mem, this),
	ppu(&mem, &cpu)
{
	mem.setCPURegisters(cpu.getRegsPtr());
	logMessage("Emulated system created.", LOG_INFO);
}



EmuSys::~EmuSys()
{
	logMessage("Emulated system destroyed.", LOG_INFO);
}


/**
 * @brief Attempts to load a ROM file into the emulator
 * @param file_path Path to ROM file
 * @throws std::ios_base::failure on file error.
 * @throws std::runtime_error on invalid ROM.
 */
void EmuSys::loadROM(std::filesystem::path file_path)
{
	cart.loadROM(file_path);
	loaded = true;
}



/**
 * @brief Runs through one frame of emulation if not paused
 * @throws std::runtime_error on system not running.
 */
void EmuSys::runFrame(void)
{
	if(!running)
	{
		throw std::runtime_error("Cannot step system that is not running!");
	}

	if(paused) { return; }

	int cycles_per_frame = cpu_speed / 59.7;
	int cycles = 0;

	while(cycles < cycles_per_frame)
	{
		if(paused) { break; } // CPU breakpoints pause in-frame
		cycles += step(true); // TEMP: Will not log by default
	}
}



/**
 * @brief Steps the system by one CPU instruction
 * @throws std::runtime_error on system not running.
 */
int EmuSys::step(bool log_instruction)
{
	if(!running)
	{
		throw std::runtime_error("Cannot step system that is not running!");
	}

	int cycles = cpu.step(log_instruction);
	ppu.step(cycles);
	return cycles;
}



/**
 * @brief Starts the system with an opened ROM.
 * @throws std::runtime_error on ROM not loaded.
 */
void EmuSys::start(void)
{
	if(!loaded)
	{
		throw std::runtime_error("Cannot start system without loaded ROM!");
	}

	cpu.initRegs();

	running = true;
	// TEMP WHILE DEBUGGING INSTRUCTIONS
	paused = true;
}



/**
 * @brief Toggles pause if the system is running.
 */
void EmuSys::togglePause(void) noexcept
{
	if(!running) { return; }
	paused = !paused;
}



/**
 * @brief Pauses the system if it is running.
 */
void EmuSys::pause(void) noexcept
{
	if(!running) { return; }
	paused = true;
}



/**
 * @brief Resumes the system if it is paused.
 */
void EmuSys::resume(void) noexcept
{
	if(!running) { return; }
	paused = false;
}



/**
 * @brief Stops the system if it is running.
 */
void EmuSys::stop(void) noexcept
{
	paused = false;
	running = false;
}



/**
 * @brief Stops and re-starts the system.
 * @throws std::runtime_error on ROM not loaded.
 */
void EmuSys::reset(void)
{
	if(!loaded)
	{
		throw std::runtime_error("Cannot start system without loaded ROM!");
	}

	stop();
	start();
}



bool EmuSys::isLoaded(void) const noexcept
{
	return loaded;
}



bool EmuSys::isRunning(void) const noexcept
{
	return running;
}



bool EmuSys::isPaused(void) const noexcept
{
	return paused;
}



/**
 * @brief Dumps information of the current system state to LOG_DEBUG
 */
void EmuSys::dumpSystem(void) const
{
	logMessage("---BEGIN SYSTEM DUMP---", LOG_DEBUG);
	mem.dumpMemory();
	logMessage("---END SYSTEM DUMP---", LOG_DEBUG);
}
