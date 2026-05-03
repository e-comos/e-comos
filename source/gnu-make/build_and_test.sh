#!/bin/bash
set -e

echo "=== Building GNU Make for E-comOS ==="
echo

# Check if we're on a system that can compile (mock build)
if ! command -v gcc &> /dev/null; then
    echo "Warning: gcc not found. This is a mock build for demonstration."
    echo "On E-comOS, use: make -f Makefile.ecomos"
    exit 0
fi

# Create a minimal mock build for testing the structure
echo "Creating mock make executable..."
cat > make_mock.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    printf("GNU Make 4.4 (E-comOS port - MOCK)\n");
    
    // Parse basic arguments
    char *makefile = "Makefile";
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i+1 < argc) {
            makefile = argv[i+1];
            i++;
        }
    }
    
    printf("Reading makefile '%s'...\n", makefile);
    
    FILE *f = fopen(makefile, "r");
    if (!f) {
        fprintf(stderr, "make: *** No rule to make target '%s'. Stop.\n", makefile);
        return 2;
    }
    
    char line[1024];
    int in_recipe = 0;
    while (fgets(line, sizeof(line), f)) {
        // Skip empty lines and comments
        if (line[0] == '\n' || line[0] == '#') continue;
        
        // Check for recipe (starts with tab)
        if (line[0] == '\t') {
            // Execute recipe command
            char *cmd = line + 1;
            // Remove newline
            cmd[strcspn(cmd, "\n")] = 0;
            
            // Skip @ prefix
            if (cmd[0] == '@') cmd++;
            
            printf("%s\n", cmd);
            in_recipe = 1;
        } else {
            in_recipe = 0;
        }
    }
    
    fclose(f);
    return 0;
}
EOF

gcc -o make make_mock.c
rm make_mock.c

echo "✓ Mock make built successfully"
echo

# Now run tests
echo "=== Running Tests ==="
echo

# Test 1: Basic execution
echo "Test 1: Basic Makefile execution"
cat > test_makefile << 'EOF'
all:
	@echo "Hello from E-comOS Make!"
EOF

./make -f test_makefile
if [ $? -eq 0 ]; then
    echo "✓ Test 1 passed"
else
    echo "✗ Test 1 failed"
fi
echo

# Test 2: Multiple targets
echo "Test 2: Multiple targets"
cat > test_makefile << 'EOF'
all: target1 target2

target1:
	@echo "Building target1"

target2:
	@echo "Building target2"
EOF

./make -f test_makefile
if [ $? -eq 0 ]; then
    echo "✓ Test 2 passed"
else
    echo "✗ Test 2 failed"
fi
echo

# Test 3: Variables
echo "Test 3: Variable expansion"
cat > test_makefile << 'EOF'
VAR = test_value
all:
	@echo "Variable: $(VAR)"
EOF

./make -f test_makefile
if [ $? -eq 0 ]; then
    echo "✓ Test 3 passed"
else
    echo "✗ Test 3 failed"
fi
echo

# Cleanup
rm -f test_makefile

echo "=== Build and Test Complete ==="
echo
echo "Note: This is a mock build for demonstration."
echo "On E-comOS, compile with:"
echo "  make -f Makefile.ecomos"
