#include <iostream>  
#include <string>
#include <thread>
#include <atomic>
#include <chrono> 
#include <functional> 
#include <mutex>

std::mutex marquee_mutex;

void print_marquee(const std::string& text) {
    std::lock_guard<std::mutex> lock(marquee_mutex);
    std::cout <<  "\0337" << "\033[2;1H" << text << "\033[K" << "\0338" << std::flush; 
}

void print (const std::string& text) {
    std::lock_guard<std::mutex> lock(marquee_mutex);
    std::cout << text << std::flush;
}

void show_help() {
    print("help - displays the commands and its description\n");
    print("start_marquee - starts the marquee \"animation\"\n");
    print("stop_marquee - stops the marquee \"animation\"\n");
    print("set_text - accepts a text input and displays it as a marquee\n");
    print("set_speed - sets the marquee animation refresh in milliseconds\n");
    print("exit - terminates the console\n");
}

void animation(std::string saved_text, int speed_ms, std::atomic<bool>& marquee_running) {
    int offset = 0;
    std::string text = saved_text + " ";
    while (marquee_running.load()) {
        std::string frame = text.substr(offset) + text.substr(0, offset);
        frame = frame.substr(0, 40);  // limit to 40 characters 
        offset = (offset + 1) % text.length();
        print_marquee(frame);
        std::this_thread::sleep_for(std::chrono::milliseconds(speed_ms));
    }
}

int main() {
    std::cout << "\033[2J" << "\033[4r" << "\033[1;1H" << std::string(40, '=') << "\033[3;1H" << std::string(40, '=') << "\033[4;1H";
    print("\nWelcome to CSOPESY!\n\n");
    print("Group Developer:\n");
    print("Espineli, Nyan\nGuarin, Raine\nMontano, Rovin\nTolentino, Winelle\n\n");
    print("Version date: 2026-09-21\n\n");

    std::string cmd;
    std::string saved_text;
    int speed_ms = 100;  // default refresh speed in milliseconds
    const std::string prompt = "\nCommand> ";
    
    bool not_exit = true;
    std::atomic<bool> marquee_running(false);
    std::thread marquee_thread;

    while (not_exit) {
        print(prompt);
        
        if (!(std::cin >> cmd)) break;  // handle EOF
        
        if (cmd == "help") {
            show_help();
        } 
        else if (cmd == "start_marquee") {
            if (saved_text.empty()) {
                print("error: no text set for marquee\n");
            } else {
                if (!marquee_running.load()) {
                    marquee_running.store(true);
                    marquee_thread = std::thread(animation, saved_text, speed_ms, std::ref(marquee_running));
                } else {
                    print("error: Marquee is already running.\n");
                }
            }
        }
        else if (cmd == "stop_marquee") {
            if (marquee_running.load()) {
                marquee_running.store(false);
                if (marquee_thread.joinable()) {
                    marquee_thread.join();
                }
                std::cout <<"\0337" << "\033[2;1H" << "\033[K" << "\0338" << std::flush;   // clear the marquee line
            } else {
                print("error: Marquee is not running.\n");
            }
        }
        else if (cmd == "set_text") {
            std::getline(std::cin, saved_text);  // read rest of line
            if (!saved_text.empty() && saved_text[0] == ' ')
                saved_text.erase(0, 1);           // trim leading space
            if (saved_text.empty()) {
                print("error: no text provided\n");
            } else {
                print("Text saved for marquee: " + saved_text + "\n");
            }
        } 
        else if (cmd == "set_speed") {
            std::string speed_str;
            std::getline(std::cin, speed_str);
            if (!speed_str.empty() && speed_str[0] == ' ')
                speed_str.erase(0, 1);
            try {
                int val = std::stoi(speed_str);
                if (val <= 0) {
                    print("error: speed must be greater than 0\n");
                } else {
                    speed_ms = val;
                    print("Speed set to: " + std::to_string(speed_ms) + " ms\n");
                }
            } catch (...) {
                print("error: invalid speed value\n");
            }
        } 
        else if (cmd == "exit") {
            not_exit = false;
            print("Terminating console...\n");
        } 
        else {
            print("error: command not found\n");
        }
    }
    if (marquee_running.load()) {
        marquee_running.store(false);
        if (marquee_thread.joinable()) {
            marquee_thread.join();
        }
    }
    std::cout <<"\0337" << "\033[2;1H" << "\033[K" << "\033[r" << "\0338" << std::flush;   // clear the marquee line
    return 0;
}