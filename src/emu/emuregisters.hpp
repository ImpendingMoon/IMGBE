/**
 * @file emu/emuregisters.hpp
 * @brief Implements the system's registers
 * @author ImpendingMoon
 * @date 2023-10-05
 */

#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>

// TODO: Create a single structure for address/value pairs instead of this mess.

 // Only works on little-endian systems. Sorry, PowerPC users.
class RegisterSet
{
public:
    RegisterSet();
    ~RegisterSet();

    /**
     * @brief Converts an address to its corresponding register, or nullptr.
     * @param address Emulated memory address
     * @return a pointer to a register, or a nullptr
     */
    uint8_t* getRegisterPtr(uint16_t address) const noexcept;

    /**
     * @brief Returns a string representation of the CPU registers.
     */
    std::string cpuToString(void);

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
        uint16_t pc = 0x0100;
        uint16_t sp = 0xFFFE;

        // The evil stuff
        // Relies on extensions and little-endian encoding. Sorry, PowerPC.
        union
        {
            uint16_t af = 0x1180;
            struct { uint8_t f, a; };
        };

        union
        {
            uint16_t bc = 0x0000;
            struct { uint8_t c, b; };
        };

        union
        {
            uint16_t de = 0x0008;
            struct { uint8_t e, d; };
        };

        union
        {
            uint16_t hl = 0x007C;
            struct { uint8_t l, h; };
        };
    } cpu{};

    struct // mem
    {
        struct // io
        {
            uint8_t iflag = 0xE1;
            uint8_t ienable = 0x00;
            uint8_t joyp = 0xC7;
            uint8_t sb = 0x00;
            uint8_t sc = 0x7F;
            uint8_t div = 0x00;
            uint8_t tima = 0x00;
            uint8_t tma = 0x00;
            uint8_t tac = 0xF8;
            uint8_t boot = 0x01;
        } io{};

        struct // video
        {
            uint8_t lcdc = 0x91;
            uint8_t stat = 0x00;
            uint8_t scy = 0x00;
            uint8_t scx = 0x00;
            uint8_t ly = 0x00;
            uint8_t lyc = 0x00;
            uint8_t bgp = 0xFC;
            uint8_t obp0 = 0x00;
            uint8_t obp1 = 0x00;
            uint8_t wy = 0x00;
            uint8_t wx = 0x00;
            uint8_t vbk = 0xFF;
            uint8_t key1 = 0xFF;
            uint8_t rp = 0xFF;
            uint8_t svbk = 0xFF;
            uint8_t bcps = 0x00;
            uint8_t bcpd = 0x00;
            uint8_t ocps = 0x00;
            uint8_t ocpd = 0x00;
            uint8_t hdma1 = 0xFF;
            uint8_t hdma2 = 0xFF;
            uint8_t hdma3 = 0xFF;
            uint8_t hdma4 = 0xFF;
            uint8_t hdma5 = 0xFF;

        } video{};

        struct // sound
        {
            uint8_t nr10 = 0x80;
            uint8_t nr11 = 0xBF;
            uint8_t nr12 = 0xF3;
            uint8_t nr13 = 0xFF;
            uint8_t nr14 = 0xBF;
            uint8_t nr21 = 0x3F;
            uint8_t nr22 = 0x00;
            uint8_t nr23 = 0xFF;
            uint8_t nr24 = 0xBF;
            uint8_t nr30 = 0x7F;
            uint8_t nr31 = 0xFF;
            uint8_t nr32 = 0x9F;
            uint8_t nr33 = 0xFF;
            uint8_t nr34 = 0xBF;
            uint8_t nr41 = 0xFF;
            uint8_t nr42 = 0x00;
            uint8_t nr43 = 0x00;
            uint8_t nr44 = 0xBF;
            uint8_t nr50 = 0x77;
            uint8_t nr51 = 0xF3;
            uint8_t nr52 = 0xF1;
            uint8_t wave[16] = {};
        } sound{};

    } mem{};

    // Write-Only and used by the CPU directly.
    uint8_t imaster = 0;

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
        { 0xFF00, &mem.io.joyp },
        { 0xFF01, &mem.io.sb },
        { 0xFF02, &mem.io.sc },
        { 0xFF04, &mem.io.div },
        { 0xFF05, &mem.io.tima },
        { 0xFF06, &mem.io.tma },
        { 0xFF07, &mem.io.tac },
        { 0xFF0F, &mem.io.iflag },
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
        { 0xFF30, &mem.sound.wave[0] },
        { 0xFF31, &mem.sound.wave[1] },
        { 0xFF32, &mem.sound.wave[2] },
        { 0xFF33, &mem.sound.wave[3] },
        { 0xFF34, &mem.sound.wave[4] },
        { 0xFF35, &mem.sound.wave[5] },
        { 0xFF36, &mem.sound.wave[6] },
        { 0xFF37, &mem.sound.wave[7] },
        { 0xFF38, &mem.sound.wave[8] },
        { 0xFF39, &mem.sound.wave[9] },
        { 0xFF3A, &mem.sound.wave[10] },
        { 0xFF3B, &mem.sound.wave[11] },
        { 0xFF3C, &mem.sound.wave[12] },
        { 0xFF3D, &mem.sound.wave[13] },
        { 0xFF3E, &mem.sound.wave[14] },
        { 0xFF3F, &mem.sound.wave[15] },
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
        { 0xFF4D, &mem.video.key1 },
        { 0xFF4F, &mem.video.vbk },
        { 0xFF50, &mem.io.boot },
        { 0xFF51, &mem.video.hdma1 },
        { 0xFF52, &mem.video.hdma2 },
        { 0xFF53, &mem.video.hdma3 },
        { 0xFF54, &mem.video.hdma4 },
        { 0xFF55, &mem.video.hdma5 },
        { 0xFF56, &mem.video.rp },
        { 0xFF68, &mem.video.bcps },
        { 0xFF69, &mem.video.bcpd },
        { 0xFF6A, &mem.video.ocps },
        { 0xFF6B, &mem.video.ocpd },
        { 0xFF70, &mem.video.svbk },
        { 0xFFFF, &mem.io.ienable },
    };

    static constexpr int ZERO_POS = 7;
    static constexpr int SUB_POS = 6;
    static constexpr int HALF_CARRY_POS = 5;
    static constexpr int CARRY_POS = 4;
};
