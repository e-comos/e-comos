CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/commandbox

.PHONY: all clean config install

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

config:
	@echo "E-comOS CommandBox Configuration"
	@echo "Available commands:"
	@echo "  lookup  - List directory contents"
	@echo "  saw     - Display file contents"
	@echo "  del     - Delete files/directories"
	@echo "  copy    - Copy/move files"
	@echo "  goto    - Change directory"
	@echo "  info-of - Show file information"
	@echo "  output  - Output text"

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

test: $(TARGET)
	@echo "Testing CommandBox..."
	@$(TARGET) output "Hello E-comOS!"
	@$(TARGET) lookup .

commands:
	$(MAKE) -C commands

clean-commands:
	$(MAKE) -C commands clean

all-commands: $(TARGET) commands