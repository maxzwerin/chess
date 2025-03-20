# Compiler and flags
CC = gcc
CFLAGS = -std=c99 -I./src -I/opt/homebrew/include
LDFLAGS = -L/opt/X11/lib -I/opt/X11/include -lX11 -lm

# Source and object file
SRC = src/chess.c
OBJ = chess.o

# Executable name
TARGET = chess

# Default target: build the executable
all: $(TARGET)

# Link object file to create the executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile the source file into an object file
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

# Clean up generated files
clean:
	rm -f $(OBJ) $(TARGET)
