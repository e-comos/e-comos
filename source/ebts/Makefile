# *
# * This program is free software: you can redistribute it and/or modify
# * it under the terms of the GNU Affero General Public License as published
# * by the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU Affero General Public License for more details.
# *
# * You should have received a copy of the GNU Affero General Public License
# * along with this program.  If not, see <https://www.gnu.org/licenses/>.

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude/ -I../ECLib/include
SRCDIR = src
OBJDIR = build
TARGET = ebts

SOURCES = $(SRCDIR)/main.c \
          $(SRCDIR)/shell/shell.c \
          $(SRCDIR)/shell/command.c \
          $(SRCDIR)/shell/object.c \
          $(SRCDIR)/shell/user.c \
          $(SRCDIR)/shell/commandbox.c \
          $(SRCDIR)/utils/memory.c \
          $(SRCDIR)/utils/string.c \
          $(SRCDIR)/shell/prompt.c

OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

.PHONY: all clean debug run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)