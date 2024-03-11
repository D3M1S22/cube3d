# Makefile for Cute Project

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror

# Directories
SRC_DIR = src
GNL_DIR = get_next_line
OBJ_DIR = obj

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c) \
            $(wildcard $(GNL_DIR)/*.c)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Executable
TARGET = Cube3D

# Dependencies
DEPS = -I$(SRC_DIR) -I$(GNL_DIR)

# Main target
all: $(TARGET)

# Rule for compiling object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEPS) -c $< -o $@

# Rule for linking object files into the executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(DEPS) $^ -o $@

# Clean target
clean:
	rm -rf $(OBJ_DIR)

# Distclean target
distclean: clean
	rm -f $(TARGET)

# Rebuild target
re: distclean all

# Phony targets
.PHONY: all clean distclean
