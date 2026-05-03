#!/bin/bash
# Build with debug symbols

CC=gcc
CFLAGS="-g -O0 -DMK_OS_ECOMOS=1 -DUSE_ECLIB=1 -I. -Isrc"
SRCS="src/eclib_adapter.c src/ecomos.c"

echo "Building with debug symbols..."
$CC $CFLAGS -c $SRCS

echo "Debug build complete"
echo "Run: gdb -x .gdbinit_ecomos ./make"
