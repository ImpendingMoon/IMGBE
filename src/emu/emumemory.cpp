/**
 * @file emu/emumemory.cpp
 * @brief Implements the system's memory
 * @author ImpendingMoon
 * @date 2023-10-05
 */

#include "emumemory.hpp"
#include <stdexcept>
#include <fmt/core.h>
#include "../logger.hpp"

EmuMemory::EmuMemory(RegisterSet* cpu_registers) :
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
{
    // Banked Work RAM is constant
    for(int i = 0; i < 8; i++)
    {
        WRAM1.push_back(MemoryBank(WRAM1_START, WRAM1_END));
    }

    CPURegisters = cpu_registers;
}

EmuMemory::~EmuMemory()
{}



/**
 * @brief Reads a byte from memory
 * @param address
 * @param ignore_illegal default=false, Ignore illegal reads (for debug)
 * @return Byte at address, 0x00 if address is locked.
 */
uint8_t EmuMemory::readByte(uint16_t address, bool ignore_illegal) const
{
    // Check if address is a memory register
    if(CPURegisters != nullptr)
    {
        uint8_t* register_ptr = CPURegisters->getRegisterPtr(address);
        if(register_ptr != nullptr)
        {
            return *register_ptr;
        }
    }



    // This could probably be done better, but this works.
    if(address >= ROM0_START && address <= ROM0_END)
    {
        return ROM0.readByte(address);
    }

    else if(address >= ROM1_START && address <= ROM1_END)
    {
        if(ROM1Index >= ROM1BankCount || ROM1BankCount == 0)
        {
            if(ignore_illegal) { return 0; }

            logMessage(fmt::format(
                "Illegal ROM1 Bank Read! Current Bank: {} - Max Bank: {}",
                ROM1Index, ROM1BankCount - 1
            ), LOG_DEBUG);

            return 0xFF;
        }

        return ROM1.at(ROM1Index).readByte(address);
    }

    else if(address >= VRAM_START && address <= VRAM_END)
    {
        return VRAM.readByte(address);
    }

    else if(address >= ERAM_START && address <= ERAM_END)
    {
        if(ERAMIndex >= ERAMBankCount || ERAMBankCount == 0)
        {
            if(ignore_illegal) { return 0; }

            logMessage(fmt::format(
                "Illegal ERAM Bank Read! Current Bank: {} - Max Bank: {}",
                ERAMIndex, ERAMBankCount - 1
            ), LOG_DEBUG);

            return 0xFF;
        }

        return ERAM.at(ERAMIndex).readByte(address);
    }

    else if(address >= WRAM0_START && address <= WRAM0_END)
    {
        return WRAM0.readByte(address);
    }

    else if(address >= WRAM1_START && address <= WRAM1_END)
    {
        if(WRAM1Index >= WRAM1BankCount || WRAM1BankCount == 0)
        {
            if(ignore_illegal) { return 0; }

            logMessage(fmt::format(
                "Illegal WRAM1 Bank Read! Current Bank: {} - Max Bank: {}",
                WRAM1Index, WRAM1BankCount - 1
            ), LOG_DEBUG);

            return 0xFF;
        }

        return WRAM1.at(WRAM1Index).readByte(address);
    }

    else if(address >= ECHO_START && address <= ECHO_END)
    {
        return readByte(address - 0x2000, ignore_illegal);
    }

    else if(address >= OAM_START && address <= OAM_END)
    {
        return OAM.readByte(address);
    }

    else if(address >= IOREG_START && address <= IOREG_END)
    {
        return IOREG.readByte(address);
    }

    else if(address >= HRAM_START && address <= HRAM_END)
    {
        return HRAM.readByte(address);
    }

    else if(address == IEREG_START)
    {
        return IEREG.readByte(address);
    }

    if(ignore_illegal) { return 0; }

    logMessage(fmt::format(
        "Illegal Memory Read! Address: ${:04X}", address
    ), LOG_DEBUG);

    return 0xFF;

    // Need to handle bugs in games. Real hardware reads 0xFF or open bus.
    //throw std::out_of_range(fmt::format(
    //    "Illegal Memory Read! Address: ${:04X}", address
    //));
}



/**
 * @brief Writes a byte to memory
 * @param address
 * @param value
 */
