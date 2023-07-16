/**
 * @file emu/emucartridge.hpp
 * @brief Handles loading ROM files into memory
 * @author ImpendingMoon
 * @date 2023-07-16
 */

#pragma once

#include <fstream>
#include <filesystem>
#include <array>
#include <cstdint>
#include "memorybank.hpp"
#include "emumemory.hpp"

class EmuCartridge
{
public:
	EmuCartridge(EmuMemory* memory);
	~EmuCartridge();

	void setMemoryPointer(EmuMemory* memory);

	/**
	 * @brief Loads a ROM file into memory.
	 * @param file_path
	 * @throws std::runtime_error if memory pointer is null.
	 * @throws std::ios_base::failure on file error.
	 * @throws std::runtime_error on invalid ROM.
	 */
	void loadROM(std::filesystem::path file_path);

	/**
	 * @brief Returns the loaded ROM name.
	 */
	std::string getROMName(void);

private:
	EmuMemory* mem;
	std::ifstream ROMFile;
	std::filesystem::path ROMFilePath;
	std::string ROMName = "NOGAME";

	/**
	 * @brief Checks if a given ROM header is valid.
	 * @param header
	 */
	static bool validateHeader(std::array<uint8_t, 80>& header) noexcept;

	/**
	 * @brief Returns a ROM's name from the header
	 */
	static std::string parseROMName(std::array<uint8_t, 80>& header) noexcept;
};



// Used to indicate MBC controls and memory persistence
enum BankController
{
	// Each token set to actual ID value

	NONE = 0x00, // No additional banks.
	NONE_RAM = 0x08, // RAM
	NONE_BAT_RAM = 0x09, // Battery RAM

	MBC1 = 0x01, // Up to 2MiB ROM, 32KiB RAM
	MBC1_RAM = 0x02, // MBC1 with RAM
	MBC1_BAT_RAM = 0x03, // MBC1 with Battery RAM

	MBC2 = 0x05, // Up to 256KiB ROM, 256 bytes of mapped memory(?)
	MBC2_BAT = 0x06, //

	MBC3 = 0x11, // Up to 2MiB ROM, 32KiB RAM, Additional Timer
	MBC3_RAM = 0x12, // MBC3 with RAM
	MBC3_BAT_RAM = 0x13, // MBC3 with Battery RAM
	MBC3_BAT_TIMER = 0x0F, // MBC3 with Battery + Timer
	MBC3_BAT_RAM_TIMER = 0x10, // MBC3 with Battery + RAM + Timer

	// MBC4 does not exist.

	MBC5 = 0x19, // Up to 8MiB ROM
	MBC5_RAM = 0x1A, // MBC5 with RAM
	MBC5_BAT_RAM = 0x1B, // MBC5 with Battery RAM
	MBC5_RUMBLE = 0x1C, // MBC5 with Rumble (rumble not emulated)
	MBC5_RUMBLE_RAM = 0x1D, // MBC5 with Rumble and RAM
	MBC5_RUMBLE_BAT_RAM = 0x1E, // MBC5 with Rumble, Battery, and RAM

	// MBC7 is used for an accelerometer, which I don't plan on emulating.
	// Will not be emulating MMM or M161 cartridges

	HuC1 = 0xFF, // Seems easy enough to emulate, if I ever find the docs.
	HuC3 = 0xFE, // Alongside banking, contains an RTC, speaker,
	             // and IR Blaster/Receiver. Fancy.
	// Not emulating Wisdom Tree MBC
	// Not emulating BANDAI TAMA5
	// Probably not emulating GB Camera
};