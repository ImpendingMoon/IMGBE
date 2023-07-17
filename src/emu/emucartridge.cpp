/**
 * @file emu/emucartridge.hpp
 * @brief Handles loading ROM files into memory
 * @author ImpendingMoon
 * @date 2023-07-16
 */

#include "emucartridge.hpp"
#include <assert.h>
#include <fmt/core.h>
#include "../logger.hpp"

EmuCartridge::EmuCartridge(EmuMemory* memory)
{
	mem = memory;
}



EmuCartridge::~EmuCartridge()
{
}

void EmuCartridge::setMemoryPointer(EmuMemory* memory)
{
	mem = memory;
}



/**
 * @brief Returns the loaded ROM name.
 */
std::string EmuCartridge::getROMName(void)
{
	return ROMName;
}



/**
 * @brief Loads a ROM file into memory.
 * @param file_path
 * @throws std::runtime_error if memory pointer is null.
 * @throws std::ios_base::failure on file error.
 * @throws bad_rom on invalid ROM header.
 */
void EmuCartridge::loadROM(std::filesystem::path file_path)
{
	logMessage(fmt::format(
		"Attempting to load rom {}...", file_path.string()
	));


	if(mem == nullptr)
	{
		throw std::runtime_error("Cannot load ROM into null memory pointer!");
	}

	if(!std::filesystem::exists(file_path))
	{
		throw std::ios_base::failure(fmt::format(
			"Cannot access file {}!", file_path.string()
		));
	}

	ROMFile.open(file_path, std::ios_base::in | std::ios_base::binary);

	if(!ROMFile.is_open())
	{
		throw std::ios_base::failure(fmt::format(
			"Cannot open file {}!", file_path.string()
		));
	}

	ROMFilePath = file_path;

	// Pull header info

	std::array<uint8_t, 80> header;
	ROMFile.seekg(0x100);
	ROMFile.read(reinterpret_cast<char*>(header.data()), 80);
	ROMFile.seekg(0);

	if(!validateHeader(header))
	{
		ROMFile.close();
		throw std::runtime_error("ROM header is invalid!");
	}

	ROMName = parseROMName(header);

	logMessage(fmt::format(
		"ROM header is valid. Attempting to load ROM {}...", ROMName
	));

	// Bank controller info is held at $0147
	BankController mbc = static_cast<BankController>(header.at(0x47));
	size_t rom_bank_count;
	size_t ram_bank_count;
	bool persistent_ram;
	std::filesystem::path sav_file_path;

	switch(mbc)
	{
	case NONE_BAT_RAM: case MBC1_BAT_RAM: case MBC2_BAT: case MBC3_BAT_RAM:
	case MBC3_BAT_RAM_TIMER: case MBC5_BAT_RAM: case MBC5_RUMBLE_BAT_RAM:
	{
		persistent_ram = true;
		sav_file_path = EmuMemory::getSAVPath(ROMFilePath);
		break;
	}
	default:
	{
		persistent_ram = false;
	}
	}

	// Get the ROM and RAM bank amounts
	switch(header[0x48])
	{
	case 0x00: rom_bank_count = 1; break;
	case 0x01: rom_bank_count = 3; break;
	case 0x02: rom_bank_count = 7; break;
	case 0x03: rom_bank_count = 15; break;
	case 0x04: rom_bank_count = 31; break;
	case 0x05: rom_bank_count = 63; break;
	case 0x06: rom_bank_count = 127; break;
	case 0x07: rom_bank_count = 255; break;
	case 0x08: rom_bank_count = 511; break;
	default:
	{
		throw std::runtime_error("Invalid ROM bank count in header!");
	}
	}

	switch(header[0x49])
	{
	case 0x00: ram_bank_count = 0; break;
		// 0x01 is unused
	case 0x02: ram_bank_count = 1; break;
	case 0x03: ram_bank_count = 4; break;
	case 0x04: ram_bank_count = 16; break;
	case 0x05: ram_bank_count = 8; break;
	default:
	{
		throw std::runtime_error("Invalid RAM bank count in header!");
	}
	}

	// Load ROM file to MemoryBanks, send to memory struct

	// ROM0 is a single bank
	std::vector<uint8_t> ROM0_data(ROM0_SIZE);
	assert(ROMFile.is_open());
	ROMFile.read(reinterpret_cast<char*>(ROM0_data.data()), ROM0_SIZE);

	MemoryBank ROM0(ROM0_START, ROM0_END);
	ROM0.loadData(ROM0_data);
	mem->initROM0(ROM0);


	// ROM1 has multiple banks (potentially)
	std::vector<MemoryBank> ROM1_data;
	for(size_t i = 0; i < rom_bank_count; i++)
	{
		std::vector<uint8_t> data(ROM1_SIZE);
		assert(ROMFile.is_open());
		ROMFile.read(reinterpret_cast<char*>(data.data()), ROM1_SIZE);

		MemoryBank bank(ROM1_START, ROM1_END);
		bank.loadData(data);
		ROM1_data.push_back(bank);
	}
	mem->initROM1(rom_bank_count, 0, ROM1_data);


	// ERAM is either zeroed or loaded from a file.
	std::fstream SAVFile;
	if(persistent_ram)
	{
		// Attempt to create save file
		SAVFile.open(sav_file_path, std::ios_base::out);
		if(!SAVFile.is_open())
		{
			persistent_ram = false;
		} else
		{
			SAVFile.close();
			SAVFile.open(
				sav_file_path,
				std::ios_base::in | std::ios_base::binary
			);
		}
	}

	std::vector<MemoryBank> ERAM_data;
	for(size_t i = 0; i < ram_bank_count; i++)
	{
		std::vector<uint8_t> data(ERAM_SIZE);
		if(persistent_ram)
		{
			assert(SAVFile.is_open());
			SAVFile.read(reinterpret_cast<char*>(data.data()), ERAM_SIZE);
		} else
		{
			std::fill(data.begin(), data.end(), 0);
		}

		MemoryBank bank(ERAM_START, ERAM_END);
		bank.loadData(data);
		ERAM_data.push_back(bank);
	}
	mem->initERAM(ram_bank_count, 0, persistent_ram, ERAM_data);

	if(SAVFile.is_open()) { SAVFile.close(); }
	ROMFile.close();

	logMessage("Successfully loaded ROM.");
}



/**
 * @brief Checks if a given ROM header is valid.
 * @param header
 */
bool EmuCartridge::validateHeader(std::array<uint8_t, 80>& header) noexcept
{
	// The checked header resides in 25 bytes from $134-$14C
	// The checksum value is at $14D
	std::array<uint8_t, 25> checked_header{};
	std::copy(
			header.begin() + 0x34,
			header.begin() + 0x4C + 1,
			checked_header.begin()
	);
	uint8_t checksum = header[0x4D];

	// The checksum is calculated by adding the inverse of all values inside
	// the checked header.
	uint8_t sum = 0;
	for(uint8_t value : checked_header) { sum += ~value; }

	return sum == checksum;
}



/**
 * @brief Returns a ROM's name from the header
 */
std::string EmuCartridge::parseROMName(std::array<uint8_t, 80>& header)
noexcept
{
	std::string title;
	// The game title is stored in ASCII at location $133-$143.
	std::copy(header.begin() + 0x34,
			  header.begin() + 0x43 + 1,
			  std::back_inserter(title)
	);

	return title;
}
