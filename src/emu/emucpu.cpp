/**
 * @file emu/emucpu.cpp
 * @brief Implements the system's CPU
 * @author ImpendingMoon
 * @date 2023-07-21
 */

#include "emucpu.hpp"
#include <assert.h>
#include <fmt/core.h>
#include "../logger.hpp"

EmuCPU::EmuCPU(EmuMemory* memory)
{
	mem = memory;
}

EmuCPU::~EmuCPU()
{
}

void EmuCPU::setMemPtr(EmuMemory* memory)
{
	mem = memory;
}



/**
 * @brief Steps the CPU by one instruction
 * @return The number of machine cycles taken
 * @throws std::runtime_error on illegal or unimplemented instruction.
 */
int EmuCPU::step(void)
{
	assert(mem != nullptr);

	int cycles = 4;
	uint16_t source = regs.cpu.pc;
	bool second_bank = false;
	std::string instruction = "UNIMPLEMENTED";

	uint8_t opcode = mem->readByte(regs.cpu.pc);
	regs.cpu.pc++;

	if(opcode == 0xCB)
	{
		second_bank = true;
		uint8_t opcode = mem->readByte(regs.cpu.pc);
		regs.cpu.pc++;
		cycles += 4;
	}

	if(!second_bank)
	{
		switch(opcode)
		{
		case 0x00:
		{
			instruction = "NOP";
			break;
		}
		case 0x01:
		{
			instruction = "LD BC, d16";
			cycles += LOAD16(&regs.cpu.bc, &regs.cpu.pc);
			break;
		}
		case 0x02:
		{
			instruction = "LD [BC], A";
			cycles += STORE8(&regs.cpu.bc, &regs.cpu.a);
			break;
		}
		case 0x03:
		{
			instruction = "INC BC";
			cycles += INC16(&regs.cpu.bc);
			break;
		}
		case 0x04:
		{
			instruction = "INC B";
			cycles += INC8(&regs.cpu.b);
			break;
		}
		case 0x05:
		{
			instruction = "DEC B";
			cycles += DEC8(&regs.cpu.b);
			break;
		}
		case 0x06:
		{
			instruction = "LD B, d8";
			cycles += LOAD8(&regs.cpu.b, &regs.cpu.pc);
			break;
		}
		case 0x07:
		{
			instruction = "RLCA";
			cycles += RLC(&regs.cpu.a);
			break;
		}
		case 0x08:
		{
			instruction = "LD [d16], SP";
			uint16_t target_address;
			cycles += LOAD16(&target_address, &regs.cpu.pc);
			cycles += STORE16(&target_address, &regs.cpu.sp);
			break;
		}
		case 0x09:
		{
			instruction = "ADD HL, BC";
			cycles += ADD16(&regs.cpu.hl, &regs.cpu.bc);
			break;
		}
		case 0x0A:
		{
			instruction = "LD A, [BC]";
			cycles += LOAD8(&regs.cpu.a, &regs.cpu.bc);
			break;
		}
		case 0x0B:
		{
			instruction = "DEC BC";
			cycles += DEC16(&regs.cpu.bc);
			break;
		}
		case 0x0C:
		{
			instruction = "INC C";
			cycles += INC8(&regs.cpu.c);
			break;
		}
		case 0x0D:
		{
			instruction = "DEC C";
			cycles += DEC8(&regs.cpu.c);
			break;
		}
		case 0x0E:
		{
			instruction = "LD C, d8";
			cycles += LOAD8(&regs.cpu.c, &regs.cpu.pc);
			break;
		}
		case 0x0F:
		{
			instruction = "RRCA";
			cycles += RRC(&regs.cpu.a);
			break;
		}
		case 0x10:
		{
			instruction = "STOP";
			// TODO: Make a way for the CPU to stop the system.
			break;
		}
		case 0x11:
		{
			instruction = "LD DE, d16";
			cycles += LOAD16(&regs.cpu.de, &regs.cpu.pc);
			break;
		}
		case 0x12:
		{
			instruction = "LD [DE], A";
			cycles += STORE8(&regs.cpu.de, &regs.cpu.a);
			break;
		}
		case 0x13:
		{
			instruction = "INC DE";
			cycles += INC16(&regs.cpu.de);
			break;
		}
		case 0x14:
		{
			instruction = "INC D";
			cycles += INC8(&regs.cpu.d);
			break;
		}
		case 0x15:
		{
			instruction = "DEC D";
			cycles += DEC8(&regs.cpu.d);
			break;
		}
		case 0x16:
		{
			instruction = "LD D, d8";
			cycles += LOAD8(&regs.cpu.d, &regs.cpu.pc);
			break;
		}
		case 0x17:
		{
			instruction = "RLA";
			cycles += RL(&regs.cpu.a);
			break;
		}
		case 0x18:
		{
			instruction = "JR s8";
			uint8_t relative_address;
			cycles += LOAD8(&relative_address, &regs.cpu.pc);
			cycles += JUMPR(&relative_address);
			break;
		}
		case 0x19:
		{
			instruction = "ADD HL, DE";
			cycles += ADD16(&regs.cpu.hl, &regs.cpu.de);
			break;
		}
		case 0x1A:
		{
			instruction = "LD A, [DE]";
			cycles += LOAD8(&regs.cpu.a, &regs.cpu.de);
			break;
		}
		case 0x1B:
		{
			instruction = "DEC DE";
			cycles += DEC16(&regs.cpu.de);
			break;
		}
		case 0x1C:
		{
			instruction = "INC E";
			cycles += INC8(&regs.cpu.e);
			break;
		}
		case 0x1D:
		{
			instruction = "DEC E";
			cycles += DEC8(&regs.cpu.e);
			break;
		}
		case 0x1E:
		{
			instruction = "LD E, d8";
			cycles += LOAD8(&regs.cpu.e, &regs.cpu.pc);
			break;
		}
		case 0x1F:
		{
			instruction = "RRA";
			cycles += RR(&regs.cpu.a);
			break;
		}
		default:
		{
			throw std::runtime_error(fmt::format(
				"Unhandled instruction! Opcode: 0x{:02X}, Source: ${:04X}",
				opcode, source
			));
		}
		}
	} else
	{
		switch(opcode)
		{
		default:
		{
			throw std::runtime_error(fmt::format(
				"Unhandled instruction! Opcode: 0x{:04X}, Source: ${:04X}",
				0xCB00 + opcode, source
			));
		}
		}
	}

	logMessage(fmt::format(
		"Executed instruction {}. Source: ${:04X} - Cycles: {}",
		instruction, source, cycles
	),
		LOG_DEBUG
	);

	return cycles;
}



