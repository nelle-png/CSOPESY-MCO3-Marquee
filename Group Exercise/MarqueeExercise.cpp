#include <iostream>  
#include <string>  

void show_help() {
    std::cout << "help = displays all commands\n";
    std::cout << "set_text [text] = parses and saves text provided\n";
    std::cout << "exit = terminates the console\n";
}

int main() {
    std::cout << "Group 7\n";
    std::cout << "Version 9-18-2026\n";
    
    std::string cmd;
    std::string saved_text; // scan variable
    const std::string prompt = "\nCommand> ";
    
    bool not_exit = true;
    
    while (not_exit) {
        std::cout << prompt;
        
        if (!(std::cin >> cmd)) break;  // handle EOF
        
        if (cmd == "help") {
            show_help();
        } 
        else if (cmd == "set_text") {
            std::cout << "filler!!!";
        } 
        else if (cmd == "exit") {
            not_exit = false;  
        } 
        else {
            std::cout << "error: command not found\n";
        }
    }
    return 0;
}