# Makefile for PowerOffer Shutdown Service

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LDFLAGS =

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

all: poweroffer

poweroffer: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

src/shutdown.o: src/shutdown.c include/poweroffer.h
src/appendix_s.o: src/appendix_s.c include/appendix_s.h
src/virtual_port.o: src/virtual_port.c include/virtual_port.h
src/error_handling.o: src/error_handling.c include/poweroffer.h

clean:
	rm -f $(OBJ) poweroffer

.PHONY: all clean