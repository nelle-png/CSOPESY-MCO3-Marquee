# CSOPESY Semi-Major Output 1: OS Emulator with Marquee System

A multi-threaded Windows console application simulating an OS emulator with an integrated command interpreter and an asynchronous, thread-safe text marquee banner.

---

## 👥 Developers
* **Espineli, Nyan**
* **Guarin, Raine**
* **Montano, Rovin**
* **Tolentino, Winelle**

* **Course**: CSOPESY (Operating Systems)  
* **Term**: Term 1, AY 2026–2027  
* **Version Date**: September 21, 2026  

---

## 🚀 Entry Point
The main function and program entry point is located in:
```text
Group Exercise/MarqueeExercise.cpp
```

---

## 📁 Project Structure

```text
CSOPESY-MCO3-Marquee/
├── Group Exercise/
│   ├── MarqueeExercise.cpp       # Main entry point & event loop
│   ├── ConsoleUI.hpp             # ANSI VT100 terminal setup, scrolling regions & thread-safe display
│   ├── KeyboardInput.hpp         # Non-blocking keyboard polling (_kbhit() / _getch()) & burst drain
│   ├── Marquee.hpp               # Animation worker thread, condition variable & circular frame generation
│   └── CommandInterpreter.hpp    # CLI command tokenization, input validation & dispatch
├── README.txt                    # Plain text submission documentation
└── README.md                     # GitHub repository documentation
```

### Module Descriptions
* [**`ConsoleUI.hpp`**](Group%20Exercise/ConsoleUI.hpp): Enables Windows VT100 virtual terminal sequences (`ENABLE_VIRTUAL_TERMINAL_PROCESSING`), sets 1ms OS timer resolution, manages fixed marquee lines (rows 1–3) and scrolling regions (`\033[4r`), and provides anti-flicker cursor hiding (`\033[?25l` / `\033[?25h`).
* [**`KeyboardInput.hpp`**](Group%20Exercise/KeyboardInput.hpp): Non-blocking keyboard polling using `_kbhit()` and `_getch()`, handling backspaces (`\b \b`), Enter (`\r`), and draining buffered keystrokes with near 0% idle CPU overhead.
* [**`Marquee.hpp`**](Group%20Exercise/Marquee.hpp): Encapsulates background worker thread (`std::thread`), condition-variable responsive sleep (`cv_anim.wait_for`) for true 0% CPU sleep with 0ms interrupt latency, and circular text rotation padded to 70 columns.
* [**`CommandInterpreter.hpp`**](Group%20Exercise/CommandInterpreter.hpp): Parses and executes CLI commands (`help`, `start_marquee`, `stop_marquee`, `set_text`, `set_speed`, `exit`) with full parameter validation.
* [**`MarqueeExercise.cpp`**](Group%20Exercise/MarqueeExercise.cpp): Coordinates system startup, runs the main event loop, and performs clean termination.

---

## 🛠️ Compilation & Execution Instructions

### Option 1: Using `g++` (MinGW / GCC)
1. Open PowerShell or Command Prompt.
2. Navigate to the project root directory:
   ```powershell
   cd CSOPESY-MCO3-Marquee
   ```
3. Compile using C++17:
   ```powershell
   g++ -std=c++17 -Wall -Wextra "Group Exercise/MarqueeExercise.cpp" -o marquee.exe
   ```
4. Run the executable:
   ```powershell
   .\marquee.exe
   ```

### Option 2: Using VS Code or Visual Studio IDE
1. Open the project root folder in VS Code or Visual Studio.
2. Open [`Group Exercise/MarqueeExercise.cpp`](Group%20Exercise/MarqueeExercise.cpp).
3. Press **Run / Debug (F5)** or execute your IDE's C/C++ build task.

---

## 💻 Command Reference

| Command | Arguments | Description |
| :--- | :--- | :--- |
| `help` | *None* | Displays all available commands and their descriptions. |
| `start_marquee` | *None* | Starts the background marquee animation thread. |
| `stop_marquee` | *None* | Stops the marquee animation thread and clears the marquee line. |
| `set_text` | `<text>` | Sets the text to display in the marquee (can be updated dynamically while running). |
| `set_speed` | `<milliseconds>` | Sets the animation frame refresh interval in milliseconds (must be > 0). |
| `exit` | *None* | Terminates the OS emulator, restores terminal state, and exits. |

---

## ⚡ Performance & Optimization Highlights
* **Zero-Lag Input Drain**: Drains all available keystrokes in a tight loop during fast typing or pasting before sleeping 10ms, eliminating input stuttering.
* **0ms Sleep Interruption**: Uses `std::condition_variable` instead of active polling; `stop_marquee` and `set_speed` interrupt the animation thread instantly with 0ms latency.
* **Anti-Flicker Rendering**: Cursor is hidden (`\033[?25l`) during frame updates and restored (`\033[?25h`) to prevent visual cursor jumping and screen tearing at high refresh rates.
* **Sub-10ms Timer Precision**: Dynamically enables 1ms timer resolution via `timeBeginPeriod(1)` to surpass the default Windows 15.6ms scheduler limitation.