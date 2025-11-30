#pragma once
#include <fstream>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <filesystem>
#include "message_queue.h"
#include "event.h"

const bool ENABLE_EVENT_LOGGING = true;

/**
 * @class EventLogger
 * @brief Singleton event logging system for CHIP-8 emulator.
 *
 * Collects, batches, and serializes events from the emulator using a thread-safe
 * message queue. Periodically writes events to a log file in JSON format.
 */
class EventLogger {
public:
    EventLogger(const EventLogger&) = delete;
    EventLogger& operator=(const EventLogger&) = delete;

    /**
     * @brief Creates the singleton EventLogger instance and its internal MessageQueue.
     *
     * @param logDir Directory for log files.
     * @param intervalMs Logging interval in milliseconds.
     * @param batchSize Number of events to log per interval.
     * @return Reference to the singleton EventLogger.
     */
    static EventLogger& createInstance(const std::string& logDir = "./logs",
                                      int intervalMs = 100,
                                      size_t batchSize = 10)
    {
        static EventLogger instance(logDir, intervalMs, batchSize);
        return instance;
    }

    /**
     * @brief Pushes an event to the logger's internal message queue.
     * @param event The event to log.
     */
    static void pushLog(const EventVariant& event) {
        EventLogger& logger = createInstance();
        logger.queue_.push(event);
    }

    ~EventLogger() {
        running_ = false;
        if (worker_.joinable()) worker_.join();
        logFile_.close();
    }

private:
    EventLogger(const std::string& logDir, int intervalMs, size_t batchSize)
        : intervalMs_(intervalMs), batchSize_(batchSize), running_(true)
    {
        std::filesystem::create_directories(logDir);
        std::string newLogFile = logDir + "/event_log_" + std::to_string(std::time(nullptr)) + ".txt";
        logFile_.open(newLogFile, std::ios::out | std::ios::trunc);
        worker_ = std::thread(&EventLogger::run, this);
    }

    void run() {
        while (running_ && ENABLE_EVENT_LOGGING) {
            std::vector<EventVariant> events;
            for (size_t i = 0; i < batchSize_; ++i) {
                EventVariant event = StackEvent(0, 0, {});
                if (queue_.try_pop(event)) {
                    events.push_back(event);
                } else {
                    break;
                }
            }
            for (const auto& ev : events) {
                logFile_ << serializeEvent(ev) << std::endl;
            }
            logFile_.flush();
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs_));
        }
    }

    MessageQueue<EventVariant> queue_;
    std::ofstream logFile_;
    std::thread worker_;
    std::atomic<bool> running_;
    int intervalMs_;
    size_t batchSize_;
};