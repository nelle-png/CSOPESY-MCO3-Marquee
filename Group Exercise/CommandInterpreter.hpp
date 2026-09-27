#pragma once

#include <string>
#include "ConsoleUI.hpp"
#include "Marquee.hpp"

/**
 * @brief CommandInterpreter parses user input, validates arguments,
 * and executes OS emulator shell commands.
 */
class CommandInterpreter {
private:
    Marquee& marquee;
    bool& should_run;

    void show_help() const {
        ConsoleUI::print("help - displays the commands and its description\n");
        ConsoleUI::print("start_marquee - starts the marquee \"animation\"\n");
        ConsoleUI::print("stop_marquee - stops the marquee \"animation\"\n");
        ConsoleUI::print("set_text - accepts a text input and displays it as a marquee\n");
        ConsoleUI::print("set_speed - sets the marquee animation refresh in milliseconds\n");
        ConsoleUI::print("exit - terminates the console\n");
    }

public:
    CommandInterpreter(Marquee& m, bool& run_flag)
        : marquee(m), should_run(run_flag) {}

    void execute(std::string input) {
        // Trim leading whitespace
        size_t first = input.find_first_not_of(" \t");
        if (first == std::string::npos) {
            return; // Empty line
        }
        input = input.substr(first);

        // Split into command name and argument
        std::string cmd;
        std::string args;
        size_t pos = input.find(' ');

        if (pos == std::string::npos) {
            cmd = input;
        } else {
            cmd = input.substr(0, pos);
            args = input.substr(pos + 1);

            // For non-set_text commands, strip leading spaces from arguments
            if (cmd != "set_text") {
                size_t arg_first = args.find_first_not_of(" \t");
                if (arg_first != std::string::npos) {
                    args = args.substr(arg_first);
                }
            }
        }

        // Dispatch command
        if (cmd == "help") {
            show_help();
        } 
        else if (cmd == "start_marquee") {
            if (!marquee.has_text()) {
                ConsoleUI::print("error: no text set for marquee\n");
            } else if (!marquee.start()) {
                ConsoleUI::print("error: Marquee is already running.\n");
            }
        } 
        else if (cmd == "stop_marquee") {
            if (!marquee.stop()) {
                ConsoleUI::print("error: Marquee is not running.\n");
            }
        } 
        else if (cmd == "set_text") {
            if (args.empty()) {
                ConsoleUI::print("error: no text provided\n");
            } else {
                marquee.set_text(args);
                ConsoleUI::print("Text saved for marquee: " + marquee.get_text() + "\n");
            }
        } 
        else if (cmd == "set_speed") {
            try {
                int val = std::stoi(args);
                if (val <= 0) {
                    ConsoleUI::print("error: speed must be greater than 0\n");
                } else {
                    marquee.set_speed(val);
                    ConsoleUI::print("Speed set to: " + std::to_string(marquee.get_speed()) + " ms\n");
                }
            } catch (...) {
                ConsoleUI::print("error: invalid speed value\n");
            }
        } 
        else if (cmd == "exit") {
            should_run = false;
            ConsoleUI::print("Terminating console...\n");
        } 
        else {
            ConsoleUI::print("error: command not found\n");
        }
    }
};
