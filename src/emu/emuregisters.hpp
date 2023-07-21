/**
 * @file emu/emucpu.cpp
 * @brief Implements the system's registers
 * @author ImpendingMoon
 * @date 2023-07-20
 */

#pragma once

#include <cstdint>
#include <unordered_map>

 // Only works on little-endian systems. Sorry, PowerPC users.
class RegisterSet
{
public:
	RegisterSet();
	~RegisterSet();

	/**
	 * @brief Converts an address to its corresponding register, or nullptr.
	 * @param address Emulated memory address
	 * @returns a pointer to a register, or a nullptr
	 */
	uint8_t* getRegisterPtr(uint16_t address) const noexcept;

	/**
	 * @brief Updates the 'f' register with the values from the flag struct.
	 */
	void flagStructToRegister(void);

	/**
	 * @brief Updates the flag struct with values from the 'f' register.
	 */
	void flagRegisterToStruct(void);

	struct // cpu
	{
		// The good stuff
		uint16_t pc = 0;
		uint16_t sp = 0;

		// The evil stuff
		union
		{
			uint16_t af = 0;
			struct { uint8_t f, a; };
		};

		union
		{
			uint16_t bc = 0;
			struct { uint8_t c, b; };
		};

		union
		{
			uint16_t de = 0;
			struct { uint8_t e, d; };
		};

		union
		{
			uint16_t hl = 0;
			struct { uint8_t l, h; };
		};
	} cpu{}; // Visual Studio was complaining about uninitialized members.

	struct // mem
	{
		struct // io
		{
			// Not named 'if' because it's a keyword
			uint8_t iflag = 0;
			uint8_t ienable = 0;
			uint8_t joyp = 0;
			uint8_t sb = 0;
			uint8_t sc = 0;
			uint8_t div = 0;
			uint8_t tima = 0;
			uint8_t tma = 0;
			uint8_t tac = 0;
		} io{};

		struct // video
		{
			uint8_t lcdc = 0;
			uint8_t stat = 0;
			uint8_t scy = 0;
			uint8_t scx = 0;
			uint8_t ly = 0;
			uint8_t lyc = 0;
			uint8_t bgp = 0;
			uint8_t obp0 = 0;
			uint8_t obp1 = 0;
			uint8_t wy = 0;
			uint8_t wx = 0;
			uint8_t dma = 0;
		} video{};

		struct // sound
		{
			uint8_t nr10 = 0;
			uint8_t nr11 = 0;
			uint8_t nr12 = 0;
			uint8_t nr13 = 0;
			uint8_t nr14 = 0;
			uint8_t nr21 = 0;
			uint8_t nr22 = 0;
			uint8_t nr23 = 0;
			uint8_t nr24 = 0;
			uint8_t nr30 = 0;
			uint8_t nr31 = 0;
			uint8_t nr32 = 0;
			uint8_t nr33 = 0;
			uint8_t nr34 = 0;
			uint8_t nr41 = 0;
			uint8_t nr42 = 0;
			uint8_t nr43 = 0;
			uint8_t nr44 = 0;
			uint8_t nr50 = 0;
			uint8_t nr51 = 0;
			uint8_t nr52 = 0;
		} sound{};

	} mem{};

	// Boolean representation of the flags register
	struct
	{
		bool zero = 0;
		bool sub = 0;
		bool half_carry = 0;
		bool carry = 0;
	} flags{};

private:
	const std::unordered_map<uint16_t, uint8_t*> MEMORY_MAP =
	{
		// TODO: Handle multi-byte registers
		{ 0xFF0F, &mem.io.iflag },
		{ 0xFFFF, &mem.io.ienable },
		{ 0xFF00, &mem.io.joyp },
		{ 0xFF01, &mem.io.sb },
		{ 0xFF02, &mem.io.sc },
		{ 0xFF04, &mem.io.div },
		{ 0xFF05, &mem.io.tima },
		{ 0xFF06, &mem.io.tma },
		{ 0xFF07, &mem.io.tac },
		{ 0xFF40, &mem.video.lcdc },
		{ 0xFF41, &mem.video.stat },
		{ 0xFF42, &mem.video.scy },
		{ 0xFF43, &mem.video.scx },
		{ 0xFF44, &mem.video.ly },
		{ 0xFF45, &mem.video.lyc },
		{ 0xFF47, &mem.video.bgp },
		{ 0xFF48, &mem.video.obp0 },
		{ 0xFF49, &mem.video.obp1 },
		{ 0xFF4A, &mem.video.wy },
		{ 0xFF4B, &mem.video.wx },
		{ 0xFF46, &mem.video.dma },
		{ 0xFF10, &mem.sound.nr10 },
		{ 0xFF11, &mem.sound.nr11 },
		{ 0xFF12, &mem.sound.nr12 },
		{ 0xFF13, &mem.sound.nr13 },
		{ 0xFF14, &mem.sound.nr14 },
		{ 0xFF16, &mem.sound.nr21 },
		{ 0xFF17, &mem.sound.nr22 },
		{ 0xFF18, &mem.sound.nr23 },
		{ 0xFF19, &mem.sound.nr24 },
		{ 0xFF1A, &mem.sound.nr30 },
		{ 0xFF1B, &mem.sound.nr31 },
		{ 0xFF1C, &mem.sound.nr32 },
		{ 0xFF1D, &mem.sound.nr33 },
		{ 0xFF1E, &mem.sound.nr34 },
		{ 0xFF20, &mem.sound.nr41 },
		{ 0xFF21, &mem.sound.nr42 },
		{ 0xFF22, &mem.sound.nr43 },
		{ 0xFF23, &mem.sound.nr44 },
		{ 0xFF24, &mem.sound.nr50 },
		{ 0xFF25, &mem.sound.nr51 },
		{ 0xFF26, &mem.sound.nr52 },
	};

	static constexpr int ZERO_POS = 7;
	static constexpr int SUB_POS = 6;
	static constexpr int HALF_CARRY_POS = 5;
	static constexpr int CARRY_POS = 4;
};
