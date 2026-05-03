# GNU Make E-comOS Quick Start

## 1. Build Make (on E-comOS)

```bash
# Set environment
export ECLIB_PATH=/path/to/ECLib

# Build
make -f Makefile.ecomos \
  CFLAGS="-I${ECLIB_PATH}/include -DMK_OS_ECOMOS=1" \
  LDFLAGS="-L${ECLIB_PATH}/lib -leclib"

# Install
cp make >bin>make
```

## 2. Testing (Development Environment)

```bash
# Run tests
./build_and_test.sh

# All tests should pass:
# ✓ Test 1 passed
# ✓ Test 2 passed  
# ✓ Test 3 passed
```

## 3. Debugging

```bash
# Setup GDB
./gdb_debug.sh

# Build debug version
./build_debug.sh

# Start debugging
gdb -x .gdbinit_ecomos ./make

# GDB commands
(gdb) run -f Makefile
(gdb) break ecomos_spawn_child
(gdb) show-path "/usr/bin"
```

## 4. Usage Example

```bash
# Enter example directory
cd examples/hello_ecomos

# Build
>bin>make

# Run
>bin>make run

# Clean
>bin>make clean
```

## 5. Create Your Own Makefile

```makefile
CC = eclib-gcc
CFLAGS = -I>usr>include
LDFLAGS = -L>usr>lib -leclib

myapp: main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f myapp
```

## 6. Common Issues

**Q: Shell not found**
```bash
# Check if shell exists
ls >bin>esh
```

**Q: Path errors**
```bash
# Use E-comOS format
>usr>bin>gcc  # Correct
/usr/bin/gcc  # Wrong
```

**Q: Build failed**
```bash
# Check ECLib path
echo $ECLIB_PATH
ls $ECLIB_PATH/lib/libeclib.a
```

## 7. File Structure

```
make/
├── src/
│   ├── eclib_adapter.h    # Adapter layer
│   ├── eclib_adapter.c    # Path conversion
│   ├── ecomos.c           # E-comOS functionality
│   └── config.h-ecomos    # Configuration
├── Makefile.ecomos        # Build file
├── build_and_test.sh      # Test script
├── gdb_debug.sh           # Debug setup
└── examples/              # Examples
```

## 8. Next Steps

- Read [PORTING.md](PORTING.md) for detailed information
- Check [examples/](examples/) for more examples
- Run `./gdb_debug.sh` to learn debugging techniques