/**
 * @brief Initializes registers to after-BIOS defaults
 * @param void Soon to be model, currently only DMG
 */
void EmuCPU::initRegs(void)
{
	// TODO: Make this a data structure, and have separate ones for each model.
	// TODO: Memory registers.
	regs.cpu.a = 0x01;
	regs.cpu.f = 0b10000000;
	regs.cpu.b = 0x00;
	regs.cpu.c = 0x13;
	regs.cpu.d = 0x00;
	regs.cpu.e = 0xD8;
	regs.cpu.h = 0x01;
	regs.cpu.l = 0x4D;
	regs.cpu.pc = 0x0100;
	regs.cpu.sp = 0xFFFE;
}



int EmuCPU::MOVE8(uint8_t* target, uint8_t* source)
{
	*target = *source;
	return 0;
}



int EmuCPU::LOAD8(uint8_t* target, uint16_t* source_address)
{
	*target = mem->readByte(*source_address);

	// Immediate accesses always increment program counter
	if(source_address == &regs.cpu.pc)
	{
		regs.cpu.pc++;
	}

	return 4;
}



int EmuCPU::STORE8(uint16_t* target_address, uint8_t* source)
{
	mem->writeByte(*target_address, *source);

	// There are no instructions that store to an immediate value.

	return 4;
}



int EmuCPU::LOAD16(uint16_t* target, uint16_t* source_address)
{
	*target = mem->readByte(*source_address);
	*target |= mem->readByte(*source_address + 1) << 8;

	// Immediate accesses always increment program counter
	if(source_address == &regs.cpu.pc)
	{
		regs.cpu.pc += 2;
	}

	return 8;
}



