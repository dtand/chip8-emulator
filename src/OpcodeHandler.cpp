#include "opcode.h"
#include <iostream>

/**
 * @brief Handles 0x0--- opcodes.
 *
 * Implements:
 *   - 0x00E0: CLS - Clear the display.
 *   - 0x00EE: RET - Return from a subroutine.
 *
 * @param chip8 Reference to the Chip8 instance.
 * @param opcode The 16-bit opcode value.
 */
void OpcodeHandler::handle_0x0(Chip8& chip8, uint16_t opcode) {
    switch(opcode & 0x00FF) {
        /* CLS */
        case 0x00E0: 
            chip8.gfx.fill(0);
            chip8.drawFlag = true;
            chip8.pc += 2;
            break;
        /* RET */
        case 0x00EE: 
            chip8.sp--;
            chip8.pc = chip8.stack[chip8.sp];
            chip8.pc += 2;
            break;
        /* Unknown opcode */
        default:
            std::cerr << "Unknown opcode [0x0000]: " << std::hex << opcode << std::endl;
            chip8.pc += 2;
            break;
    }
    
}

/**
 * @brief Handles 0x1NNN opcode.
 *
 * Implements:
 *   - 0x1NNN: JP addr - Jump to address NNN.
 *
 * @param chip8 Reference to the Chip8 instance.
 * @param opcode The 16-bit opcode value.
 */
void OpcodeHandler::handle_0x1(Chip8& chip8, uint16_t opcode) {
    /* JP addr */
    chip8.pc = opcode & 0x0FFF;
}

/**
 * @brief Handles 0x2NNN opcode.
 *
 * Implements:
 *   - 0x2NNN: CALL addr - Call subroutine at address NNN.
 *
 * @param chip8 Reference to the Chip8 instance.
 * @param opcode The 16-bit opcode value.
 */
void OpcodeHandler::handle_0x2(Chip8& chip8, uint16_t opcode) {
    /* CALL addr */
    chip8.stack[chip8.sp] = chip8.pc;
    chip8.sp++;
    chip8.pc = opcode & 0x0FFF;
}

/**
 * @brief Handles 0x3XNN opcode.
 *
 * Implements:
 *   - 0x3XNN: SE Vx, NN - Skip next instruction if Vx == NN.
 *
 * @param chip8 Reference to the Chip8 instance.
 * @param opcode The 16-bit opcode value.
 */
void OpcodeHandler::handle_0x3(Chip8& chip8, uint16_t opcode) {
    /* SE Vx, byte */
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t byte = opcode & 0x00FF;
    if (chip8.V[Vx] == byte) {
        chip8.pc += 4;
    } else {
        chip8.pc += 2;
    }
}

/**
 * @brief Handles 0x4XNN opcode.
 *
 * Implements:
 *   - 0x4XNN: SNE Vx, NN - Skip next instruction if Vx != NN.
 *
 * @param chip8 Reference to the Chip8 instance.
 * @param opcode The 16-bit opcode value.
 */
void OpcodeHandler::handle_0x4(Chip8& chip8, uint16_t opcode) {
    /* SNE Vx, byte */
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t byte = opcode & 0x00FF;
    if (chip8.V[Vx] != byte) {
        chip8.pc += 4;
    } else {
        chip8.pc += 2;
    }
}

/**
 * @brief Handles 0x5XY0 opcode.
 *
 * Implements:
 *   - 0x5XY0: SE Vx, Vy - Skip next instruction if Vx == Vy.
 *
 * @param chip8 Reference to the Chip8 instance.
 * @param opcode The 16-bit opcode value.
 */
void OpcodeHandler::handle_0x5(Chip8& chip8, uint16_t opcode) {
    /* SE Vx, Vy */
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;
    if (chip8.V[Vx] == chip8.V[Vy]) {
        chip8.pc += 4;
    } else {
        chip8.pc += 2;
    }
}

/**
 * @brief Handles 0x6XNN opcode.
 *
 * Implements:
 *   - 0x6XNN: LD Vx, NN - Set Vx = NN.
 *
 * @param chip8 Reference to the Chip8 instance.
 * @param opcode The 16-bit opcode value.
 */
void OpcodeHandler::handle_0x6(Chip8& chip8, uint16_t opcode) {
    /* LD Vx, byte */
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t byte = opcode & 0x00FF;
    chip8.V[Vx] = byte;
    chip8.pc += 2;
}

