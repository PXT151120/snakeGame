✅ Read game controller input using C + Linux system calls
✅ Implement game logic using C++
✅ Display the game in the terminal (text mode)
✅ Target Ubuntu 25.04 (Linux)
✅ Optionally include multiprocessing or IPC for deeper system knowledge

snake_game/
├── CMakeLists.txt
├── README.md

├── controller/                     # 🕹️ Raw input from gamepad (pure C)
│   ├── input.c                     # C code using /dev/input/eventX
│   ├── input.h                     # C interface, wrapped with extern "C"
│   └── udev_helper.c (optional)   # If you want udev device detection

├── game/                           # 🐍 Game logic (pure modern C++)
│   ├── snake.cpp                   # Game loop, logic, collision, apple
│   ├── snake.hpp                   # Snake state and functions
│   ├── renderer.cpp                # Terminal renderer (ncurses or ANSI)
│   ├── renderer.hpp

├── core/                           # 🧠 Core utilities
│   ├── timer.hpp                   # Frame rate limiter
│   ├── ipc.hpp                     # Optional: shared memory or pipes
│   └── ipc.cpp

├── multiprocessing/               # 🧵 Optional multiprocess control
│   ├── game_process.cpp           # Separate game logic process
│   ├── input_process.cpp          # Separate input polling process
│   └── ipc_channel.hpp            # Pipe/fifo interface

├── main.cpp                        # 🔁 Main loop: calls game + input
