#include <iostream>  
#include <string>  

void show_help() {
    std::cout << "help - displays the commands and its description\n";
    // std::cout << "start_marquee - starts the marquee \"animation\"\n";
    // std::cout << "stop_marquee - stops the marquee \"animation\"\n";
    std::cout << "set_text - accepts a text input and displays it as a marquee\n";
    // std::cout << "set_speed - sets the marquee animation refresh in milliseconds\n";
    std::cout << "exit - terminates the console\n";
}

int main() {
    std::cout << "\nWelcome to CSOPESY!\n\n";
    std::cout << "Group Developer:\n";
    std::cout << "Espineli, Nyan\nGuarin, Raine\nMontano, Rovin\nTolentino, Winelle\n\n";
    std::cout << "Version date: 2026-09-21\n\n";
    
    std::string cmd;
    std::string saved_text;
    int speed_ms = 100;  // default refresh speed in milliseconds
    const std::string prompt = "\nCommand> ";
    
    bool not_exit = true;
    
    while (not_exit) {
        std::cout << prompt;
        
        if (!(std::cin >> cmd)) break;  // handle EOF
        
        if (cmd == "help") {
            show_help();
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
    return 0;
}