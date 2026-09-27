#pragma once

#include <string>
#include <chrono>
#include <thread>
#include <conio.h> // for _kbhit() and _getch() on Windows
#include "ConsoleUI.hpp"

/**
 * @brief KeyboardInput provides non-blocking keyboard polling and input buffering.
 */
class KeyboardInput {
public:
    /**
     * @brief Polls a single keypress from the console input buffer.
     * @param input_buffer Accumulated string of characters for the current command.
     * @return true if a complete line has been entered (Enter pressed), false otherwise.
     */
    static bool poll_char(std::string& input_buffer) {
        if (!_kbhit()) {
            return false;
        }

        int key = _getch();

        // Handle extended/special keys (arrows, function keys, etc.) which come as 2-byte sequences
        if (key == 0 || key == 224) {
            _getch(); // discard the second byte
            return false;
        }

        // Enter key: signals command completion
        if (key == '\r') {
            ConsoleUI::print("\n");
            return true;
        }

        // Backspace key: erase last character from buffer and terminal
        if (key == '\b') {
            if (!input_buffer.empty()) {
                input_buffer.pop_back();
                ConsoleUI::print("\b \b");
            }
            return false;
        }

        // Printable character: append to buffer and echo to console
        if (key >= 32 && key <= 126) {
            input_buffer += static_cast<char>(key);
            std::string echo(1, static_cast<char>(key));
            ConsoleUI::print(echo);
        }

        return false;
    }

    /**
     * @brief Drains all available keystrokes in the buffer.
     * Handles fast typing and pasting with zero artificial delay, while
     * sleeping 10ms only when idle to keep CPU usage < 0.1%.
     * @param input_buffer Accumulated string of characters.
     * @return true if Enter was pressed and a line is ready to process.
     */
    static bool poll_line(std::string& input_buffer) {
        bool line_ready = false;
        while (_kbhit()) {
            if (poll_char(input_buffer)) {
                line_ready = true;
                break;
            }
        }

        if (!line_ready) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            return false;
        }

        return true;
    }
};
