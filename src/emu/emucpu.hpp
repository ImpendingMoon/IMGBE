/**
 * @file emu/emucpu.cpp
 * @brief Implements the system's CPU
 * @author ImpendingMoon
 * @date 2023-07-23
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
	 * @param log_instruction Whether to log internal operations (slow!)
	 * @throws std::runtime_error on illegal or unimplemented instruction.
	 */
	int step(bool log_instruction);

	/**
	 * @brief Initializes registers to after-BIOS defaults
	 * @param void Soon to be model, currently only DMG
	 */
	void initRegs(void);

	/**
	 * @brief Returns a pointer to the RegisterSet
	 */
	RegisterSet* getRegsPtr(void);

private:
	RegisterSet regs;
	EmuMemory* mem;

	// EI and DI are delayed for one instruction.
	bool nextInterruptState = true;

	// Instructions return additional cycles used for memory access.
	int MOVE8(uint8_t* target, uint8_t* source);
	int LOAD8(uint8_t* target, uint16_t* source_address);
	int STORE8(uint16_t* target_address, uint8_t* source);

	int MOVE16(uint16_t* target, uint16_t* source);
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
	int ADDSIGNED16(uint16_t* target, uint8_t* source);

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
	int RLSTORE8(uint16_t* target_address);
	int RR(uint8_t* target);
	int RRSTORE8(uint16_t* target_address);
	int RLC(uint8_t* target);
	int RLCSTORE8(uint16_t* target_address);
	int RRC(uint8_t* target);
	int RRCSTORE8(uint16_t* target_address);
	int SLA(uint8_t* target);
	int SLASTORE8(uint16_t* target_address);
	int SRA(uint8_t* target);
	int SRASTORE8(uint16_t* target_address);
	int SRL(uint8_t* target);
	int SRLSTORE8(uint16_t* target_address);
	int SWAP(uint8_t* target);
	int SWAPSTORE8(uint16_t* target_address);

	int BIT(uint8_t* target, uint8_t bit);
	int BITLOAD8(uint16_t* target_address, uint8_t bit);
	int RES(uint8_t* target, uint8_t bit);
	int RESSTORE8(uint16_t* target_address, uint8_t bit);
	int SET(uint8_t* target, uint8_t bit);
	int SETSTORE8(uint16_t* target_address, uint8_t bit);

	int DAA(void);

	int JUMP(uint16_t* address, bool* condition);
	int JUMPR(uint8_t* address, bool* condition);
	int CALL(uint16_t* address, bool* condition);
	int RET(bool* condition);
	int RETI(void);
	int RST(uint16_t vector);

	int EI(void);
	int DI(void);

	void ILLEGAL_INSTRUCTION(uint8_t opcode, uint16_t source);

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