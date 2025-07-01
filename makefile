# Compiler and flags
CC = gcc
CFLAGS = -std=c99 -I./src -I/opt/homebrew/include
LDFLAGS = -L/opt/X11/lib -I/opt/X11/include -lX11 -lm

# Source files and object files
SRC = $(wildcard src2/*.c)
OBJ = $(SRC:src/%.c=build/%.o)

# Executable name
TARGET = x

# Default target
all: $(TARGET)

# Link object files into the executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Compile source files into object files
build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

run: $(TARGET)
	./$(TARGET)
