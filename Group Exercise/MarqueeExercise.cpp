#include <iostream>  
#include <string>
#include <thread>
#include <atomic>
#include <chrono> 
#include <functional> 

void show_help() {
    std::cout << "help - displays the commands and its description\n";
    std::cout << "start_marquee - starts the marquee \"animation\"\n";
    std::cout << "stop_marquee - stops the marquee \"animation\"\n";
    std::cout << "set_text - accepts a text input and displays it as a marquee\n";
    std::cout << "set_speed - sets the marquee animation refresh in milliseconds\n";
    std::cout << "exit - terminates the console\n";
}

void animation(std::string saved_text, int speed_ms, std::atomic<bool>& marquee_running) {
    int offset = 0;
    std::string text = saved_text + " ";
    while (marquee_running.load()) {
        std::string frame = text.substr(offset) + text.substr(0, offset);
        offset = (offset + 1) % text.length();
        std::cout <<  "\0337" << "\033[2;1H" << frame << "\033[K" << "\0338" << std::flush; 
        std::this_thread::sleep_for(std::chrono::milliseconds(speed_ms));
    }
}

int main() {
    std::cout << "\033[2J" << "\033[4r" << "\033[1;H" << std::string(40, '=') << "\033[3;1H" << std::string(40, '=') << "\033[4;1H";
    std::cout << "\nWelcome to CSOPESY!\n\n";
    std::cout << "Group Developer:\n";
    std::cout << "Espineli, Nyan\nGuarin, Raine\nMontano, Rovin\nTolentino, Winelle\n\n";
    std::cout << "Version date: 2026-09-21\n\n";
    
    std::string cmd;
    std::string saved_text;
    int speed_ms = 100;  // default refresh speed in milliseconds
    const std::string prompt = "\nCommand> ";
    
    bool not_exit = true;
    std::atomic<bool> marquee_running(false);
    std::thread marquee_thread;

    while (not_exit) {
        std::cout << prompt;
        
        if (!(std::cin >> cmd)) break;  // handle EOF
        
        if (cmd == "help") {
            show_help();
        } 
        else if (cmd == "start_marquee") {
            if (saved_text.empty()) {
                std::cout << "error: no text set for marquee\n";
            } else {
                if (!marquee_running.load()) {
                    marquee_running.store(true);
                    marquee_thread = std::thread(animation, saved_text, speed_ms, std::ref(marquee_running));
                } else {
                    std::cout << "error: Marquee is already running.\n";
                }
            }
        }
        else if (cmd == "set_text") {
            std::getline(std::cin, saved_text);  // read rest of line
            if (!saved_text.empty() && saved_text[0] == ' ')
                saved_text.erase(0, 1);           // trim leading space
            if (saved_text.empty()) {
                std::cout << "error: no text provided\n";
            } else {
                std::cout << "Text saved for marquee: " << saved_text << "\n";
            }
        } 
        /* else if (cmd == "set_speed") {
            std::string speed_str;
            std::getline(std::cin, speed_str);
            if (!speed_str.empty() && speed_str[0] == ' ')
                speed_str.erase(0, 1);
            try {
                int val = std::stoi(speed_str);
                if (val <= 0) {
                    std::cout << "error: speed must be greater than 0\n";
                } else {
                    speed_ms = val;
                    std::cout << "Speed set to: " << speed_ms << " ms\n";
                }
            } catch (...) {
                std::cout << "error: invalid speed value\n";
            }
        } */
        else if (cmd == "exit") {
            not_exit = false;
            std::cout << "Terminating console...\n";
        } 
        else {
            std::cout << "error: command not found\n";
        }
    }
    std::cout << "\033[r";
    return 0;
}