CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address --pedantic-errors

# Using CURDIR to set the SRCDIR
SRCDIR = $(CURDIR)/src
INCDIR = $(CURDIR)/inc
BUILDDIR = $(CURDIR)/build
TARGET = my_zsh

# Gather all source files recursively
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
INCLUDES = -I$(INCDIR)

all: $(TARGET)

$(TARGET): $(OBJECTS)
		$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
		$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
		rm -f $(OBJECTS) $(TARGET)

fclean: clean