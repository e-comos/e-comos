# ebts Debug Guide

## Build and Run

### Basic Build
```bash
make
```

### Debug Build
```bash
make debug
```

### Run Program
```bash
make run
# or run directly
./ebts
```

### Clean Build Files
```bash
make clean
```

## Debug Methods

### 1. Using GDB Debugger
```bash
# Build debug version
make debug

# Start GDB
gdb ./ebts

# Common GDB commands
(gdb) run                    # Run program
(gdb) break main             # Set breakpoint at main function
(gdb) break shell.c:45       # Set breakpoint at specific file:line
(gdb) continue               # Continue execution
(gdb) step                   # Step into
(gdb) next                   # Next line
(gdb) print variable_name    # Print variable value
(gdb) backtrace             # Show call stack
(gdb) quit                  # Exit GDB
```

### 2. Using Valgrind for Memory Check
```bash
# Install valgrind (if not installed)
brew install valgrind  # macOS
# or sudo apt-get install valgrind  # Linux

# Check memory leaks
valgrind --leak-check=full ./ebts
```

### 3. Adding Debug Output
Add debug information in code:
```c
#ifdef DEBUG
    printf("DEBUG: variable value = %d\n", variable);
#endif
```

### 4. Common Issue Troubleshooting
- Segmentation fault: Use GDB to find crash location
- Memory leaks: Use Valgrind to check
- Compile errors: Check header includes and function declarations