/**
 * @file emu/emumemory.hpp
 * @brief Implements the system's memory
 * @author ImpendingMoon
 * @date 2023-07-16
 */

#pragma once

#include "memorybank.hpp"

#include <vector>
#include <cstdint>
#include <filesystem>

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
	uint8_t readByte(uint16_t address, bool ignore_illegal = false) const;

	/**
	 * @brief Writes a byte to memory
	 * @param address
	 * @param value
	 * @throws std::out_of_range if illegal address is accessed.
	 */
	void writeByte(uint16_t address, uint8_t value);

	/**
	 * @brief Initializes ROM0 with a set of data.
	 * @param data MemoryBank with range ROM0_START to ROM0_END.
	 * @throws std::invalid_argument if memory bank has an incorrect range.
	 */
	void initROM0(MemoryBank& data);

	/**
	 * @brief Initializes ROM1 with a set of data.
	 * @param bank_count Number of memory banks avaliable.
	 * @param initial_bank Initially referenced bank.
	 * @param data Vector of MemoryBanks with range ROM1_START to ROM1_END.
	 * @throws std::invalid_argument if data and bank count are mismatched, or
	 * if any memory bank has an incorrect range.
	 */
	void initROM1(
		size_t bank_count,
		size_t initial_bank,
		std::vector<MemoryBank>& data
	);

	/**
	 * @brief Initializes ERAM with a set of data.
	 * @param bank_count Number of memory banks avaliable.
	 * @param initial_bank Initially referenced bank.
	 * @param battery_backed Whether to save to a file (saved to ROM_NAME.sav).
	 * @param data Vector of MemoryBanks with range ERAM_START to ERAM_END.
	 * @throws std::invalid_argument if data and bank count are mismatched, or
	 * if any memory bank has an incorrect range.
	 */
	void initERAM(
		size_t bank_count,
		size_t initial_bank,
		bool battery_backed,
		std::vector<MemoryBank>& data
	);

	/**
	 * @brief Sets the currently-addressed ROM1 bank
	 * @param value
	 * @throws std::out_of_range if >= bank count.
	 */
	void setROM1Index(size_t value);

	/**
	 * @brief Sets the currently-addressed WRAM1 bank
	 * @param value
	 * @throws std::out_of_range if >= bank count.
	 */
	void setWRAM1Index(size_t value);

	/**
	 * @brief Sets the currently-addressed ERAM bank
	 * @param value
	 * @throws std::out_of_range if >= bank count.
	 */
	void setERAMIndex(size_t value);

	/**
	 * @brief If ERAM has changed and is battery backed, writes ERAM vectors to
	 * the save file.
	 */
	void writeERAM(void);

	/**
	 * @brief Returns a .sav file path from an existing file path
	 * @param rom_file_path 
	 */
	static std::filesystem::path getSAVPath(
		std::filesystem::path rom_file_path
	) noexcept;

	/**
	 * @brief Dumps memory contents to the log as LOG_DEBUG.
	 */
	void dumpMemory(void) const noexcept;

private:
	MemoryBank ROM0;

	std::vector<MemoryBank> ROM1;
	size_t ROM1BankCount = 0;
	size_t ROM1Index = 0;

	MemoryBank VRAM;

	std::vector<MemoryBank> ERAM;
	size_t ERAMBankCount = 0;
	size_t ERAMIndex = 0;
	bool ERAMBatteryBacked = false;
	bool ERAMDirty = false;

	MemoryBank WRAM0;

	std::vector<MemoryBank> WRAM1;
	size_t WRAM1Index = 0;
	size_t WRAM1BankCount = 8;

	MemoryBank OAM;
	MemoryBank IOREG;
	MemoryBank HRAM;
	MemoryBank IEREG;
};

// Memory Segment Addresses
constexpr size_t ROM0_START = 0x0000;
constexpr size_t ROM0_END = 0x3FFF;
constexpr size_t ROM0_SIZE = ROM0_END - ROM0_START + 1;
constexpr size_t ROM1_START = 0x4000;
constexpr size_t ROM1_END = 0x7FFF;
constexpr size_t ROM1_SIZE = ROM1_END - ROM1_START + 1;
constexpr size_t VRAM_START = 0x8000;
constexpr size_t VRAM_END = 0x9FFF;
constexpr size_t VRAM_SIZE = VRAM_END - VRAM_START + 1;
constexpr size_t ERAM_START = 0xA000;
constexpr size_t ERAM_END = 0xBFFF;
constexpr size_t ERAM_SIZE = ERAM_END - ERAM_START + 1;
constexpr size_t WRAM0_START = 0xC000;
constexpr size_t WRAM0_END = 0xCFFF;
constexpr size_t WRAM0_SIZE = WRAM0_END - WRAM0_START + 1;
constexpr size_t WRAM1_START = 0xD000;
constexpr size_t WRAM1_END = 0xDFFF;
constexpr size_t WRAM1_SIZE = WRAM1_END - WRAM1_START + 1;
constexpr size_t ECHO_START = 0xE000;
constexpr size_t ECHO_END = 0xFDFF;
constexpr size_t ECHO_SIZE = ECHO_END - ECHO_START + 1;
constexpr size_t OAM_START = 0xFE00;
constexpr size_t OAM_END = 0xFE9F;
constexpr size_t OAM_SIZE = OAM_END - OAM_START + 1;
constexpr size_t IOREG_START = 0xFEA0;
constexpr size_t IOREG_END = 0xFF00;
constexpr size_t IOREG_SIZE = IOREG_END - IOREG_START + 1;
constexpr size_t HRAM_START = 0xFF80;
constexpr size_t HRAM_END = 0xFFFE;
constexpr size_t HRAM_SIZE = HRAM_END - HRAM_START + 1;
constexpr size_t IEREG_START = 0xFFFF;
constexpr size_t IEREG_END = 0xFFFF;
constexpr size_t IEREG_SIZE = IEREG_END - IEREG_START + 1;
