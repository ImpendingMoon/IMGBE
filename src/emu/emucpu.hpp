/**
 * @file emu/emucpu.cpp
 * @brief Implements the system's CPU
 * @author ImpendingMoon
 * @date 2023-07-21
 */

#pragma once

#include <cstdint>
#include "emuregisters.hpp"
#include "emumemory.hpp"

class EmuCPU
{
public:
	EmuCPU(EmuMemory* memory = nullptr);
	~EmuCPU();

	void setMemPtr(EmuMemory* memory);

	/**
	 * @brief Steps the CPU by one instruction
	 * @return The number of machine cycles taken
	 * @throws std::runtime_error on illegal or unimplemented instruction.
	 */
	int step(void);

	/**
	 * @brief Initializes registers to after-BIOS defaults
	 * @param void Soon to be model, currently only DMG
	 */
	void initRegs(void);

private:
	RegisterSet regs;
	EmuMemory* mem;

	// Instructions return additional cycles used for memory access.
	int MOVE8(uint8_t* target, uint8_t* source);
	int LOAD8(uint8_t* target, uint16_t* source_address);
	int STORE8(uint16_t* target_address, uint8_t* source);

	int LOAD16(uint16_t* target, uint16_t* source_address);
	int STORE16(uint16_t* target_address, uint16_t* source);

	int PUSH(uint16_t* source);
	int POP(uint16_t* target);

	int INC8(uint8_t* target);
	int INCSTORE8(uint16_t* target_address);
	int INC16(uint16_t* target);
	
	int DEC8(uint8_t* target);
	int DECSTORE8(uint16_t* target_address);
	int DEC16(uint16_t* target);

	int ADD8(uint8_t* target, uint8_t* source);
	int ADDLOAD8(uint8_t* target, uint16_t* source_address);
	int ADD16(uint16_t* target, uint16_t* source);

	int ADC8(uint8_t* target, uint8_t* source);
	int ADCLOAD8(uint8_t* target, uint16_t* source_address);
	
	int SUB8(uint8_t* target, uint8_t* source);
	int SUBLOAD8(uint8_t* target, uint16_t* source_address);

	int SBC8(uint8_t* target, uint8_t* source);
	int SBCLOAD8(uint8_t* target, uint16_t* source_address);

	int AND8(uint8_t* target, uint8_t* source);
	int ANDLOAD8(uint8_t* target, uint16_t* source_address);

	int XOR8(uint8_t* target, uint8_t* source);
	int XORLOAD8(uint8_t* target, uint16_t* source_address);

	int OR8(uint8_t* target, uint8_t* source);
	int ORLOAD8(uint8_t* target, uint16_t* source_address);

	int CP8(uint8_t* target, uint8_t* source);
	int CPLOAD8(uint8_t* target, uint16_t* source_address);

	int RL(uint8_t* target);
	int RR(uint8_t* target);
	int RLC(uint8_t* target);
	int RRC(uint8_t* target);

	int DAA(void);

	int JUMPR(uint8_t* address, bool* condition);

	static inline bool willOverflow8(uint8_t a, uint8_t b)
	{
		return a > (UINT8_MAX - b);
	}

	static inline bool willUnderflow8(uint8_t a, uint8_t b)
	{
		return a < b;
	}

	static inline bool willHalfOverflow8(uint8_t a, uint8_t b)
	{
		// Add lower nibbles, check if overflowed to upper nibble.
		return (((a & 0x0F) + (b & 0x0F)) & 0xF0) != 0;
	}

	static inline bool willHalfUnderflow8(uint8_t a, uint8_t b)
	{
		return (b & 0x0F) > (a & 0x0F);
	}

	static inline bool willOverflow16(uint16_t a, uint16_t b)
	{
		return a > (UINT16_MAX - b);
	}

	static inline bool willUnderflow16(uint16_t a, uint16_t b)
	{
		return a < b;
	}
};