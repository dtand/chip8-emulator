#pragma once
#include "chip8.h"
#include <cstdint>

/**
 * @class OpcodeHandler
 * @brief Static class for handling CHIP-8 opcodes.
 *
 * Provides static methods to decode and execute CHIP-8 opcodes.
 * Each method corresponds to a specific opcode or group of opcodes.
 */
class OpcodeHandler {
public:
    static void handle_0x0(Chip8& chip8, uint16_t opcode);
    static void handle_0x1(Chip8& chip8, uint16_t opcode);
    static void handle_0x2(Chip8& chip8, uint16_t opcode);
    static void handle_0x3(Chip8& chip8, uint16_t opcode);
    static void handle_0x4(Chip8& chip8, uint16_t opcode);
    static void handle_0x5(Chip8& chip8, uint16_t opcode);
    static void handle_0x6(Chip8& chip8, uint16_t opcode);
    static void handle_0x7(Chip8& chip8, uint16_t opcode);
    static void handle_0x8(Chip8& chip8, uint16_t opcode);
    static void handle_0x9(Chip8& chip8, uint16_t opcode);
    static void handle_0xA(Chip8& chip8, uint16_t opcode);
    static void handle_0xB(Chip8& chip8, uint16_t opcode);
    static void handle_0xC(Chip8& chip8, uint16_t opcode);
    static void handle_0xD(Chip8& chip8, uint16_t opcode);
    static void handle_0xE(Chip8& chip8, uint16_t opcode);
    static void handle_0xF(Chip8& chip8, uint16_t opcode);
    static void dispatchOpcode(Chip8& chip8, uint16_t opcode);
};