void EmuMemory::writeByte(uint16_t address, uint8_t value)
{
    // Check if address is a memory register
    if(CPURegisters != nullptr)
    {
        uint8_t* register_ptr = CPURegisters->getRegisterPtr(address);
        if(register_ptr != nullptr)
        {
            *register_ptr = value;
            return;
        }
    }

    if(address >= ROM0_START && address <= ROM1_END)
    {
        // TODO: Bank switching
        logMessage(fmt::format(
            "Attempted write to ROM! Address: ${:04X} - Value 0x{:02X}",
            address, value
        ), LOG_DEBUG);

        return;
    }

    if(address >= VRAM_START && address <= VRAM_END)
    {
        VRAM.writeByte(address, value);
        return;
    }

    else if(address >= ERAM_START && address <= ERAM_END)
    {
        if(ERAMIndex >= ERAMBankCount || ERAMBankCount == 0)
        {
            logMessage(fmt::format(
                "Illegal ERAM Bank Write! Current Bank: {} - Max Bank: {}",
                ERAMIndex, ERAMBankCount - 1
            ), LOG_DEBUG);

            return;
        }

        ERAM.at(ERAMIndex).writeByte(address, value);
        ERAMDirty = true;
        return;
    }

    else if(address >= WRAM0_START && address <= WRAM0_END)
    {
        WRAM0.writeByte(address, value);
        return;
    }

    else if(address >= WRAM1_START && address <= WRAM1_END)
    {
        if(WRAM1Index >= WRAM1BankCount || WRAM1BankCount == 0)
        {
            logMessage(fmt::format(
                "Illegal ERAM Bank Write! Current Bank: {} - Max Bank: {}",
                ERAMIndex, ERAMBankCount - 1
            ), LOG_DEBUG);

            return;
        }

        WRAM1.at(WRAM1Index).writeByte(address, value);
        return;
    }

    else if(address >= ECHO_START && address <= ECHO_END)
    {
        writeByte(address - 0x2000, value);
        return;
    }

    else if(address >= OAM_START && address <= OAM_END)
    {
        OAM.writeByte(address, value);
        return;
    }

    else if(address >= IOREG_START && address <= IOREG_END)
    {
        IOREG.writeByte(address, value);
        return;
    }

    else if(address >= HRAM_START && address <= HRAM_END)
    {
        HRAM.writeByte(address, value);
        return;
    }

    else if(address == IEREG_START)
    {
        IEREG.writeByte(address, value);
        return;
    }

    logMessage(fmt::format(
        "Illegal Memory Write! Address: ${:04X} - Value: 0x{:02X}", address, value
    ), LOG_DEBUG);
}



/**
 * @brief Sets the CPU registers pointer
 * @param cpu_registers
 */
void EmuMemory::setCPURegisters(RegisterSet* cpu_registers)
{
    CPURegisters = cpu_registers;
}



/**
 * @brief Initializes ROM0 with a set of data.
 * @param data MemoryBank with range ROM0_START to ROM0_END.
 * @throws std::invalid_argument if memory bank has an incorrect range.
 */
void EmuMemory::initROM0(MemoryBank& data)
{
    if(data.getStartAddress() != ROM0_START
       || data.getEndAddress() != ROM0_END
    )
    {
        throw std::invalid_argument(fmt::format(
            "ROM0 Address Mismatch! Required address: ${}-${} - "
            "Actual address: ${}-${}",
            ROM0_START, ROM0_END,
            data.getStartAddress(), data.getEndAddress()
        ));
    }

    ROM0 = std::move(data);
}



/**
 * @brief Initializes ROM1 with a set of data.
 * @param bank_count Number of memory banks avaliable.
 * @param initial_bank Initially referenced bank.
 * @param data Vector of MemoryBanks with range ROM1_START to ROM1_END.
 * @throws std::invalid_argument if data and bank count are mismatched, or
 * if any memory bank has an incorrect range.
 */
void EmuMemory::initROM1(
    size_t bank_count,
    size_t initial_bank,
    std::vector<MemoryBank>& data
)
{
    if(bank_count != data.size())
    {
        throw std::invalid_argument(fmt::format(
            "ROM1 Size Mismatch! Reported bank count: {} - "
            "Actual bank count: {}",
            bank_count,
            data.size()
        ));
    }

    for(MemoryBank bank : data)
    {
        if(bank.getStartAddress() != ROM1_START
           || bank.getEndAddress() != ROM1_END
        )
        {
            throw std::invalid_argument(fmt::format(
                "ROM1 Address Mismatch! Required address: ${}-${} - "
                "Actual address: ${}-${}",
                ROM1_START, ROM1_END,
                bank.getStartAddress(), bank.getEndAddress()
            ));
        }
    }

    ROM1BankCount = bank_count;
    ROM1Index = initial_bank;
    ROM1 = std::move(data);
}



/**
 * @brief Initializes ERAM with a set of data.
 * @param bank_count Number of memory banks avaliable.
 * @param initial_bank Initially referenced bank.
 * @param battery_backed Whether to save to a file (saved to ROM_NAME.sav).
 * @param data Vector of MemoryBanks with range ERAM_START to ERAM_END.
 * @throws std::invalid_argument if data and bank count are mismatched, or
 * if any memory bank has an incorrect range.
 */
