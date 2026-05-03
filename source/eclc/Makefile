# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I$(ICDDIR) -I$(FCEFDIR)
LDFLAGS = 
LDLIBS = 

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin
ICDDIR = include
FCEFDIR = fcef/include

# Source files - organized by module
SOURCES = $(SRCDIR)/main.c \
          $(SRCDIR)/common/men.c \
          $(SRCDIR)/driver/args.c \
          $(SRCDIR)/frontend/lexer.c \
          $(SRCDIR)/frontend/parser.c \
          $(SRCDIR)/frontend/ast.c \
          $(SRCDIR)/frontend/error.c \
          $(SRCDIR)/fcef/fcef.c

# Object files
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Targets
TARGET = $(BINDIR)/eclc

.PHONY: all clean

all: $(TARGET)

# Main ECLC compiler
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) $(LDLIBS)

# Object file compilation
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Directory creation
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Clean
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Install (optional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/eclc

# Debug info
debug-info:
	@echo "Sources: $(SOURCES)"
	@echo "Objects: $(OBJECTS)"
	@echo "Target: $(TARGET)"
	@echo "CFLAGS: $(CFLAGS)"