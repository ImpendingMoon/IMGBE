/**
 * @file emu/emucpu.cpp
 * @brief Implements the system's CPU
 * @author ImpendingMoon
 * @date 2023-07-19
 */

#include "emucpu.hpp"
#include <assert.h>
#include <fmt/core.h>

EmuCPU::EmuCPU(EmuMemory* memory)
{
	mem = memory;
}

EmuCPU::~EmuCPU()
{}



/**
 * @brief Steps the CPU by one instruction
 * @return The number of machine cycles taken
 * @throws std::runtime_error on illegal or unimplemented instruction.
 */
int EmuCPU::step(void)
{
	assert(mem != nullptr);

	int cycles = 4;
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
		default:
		{
			throw std::runtime_error(fmt::format(
				"Unhandled instruction! Opcode: {:02X}, Source: {:04X}",
				opcode, regs.cpu.pc - 1
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
				"Unhandled 2 byte instruction! Opcode: {:04X}, Source: {:04X}",
				0xCB00 + opcode, regs.cpu.pc - 2
			));
		}
		}
	}

	return cycles;
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

