/**
 * @file emu/emucpu.cpp
 * @brief Implements the system's CPU
 * @author ImpendingMoon
 * @date 2023-07-20
 */

#pragma once

#include <cstdint>
#include "emuregisters.hpp"

class EmuCPU
{
public:
	EmuCPU();
	~EmuCPU();

private:
	RegisterSet regs;
};