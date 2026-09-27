#include <iostream>
#include <string>
#include "ConsoleUI.hpp"
#include "KeyboardInput.hpp"
#include "Marquee.hpp"
#include "CommandInterpreter.hpp"

/**
 * @brief CSOPESY Semi-Major Output 1
 * Entry Point for the OS Emulator and Marquee System.
 * 
 * Group Developers:
 * - Espineli, Nyan
 * - Guarin, Raine
 * - Montano, Rovin
 * - Tolentino, Winelle
 * 
 * Version Date: 2026-09-21
 */
int main() {
    // 1. Initialize Windows Virtual Terminal Processing and 1ms timer resolution
    ConsoleUI::enable_virtual_terminal();

    // 2. Setup screen: 70-character wide border, scrolling region starting at row 4
    ConsoleUI::setup_screen(70, 4);

    ConsoleUI::print("\nWelcome to CSOPESY!\n\n");
    ConsoleUI::print("Group Developer:\n");
    ConsoleUI::print("Espineli, Nyan\nGuarin, Raine\nMontano, Rovin\nTolentino, Winelle\n\n");
    ConsoleUI::print("Version date: 2026-09-21\n\n");

    // 3. Instantiate core components
    Marquee marquee(70);
    bool running = true;
    CommandInterpreter interpreter(marquee, running);

    std::string input_buffer;
    const std::string prompt = "\nCommand> ";

    ConsoleUI::print(prompt);

    // 4. Main Event Loop
    while (running) {
        // Non-blocking keyboard drain: returns true only when Enter is pressed
        if (!KeyboardInput::poll_line(input_buffer)) {
            continue;
        }

        // Parse and execute command
        interpreter.execute(input_buffer);
        input_buffer.clear();

        if (running) {
            ConsoleUI::print(prompt);
        }
    }

    // 5. Cleanup upon exit
    marquee.stop();
    ConsoleUI::reset_screen();
    ConsoleUI::cleanup_virtual_terminal();

    return 0;
}