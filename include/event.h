#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <sstream>
#include <chrono>


/**
 * @struct Event
 * @brief Base event type for CHIP-8 event logging.
 *
 * Contains event type and timestamp. All specific event types inherit from this.
 */
struct Event {
    std::string type;
    std::chrono::time_point<std::chrono::steady_clock> timestamp;

    Event(const std::string& t)
        : type(t), timestamp(std::chrono::steady_clock::now()) {}
    virtual ~Event() = default;
};


/**
 * @struct StackEvent
 * @brief Event representing changes to the CHIP-8 stack (CALL/RET).
 *
 * Contains program counter, target address, and full stack snapshot.
 */
struct StackEvent : Event {
    uint16_t pc;
    uint16_t target;
    std::vector<uint16_t> stack;

    StackEvent(uint16_t pc_, uint16_t target_, std::vector<uint16_t> stack_)
        : Event("StackEvent"), pc(pc_), target(target_), stack(std::move(stack_)) {}
};

/**
 * @struct OpcodeEvent
 * @brief Event representing execution of a CHIP-8 opcode.
 *
 * Contains program counter and opcode value.
 */
struct OpcodeEvent : Event {
    uint16_t pc;
    uint16_t opcode;

    OpcodeEvent(uint16_t pc_, uint16_t opcode_)
        : Event("OpcodeEvent"), pc(pc_), opcode(opcode_) {}
};

/**
 * @struct RegisterEvent
 * @brief Event representing changes to CHIP-8 general purpose registers (V0-VF).
 *
 * Contains a map of register indices to their new values.
 */
struct RegisterEvent : Event {
    std::map<int, int> changes;

    RegisterEvent(std::map<int, int> changes_)
        : Event("RegisterEvent"), changes(std::move(changes_)) {}
};

/**
 * @struct MemoryEvent
 * @brief Event representing changes to CHIP-8 memory regions.
 *
 * Contains a map of memory addresses to their new values.
 */
struct MemoryEvent : Event {
    std::map<uint16_t, int> memoryDiff;

    MemoryEvent(std::map<uint16_t, int> diff_)
        : Event("MemoryEvent"), memoryDiff(std::move(diff_)) {}
};

/**
 * @struct InputEvent
 * @brief Event representing key input (press/release) in CHIP-8.
 *
 * Contains key index and pressed state.
 */
struct InputEvent : Event {
    int key;
    bool pressed;

    InputEvent(int key_, bool pressed_)
        : Event("InputEvent"), key(key_), pressed(pressed_) {}
};


using EventVariant = std::variant<StackEvent, OpcodeEvent, RegisterEvent, MemoryEvent, InputEvent>;

std::string serializeEvent(const EventVariant& ev);