/**
 * @file emu/emuppu.hpp
 * @brief Implement's the system's Picture Processing Unit
 * @author ImpendingMoon
 * @date 2023-08-21
 */

#pragma once

#include "emumemory.hpp"
#include "emucpu.hpp"

class EmuPPU
{
public:
	EmuPPU(EmuMemory* memory = nullptr, EmuCPU* cpu = nullptr);
	virtual ~EmuPPU();

	void setMemory(EmuMemory* memory);

	void setCPU(EmuCPU* cpu = nullptr);

	/**
	 * @brief Steps the PPU by a given number of cycles
	 * @param cycles
	 * @throws std::runtime_exception when memory or CPU pointer is null
	 */
	void step(int cycles);

private:
	EmuMemory* mem;
	EmuCPU* cpu;

	enum PPUStates
	{
		OAMSearch,
		PixelTransfer,
		HBlank,
		VBlank,
	};

	PPUStates state = OAMSearch;
	int cycle = 0;
	uint8_t lx, ly;
};