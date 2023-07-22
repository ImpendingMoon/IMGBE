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
			// Base string kept for ease of lookup
			instruction = "LD BC, d16";
			cycles += LOAD16(&regs.cpu.bc, &regs.cpu.pc);

			instruction = fmt::format("LD BC, 0x{:04X}", regs.cpu.bc);

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

			instruction = fmt::format("LD B, 0x{:02X}", regs.cpu.b);

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

			instruction = fmt::format("LD [0x{:04X}], SP", target_address);

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

			instruction = fmt::format("LD C, 0x{:02X}", regs.cpu.c);

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

			instruction = fmt::format("LD DE, 0x{:04X}", regs.cpu.de);

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

			instruction = fmt::format("LD D, 0x{:02X}", regs.cpu.d);

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
			cycles += JUMPR(&relative_address, nullptr);

			// Use C-Style cast because it's a pain in the ass to do it right.
			instruction = fmt::format("JR ${:02X}", (int8_t)relative_address);

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

			instruction = fmt::format("LD E, 0x{:02X}", regs.cpu.e);

			break;
		}
		case 0x1F:
		{
			instruction = "RRA";
			cycles += RR(&regs.cpu.a);
			break;
		}
		case 0x20:
		{
			instruction = "JR NZ s8";
			bool not_zero = !regs.flags.zero;
			uint8_t relative_address;
			cycles += LOAD8(&relative_address, &regs.cpu.pc);
			cycles += JUMPR(&relative_address, &not_zero);

			instruction = fmt::format("JR NZ ${:02X}", (int8_t)relative_address);

			break;
		}
		case 0x21:
		{
			instruction = "LD HL, d16";
			cycles += LOAD16(&regs.cpu.hl, &regs.cpu.pc);

			instruction = fmt::format("LD HL, 0x{:04X}", regs.cpu.hl);

			break;
		}
		case 0x22:
		{
			instruction = "LD [HL+], A";
			cycles += STORE8(&regs.cpu.hl, &regs.cpu.a);
			regs.cpu.hl++; // Simultaneous, no performance penalty.
			break;
		}
		case 0x23:
		{
			instruction = "INC HL";
			cycles += INC16(&regs.cpu.hl);
			break;
		}
		case 0x24:
		{
			instruction = "INC H";
			cycles += INC8(&regs.cpu.h);
			break;
		}
		case 0x25:
		{
			instruction = "DEC H";
			cycles += DEC8(&regs.cpu.h);
			break;
		}
		case 0x26:
		{
			instruction = "LD H, d8";
			cycles += LOAD8(&regs.cpu.h, &regs.cpu.pc);

			instruction = fmt::format("LD H, 0x{:02X}", regs.cpu.h);

			break;
		}
		case 0x27:
		{
			instruction = "DAA";
			cycles += DAA();
			break;
		}
		case 0x28:
		{
			instruction = "JR Z, s8";
			uint8_t relative_address;
			cycles += LOAD8(&relative_address, &regs.cpu.pc);
			cycles += JUMPR(&relative_address, &regs.flags.zero);

			instruction = fmt::format("JR Z ${:02X}", (int8_t)relative_address);

			break;
		}
		case 0x29:
		{
			instruction = "ADD HL, HL";
			cycles += ADD16(&regs.cpu.hl, &regs.cpu.hl);
			break;
		}
		case 0x2A:
		{
			instruction = "LD A, [HL+]";
			cycles += LOAD8(&regs.cpu.a, &regs.cpu.hl);
			regs.cpu.hl++; // Simultaneous, no performance penalty.
			break;
		}
		case 0x2B:
		{
			instruction = "DEC HL";
			cycles += DEC16(&regs.cpu.hl);
			break;
		}
		case 0x2C:
		{
			instruction = "INC L";
			cycles += INC8(&regs.cpu.l);
			break;
		}
		case 0x2D:
		{
			instruction = "DEC L";
			cycles += DEC8(&regs.cpu.l);
			break;
		}
		case 0x2E:
		{
			instruction = "LD L, d8";
			cycles += LOAD8(&regs.cpu.l, &regs.cpu.pc);

			instruction = fmt::format("LD L, 0x{:02X}", regs.cpu.l);

			break;
		}
		case 0x2F:
		{
			instruction = "CPL";
			regs.cpu.a = ~regs.cpu.a;
			break;
		}
		case 0x30:
		{
			instruction = "JR NC, s8";
			bool not_carry = !regs.flags.carry;
			uint8_t relative_address;
			cycles += LOAD8(&relative_address, &regs.cpu.pc);
			cycles += JUMPR(&relative_address, &not_carry);

			instruction = fmt::format("JR NC ${:02X}", (int8_t)relative_address);

			break;
		}
		case 0x31:
		{
			instruction = "LD SP, d16";
			cycles += LOAD16(&regs.cpu.sp, &regs.cpu.pc);

			instruction = fmt::format("LD SP, 0x{:04X}", regs.cpu.sp);
			
			break;
		}
		case 0x32:
		{
			instruction = "LD [HL-], A";
			cycles += STORE8(&regs.cpu.hl, &regs.cpu.a);
			regs.cpu.hl--;
			break;
		}
		case 0x33:
		{
			instruction = "INC SP";
			cycles += INC16(&regs.cpu.sp);
			break;
		}
		case 0x34:
		{
			instruction = "INC [HL]";
			cycles += INCSTORE8(&regs.cpu.hl);
			break;
		}
		case 0x35:
		{
			instruction = "DEC [HL]";
			cycles += DECSTORE8(&regs.cpu.hl);
			break;
		}
		case 0x36:
		{
			instruction = "LD [HL], d8";
			uint8_t value;
			cycles += LOAD8(&value, &regs.cpu.pc);
			cycles += STORE8(&regs.cpu.hl, &value);
			break;
		}
		case 0x37:
		{
			instruction = "SCF";
			regs.flags.carry = true;
			break;
		}
		case 0x38:
		{
			instruction = "JR C, s8";

			uint8_t relative_address;
			cycles += LOAD8(&relative_address, &regs.cpu.pc);
			cycles += JUMPR(&relative_address, &regs.flags.carry);

			instruction = fmt::format("JR C ${:02X}", (int8_t)relative_address);

			break;
		}
		case 0x39:
		{
			instruction = "ADD HL, SP";
			cycles += ADD16(&regs.cpu.hl, &regs.cpu.sp);
			break;
		}
		case 0x3A:
		{
			instruction = "LD A, [HL-]";
			cycles += LOAD8(&regs.cpu.a, &regs.cpu.hl);
			regs.cpu.hl--;
			break;
		}
		case 0x3B:
		{
			instruction = "DEC SP";
			cycles += DEC16(&regs.cpu.sp);
			break;
		}
		case 0x3C:
		{
			instruction = "INC A";
			cycles += INC8(&regs.cpu.a);
			break;
		}
		case 0x3D:
		{
			instruction = "DEC A";
			cycles += DEC8(&regs.cpu.a);
			break;
		}
		case 0x3E:
		{
			instruction = "LD A, d8";
			cycles += LOAD8(&regs.cpu.a, &regs.cpu.pc);

			instruction = fmt::format("LD A, 0x{:02X}", regs.cpu.a);

			break;
		}
		case 0x3F:
		{
			instruction = "CCF";
			regs.flags.carry = !regs.flags.carry;
			break;
		}
		case 0x40:
		{
			instruction = "LD B, B";
			cycles += MOVE8(&regs.cpu.b, &regs.cpu.b);
			break;
		}
		case 0x41:
		{
			instruction = "LD B, C";
			cycles += MOVE8(&regs.cpu.b, &regs.cpu.c);
			break;
		}
		case 0x42:
		{
			instruction = "LD B, D";
			cycles += MOVE8(&regs.cpu.b, &regs.cpu.d);
			break;
		}
		case 0x43:
		{
			instruction = "LD B, E";
			cycles += MOVE8(&regs.cpu.b, &regs.cpu.e);
			break;
		}
		case 0x44:
		{
			instruction = "LD B, H";
			cycles += MOVE8(&regs.cpu.b, &regs.cpu.h);
			break;
		}
		case 0x45: // nice
		{
			instruction = "LD B, L";
			cycles += MOVE8(&regs.cpu.b, &regs.cpu.l);
			break;
		}
		case 0x46:
		{
			instruction = "LD B, [HL]";
			cycles += LOAD8(&regs.cpu.b, &regs.cpu.hl);
			break;
		}
		case 0x47:
		{
			instruction = "LD B, A";
			cycles += MOVE8(&regs.cpu.b, &regs.cpu.a);
			break;
		}
		case 0x48:
		{
			instruction = "LD C, B";
			cycles += MOVE8(&regs.cpu.c, &regs.cpu.b);
			break;
		}
		case 0x49:
		{
			instruction = "LD C, C";
			cycles += MOVE8(&regs.cpu.c, &regs.cpu.c);
			break;
		}
		case 0x4A:
		{
			instruction = "LD C, D";
			cycles += MOVE8(&regs.cpu.c, &regs.cpu.d);
			break;
		}
		case 0x4B:
		{
			instruction = "LD C, E";
			cycles += MOVE8(&regs.cpu.c, &regs.cpu.e);
			break;
		}
		case 0x4C:
		{
			instruction = "LD C, H";
			cycles += MOVE8(&regs.cpu.c, &regs.cpu.h);
			break;
		}
		case 0x4D:
		{
			instruction = "LD C, L";
			cycles += MOVE8(&regs.cpu.c, &regs.cpu.l);
			break;
		}
		case 0x4E:
		{
			instruction = "LD C, [HL]";
			cycles += LOAD8(&regs.cpu.c, &regs.cpu.hl);
			break;
		}
		case 0x4F:
		{
			instruction = "LD C, A";
			cycles += MOVE8(&regs.cpu.c, &regs.cpu.a);
			break;
		}
		case 0x50:
		{
			instruction = "LD D, B";
			cycles += MOVE8(&regs.cpu.d, &regs.cpu.b);
			break;
		}
		case 0x51:
		{
			instruction = "LD D, C";
			cycles += MOVE8(&regs.cpu.d, &regs.cpu.c);
			break;
		}
		case 0x52:
		{
			instruction = "LD D, D";
			cycles += MOVE8(&regs.cpu.d, &regs.cpu.d);
			break;
		}
		case 0x53:
		{
			instruction = "LD D, E";
			cycles += MOVE8(&regs.cpu.d, &regs.cpu.e);
			break;
		}
		case 0x54:
		{
			instruction = "LD D, H";
			cycles += MOVE8(&regs.cpu.d, &regs.cpu.h);
			break;
		}
		case 0x55:
		{
			instruction = "LD D, L";
			cycles += MOVE8(&regs.cpu.d, &regs.cpu.l);
			break;
		}
		case 0x56:
		{
			instruction = "LD D, [HL]";
			cycles += LOAD8(&regs.cpu.d, &regs.cpu.hl);
			break;
		}
		case 0x57:
		{
			instruction = "LD D, A";
			cycles += MOVE8(&regs.cpu.d, &regs.cpu.a);
			break;
		}
		case 0x58:
		{
			instruction = "LD E, B";
			cycles += MOVE8(&regs.cpu.e, &regs.cpu.b);
			break;
		}
		case 0x59:
		{
			instruction = "LD E, C";
			cycles += MOVE8(&regs.cpu.e, &regs.cpu.c);
			break;
		}
		case 0x5A:
		{
			instruction = "LD E, D";
			cycles += MOVE8(&regs.cpu.e, &regs.cpu.d);
			break;
		}
		case 0x5B:
		{
			instruction = "LD E, E";
			cycles += MOVE8(&regs.cpu.e, &regs.cpu.e);
			break;
		}
		case 0x5C:
		{
			instruction = "LD E, H";
			cycles += MOVE8(&regs.cpu.e, &regs.cpu.h);
			break;
		}
		case 0x5D:
		{
			instruction = "LD E, L";
			cycles += MOVE8(&regs.cpu.e, &regs.cpu.l);
			break;
		}
		case 0x5E:
		{
			instruction = "LD E, [HL]";
			cycles += LOAD8(&regs.cpu.e, &regs.cpu.hl);
			break;
		}
		case 0x5F:
		{
			instruction = "LD E, A";
			cycles += MOVE8(&regs.cpu.e, &regs.cpu.a);
			break;
		}
		case 0x60:
		{
			instruction = "LD H, B";
			cycles += MOVE8(&regs.cpu.h, &regs.cpu.b);
			break;
		}
		case 0x61:
		{
			instruction = "LD H, C";
			cycles += MOVE8(&regs.cpu.h, &regs.cpu.c);
			break;
		}
		case 0x62:
		{
			instruction = "LD H, D";
			cycles += MOVE8(&regs.cpu.h, &regs.cpu.d);
			break;
		}
		case 0x63:
		{
			instruction = "LD H, E";
			cycles += MOVE8(&regs.cpu.h, &regs.cpu.e);
			break;
		}
		case 0x64:
		{
			instruction = "LD H, H";
			cycles += MOVE8(&regs.cpu.h, &regs.cpu.h);
			break;
		}
		case 0x65:
		{
			instruction = "LD H, L";
			cycles += MOVE8(&regs.cpu.h, &regs.cpu.l);
			break;
		}
		case 0x66:
		{
			instruction = "LD H, [HL]";
			cycles += LOAD8(&regs.cpu.h, &regs.cpu.hl);
			break;
		}
		case 0x67:
		{
			instruction = "LD H, A";
			cycles += MOVE8(&regs.cpu.h, &regs.cpu.a);
			break;
		}
		case 0x68:
		{
			instruction = "LD L, B";
			cycles += MOVE8(&regs.cpu.l, &regs.cpu.b);
			break;
		}
		case 0x69: // kinda nice
		{
			instruction = "LD L, C";
			cycles += MOVE8(&regs.cpu.l, &regs.cpu.c);
			break;
		}
		case 0x6A:
		{
			instruction = "LD L, D";
			cycles += MOVE8(&regs.cpu.l, &regs.cpu.d);
			break;
		}
		case 0x6B:
		{
			instruction = "LD L, E";
			cycles += MOVE8(&regs.cpu.l, &regs.cpu.e);
			break;
		}
		case 0x6C:
		{
			instruction = "LD L, H";
			cycles += MOVE8(&regs.cpu.l, &regs.cpu.h);
			break;
		}
		case 0x6D:
		{
			instruction = "LD L, L";
			cycles += MOVE8(&regs.cpu.l, &regs.cpu.l);
			break;
		}
		case 0x6E:
		{
			instruction = "LD L, [HL]";
			cycles += LOAD8(&regs.cpu.l, &regs.cpu.hl);
			break;
		}
		case 0x6F:
		{
			instruction = "LD L, A";
			cycles += MOVE8(&regs.cpu.l, &regs.cpu.a);
			break;
		}
		case 0x70:
		{
			instruction = "LD [HL], B";
			cycles += STORE8(&regs.cpu.hl, &regs.cpu.b);
			break;
		}
		case 0x71:
		{
			instruction = "LD [HL], C";
			cycles += STORE8(&regs.cpu.hl, &regs.cpu.c);
			break;
		}
		case 0x72:
		{
			instruction = "LD [HL], D";
			cycles += STORE8(&regs.cpu.hl, &regs.cpu.d);
			break;
		}
		case 0x73:
		{
			instruction = "LD [HL], E";
			cycles += STORE8(&regs.cpu.hl, &regs.cpu.e);
			break;
		}
		case 0x74:
		{
			instruction = "LD [HL], H";
			cycles += STORE8(&regs.cpu.hl, &regs.cpu.h);
			break;
		}
		case 0x75:
		{
			instruction = "LD [HL], L";
			cycles += STORE8(&regs.cpu.hl, &regs.cpu.l);
			break;
		}
		case 0x76:
		{
			instruction = "HALT";
			// TODO: Halt
			break;
		}
		case 0x77:
		{
			instruction = "LD [HL], A";
			cycles += STORE8(&regs.cpu.hl, &regs.cpu.a);
			break;
		}
		case 0x78:
		{
			instruction = "LD A, B";
			cycles += MOVE8(&regs.cpu.a, &regs.cpu.b);
			break;
		}
		case 0x79: // kinda nice
		{
			instruction = "LD A, C";
			cycles += MOVE8(&regs.cpu.a, &regs.cpu.c);
			break;
		}
		case 0x7A:
		{
			instruction = "LD A, D";
			cycles += MOVE8(&regs.cpu.a, &regs.cpu.d);
			break;
		}
		case 0x7B:
		{
			instruction = "LD A, E";
			cycles += MOVE8(&regs.cpu.a, &regs.cpu.e);
			break;
		}
		case 0x7C:
		{
			instruction = "LD A, H";
			cycles += MOVE8(&regs.cpu.a, &regs.cpu.h);
			break;
		}
		case 0x7D:
		{
			instruction = "LD A, L";
			cycles += MOVE8(&regs.cpu.a, &regs.cpu.l);
			break;
		}
		case 0x7E:
		{
			instruction = "LD A, [HL]";
			cycles += LOAD8(&regs.cpu.a, &regs.cpu.hl);
			break;
		}
		case 0x7F:
		{
			instruction = "LD A, A";
			cycles += MOVE8(&regs.cpu.a, &regs.cpu.a);
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
		"Executed instruction {}. Opcode: 0x{:02X} - Source: ${:04X} - Cycles: {}",
		instruction, opcode, source, cycles
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



int EmuCPU::DAA(void)
{
	// Taken from user AWJ @ https://forums.nesdev.org/viewtopic.php?t=15944
	if(!regs.flags.sub)
	{  // after an addition, adjust if (half-)carry occurred or if result is out of bounds
		if(regs.flags.carry || regs.cpu.a > 0x99)
		{
			regs.cpu.a += 0x60; regs.flags.carry = true;
		}
		if(regs.flags.half_carry || (regs.cpu.a & 0x0f) > 0x09)
		{
			regs.cpu.a += 0x6;
		}
	} else
	{  // after a subtraction, only adjust if (half-)carry occurred
		if(regs.flags.carry)
		{
			regs.cpu.a -= 0x60;
		}
		if(regs.flags.half_carry)
		{
			regs.cpu.a -= 0x60;
		}
	}
	// these flags are always updated
	regs.flags.zero = (regs.cpu.a == 0); // the usual z flag
	regs.flags.half_carry = false; // h flag is always cleared

	return 0;
}



int EmuCPU::JUMPR(uint8_t* address, bool* condition)
{
	// If condition exists and is false, do not jump.
	if(condition != nullptr && !(*condition))
	{
		return 0;
	}

	int8_t relative_address;
	// Because it would be too sane to have static_cast do a bit-exact copy.
	memcpy_s(&relative_address, 1, &address, 1);

	regs.cpu.pc += relative_address;

	return 4;
}

