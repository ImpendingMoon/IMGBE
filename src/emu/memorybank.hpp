/**
 * @file emu/memorybank.hpp
 * @brief Implements a memory bank
 * @author ImpendingMoon
 * @date 2023-07-16
 */

#pragma once

#include <vector>
#include <cstdint>

class MemoryBank
{
public:

	/**
	 * @brief Constructor
	 * @param startAddress
	 * @param endAddress
	 * @param read_locked default=false
	 * @param write_locked default=false
	 * @throws std::invalid_argument if startAddress is greater than endAddress.
	 */
	MemoryBank(
		size_t start_address,
		size_t end_address,
		bool read_locked = false,
		bool write_locked = false
	);

	~MemoryBank();

	/**
	 * @brief Reads a byte from memory.
	 * @param address Address must be within bank's range.
	 * @return Byte at address.
	 * @throws std::out_of_range if address is out of range.
	 * @throws std::runtime_error if internal error occurred.
	 * @returns Byte at address, 0x00 if locked.
	 */
	uint8_t readByte(size_t address) const;

	/**
	 * @brief Writes a value to memory. Fails when write locked.
	 * @param address Address must be within bank's range.
	 * @param value Value to write.
	 * @throws std::out_of_range if address is out of range.
	 * @throws std::runtime_error if internal error occurred.
	 */
	void writeByte(size_t address, uint8_t value);

	bool isReadLocked(void) const noexcept;
	bool isWriteLocked(void) const noexcept;

	void setReadLocked(bool value) noexcept;
	void setWriteLocked(bool value) noexcept;

	size_t getStartAddress(void) const noexcept;
	size_t getEndAddress(void) const noexcept;

	/**
	 * @brief Copies an existing vector of data. Must be <= bank size.
	 * @param data 
	 * @throws std::invalid_argument if new_data is > bank size.
	 */
	void loadData(std::vector<uint8_t>& new_data);

private:
	std::vector<uint8_t> data;
	bool readLocked;
	bool writeLocked;
	size_t startAddress;
	size_t endAddress;
};
