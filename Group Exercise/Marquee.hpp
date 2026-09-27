#pragma once

#include <string>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include "ConsoleUI.hpp"

/**
 * @brief Marquee manages the marquee animation state, background worker thread,
 * and circular text scrolling logic.
 */
class Marquee {
private:
    int box_width = 70;
    std::string saved_text;
    std::atomic<int> speed_ms{100};              // Default refresh speed in ms
    std::atomic<bool> marquee_running{false};     // Thread running flag
    std::thread marquee_thread;

    std::mutex text_mutex;                       // Synchronizes read/write of saved_text
    std::condition_variable cv_anim;             // Allows 0ms latency interrupts on stop/speed change
    std::mutex cv_mutex;

    // Worker thread loop for animating the marquee
    void animation_loop() {
        int offset = 0;
        std::string text;

        while (marquee_running.load()) {
            {
                std::lock_guard<std::mutex> lock(text_mutex);
                if (static_cast<int>(saved_text.length()) < box_width) {
                    // Pad with spaces to allow short strings to travel across the full width
                    text = saved_text + std::string(box_width - saved_text.length() + 3, ' ');
                } else {
                    text = saved_text + "    ";
                }
            }

            if (text.empty()) {
                text = " ";
            }

            offset = offset % text.length();
            std::string frame = text.substr(offset) + text.substr(0, offset);
            frame = frame.substr(0, box_width); // limit to box_width columns
            offset = (offset + 1) % text.length();

            ConsoleUI::print_marquee(frame);

            // Responsive condition-variable sleep:
            // Sleeps for the exact speed_ms duration without CPU polling,
            // but wakes up instantly when stop() or set_speed() is called.
            int target_speed = speed_ms.load();
            if (target_speed <= 0) target_speed = 10;

            std::unique_lock<std::mutex> lock(cv_mutex);
            cv_anim.wait_for(lock, std::chrono::milliseconds(target_speed), [&]() {
                return !marquee_running.load();
            });
        }
    }

public:
    explicit Marquee(int width = 70) : box_width(width) {}

    ~Marquee() {
        stop();
    }

    bool is_running() const {
        return marquee_running.load();
    }

    bool has_text() {
        std::lock_guard<std::mutex> lock(text_mutex);
        return !saved_text.empty();
    }

    void set_text(const std::string& text) {
        std::lock_guard<std::mutex> lock(text_mutex);
        saved_text = text;
    }

    std::string get_text() {
        std::lock_guard<std::mutex> lock(text_mutex);
        return saved_text;
    }

    void set_speed(int ms) {
        speed_ms.store(ms);
        cv_anim.notify_all(); // Instantly apply new speed without waiting for previous timer
    }

    int get_speed() const {
        return speed_ms.load();
    }

    bool start() {
        if (!has_text()) {
            return false;
        }
        if (marquee_running.load()) {
            return false; // Already running
        }

        marquee_running.store(true);
        marquee_thread = std::thread(&Marquee::animation_loop, this);
        return true;
    }

    bool stop() {
        if (!marquee_running.load()) {
            return false; // Not running
        }

        marquee_running.store(false);
        cv_anim.notify_all(); // Instantly break out of any active sleep

        if (marquee_thread.joinable()) {
            marquee_thread.join();
        }

        ConsoleUI::clear_marquee_line();
        return true;
    }
};
