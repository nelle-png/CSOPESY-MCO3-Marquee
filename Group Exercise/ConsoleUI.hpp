#pragma once

#include <iostream>
#include <string>
#include <mutex>

#ifdef _WIN32
#include <windows.h>
#endif

/**
 * @brief ConsoleUI handles terminal layout, ANSI VT100 sequence processing,
 * cursor positioning, and thread-safe console output.
 */
class ConsoleUI {
private:
    static inline std::mutex console_mutex;

public:
    // Enables VT100 / ANSI escape processing on Windows cmd.exe / conhost
    // and sets OS timer resolution to 1ms for sub-10ms precision.
    static void enable_virtual_terminal() {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }

        // Dynamically request 1ms timer interrupt resolution via winmm.dll
        typedef UINT(WINAPI *PFN_timeBeginPeriod)(UINT);
        HMODULE hWinmm = GetModuleHandleA("winmm.dll");
        if (!hWinmm) hWinmm = LoadLibraryA("winmm.dll");
        if (hWinmm) {
            auto pfn = reinterpret_cast<PFN_timeBeginPeriod>(
                reinterpret_cast<uintptr_t>(GetProcAddress(hWinmm, "timeBeginPeriod")));
            if (pfn) pfn(1);
        }
#endif
    }

    // Restores default Windows timer resolution upon exit
    static void cleanup_virtual_terminal() {
#ifdef _WIN32
        typedef UINT(WINAPI *PFN_timeEndPeriod)(UINT);
        HMODULE hWinmm = GetModuleHandleA("winmm.dll");
        if (hWinmm) {
            auto pfn = reinterpret_cast<PFN_timeEndPeriod>(
                reinterpret_cast<uintptr_t>(GetProcAddress(hWinmm, "timeEndPeriod")));
            if (pfn) pfn(1);
        }
#endif
    }

    // Thread-safe output function for regular console messages and keystroke echoing
    static void print(const std::string& text) {
        std::lock_guard<std::mutex> lock(console_mutex);
        std::cout << text << std::flush;
    }

    // Thread-safe, anti-flicker output for the marquee line at row 2
    // Hides the cursor during redraw to prevent screen tearing and visible cursor jumping
    static void print_marquee(const std::string& text) {
        std::lock_guard<std::mutex> lock(console_mutex);
        // \033[?25l = hide cursor
        // \0337     = save cursor position
        // \033[2;1H = move cursor to row 2, col 1
        // \033[K    = clear from cursor to end of line
        // \0338     = restore cursor position
        // \033[?25h = show cursor
        std::cout << "\033[?25l\0337\033[2;1H" << text << "\033[K\0338\033[?25h" << std::flush;
    }

    // Initializes the console: clears screen, draws top/bottom borders (rows 1 & 3),
    // and locks scrolling to row 4 downwards.
    static void setup_screen(int box_width = 70, int scroll_start_row = 4) {
        std::lock_guard<std::mutex> lock(console_mutex);
        std::string border(box_width, '=');
        // \033[2J           = clear entire screen
        // \033[<row>r       = set DECSTBM scrolling region (from scroll_start_row to bottom)
        // \033[1;1H         = move cursor to row 1, col 1
        // \033[3;1H         = move cursor to row 3, col 1
        // \033[<row>;1H     = move cursor into scrolling region
        std::cout << "\033[2J" 
                  << "\033[" + std::to_string(scroll_start_row) + "r"
                  << "\033[1;1H" << border
                  << "\033[3;1H" << border
                  << "\033[" + std::to_string(scroll_start_row) + ";1H"
                  << std::flush;
    }

    // Resets the scrolling region to the full window and clears the marquee line
    static void reset_screen() {
        std::lock_guard<std::mutex> lock(console_mutex);
        std::cout << "\033[?25l\0337\033[2;1H\033[K\033[r\0338\033[?25h" << std::flush;
    }
};
