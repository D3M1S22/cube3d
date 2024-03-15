# Compiler
CC = gcc
MAKE = make
UNAME := $(shell uname)

# Compiler flags
CFLAGS = -Wextra -Wall -Werror -Wunreachable-code -Ofast
CFLAGS_MAC = -Imlx
CFLAGS_LINUX = -I/usr/include -Imlx_linux -O3 -c 
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

# Target to compile mlx
ifeq ($(UNAME), Darwin)

# Rule for compiling object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CFLAGS_MAC) $(DEPS) -c $< -o $@

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(DEPS) -Lmlx -lmlx -framework OpenGL -framework AppKit $^ -o $@

mlx_b:
	$(MAKE) -C mlx

# Clean target
clean:
	rm -rf $(OBJ_DIR)

# Distclean target
fclean:
	rm -f $(TARGET) ./obj/mlx/*.a

endif

ifeq ($(UNAME), Linux)
# Rule for compiling object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CFLAGS_LINUX) $(DEPS) -c $< -o $@

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(CFLAGS_LINUX) $(DEPS) -Imlx_linux -Lmlx_linux -lmlx -lmlx_Linux -L/usr/lib -lXext -lX11 -lm $^ -o $@

mlx_b:
	$(MAKE) -C mlx_linux

# Clean target
clean:
	rm -rf $(OBJ_DIR) mlx_linux/obj

# Distclean target
fclean: 
	rm -f $(TARGET) mlx_linux/*.a

endif


# Rebuild target
re: fclean all

# Phony targets
.PHONY: all re clean fclean