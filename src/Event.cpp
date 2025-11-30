#include "event.h"

/**
 * @brief Serializes an EventVariant to a JSON-like string.
 *
 * Converts the event data into a string representation suitable for logging.
 *
 * @param ev The event variant to serialize.
 * @return std::string The serialized event as a string.
 */
std::string serializeEvent(const EventVariant& ev) {
    std::ostringstream oss;

    std::visit([&oss](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        oss << "{ \"type\": \"" << arg.type << "\", ";
        // Serialize timestamp as milliseconds since epoch
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(arg.timestamp.time_since_epoch()).count();
        oss << "\"timestamp\": " << ms << ", ";

        if constexpr (std::is_same_v<T, StackEvent>) {
            oss << "\"pc\": " << arg.pc << ", \"target\": " << arg.target << ", \"stack\": [";
            for (size_t i = 0; i < arg.stack.size(); ++i) {
                oss << arg.stack[i];
                if (i + 1 < arg.stack.size()) oss << ", ";
            }
            oss << "]";
        } else if constexpr (std::is_same_v<T, OpcodeEvent>) {
            oss << "\"pc\": " << arg.pc << ", \"opcode\": " << arg.opcode;
        } else if constexpr (std::is_same_v<T, RegisterEvent>) {
            oss << "\"changes\": {";
            bool first = true;
            for (auto& [reg, val] : arg.changes) {
                if (!first) oss << ", ";
                oss << "\"V" << reg << "\": " << val;
                first = false;
            }
            oss << "}";
        } else if constexpr (std::is_same_v<T, MemoryEvent>) {
            oss << "\"memoryDiff\": {";
            bool first = true;
            for (auto& [addr, val] : arg.memoryDiff) {
                if (!first) oss << ", ";
                oss << "\"0x" << std::hex << addr << "\": " << std::dec << val;
                first = false;
            }
            oss << "}";
        } else if constexpr (std::is_same_v<T, InputEvent>) {
            oss << "\"key\": " << arg.key << ", \"pressed\": " << (arg.pressed ? "true" : "false");
        }

        oss << " }";
    }, ev);

    return oss.str();
}