void EmuMemory::initERAM(
    size_t bank_count,
    size_t initial_bank,
    bool battery_backed,
    std::vector<MemoryBank>& data
)
{
    if(bank_count != data.size())
    {
        throw std::invalid_argument(fmt::format(
            "ERAM Size Mismatch! Reported bank count: {} - "
            "Actual bank count: {}",
            bank_count,
            data.size()
        ));
    }

    for(MemoryBank bank : data)
    {
        if(bank.getStartAddress() != ERAM_START
           || bank.getEndAddress() != ERAM_END
        )
        {
            throw std::invalid_argument(fmt::format(
                "ERAM Address Mismatch! Required address: ${}-${} - "
                "Actual address: ${}-${}",
                ERAM_START, ERAM_END,
                bank.getStartAddress(), bank.getEndAddress()
            ));
        }
    }

    ERAMBankCount = bank_count;
    ERAMIndex = initial_bank;
    ERAMBatteryBacked = battery_backed;
    ERAM = std::move(data);
}



/**
 * @brief Sets the currently-addressed ROM1 bank
 * @param value
 * @throws std::out_of_range if >= bank count.
 */
void EmuMemory::setROM1Index(size_t value)
{
    if(value >= ROM1BankCount)
    {
        throw new std::out_of_range(fmt::format(
            "Illegal ROM1 Bank Switch! New Bank: {} - Max Bank: {}",
            value, ROM1BankCount - 1
        ));
    }

    ROM1Index = value;
}



/**
 * @brief Sets the currently-addressed WRAM1 bank
 * @param value
 * @throws std::out_of_range if >= bank count.
 */
void EmuMemory::setWRAM1Index(size_t value)
{
    if(value >= WRAM1BankCount)
    {
        throw new std::out_of_range(fmt::format(
            "Illegal WRAM1 Bank Switch! New Bank: {} - Max Bank: {}",
            value, WRAM1BankCount - 1
        ));
    }

    WRAM1Index = value;
}



/**
 * @brief Sets the currently-addressed ERAM bank
 * @param value
 * @throws std::out_of_range if >= bank count.
 */
void EmuMemory::setERAMIndex(size_t value)
{
    if(value >= ERAMBankCount)
    {
        throw new std::out_of_range(fmt::format(
            "Illegal ERAM Bank Switch! New Bank: {} - Max Bank: {}",
            value, ERAMBankCount - 1
        ));
    }

    ERAMIndex = value;
}



/**
 * @brief If ERAM has changed and is battery backed, writes ERAM vectors to
 * the save file.
 */
void EmuMemory::writeERAM(void)
{
    if(!ERAMBatteryBacked || !ERAMDirty) { return; }
}



/**
 * @brief Returns a .sav file path from an existing file path
 * @param rom_file_path
 */
std::filesystem::path EmuMemory::getSAVPath(
    std::filesystem::path rom_file_path
) noexcept
{
    std::string sav_file_path = rom_file_path.string();
    size_t extension_pos = sav_file_path.find_last_of('.');

    if(extension_pos == std::string::npos)
    {
        sav_file_path += ".sav";
    } else
    {
        sav_file_path.replace(extension_pos, sav_file_path.length(), ".sav");
    }

    return std::filesystem::path(sav_file_path);
}



/**
 * @brief Dumps memory contents to the log as LOG_DEBUG.
 */
void EmuMemory::dumpMemory(void) const noexcept
{
    using fmt::format;

    logMessage("---BEGIN MEMORY DUMP---", LOG_DEBUG);

    logMessage(format(
        "ROM1 BC: {} - ROM1 Index: {}", ROM1BankCount, ROM1Index
    ), LOG_DEBUG);

    logMessage(format(
        "ERAM BC: {} - ERAM Index: {} - ERAM Persistent: {} - ERAM Dirty: {}",
        ERAMBankCount, ERAMIndex, ERAMBatteryBacked, ERAMDirty
    ), LOG_DEBUG);

    logMessage(format(
        "WRAM1 BC: {} - WRAM1 Index: {}", WRAM1BankCount, WRAM1Index
    ), LOG_DEBUG);

    // Print contents of memory
    constexpr uint16_t SPLIT_AT = 32;
    for(uint16_t i = 0; i < (UINT16_MAX / SPLIT_AT); i++)
    {
        // Print memory address
        std::string outstr = format("${:04X} ", i * SPLIT_AT);

        // Print each byte
        for(uint16_t j = 0; j < SPLIT_AT; j++)
        {
            outstr += format("{:02X} ", readByte((i * SPLIT_AT) + j, true));
        }

        logMessage(outstr, LOG_DEBUG);
    }

    logMessage("---END MEMORY DUMP---", LOG_DEBUG);
}

