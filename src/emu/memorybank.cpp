/**
 * @file emu/memorybank.cpp
 * @brief Implements a memory bank
 * @author ImpendingMoon
 * @date 2023-07-15
 */

#include "memorybank.hpp"

#include <stdexcept>
#include <assert.h>



/**
 * @brief Constructor
 * @param startAddress
 * @param endAddress
 * @param read_locked default=false
 * @param write_locked default=false
 * @throws std::invalid_argument if startAddress is greater than endAddress.
 */
MemoryBank::MemoryBank(
		size_t start_address,
		size_t end_address,
		bool read_locked,
		bool write_locked
)
{
	if(start_address > end_address)
	{
		throw std::invalid_argument(
			"Start address cannot be greater than end address!"
		);
	}

	data.reserve(end_address - start_address + 1);
	data.resize(end_address - start_address + 1);
	std::fill(data.begin(), data.end(), 0);

	startAddress = start_address;
	endAddress = end_address;

	readLocked = read_locked;
	writeLocked = write_locked;
}



MemoryBank::~MemoryBank()
{}



/**
 * @brief Reads a byte from memory. Returns 0x00 when read locked.
 * @param address Address must be within bank's range.
 * @return Byte at address.
 * @throws std::out_of_range if address is out of range.
 * @throws std::runtime_error if internal error occurred.
 */
uint8_t MemoryBank::readByte(size_t address) const
{
	if(address < startAddress || address > endAddress)
	{
		throw std::out_of_range(
			"Cannot read memory bank out of address range!"
		);
	}

	assert(data.size() == (endAddress - startAddress + 1));

	return data.at(address - startAddress);
}



/**
 * @brief Writes a value to memory. Fails when write locked.
 * @param address Address must be within bank's range.
 * @param value Value to write.
 * @throws std::out_of_range if address is out of range.
 * @throws std::runtime_error if internal error occurred.
 */
void MemoryBank::writeByte(size_t address, uint8_t value)
{
	if(address < startAddress || address > endAddress)
	{
		throw std::out_of_range(
			"Cannot write memory bank out of address range!"
		);
	}

	assert(data.size() == (endAddress - startAddress + 1));

	data.at(address - startAddress) = value;
}



bool MemoryBank::isReadLocked(void) const noexcept
{
	return readLocked;
}



bool MemoryBank::isWriteLocked(void) const noexcept
{
	return writeLocked;
}



void MemoryBank::setReadLocked(bool value) noexcept
{
	readLocked = value;
}



void MemoryBank::setWriteLocked(bool value) noexcept
{
	writeLocked = value;
}



size_t MemoryBank::getStartAddress(void) const noexcept
{
	return startAddress;
}



size_t MemoryBank::getEndAddress(void) const noexcept
{
	return endAddress;
}
