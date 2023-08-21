/**
 * @file emu/emuppu.cpp
 * @brief Implement's the system's Picture Processing Unit
 * @author ImpendingMoon
 * @date 2023-08-21
 */

#include "emuppu.hpp"
#include <stdexcept>
#include <fmt/core.h>
#include "../logger.hpp"

EmuPPU::EmuPPU(EmuMemory* memory, EmuCPU* cpu)
{
	this->mem = memory;
	this->cpu = cpu;
}

EmuPPU::~EmuPPU()
{}



void EmuPPU::setMemory(EmuMemory * memory)
{
	this->mem = memory;
}



void EmuPPU::setCPU(EmuCPU* cpu)
{
	this->cpu = cpu;
}



/**
 * @brief Steps the PPU by a given number of cycles
 * @param cycles
 * @throws std::runtime_exception when memory or CPU pointer is null
 */
void EmuPPU::step(int cycles)
{
	if(mem == nullptr)
	{
		throw std::runtime_error("Cannot step PPU with NULL memory!");
	}
	if(cpu == nullptr)
	{
		throw std::runtime_error("Cannot step PPU with NULL CPU!");
	}

	for(int i = 0; i < cycles; i++)
	{
		switch(state)
		{
		case OAMSearch:
		{
			if(cycle >= 40)
			{
				state = PixelTransfer;
				logMessage(fmt::format(
					"Finished OAM search on line {}.",
					ly),
					LOG_DEBUG
				);
			}
			break;
		}

		case PixelTransfer:
		{
			lx++;
			if(lx >= 160)
			{
				lx = 0;
				state = HBlank;
				logMessage(fmt::format(
					"Finished pixel transfer on line {}.",
					ly),
					LOG_DEBUG
				);
			}
			break;
		}

		case HBlank:
		{
			if(cycle >= 456)
			{
				cycle = 0;
				ly = mem->readByte(0xFF44);
				ly++;
				mem->writeByte(0xFF44, ly);

				logMessage(fmt::format(
					"Finished HBlank on line {}.",
					ly),
					LOG_DEBUG
				);

				if(ly >= 144)
				{
					cpu->sendInterrupt(0);
					state = VBlank;
				} else
				{
					state = OAMSearch;
				}
			}

			break;
		}

		case VBlank:
		{
			// 10 lines of VBlank
			if(cycle >= 456)
			{
				cycle = 0;
				ly = mem->readByte(0xFF44);
				ly++;
				mem->writeByte(0xFF44, ly);

				if(ly >= 153)
				{
					mem->writeByte(0xFF44, 0);
					logMessage("Finished VBlank.", LOG_DEBUG);
					state = OAMSearch;
				}
			}
		}
		}

		cycle++;
	}
}
