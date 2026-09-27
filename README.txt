========================================================================
CSOPESY Semi-Major Output 1 - OS Emulator with Marquee System
========================================================================

Developers:
- Espineli, Nyan
- Guarin, Raine
- Montano, Rovin
- Tolentino, Winelle

Course: CSOPESY
Term: Term 1, AY 2026-2027
Date: September 21, 2026

------------------------------------------------------------------------
ENTRY POINT
------------------------------------------------------------------------
The main function and program entry point is located in:
  Group Exercise/MarqueeExercise.cpp

------------------------------------------------------------------------
PROJECT STRUCTURE
------------------------------------------------------------------------
- Group Exercise/MarqueeExercise.cpp
    Main entry point containing program initialization and the event loop.
- Group Exercise/ConsoleUI.hpp
    Handles ANSI VT100 terminal escape sequences, scrolling regions,
    anti-flicker cursor hiding, and thread-safe console output.
- Group Exercise/KeyboardInput.hpp
    Non-blocking keyboard polling (_kbhit() / _getch()), backspace handling,
    and burst keystroke draining with near 0% idle CPU overhead.
- Group Exercise/Marquee.hpp
    Manages marquee thread lifecycle, condition-variable responsive sleep
    (0ms interrupt latency), and circular text animation frames.
- Group Exercise/CommandInterpreter.hpp
    Tokenizes and dispatches CLI commands (help, start_marquee, stop_marquee,
    set_text, set_speed, exit) with input validation.

------------------------------------------------------------------------
COMPILATION & EXECUTION INSTRUCTIONS
------------------------------------------------------------------------

Option 1: Using g++ (MinGW / GCC)
1. Open PowerShell or Command Prompt.
2. Navigate to the project root directory.
3. Compile the program:
     g++ -std=c++17 -Wall -Wextra "Group Exercise/MarqueeExercise.cpp" -o marquee.exe
4. Run the executable:
     .\marquee.exe

Option 2: Using VS Code or Visual Studio IDE
1. Open the project folder in VS Code / Visual Studio.
2. Open "Group Exercise/MarqueeExercise.cpp".
3. Press Run / Debug (F5) or use the C/C++ build task.

------------------------------------------------------------------------
COMMAND REFERENCE
------------------------------------------------------------------------
- help
    Displays all available commands and descriptions.
- start_marquee
    Starts the background animation thread.
- stop_marquee
    Stops the background animation thread and clears the marquee line.
- set_text <text>
    Sets the text to display in the marquee (can be updated live).
- set_speed <milliseconds>
    Sets the animation frame interval in milliseconds (e.g., 50 or 100).
- exit
    Terminates the OS emulator and restores terminal state.
========================================================================
