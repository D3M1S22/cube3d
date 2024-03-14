# Compiler
CC = gcc
MAKE = make
UNAME := $(shell uname)

# Compiler flags
CFLAGS = -Wextra -Wall -Werror -Wunreachable-code -Ofast -Imlx

# Directories
SRC_DIR = src
LIBFT_DIR = libft
GNL_DIR = get_next_line
OBJ_DIR = obj

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c) \
						$(wildcard $(LIBFT_DIR)/*.c) \
            $(wildcard $(GNL_DIR)/*.c) \
						$(wildcard $(SRC_DIR)/parser/*.c) \
						$(wildcard $(SRC_DIR)/checker/*.c)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Executable
TARGET = Cube3D

# Dependencies
DEPS = -I$(SRC_DIR) -I$(GNL_DIR) -I$(LIBFT_DIR)

all: mlx_b $(TARGET)

# Rule for compiling object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEPS) -c $< -o $@

# Rule for linking object files into the executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(DEPS) -Lmlx -lmlx -framework OpenGL -framework AppKit $^ -o $@

# Target to compile mlx
ifeq ($(UNAME), Darwin)
mlx_b:
	$(MAKE) -C mlx
endif

ifeq ($(UNAME), Linux)
mlx_b:
	$(MAKE) -C minilibx-linux
endif

# Clean target
clean:
	rm -rf $(OBJ_DIR) mlx_o

# Distclean target
fclean: clean
	rm -f $(TARGET)

# Rebuild target
re: fclean all

# Phony targets
.PHONY: all re clean fclean