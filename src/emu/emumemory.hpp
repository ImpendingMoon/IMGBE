/**
 * @file emu/emumemory.hpp
 * @brief Implements the system's memory
 * @author ImpendingMoon
 * @date 2023-07-15
 */

#pragma once

#include "memorybank.hpp"

#include <vector>
#include <cstdint>

class EmuMemory
{
public:
	EmuMemory();
	~EmuMemory();

	/**
	 * @brief Reads a byte from memory
	 * @param address
	 * @param ignore_illegal default=false, Ignore illegal reads (for debug)
	 * @return Byte at address, 0x00 if address is locked.
	 * @throws std::out_of_range if illegal address is accessed.
	 */
	uint8_t readByte(uint16_t address, bool ignore_illegal = false);

	/**
	 * @brief Writes a byte to memory
	 * @param address
	 * @param value
	 * @throws std::out_of_range if illegal address is accessed.
	 */
	void writeByte(uint16_t address, uint8_t value);

	/**
	 * @brief Dumps memory contents to the log as LOG_DEBUG.
	 */
	void dumpMemory(void);

private:
	MemoryBank ROM0;
	std::vector<MemoryBank> ROM1;
	size_t ROM1Index = 0;
	MemoryBank VRAM;
	std::vector<MemoryBank> ERAM;
	size_t ERAMIndex = 0;
	MemoryBank WRAM0;
	std::vector<MemoryBank> WRAM1;
	size_t WRAM1Index = 0;
	MemoryBank OAM;
	MemoryBank IOREG;
	MemoryBank HRAM;
	MemoryBank IEREG;
};

// Fun data junk
constexpr size_t ROM0_START = 0x0000;
constexpr size_t ROM0_END = 0x3FFF;
constexpr size_t ROM1_START = 0x4000;
constexpr size_t ROM1_END = 0x7FFF;
constexpr size_t VRAM_START = 0x8000;
constexpr size_t VRAM_END = 0x9FFF;
constexpr size_t ERAM_START = 0xA000;
constexpr size_t ERAM_END = 0xBFFF;
constexpr size_t WRAM0_START = 0xC000;
constexpr size_t WRAM0_END = 0xCFFF;
constexpr size_t WRAM1_START = 0xD000;
constexpr size_t WRAM1_END = 0xDFFF;
constexpr size_t ECHO_START = 0xE000;
constexpr size_t ECHO_END = 0xFDFF;
constexpr size_t OAM_START = 0xFE00;
constexpr size_t OAM_END = 0xFE9F;
constexpr size_t IOREG_START = 0xFEA0;
constexpr size_t IOREG_END = 0xFF00;
constexpr size_t HRAM_START = 0xFF80;
constexpr size_t HRAM_END = 0xFFFE;
constexpr size_t IEREG_START = 0xFFFF;
constexpr size_t IEREG_END = 0xFFFF;
