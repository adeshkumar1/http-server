# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -Wextra -std=c99 -g -Iinclude

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

# Target executable
TARGET  := server

# Default rule
all: $(TARGET)

# Link object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean up build artifacts
clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
