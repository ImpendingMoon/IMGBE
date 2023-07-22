/**
 * @file emu/emucpu.cpp
 * @brief Implements the system's CPU
 * @author ImpendingMoon
 * @date 2023-07-22
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

	// TODO: Handle interrupts

	regs.mem.io.ienable = nextInterruptState;

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
		case 0x80:
		{
			instruction = "ADD A, B";
			cycles += ADD8(&regs.cpu.a, &regs.cpu.b);
			break;
		}
		case 0x81:
		{
			instruction = "ADD A, C";
			cycles += ADD8(&regs.cpu.a, &regs.cpu.c);
			break;
		}
		case 0x82:
		{
			instruction = "ADD A, D";
			cycles += ADD8(&regs.cpu.a, &regs.cpu.d);
			break;
		}
		case 0x83:
		{
			instruction = "ADD A, E";
			cycles += ADD8(&regs.cpu.a, &regs.cpu.e);
			break;
		}
		case 0x84:
		{
			instruction = "ADD A, H";
			cycles += ADD8(&regs.cpu.a, &regs.cpu.h);
			break;
		}
		case 0x85:
		{
			instruction = "ADD A, L";
			cycles += ADD8(&regs.cpu.a, &regs.cpu.l);
			break;
		}
		case 0x86:
		{
			instruction = "ADD A, [HL]";
			cycles += ADDLOAD8(&regs.cpu.a, &regs.cpu.hl);
			break;
		}
		case 0x87:
		{
			instruction = "ADD A, A";
			cycles += ADD8(&regs.cpu.a, &regs.cpu.a);
			break;
		}
		case 0x88:
		{
			instruction = "ADC A, B";
			cycles += ADC8(&regs.cpu.a, &regs.cpu.b);
			break;
		}
		case 0x89:
		{
			instruction = "ADC A, C";
			cycles += ADC8(&regs.cpu.a, &regs.cpu.c);
			break;
		}
		case 0x8A:
		{
			instruction = "ADC A, D";
			cycles += ADC8(&regs.cpu.a, &regs.cpu.d);
			break;
		}
		case 0x8B:
		{
			instruction = "ADC A, E";
			cycles += ADC8(&regs.cpu.a, &regs.cpu.e);
			break;
		}
		case 0x8C:
		{
			instruction = "ADC A, H";
			cycles += ADC8(&regs.cpu.a, &regs.cpu.h);
			break;
		}
		case 0x8D:
		{
			instruction = "ADC A, L";
			cycles += ADC8(&regs.cpu.a, &regs.cpu.l);
			break;
		}
		case 0x8E:
		{
			instruction = "ADC A, [HL]";
			cycles += ADCLOAD8(&regs.cpu.a, &regs.cpu.hl);
			break;
		}
		case 0x8F:
		{
			instruction = "ADC A, A";
			cycles += ADC8(&regs.cpu.a, &regs.cpu.a);
			break;
		}
		case 0x90:
		{
			instruction = "SUB A, B";
			cycles += SUB8(&regs.cpu.a, &regs.cpu.b);
			break;
		}
		case 0x91:
		{
			instruction = "SUB A, C";
			cycles += SUB8(&regs.cpu.a, &regs.cpu.c);
			break;
		}
		case 0x92:
		{
			instruction = "SUB A, D";
			cycles += SUB8(&regs.cpu.a, &regs.cpu.d);
			break;
		}
		case 0x93:
		{
			instruction = "SUB A, E";
			cycles += SUB8(&regs.cpu.a, &regs.cpu.e);
			break;
		}
		case 0x94:
		{
			instruction = "SUB A, H";
			cycles += SUB8(&regs.cpu.a, &regs.cpu.h);
			break;
		}
		case 0x95:
		{
			instruction = "SUB A, L";
			cycles += SUB8(&regs.cpu.a, &regs.cpu.l);
			break;
		}
		case 0x96:
		{
			instruction = "SUB A, [HL]";
			cycles += SUBLOAD8(&regs.cpu.a, &regs.cpu.hl);
			break;
		}
		case 0x97:
		{
			instruction = "SUB A, A";
			cycles += SUB8(&regs.cpu.a, &regs.cpu.a);
			break;
		}
		case 0x98:
		{
			instruction = "SBC A, B";
			cycles += SBC8(&regs.cpu.a, &regs.cpu.b);
			break;
		}
		case 0x99:
		{
			instruction = "SBC A, C";
			cycles += SBC8(&regs.cpu.a, &regs.cpu.c);
			break;
		}
		case 0x9A:
		{
			instruction = "SBC A, D";
			cycles += SBC8(&regs.cpu.a, &regs.cpu.d);
			break;
		}
		case 0x9B:
		{
			instruction = "SBC A, E";
			cycles += SBC8(&regs.cpu.a, &regs.cpu.e);
			break;
		}
		case 0x9C:
		{
			instruction = "SBC A, H";
			cycles += SBC8(&regs.cpu.a, &regs.cpu.h);
			break;
		}
		case 0x9D:
		{
			instruction = "SBC A, L";
			cycles += SBC8(&regs.cpu.a, &regs.cpu.l);
			break;
		}
		case 0x9E:
		{
			instruction = "SBC A, [HL]";
			cycles += SBCLOAD8(&regs.cpu.a, &regs.cpu.hl);
			break;
		}
		case 0x9F:
		{
			instruction = "SBC A, A";
			cycles += SBC8(&regs.cpu.a, &regs.cpu.a);
			break;
		}
		case 0xA0:
		{
			instruction = "AND A, B";
			cycles += AND8(&regs.cpu.a, &regs.cpu.b);
			break;
		}
		case 0xA1:
		{
			instruction = "AND A, C";
			cycles += AND8(&regs.cpu.a, &regs.cpu.c);
			break;
		}
		case 0xA2:
		{
			instruction = "AND A, D";
			cycles += AND8(&regs.cpu.a, &regs.cpu.d);
			break;
		}
		case 0xA3:
		{
			instruction = "AND A, E";
			cycles += AND8(&regs.cpu.a, &regs.cpu.e);
			break;
		}
		case 0xA4:
		{
			instruction = "AND A, H";
			cycles += AND8(&regs.cpu.a, &regs.cpu.h);
			break;
		}
		case 0xA5:
		{
			instruction = "AND A, L";
			cycles += AND8(&regs.cpu.a, &regs.cpu.l);
			break;
		}
		case 0xA6:
		{
			instruction = "AND A, [HL]";
			cycles += ANDLOAD8(&regs.cpu.a, &regs.cpu.hl);
			break;
		}
		case 0xA7:
		{
			instruction = "AND A, A";
			cycles += AND8(&regs.cpu.a, &regs.cpu.a);
			break;
		}
		case 0xA8:
		{
			instruction = "XOR A, B";
			cycles += XOR8(&regs.cpu.a, &regs.cpu.b);
			break;
		}
		case 0xA9:
		{
			instruction = "XOR A, C";
			cycles += XOR8(&regs.cpu.a, &regs.cpu.c);
			break;
		}
		case 0xAA:
		{
			instruction = "XOR A, D";
			cycles += XOR8(&regs.cpu.a, &regs.cpu.d);
			break;
		}
		case 0xAB:
		{
			instruction = "XOR A, E";
			cycles += XOR8(&regs.cpu.a, &regs.cpu.e);
			break;
		}
		case 0xAC:
		{
			instruction = "XOR A, H";
			cycles += XOR8(&regs.cpu.a, &regs.cpu.h);
			break;
		}
		case 0xAD:
		{
			instruction = "XOR A, L";
			cycles += XOR8(&regs.cpu.a, &regs.cpu.l);
			break;
		}
		case 0xAE:
		{
			instruction = "XOR A, [HL]";
			cycles += XORLOAD8(&regs.cpu.a, &regs.cpu.hl);
			break;
		}
		case 0xAF:
		{
			instruction = "XOR A, A";
			cycles += XOR8(&regs.cpu.a, &regs.cpu.a);
			break;
		}
		case 0xB0:
		{
			instruction = "OR A, B";
			cycles += OR8(&regs.cpu.a, &regs.cpu.b);
			break;
		}
		case 0xB1:
		{
			instruction = "OR A, C";
			cycles += OR8(&regs.cpu.a, &regs.cpu.c);
			break;
		}
		case 0xB2:
		{
			instruction = "OR A, D";
			cycles += OR8(&regs.cpu.a, &regs.cpu.d);
			break;
		}
		case 0xB3:
		{
			instruction = "OR A, E";
			cycles += OR8(&regs.cpu.a, &regs.cpu.e);
			break;
		}
		case 0xB4:
		{
			instruction = "OR A, H";
			cycles += OR8(&regs.cpu.a, &regs.cpu.h);
			break;
		}
		case 0xB5:
		{
			instruction = "OR A, L";
			cycles += OR8(&regs.cpu.a, &regs.cpu.l);
			break;
		}
		case 0xB6:
		{
			instruction = "OR A, [HL]";
			cycles += ORLOAD8(&regs.cpu.a, &regs.cpu.hl);
			break;
		}
		case 0xB7:
		{
			instruction = "OR A, A";
			cycles += OR8(&regs.cpu.a, &regs.cpu.a);
			break;
		}
		case 0xB8:
		{
			instruction = "CP A, B";
			cycles += CP8(&regs.cpu.a, &regs.cpu.b);
			break;
		}
		case 0xB9:
		{
			instruction = "CP A, C";
			cycles += CP8(&regs.cpu.a, &regs.cpu.c);
			break;
		}
		case 0xBA:
		{
			instruction = "CP A, D";
			cycles += CP8(&regs.cpu.a, &regs.cpu.d);
			break;
		}
		case 0xBB:
		{
			instruction = "CP A, E";
			cycles += CP8(&regs.cpu.a, &regs.cpu.e);
			break;
		}
		case 0xBC:
		{
			instruction = "CP A, H";
			cycles += CP8(&regs.cpu.a, &regs.cpu.h);
			break;
		}
		case 0xBD:
		{
			instruction = "CP A, L";
			cycles += CP8(&regs.cpu.a, &regs.cpu.l);
			break;
		}
		case 0xBE:
		{
			instruction = "CP A, [HL]";
			cycles += CPLOAD8(&regs.cpu.a, &regs.cpu.hl);
			break;
		}
		case 0xBF:
		{
			instruction = "CP A, A";
			cycles += CP8(&regs.cpu.a, &regs.cpu.a);
			break;
		}
		case 0xC0:
		{
			instruction = "RET NZ";
			bool not_zero = !regs.flags.zero;
			cycles += RET(&not_zero);
			break;
		}
		case 0xC1:
		{
			instruction = "POP BC";
			cycles += POP(&regs.cpu.bc);
			break;
		}
		case 0xC2:
		{
			instruction = "JP NZ a16";
			bool not_zero = !regs.flags.zero;
			uint16_t absolute_address;
			cycles += LOAD16(&absolute_address, &regs.cpu.pc);
			cycles += JUMP(&absolute_address, &not_zero);

			instruction = fmt::format("JP NZ ${:04X}", absolute_address);

			break;
		}
		case 0xC3:
		{
			instruction = "JP a16";
			uint16_t absolute_address;
			cycles += LOAD16(&absolute_address, &regs.cpu.pc);
			cycles += JUMP(&absolute_address, nullptr);

			instruction = fmt::format("JP ${:04X}", absolute_address);

			break;
		}
		case 0xC4:
		{
			instruction = "CALL NZ a16";
			bool not_zero = !regs.flags.zero;
			uint16_t absolute_address;
			cycles += LOAD16(&absolute_address, &regs.cpu.pc);
			cycles += CALL(&absolute_address, &not_zero);

			instruction = fmt::format("CALL NZ ${:04X}", absolute_address);

			break;
		}
		case 0xC5:
		{
			instruction = "PUSH BC";
			cycles += PUSH(&regs.cpu.bc);
			break;
		}
		case 0xC6:
		{
			instruction = "ADD A, d8";
			instruction = fmt::format("ADD A, 0x{:02X}", mem->readByte(regs.cpu.pc));

			cycles += ADDLOAD8(&regs.cpu.a, &regs.cpu.pc);
			break;
		}
		case 0xC7:
		{
			instruction = "RST $0000";
			cycles += RST(0x0000);
			break;
		}
		case 0xC8:
		{
			instruction = "RET Z";
			cycles += RET(&regs.flags.zero);
			break;
		}
		case 0xC9:
		{
			instruction = "RET";
			cycles += RET(nullptr);
			break;
		}
		case 0xCA:
		{
			instruction = "JP Z a16";
			uint16_t absolute_address;
			cycles += LOAD16(&absolute_address, &regs.cpu.pc);
			cycles += JUMP(&absolute_address, &regs.flags.zero);

			instruction = fmt::format("JP Z ${:04X}", absolute_address);

			break;
		}
		case 0xCB:
		{
			ILLEGAL_INSTRUCTION(opcode, source);
			break;
		}
		case 0xCC:
		{
			instruction = "CALL Z a16";
			uint16_t absolute_address;
			cycles += LOAD16(&absolute_address, &regs.cpu.pc);
			cycles += CALL(&absolute_address, &regs.flags.zero);

			instruction = fmt::format("CALL Z ${:04X}", absolute_address);

			break;
		}
		case 0xCD:
		{
			instruction = "CALL a16";
			uint16_t absolute_address;
			cycles += LOAD16(&absolute_address, &regs.cpu.pc);
			cycles += CALL(&absolute_address, nullptr);

			instruction = fmt::format("CALL ${:04X}", absolute_address);

			break;
		}
		case 0xCE:
		{
			instruction = "ADC A, d8";
			instruction = fmt::format("ADC A, 0x{:02X}", mem->readByte(regs.cpu.pc));

			cycles += ADCLOAD8(&regs.cpu.a, &regs.cpu.pc);
			break;
		}
		case 0xCF:
		{
			instruction = "RST $0008";
			cycles += RST(0x0008);
			break;
		}
		case 0xD0:
		{
			instruction = "RET NC";
			bool not_carry = !regs.flags.carry;
			cycles += RET(&not_carry);
			break;
		}
		case 0xD1:
		{
			instruction = "POP DE";
			cycles += POP(&regs.cpu.de);
			break;
		}
		case 0xD2:
		{
			instruction = "JP NC a16";
			bool not_carry = !regs.flags.carry;
			uint16_t absolute_address;
			cycles += LOAD16(&absolute_address, &regs.cpu.pc);
			cycles += JUMP(&absolute_address, &not_carry);

			instruction = fmt::format("JP NC ${:04X}", absolute_address);

			break;
		}
		case 0xD3:
		{
			ILLEGAL_INSTRUCTION(opcode, source);
			break;
		}
		case 0xD4:
		{
			instruction = "CALL NC a16";
			bool not_carry = !regs.flags.carry;
			uint16_t absolute_address;
			cycles += LOAD16(&absolute_address, &regs.cpu.pc);
			cycles += CALL(&absolute_address, &not_carry);

			instruction = fmt::format("CALL NC ${:04X}", absolute_address);

			break;
		}
		case 0xD5:
		{
			instruction = "PUSH DE";
			cycles += PUSH(&regs.cpu.de);
			break;
		}
		case 0xD6:
		{
			instruction = "SUB A, d8";
			instruction = fmt::format("SUB A, 0x{:02X}", mem->readByte(regs.cpu.pc));

			cycles += SUBLOAD8(&regs.cpu.a, &regs.cpu.pc);
			break;
		}
		case 0xD7:
		{
			instruction = "RST $0010";
			cycles += RST(0x0010);
			break;
		}
		case 0xD8:
		{
			instruction = "RET C";
			cycles += RET(&regs.flags.carry);
			break;
		}
		case 0xD9:
		{
			instruction = "RETI";
			cycles += RETI();
			break;
		}
		case 0xDA:
		{
			instruction = "JP C a16";
			uint16_t absolute_address;
			cycles += LOAD16(&absolute_address, &regs.cpu.pc);
			cycles += JUMP(&absolute_address, &regs.flags.carry);

			instruction = fmt::format("JP C ${:04X}", absolute_address);

			break;
		}
		case 0xDB:
		{
			ILLEGAL_INSTRUCTION(opcode, source);
			break;
		}
		case 0xDC:
		{
			instruction = "CALL C a16";
			uint16_t absolute_address;
			cycles += LOAD16(&absolute_address, &regs.cpu.pc);
			cycles += CALL(&absolute_address, &regs.flags.carry);

			instruction = fmt::format("CALL C ${:04X}", absolute_address);

			break;
		}
		case 0xDD:
		{
			ILLEGAL_INSTRUCTION(opcode, source);
			break;
		}
		case 0xDE:
		{
			instruction = "SBC A, d8";
			instruction = fmt::format("SBC A, 0x{:02X}", mem->readByte(regs.cpu.pc));

			cycles += SBCLOAD8(&regs.cpu.a, &regs.cpu.pc);
			break;
		}
		case 0xDF:
		{
			instruction = "RST $0018";
			cycles += RST(0x0018);
			break;
		}
		case 0xE0:
		{
			instruction = "LDH [a8], A";
			uint8_t offset;
			cycles += LOAD8(&offset, &regs.cpu.pc);
			uint16_t absolute_address = 0xFF00 + offset;
			cycles += STORE8(&absolute_address, &regs.cpu.a);

			instruction = fmt::format("LDH [{}], A", offset);

			break;
		}
		case 0xE1:
		{
			instruction = "POP HL";
			cycles += POP(&regs.cpu.hl);
			break;
		}
		case 0xE2:
		{
			instruction = "LD [C], A";
			uint16_t absolute_address = 0xFF00 + regs.cpu.c;
			cycles += STORE8(&absolute_address, &regs.cpu.a);
			break;
		}
		case 0xE3:
		{
			ILLEGAL_INSTRUCTION(opcode, source);
			break;
		}
		case 0xE4:
		{
			ILLEGAL_INSTRUCTION(opcode, source);
			break;
		}
		case 0xE5:
		{
			instruction = "PUSH HL";
			cycles += PUSH(&regs.cpu.hl);
			break;
		}
		case 0xE6:
		{
			instruction = "AND A, d8";
			instruction = fmt::format("AND A, 0x{:02X}", mem->readByte(regs.cpu.pc));

			cycles += ANDLOAD8(&regs.cpu.a, &regs.cpu.pc);
			break;
		}
		case 0xE7:
		{
			instruction = "RST $0020";
			cycles += RST(0x0020);
			break;
		}
		case 0xE8:
		{
			instruction = "ADD SP, s8";
			uint8_t value;
			cycles += LOAD8(&value, &regs.cpu.pc);
			cycles += ADDSIGNED16(&regs.cpu.sp, &value);

			instruction = fmt::format("ADD SP, 0x{:02X}", (int8_t)value);

			break;
		}
		case 0xE9:
		{
			instruction = "JP HL";
			cycles += JUMP(&regs.cpu.hl, nullptr);
			break;
		}
		case 0xEA:
		{
			instruction = "LD [a16], A";
			uint16_t absolute_address;
			cycles += LOAD16(&absolute_address, &regs.cpu.pc);
			cycles += STORE8(&absolute_address, &regs.cpu.a);

			instruction = fmt::format("LD [${:04X}], A", absolute_address);

			break;
		}
		case 0xEB:
		{
			ILLEGAL_INSTRUCTION(opcode, source);
			break;
		}
		case 0xEC:
		{
			ILLEGAL_INSTRUCTION(opcode, source);
			break;
		}
		case 0xED:
		{
			ILLEGAL_INSTRUCTION(opcode, source);
			break;
		}
		case 0xEE:
		{
			instruction = "XOR A, d8";
			instruction = fmt::format("XOR A, 0x{:02X}", mem->readByte(regs.cpu.pc));

			cycles += XORLOAD8(&regs.cpu.a, &regs.cpu.pc);
			break;
		}
		case 0xEF:
		{
			instruction = "RST $0028";
			cycles += RST(0x0028);
			break;
		}
		case 0xF0:
		{
			instruction = "LDH A, [a8]";
			uint8_t offset;
			cycles += LOAD8(&offset, &regs.cpu.pc);
			uint16_t absolute_address = 0xFF00 + offset;
			cycles += LOAD8(&regs.cpu.a, &absolute_address);

			instruction = fmt::format("LDH A, [{}]", offset);

			break;
		}
		case 0xF1:
		{
			instruction = "POP AF";
			cycles += POP(&regs.cpu.af);
			break;
		}
		case 0xF2:
		{
			instruction = "LDH A, [c]";
			uint16_t absolute_address = 0xFF00 + regs.cpu.c;
			cycles += LOAD8(&regs.cpu.a, &absolute_address);
			break;
		}
		case 0xF3:
		{
			instruction = "DI";
			cycles += DI();
			break;
		}
		case 0xF4:
		{
			ILLEGAL_INSTRUCTION(opcode, source);
			break;
		}
		case 0xF5:
		{
			instruction = "PUSH AF";
			cycles += PUSH(&regs.cpu.af);
			break;
		}
		case 0xF6:
		{
			instruction = "OR A, d8";
			instruction = fmt::format("OR A, 0x{:02X}", mem->readByte(regs.cpu.pc));

			cycles += ORLOAD8(&regs.cpu.a, &regs.cpu.pc);
			break;
		}
		case 0xF7:
		{
			instruction = "RST $0030";
			cycles += RST(0x0030);
			break;
		}
		case 0xF8:
		{
			instruction = "LD HL, SP + s8";
			uint16_t sp = regs.cpu.sp;
			uint8_t offset;
			cycles += LOAD8(&offset, &regs.cpu.pc);
			cycles += ADDSIGNED16(&sp, &offset);
			cycles += MOVE16(&regs.cpu.hl, &sp);

			instruction = fmt::format("LD HL, SP + 0x{:02X}", (int8_t)offset);

			break;
		}
		case 0xF9:
		{
			instruction = "LD SP, HL";
			cycles += MOVE16(&regs.cpu.sp, &regs.cpu.hl);
			break;
		}
		case 0xFA:
		{
			instruction = "LD A, [a16]";
			uint16_t absolute_address;
			cycles += LOAD16(&absolute_address, &regs.cpu.pc);
			cycles += LOAD8(&regs.cpu.a, &absolute_address);

			instruction = fmt::format("LD A, [${:04X}", absolute_address);

			break;
		}
		case 0xFB:
		{
			instruction = "EI";
			cycles += EI();
			break;
		}
		case 0xFC:
		{
			ILLEGAL_INSTRUCTION(opcode, source);
			break;
		}
		case 0xFD:
		{
			ILLEGAL_INSTRUCTION(opcode, source);
			break;
		}
		case 0xFE:
		{
			instruction = "CP A, d8";
			instruction = fmt::format("CP A, 0x{:02X}", mem->readByte(regs.cpu.pc));

			cycles += CPLOAD8(&regs.cpu.a, &regs.cpu.pc);
			break;
		}
		case 0xFF:
		{
			instruction = "RST $0038";
			cycles += RST(0x0038);
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



int EmuCPU::MOVE16(uint16_t* target, uint16_t* source)
{
	*target = *source;

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



int EmuCPU::ADD8(uint8_t* target, uint8_t* source)
{
	regs.flags.carry = willOverflow8(*target, *source);
	regs.flags.half_carry = willHalfOverflow8(*target, *source);

	*target += *source;

	regs.flags.zero = *target == 0;
	regs.flags.sub = false;

	return 0;
}



int EmuCPU::ADDLOAD8(uint8_t* target, uint16_t* source_address)
{
	uint8_t value = mem->readByte(*source_address);

	regs.flags.carry = willOverflow8(*target, value);
	regs.flags.half_carry = willHalfOverflow8(*target, value);

	*target += value;

	regs.flags.zero = *target == 0;
	regs.flags.sub = false;

	// Immediate accesses always increment program counter
	if(source_address == &regs.cpu.pc)
	{
		regs.cpu.pc++;
	}

	return 4;
}



int EmuCPU::ADD16(uint16_t* target, uint16_t* source)
{
	*target += *source;

	return 4;
}



int EmuCPU::ADDSIGNED16(uint16_t* target, uint8_t* source)
{
	int8_t signed_source;
	memcpy_s(&signed_source, 1, source, 1);

	*target += signed_source;

	return 4;
}



int EmuCPU::ADC8(uint8_t* target, uint8_t* source)
{
	int carry_val = static_cast<int>(regs.flags.carry);
	regs.flags.carry = willOverflow8(*target, (*source) + carry_val);
	regs.flags.half_carry = willHalfOverflow8(*target, (*source) + carry_val);

	*target += (*source) + carry_val;

	regs.flags.zero = *target == 0;
	regs.flags.sub = false;

	return 0;
}



int EmuCPU::ADCLOAD8(uint8_t* target, uint16_t* source_address)
{
	int carry_val = static_cast<int>(regs.flags.carry);
	uint8_t value = mem->readByte(*source_address);

	regs.flags.carry = willOverflow8(*target, value + carry_val);
	regs.flags.half_carry = willHalfOverflow8(*target, value + carry_val);

	*target += value + carry_val;

	regs.flags.zero = *target == 0;
	regs.flags.sub = false;

	// Immediate accesses always increment program counter
	if(source_address == &regs.cpu.pc)
	{
		regs.cpu.pc++;
	}

	return 4;
}



int EmuCPU::SUB8(uint8_t* target, uint8_t* source)
{
	regs.flags.carry = willUnderflow8(*target, *source);
	regs.flags.half_carry = willHalfUnderflow8(*target, *source);

	*target -= *source;

	regs.flags.zero = *target == 0;
	regs.flags.sub = true;

	return 0;
}



int EmuCPU::SUBLOAD8(uint8_t* target, uint16_t* source_address)
{
	uint8_t value = mem->readByte(*source_address);

	regs.flags.carry = willUnderflow8(*target, value);
	regs.flags.half_carry = willHalfUnderflow8(*target, value);

	*target -= value;

	regs.flags.zero = *target == 0;
	regs.flags.sub = true;

	// Immediate accesses always increment program counter
	if(source_address == &regs.cpu.pc)
	{
		regs.cpu.pc++;
	}

	return 4;
}



int EmuCPU::SBC8(uint8_t* target, uint8_t* source)
{
	int carry_val = static_cast<int>(regs.flags.carry);
	regs.flags.carry = willUnderflow8(*target, (*source) + carry_val);
	regs.flags.half_carry = willHalfUnderflow8(*target, (*source) + carry_val);

	*target -= ((*source) + carry_val);

	regs.flags.zero = *target == 0;
	regs.flags.sub = true;

	return 0;
}



int EmuCPU::SBCLOAD8(uint8_t* target, uint16_t* source_address)
{
	int carry_val = static_cast<int>(regs.flags.carry);
	uint8_t value = mem->readByte(*source_address);

	regs.flags.carry = willUnderflow8(*target, value + carry_val);
	regs.flags.half_carry = willHalfUnderflow8(*target, value + carry_val);

	*target -= (value + carry_val);

	regs.flags.zero = *target == 0;
	regs.flags.sub = true;

	// Immediate accesses always increment program counter
	if(source_address == &regs.cpu.pc)
	{
		regs.cpu.pc++;
	}

	return 4;
}



int EmuCPU::AND8(uint8_t* target, uint8_t* source)
{
	*target &= *source;

	regs.flags.zero = *target == 0;
	regs.flags.sub = false;
	regs.flags.half_carry = true;
	regs.flags.carry = false;

	return 0;
}



int EmuCPU::ANDLOAD8(uint8_t* target, uint16_t* source_address)
{
	uint8_t value = mem->readByte(*source_address);

	*target &= value;

	regs.flags.zero = *target == 0;
	regs.flags.sub = false;
	regs.flags.half_carry = true;
	regs.flags.carry = false;

	// Immediate accesses always increment program counter
	if(source_address == &regs.cpu.pc)
	{
		regs.cpu.pc++;
	}

	return 4;
}



int EmuCPU::XOR8(uint8_t* target, uint8_t* source)
{
	*target ^= *source;

	regs.flags.zero = *target == 0;
	regs.flags.sub = false;
	regs.flags.half_carry = false;
	regs.flags.carry = false;

	return 0;
}



int EmuCPU::XORLOAD8(uint8_t* target, uint16_t* source_address)
{
	uint8_t value = mem->readByte(*source_address);

	*target ^= value;

	regs.flags.zero = *target == 0;
	regs.flags.sub = false;
	regs.flags.half_carry = false;
	regs.flags.carry = false;

	// Immediate accesses always increment program counter
	if(source_address == &regs.cpu.pc)
	{
		regs.cpu.pc++;
	}

	return 4;
}


int EmuCPU::OR8(uint8_t* target, uint8_t* source)
{
	*target |= *source;

	regs.flags.zero = *target == 0;
	regs.flags.sub = false;
	regs.flags.half_carry = false;
	regs.flags.carry = false;

	return 0;
}



int EmuCPU::ORLOAD8(uint8_t* target, uint16_t* source_address)
{
	uint8_t value = mem->readByte(*source_address);

	*target |= value;

	regs.flags.zero = *target == 0;
	regs.flags.sub = false;
	regs.flags.half_carry = false;
	regs.flags.carry = false;

	// Immediate accesses always increment program counter
	if(source_address == &regs.cpu.pc)
	{
		regs.cpu.pc++;
	}

	return 4;
}



int EmuCPU::CP8(uint8_t* target, uint8_t* source)
{
	regs.flags.carry = willUnderflow8(*target, *source);
	regs.flags.half_carry = willHalfUnderflow8(*target, *source);
	regs.flags.zero = *target - *source == 0;
	regs.flags.sub = true;

	return 0;
}



int EmuCPU::CPLOAD8(uint8_t* target, uint16_t* source_address)
{
	uint8_t value = mem->readByte(*source_address);

	regs.flags.carry = willUnderflow8(*target, value);
	regs.flags.half_carry = willHalfUnderflow8(*target, value);
	regs.flags.zero = *target - value == 0;
	regs.flags.sub = true;

	// Immediate accesses always increment program counter
	if(source_address == &regs.cpu.pc)
	{
		regs.cpu.pc++;
	}

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



int EmuCPU::JUMP(uint16_t* address, bool* condition)
{
	// If condition exists and is false, do not jump.
	if(condition != nullptr && !(*condition))
	{
		return 0;
	}

	regs.cpu.pc = *address;

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



int EmuCPU::CALL(uint16_t* address, bool* condition)
{
	// If condition exists and is false, do not jump.
	if(condition != nullptr && !(*condition))
	{
		return 0;
	}

	PUSH(&regs.cpu.pc);
	JUMP(address, nullptr);

	return 20;
}



int EmuCPU::RET(bool* condition)
{
	// If condition exists and is false, do not jump.
	if(condition != nullptr && !(*condition))
	{
		return 0;
	}

	POP(&regs.cpu.pc);
	regs.cpu.pc++; // So we don't land at the same CALL instruction.

	return 12;
}



int EmuCPU::RETI(void)
{
	nextInterruptState = true;
	RET(nullptr);
	return 12;
}



int EmuCPU::RST(uint16_t vector)
{
	PUSH(&regs.cpu.pc);
	JUMP(&vector, nullptr);
	return 12;
}



int EmuCPU::EI(void)
{
	nextInterruptState = true;
	return 0;
}

int EmuCPU::DI(void)
{
	nextInterruptState = false;
	return 0;
}



void EmuCPU::ILLEGAL_INSTRUCTION(uint8_t opcode, uint16_t source)
{
	throw std::runtime_error(fmt::format(
		"Illegal instruction! Opcode: 0x{:02X}, Source: ${:04X}",
		opcode, source
	));
}



// Note: this file is massive.