/**
 * @brief Handles 0x7XNN opcode.
 *
 * Implements:
 *   - 0x7XNN: ADD Vx, NN - Set Vx = Vx + NN.
 *
 * @param chip8 Reference to the Chip8 instance.
 * @param opcode The 16-bit opcode value.
 */
void OpcodeHandler::handle_0x7(Chip8& chip8, uint16_t opcode) {
    /* ADD Vx, byte */
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t byte = opcode & 0x00FF;
    chip8.V[Vx] += byte;
    chip8.pc += 2;
}

/**
 * @brief Handles 0x8XY_ opcodes.
 *
 * Implements:
 *   - 0x8XY0: LD Vx, Vy - Set Vx = Vy.
 *   - 0x8XY1: OR Vx, Vy - Set Vx = Vx OR Vy.
 *   - 0x8XY2: AND Vx, Vy - Set Vx = Vx AND Vy.
 *   - 0x8XY3: XOR Vx, Vy - Set Vx = Vx XOR Vy.
 *   - 0x8XY4: ADD Vx, Vy - Set Vx = Vx + Vy, set VF = carry.
 *   - 0x8XY5: SUB Vx, Vy - Set Vx = Vx - Vy, set VF = NOT borrow.
 *   - 0x8XY6: SHR Vx - Set Vx = Vx >> 1, set VF = least significant bit of Vx.
 *   - 0x8XY7: SUBN Vx, Vy - Set Vx = Vy - Vx, set VF = NOT borrow.
 *   - 0x8XYE: SHL Vx - Set Vx = Vx << 1, set VF = most significant bit of Vx.
 *
 * @param chip8 Reference to the Chip8 instance.
 * @param opcode The 16-bit opcode value.
 */
void OpcodeHandler::handle_0x8(Chip8& chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    switch (opcode & 0x000F) {
        case 0x0000: { /* 8XY0: LD Vx, Vy */
            chip8.V[x] = chip8.V[y];
            break;
        }
        case 0x0001: { /* 8XY1: OR Vx, Vy */
            chip8.V[x] |= chip8.V[y];
            break;
        }
        case 0x0002: { /* 8XY2: AND Vx, Vy */
            chip8.V[x] &= chip8.V[y];
            break;
        }
        case 0x0003: { /* 8XY3: XOR Vx, Vy */
            chip8.V[x] ^= chip8.V[y];
            break;
        }
        case 0x0004: { /* 8XY4: ADD Vx, Vy */
            uint16_t sum = chip8.V[x] + chip8.V[y];
            chip8.V[0xF] = (sum > 255) ? 1 : 0; // Set carry flag
            chip8.V[x] = sum & 0xFF;
            break;
        }
        case 0x0005: { /* 8XY5: SUB Vx, Vy */
            chip8.V[0xF] = (chip8.V[x] > chip8.V[y]) ? 1 : 0; // Set borrow flag
            chip8.V[x] -= chip8.V[y];
            break;
        }
        case 0x0006: { /* 8XY6: SHR Vx {, Vy} */
            chip8.V[0xF] = chip8.V[x] & 0x1; // Store least significant bit
            chip8.V[x] >>= 1;
            break;
        }
        case 0x0007: { /* 8XY7: SUBN Vx, Vy */
            chip8.V[0xF] = (chip8.V[y] > chip8.V[x]) ? 1 : 0; // Set borrow flag
            chip8.V[x] = chip8.V[y] - chip8.V[x];
            break;
        }
        case 0x000E: { /* 8XYE: SHL Vx {, Vy} */
            chip8.V[0xF] = (chip8.V[x] & 0x80) >> 7; // Store most significant bit
            chip8.V[x] <<= 1;
            break;
        }
        default: {
            std::cerr << "Unknown opcode [0x8000]: " << std::hex << opcode << std::endl;
            break;
        }
    }
    chip8.pc += 2;
}

/**
 * @brief Handles 0x9XY0 opcode.
 *
 * Implements:
 *   - 0x9XY0: SNE Vx, Vy - Skip next instruction if Vx != Vy.
 *
 * @param chip8 Reference to the Chip8 instance.
 * @param opcode The 16-bit opcode value.
 */
void OpcodeHandler::handle_0x9(Chip8& chip8, uint16_t opcode) {
    /* SNE Vx, Vy */
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;
    if (chip8.V[Vx] != chip8.V[Vy]) {
        chip8.pc += 4;
    } else {
        chip8.pc += 2;
    }
}

