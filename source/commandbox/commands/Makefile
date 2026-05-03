CC = gcc
CFLAGS = -Wall -Wextra -std=c99
BINDIR = ../bin/commands

COMMANDS = lookup saw del copy goto info-of output hostname
TARGETS = $(addprefix $(BINDIR)/, $(COMMANDS))

.PHONY: all clean

all: $(BINDIR) $(TARGETS)

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BINDIR)

test: all
	@echo "Testing individual commands..."
	@$(BINDIR)/output "Hello from E-comOS commands!"
	@$(BINDIR)/lookup .