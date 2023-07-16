/**
 * @file emu/emumemory.cpp
 * @brief Implements the system's memory
 * @author ImpendingMoon
 * @date 2023-07-15
 */

#include "emumemory.hpp"

EmuMemory::EmuMemory() :
	ROM0(ROM0_START, ROM0_END, false, true),
	ROM1(),
	VRAM(VRAM_START, VRAM_END),
	ERAM(),
	WRAM0(WRAM0_START, WRAM0_END),
	WRAM1(),
	OAM(OAM_START, OAM_END),
	IOREG(IOREG_START, IOREG_END),
	HRAM(HRAM_START, HRAM_END),
	IEREG(IEREG_START, IEREG_END)
{}

EmuMemory::~EmuMemory()
{}



/**
 * @brief Reads a byte from memory
 * @param address
 * @param ignore_illegal default=false, Ignore illegal reads (for debug)
 * @return Byte at address, 0x00 if address is locked.
 * @throws std::out_of_range if illegal address is accessed.
 */
uint8_t EmuMemory::readByte(uint16_t address, bool ignore_illegal)
{
	return 0;
}



/**
 * @brief Writes a byte to memory
 * @param address
 * @param value
 * @throws std::out_of_range if illegal address is accessed.
 */
void EmuMemory::writeByte(uint16_t address, uint8_t value)
{
}



/**
 * @brief Dumps memory contents to the log as LOG_DEBUG.
 */
void EmuMemory::dumpMemory(void)
{
}