/**
 * @brief Handles 0xANNN opcode.
 *
 * Implements:
 *   - 0xANNN: LD I, addr - Set I = NNN.
 *
 * @param chip8 Reference to the Chip8 instance.
 * @param opcode The 16-bit opcode value.
 */
void OpcodeHandler::handle_0xA(Chip8& chip8, uint16_t opcode) {
    /* LD I, addr */
    chip8.I = opcode & 0x0FFF;
    chip8.pc += 2;
}

/**
 * @brief Handles 0xBNNN opcode.
 *
 * Implements:
 *   - 0xBNNN: JP V0, addr - Jump to address NNN + V0.
 *
 * @param chip8 Reference to the Chip8 instance.
 * @param opcode The 16-bit opcode value.
 */
void OpcodeHandler::handle_0xB(Chip8& chip8, uint16_t opcode) {
    /* JP V0, addr */
    chip8.pc = (opcode & 0x0FFF) + chip8.V[0];
}

/**
 * @brief Handles 0xCXNN opcode.
 *
 * Implements:
 *   - 0xCXNN: RND Vx, byte - Set Vx = random byte AND NN.
 *
 * @param chip8 Reference to the Chip8 instance.
 * @param opcode The 16-bit opcode value.
 */
void OpcodeHandler::handle_0xC(Chip8& chip8, uint16_t opcode) {
    /* RND Vx, byte */
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t byte = opcode & 0x00FF;
    uint8_t randByte = rand() % 256; // Generate random byte
    chip8.V[Vx] = randByte & byte;
    chip8.pc += 2;

}

/**
* @brief Handles 0xDXYN opcode.
* Implements:
*   - 0xDXYN: DRW Vx, Vy, nibble - Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
* @param chip8 Reference to the Chip8 instance.
* @param opcode The 16-bit opcode value.
*/
void OpcodeHandler::handle_0xD(Chip8& chip8, uint16_t opcode) {
    /* DRW Vx, Vy, nibble */
    uint8_t x = chip8.V[(opcode & 0x0F00) >> 8];
    uint8_t y = chip8.V[(opcode & 0x00F0) >> 4];
    uint8_t height = opcode & 0x000F;   
    chip8.V[0x0F] = 0;
    for (int row = 0; row < height; ++row) {
        uint8_t spriteByte = chip8.memory[chip8.I + row];
        for (int col = 0; col < 8; ++col) {
            if ((spriteByte & (0x80 >> col)) != 0) {
                uint16_t gfxIndex = (x + col + ((y + row) * 64)) % 2048;
                if (chip8.gfx[gfxIndex] == 1) {
                    chip8.V[0x0F] = 1; // Collision detected
                }
                chip8.gfx[gfxIndex] ^= 1;
            }
        }
    }
    chip8.drawFlag = true;
    chip8.pc += 2;
}

/**
 * @brief Handles 0xEXNN opcodes.
 *
 * Implements:
 *   - 0xEX9E: SKP Vx - Skip next instruction if key with the value of Vx is pressed.
 *   - 0xEXA1: SKNP Vx - Skip next instruction if key with the value of Vx is not pressed.
 *
 * @param chip8 Reference to the Chip8 instance.
 * @param opcode The 16-bit opcode value.
 */
void OpcodeHandler::handle_0xE(Chip8& chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    switch (opcode & 0x00FF) {
        case 0x009E: /* EX9E: SKP Vx */
            if (chip8.key[chip8.V[x]] != 0) {
                chip8.pc += 4;
            } else {
                chip8.pc += 2;
            }
            break;
        case 0x00A1: /* EXA1: SKNP Vx */
            if (chip8.key[chip8.V[x]] == 0) {
                chip8.pc += 4;
            } else {
                chip8.pc += 2;
            }
            break;
        default:
            std::cerr << "Unknown opcode [0xE000]: " << std::hex << opcode << std::endl;
            chip8.pc += 2;
            break;
    }
}

