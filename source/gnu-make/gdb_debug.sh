#!/bin/bash
# GDB debugging helper for E-comOS Make port

echo "=== GDB Debugging Setup for E-comOS Make ==="
echo

# Create GDB command file
cat > .gdbinit_ecomos << 'EOF'
# GDB initialization for E-comOS Make debugging

# Break on main
break main

# Break on key functions
break ecomos_spawn_child
break ecomos_file_stat
break ecomos_path

# Display settings
set print pretty on
set print array on
set pagination off

# Custom commands
define show-path
  printf "Unix path: %s\n", $arg0
  call ecomos_path($arg0)
  printf "E-comOS path: %s\n", $
end

define show-stat
  printf "File: %s\n", $arg0
  call ecomos_file_stat($arg0, &st)
  printf "mtime: %lld\n", st.mtime_sec
end

echo \n=== E-comOS Make Debugger Ready ===\n
echo Commands:\n
echo   show-path <path>  - Convert and show path\n
echo   show-stat <file>  - Show file stats\n
echo \n
EOF

echo "GDB init file created: .gdbinit_ecomos"
echo

# Create debug build script
cat > build_debug.sh << 'EOF'
#!/bin/bash
# Build with debug symbols

CC=gcc
CFLAGS="-g -O0 -DMK_OS_ECOMOS=1 -DUSE_ECLIB=1 -I. -Isrc"
SRCS="src/eclib_adapter.c src/ecomos.c"

echo "Building with debug symbols..."
$CC $CFLAGS -c $SRCS

echo "Debug build complete"
echo "Run: gdb -x .gdbinit_ecomos ./make"
EOF

chmod +x build_debug.sh

echo "Debug build script created: build_debug.sh"
echo
echo "Usage:"
echo "  1. ./build_debug.sh          # Build with debug symbols"
echo "  2. gdb -x .gdbinit_ecomos ./make"
echo "  3. (gdb) run -f Makefile"
echo
echo "Useful GDB commands:"
echo "  break ecomos_spawn_child     # Break on process spawn"
echo "  watch job_slots              # Watch variable"
echo "  backtrace                    # Show call stack"
echo "  print variable               # Print variable value"
echo "  show-path \"/usr/bin\"         # Custom: show path conversion"
