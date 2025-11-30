#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

/**
 * @class MessageQueue
 * @brief Thread-safe message queue for inter-thread communication.
 *
 * Provides blocking and non-blocking methods for pushing and popping messages.
 * Used for event transport in the CHIP-8 emulator logging system.
 *
 * @tparam T Type of message stored in the queue.
 */
template<typename T>
class MessageQueue {
public:
    // Push a message into the queue
    void push(const T& msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(msg);
        cond_.notify_one();
    }

    // Pop a message from the queue (blocks until available)
    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this]{ return !queue_.empty(); });
        T msg = queue_.front();
        queue_.pop();
        return msg;
    }

    // Try to pop a message (non-blocking)
    bool try_pop(T& msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) return false;
        msg = queue_.front();
        queue_.pop();
        return true;
    }

    // Check if the queue is empty
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
};