/** 
* @brief Handles 0xFXNN opcodes.
*
* Implements:
*   - 0xFX07: LD Vx, DT - Set Vx = delay timer value.
*   - 0xFX0A: LD Vx, K - Wait for a key press, store the value of the key in Vx.
*   - 0xFX15: LD DT, Vx - Set delay timer = Vx.
*   - 0xFX18: LD ST, Vx - Set sound timer = Vx.
*   - 0xFX1E: ADD I, Vx - Set I = I + Vx.
*   - 0xFX29: LD F, Vx - Set I = location of sprite for digit Vx.
*   - 0xFX33: LD B, Vx - Store BCD representation of Vx in memory locations I, I+1, and I+2.
*   - 0xFX55: LD [I], Vx - Store registers V0 through Vx in memory starting at location I.
*   - 0xFX65: LD Vx, [I] - Read registers V0 through Vx from memory starting at location I.
*
* @param chip8 Reference to the Chip8 instance.
* @param opcode The 16-bit opcode value.
*/
void OpcodeHandler::handle_0xF(Chip8& chip8, uint16_t opcode) {

    uint8_t x = (opcode & 0x0F00) >> 8;

    switch(opcode & 0x00FF) {
        case 0x0007: { /* FX07: LD Vx, DT */
            chip8.V[x] = chip8.delay_timer;
            chip8.pc += 2;
            break;
        }
        case 0x000A: { /* FX0A: LD Vx, K */
            bool keyPressed = false;
            for (int i = 0; i < 16; ++i) {
                if (chip8.key[i] != 0) {
                    chip8.V[x] = i;
                    keyPressed = true;
                    break;
                }
            }
            if (!keyPressed) {
                return; // Skip incrementing pc to wait for a key press
            }
            chip8.pc += 2;
            break;
        }
        case 0x0015: { /* FX15: LD DT, Vx */
            chip8.delay_timer = chip8.V[x];
            chip8.pc += 2;
            break;
        }
        case 0x0018: { /* FX18: LD ST, Vx */
            chip8.sound_timer = chip8.V[x];
            chip8.pc += 2;
            break;
        }
        case 0x001E: { /* FX1E: ADD I, Vx */
            chip8.I += chip8.V[x];
            chip8.pc += 2;
            break;
        }
        case 0x0029: { /* FX29: LD F, Vx */
            chip8.I = chip8.V[x] * 5; // Each font character is 5 bytes
            chip8.pc += 2;
            break;
        }
        case 0x0033: { /* FX33: LD B, Vx */
            uint8_t value = chip8.V[x];
            chip8.memory[chip8.I]     = value / 100;
            chip8.memory[chip8.I + 1] = (value / 10) % 10;
            chip8.memory[chip8.I + 2] = value % 10;
            chip8.pc += 2;
            break;
        }
        case 0x0055: { /* FX55: LD [I], Vx */
            for (int i = 0; i <= x; ++i) {
                chip8.memory[chip8.I + i] = chip8.V[i];
            }
            chip8.pc += 2;
            break;
        }
        case 0x0065: { /* FX65: LD Vx, [I] */
            for (int i = 0; i <= x; ++i) {
                chip8.V[i] = chip8.memory[chip8.I + i];
            }
            chip8.pc += 2;
            break;
        }
        default: {
            std::cerr << "Unknown opcode [0xF000]: " << std::hex << opcode << std::endl;
            chip8.pc += 2;
            break;
        }
    }
}

/**
 * @brief Dispatches the given opcode to the appropriate handler based on its family.
 *
 * @param chip8 Reference to the Chip8 instance.
 * @param opcode The 16-bit opcode value.
 */
void OpcodeHandler::dispatchOpcode(Chip8& chip8, uint16_t opcode) {
    switch (opcode & 0xF000) {
        case 0x0000: handle_0x0(chip8, opcode); break;
        case 0x1000: handle_0x1(chip8, opcode); break;
        case 0x2000: handle_0x2(chip8, opcode); break;
        case 0x3000: handle_0x3(chip8, opcode); break;
        case 0x4000: handle_0x4(chip8, opcode); break;
        case 0x5000: handle_0x5(chip8, opcode); break;
        case 0x6000: handle_0x6(chip8, opcode); break;
        case 0x7000: handle_0x7(chip8, opcode); break;
        case 0x8000: handle_0x8(chip8, opcode); break;
        case 0x9000: handle_0x9(chip8, opcode); break;
        case 0xA000: handle_0xA(chip8, opcode); break;
        case 0xB000: handle_0xB(chip8, opcode); break;
        case 0xC000: handle_0xC(chip8, opcode); break;
        case 0xD000: handle_0xD(chip8, opcode); break;
        case 0xE000: handle_0xE(chip8, opcode); break;
        case 0xF000: handle_0xF(chip8, opcode); break;
        default:
            std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
            chip8.pc += 2;
            break;
    }
}