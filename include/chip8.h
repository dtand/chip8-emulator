
/**
 * @class Chip8
 * @brief CHIP-8 virtual machine core implementation.
 *
 * Emulates the CHIP-8 system, including memory, registers, stack, timers,
 * graphics buffer, keypad state, and opcode execution. Provides methods for
 * loading ROMs, running emulation cycles, and managing system state.
 */
#pragma once
#include <array>

class Chip8 {

    friend class OpcodeHandler;

    public:
        Chip8();
        void loadRom(const char* filename);
        void emulateCycle();
        bool drawFlag;
        std::array<uint8_t, 2048> gfx;
        std::array<uint8_t, 16> key;
    
    
    private:
        std::array<uint8_t, 4096> memory;
        std::array<uint8_t, 16> V;
        std::array<uint16_t, 16> stack;

        uint16_t I;
        uint16_t pc;
        uint16_t sp;
        uint8_t delay_timer;
        uint8_t sound_timer;
        uint16_t opcode;

        void initialize();
};