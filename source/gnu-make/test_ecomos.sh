#!/bin/bash
# Test script for E-comOS port of GNU Make

echo "=== GNU Make E-comOS Port Test Suite ==="
echo

# Test 1: Basic compilation
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

# Test 2: Dependency checking
echo "Test 2: Dependency checking"
cat > test.c << 'EOF'
#include <stdio.h>
int main() { printf("Test\n"); return 0; }
EOF

cat > test_makefile << 'EOF'
test: test.o
	@echo "Linking test"

test.o: test.c
	@echo "Compiling test.c"
EOF

./make -f test_makefile test
if [ $? -eq 0 ]; then
    echo "✓ Test 2 passed"
else
    echo "✗ Test 2 failed"
fi
echo

# Test 3: Variable expansion
echo "Test 3: Variable expansion"
cat > test_makefile << 'EOF'
VAR = value
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

# Test 4: Pattern rules
echo "Test 4: Pattern rules"
cat > test_makefile << 'EOF'
%.o: %.c
	@echo "Pattern rule: $< -> $@"

test.o: test.c
EOF

./make -f test_makefile test.o
if [ $? -eq 0 ]; then
    echo "✓ Test 4 passed"
else
    echo "✗ Test 4 failed"
fi
echo

# Cleanup
rm -f test_makefile test.c test.o test

echo "=== Test suite completed ==="
