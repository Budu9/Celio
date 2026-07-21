# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDLIBS = -lsodium -lz

# Output binary name
TARGET = celio

# Source files and objects
SRC = src/main.c src/header.c src/encoder.c src/transformer.c src/decoder.c src/crypto.c
OBJ = $(SRC:.c=.o)

# Default build rule
all: $(TARGET)

# Linking the object files
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDLIBS)

# Cleanup build artifacts
clean:
	rm -f $(OBJ) $(TARGET)