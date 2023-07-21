/**
 * @file emu/emucpu.cpp
 * @brief Implements the system's registers
 * @author ImpendingMoon
 * @date 2023-07-20
 */

#include "emuregisters.hpp"

RegisterSet::RegisterSet()
{}

RegisterSet::~RegisterSet()
{}



/**
 * @brief Converts an address to its corresponding register, or nullptr.
 * @param address Emulated memory address
 * @returns a pointer to a register, or a nullptr
 */
uint8_t* RegisterSet::getRegisterPtr(uint16_t address) const noexcept
{
	auto it = MEMORY_MAP.find(address);
	if(it != MEMORY_MAP.end())
	{
		return it->second;
	}

	return nullptr;
}



/**
 * @brief Updates the 'f' register with the values from the flag struct.
 */
void RegisterSet::flagStructToRegister(void)
{
	uint8_t value = 0;
	value |= (flags.zero << ZERO_POS);
	value |= (flags.sub << SUB_POS);
	value |= (flags.half_carry << HALF_CARRY_POS);
	value |= (flags.carry << CARRY_POS);

	cpu.f = value;
}



/**
 * @brief Updates the flag struct with values from the 'f' register.
 */
void RegisterSet::flagRegisterToStruct(void)
{
	flags.zero = (cpu.f >> ZERO_POS) & 1;
	flags.sub = (cpu.f >> SUB_POS) & 1;
	flags.half_carry = (cpu.f >> HALF_CARRY_POS) & 1;
	flags.carry = (cpu.f >> CARRY_POS) & 1;
}
