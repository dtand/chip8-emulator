#include "chip8.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <opcode.h>

/**
 * @brief Constructs a Chip8 instance and initializes the emulator state.
 */
Chip8::Chip8() {
    initialize();
}

/**
 * @brief Initializes the CHIP-8 system state.
 *
 * Sets up memory, registers, display, stack, timers, and loads the fontset.
 */
void Chip8::initialize() {
    pc = 0x200; // Program counter starts at 0x200
    opcode = 0;
    I = 0;
    sp = 0;

    // Clear display
    gfx.fill(0);
    drawFlag = false;

    // Clear stack, registers, and memory
    stack.fill(0);
    V.fill(0);
    memory.fill(0);

    // Reset timers
    delay_timer = 0;
    sound_timer = 0;

    // Initialize fontset
    uint8_t chip8_fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // Chip8 standard loads fontset into memory starting at 0x00
    for(int i = 0; i < 80; ++i) {
        memory[i] = chip8_fontset[i];
    }
}

/**
 * @brief Loads a CHIP-8 ROM into memory.
 *
 * Loads the ROM file contents into memory starting at 0x200.
 * Prints status messages on success or failure.
 *
 * @param filename Path to the ROM file.
 */
void Chip8::loadRom(const char* filename) {
    
    std::ifstream rom(filename, std::ios::binary | std::ios::ate);
    
    if (!rom.is_open()) {
        std::cerr << "Failed to open ROM: " << filename << std::endl;
        return;
    }

    std::streamsize size = rom.tellg();
    rom.seekg(0, std::ios::beg);


    std::vector<char> buffer(size);

    if (rom.read(buffer.data(), size)) {
        for(size_t i = 0; i < buffer.size(); ++i) {
            memory[0x200 + i] = static_cast<uint8_t>(buffer[i]);
        }
        std::cout << "Loaded ROM: " << filename << " (" << size << " bytes)" << std::endl;
    } else {
        std::cerr << "Failed to read ROM data." << std::endl;
    }

    rom.close();
}

/**
 * @brief Executes one emulation cycle.
 *
 * Fetches, decodes, and executes the next opcode. Updates timers and handles sound.
 */
void Chip8::emulateCycle() {
    // Fetch Opcode
    opcode = memory[pc] << 8 | memory[pc + 1];
    
    // Decode and Execute Opcode
    OpcodeHandler::dispatchOpcode(*this, opcode);

    // Update timers
    if (delay_timer > 0) {
        --delay_timer;
    }
    if (sound_timer > 0) {
        --sound_timer;
        if (sound_timer == 1) {
            std::cout << "BEEP!" << std::endl; // Placeholder for sound
        }
    }
}