int EmuCPU::STORE16(uint16_t* target_address, uint16_t* source)
{
	mem->writeByte(*target_address, (*source & 0x00FF));
	mem->writeByte(*target_address + 1, (*source & 0xFF00) >> 8);

	// There are no instructions that store to an immediate value.

	return 8;
}



int EmuCPU::PUSH(uint16_t* source)
{
	regs.cpu.sp--;
	mem->writeByte(regs.cpu.sp, (*source & 0xFF00) >> 8); // MSB
	regs.cpu.sp--;
	mem->writeByte(regs.cpu.sp, (*source & 0x00FF)); // LSB

	return 12;
}



int EmuCPU::POP(uint16_t* target)
{
	uint16_t value;
	value = mem->readByte(regs.cpu.sp);
	regs.cpu.sp++;
	value |= mem->readByte(regs.cpu.sp) & 0xFF;
	regs.cpu.sp++;

	*target = value;

	return 8;
}



int EmuCPU::INC8(uint8_t* target)
{
	regs.flags.half_carry = willHalfOverflow8(*target, 1);

	(*target)++;

	regs.flags.zero = *target == 0;
	regs.flags.sub = false;

	return 0;
}



int EmuCPU::INCSTORE8(uint16_t* target_address)
{
	uint8_t value = mem->readByte(*target_address);

	regs.flags.half_carry = willHalfOverflow8(value, 1);

	value++;
	mem->writeByte(*target_address, value);

	regs.flags.zero = value == 0;
	regs.flags.sub = false;

	return 8;
}



int EmuCPU::INC16(uint16_t* target)
{
	(*target)++;

	return 4;
}



int EmuCPU::DEC8(uint8_t* target)
{
	regs.flags.half_carry = willHalfUnderflow8(*target, 1);

	(*target)--;

	regs.flags.zero = *target == 0;
	regs.flags.sub = true;

	return 0;
}



int EmuCPU::DECSTORE8(uint16_t* target_address)
{
	uint8_t value = mem->readByte(*target_address);

	regs.flags.half_carry = willHalfUnderflow8(value, 1);

	value--;
	mem->writeByte(*target_address, value);

	regs.flags.zero = value == 0;
	regs.flags.sub = true;

	return 8;
}



int EmuCPU::DEC16(uint16_t* target)
{
	(*target)--;

	return 0;
}



int EmuCPU::ADD16(uint16_t* target, uint16_t* source)
{
	*target += *source;

	return 4;
}



int EmuCPU::RL(uint8_t* target)
{
	uint8_t bit_7 = ((*target) >> 7) & 1;
	uint8_t prev_carry = static_cast<int>(regs.flags.carry);

	*target = (*target) << 1;
	*target |= prev_carry;

	regs.flags.zero = *target == 0;
	regs.flags.sub = false;
	regs.flags.half_carry = false;
	regs.flags.carry = bit_7;

	return 0;
}



int EmuCPU::RR(uint8_t* target)
{
	uint8_t bit_0 = (*target) & 1;
	uint8_t prev_carry = static_cast<int>(regs.flags.carry);

	*target = (*target >> 1);
	*target |= ((prev_carry) << 7);

	regs.flags.zero = *target == 0;
	regs.flags.sub = false;
	regs.flags.half_carry = false;
	regs.flags.carry = bit_0;

	return 0;
}



int EmuCPU::RLC(uint8_t* target)
{
	uint8_t bit_7 = ((*target) >> 7) & 1;

	*target = (*target) << 1;
	*target |= bit_7;

	regs.flags.zero = *target == 0;
	regs.flags.sub = false;
	regs.flags.half_carry = false;
	regs.flags.carry = bit_7;

	return 0;
}



int EmuCPU::RRC(uint8_t* target)
{
	uint8_t bit_0 = (*target) & 1;

	*target = (*target >> 1);
	*target |= ((bit_0) << 7);

	regs.flags.zero = *target == 0;
	regs.flags.sub = false;
	regs.flags.half_carry = false;
	regs.flags.carry = bit_0;

	return 0;
}



int EmuCPU::JUMPR(uint8_t* address)
{
	int8_t relative_address;
	// Because it would be too sane to have static_cast do a bit-exact copy.
	memcpy_s(&relative_address, 1, &address, 1);

	regs.cpu.pc += relative_address;

	return 4;
}

