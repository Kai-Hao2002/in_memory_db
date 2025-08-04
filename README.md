# C_project

In-Memory SQL Library (C++)

This is a simple in-memory SQL library implemented in C++. It supports standard SQL operations, including `CREATE`, `INSERT`, `DELETE`, `UPDATE`, and `SELECT` statements, and features concise ASCII table and CSV output.

---
## Project Structure

in_memory_db/
├── include/ # All public .hpp header files (interface)
│ ├── db/ # Database structure (Database, Table, Row, Column)
│ ├── parser/ # SQL Parser
│ ├── statement/ # Various SQL Statement classes
│ ├── util/ # Utility classes: Value, Formatter, StringUtil
│ └── exception/ # Custom error handling
├── src/ # Implementation (.cpp)
├── tests/ # Unit tests (using Catch2)
├── examples/ # Example SQL files
├── main.cpp # Main program: reads and executes SQL statements
├── CMakeLists.txt # CMake compilation settings
└── README.md # This documentation
---

## Supported Functions

- SQL syntax parsing and execution
- `CREATE TABLE`
- `INSERT INTO`
- `DELETE FROM`
- `UPDATE`
- `SELECT`
- Data is in-memory (no persistence)
- but can use save to / load from filename
- Output formats:
- ASCII table (default)
- CSV format (optional)
- Type support: integers, floating-point numbers, text strings, NULL

---
## Compilation Method

### Prerequisites

- CMake 3.14+
- C++17 compiler (GCC/Clang/MSVC)
- Catch2 (unit tests)
- SFML MSYS2
- pacman -S mingw-w64-ucrt-x86_64-sfml

```bash
git clone https://github.com/your_username/in_memory_db.git
mkdir build && cd build
cmake ..
ninja

#Test case 
ctest --verbose

#main.cpp
in_memory_db/in_memory_db.exe

