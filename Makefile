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
PARSER_DIR = $(SRC_DIR)/parser
CHECKER_DIR = $(SRC_DIR)/checker
OBJ_DIR = obj

# Source files
SRC_FILES = $(SRC_DIR)/main.c \
            $(GNL_DIR)/get_next_line.c \
            $(GNL_DIR)/get_next_line_utils.c \
            $(PARSER_DIR)/check_input.c \
            $(PARSER_DIR)/ft_split.c \
            $(PARSER_DIR)/ft_strdup.c \
            $(PARSER_DIR)/parse_args.c \
            $(PARSER_DIR)/parse_grid.c \
            $(PARSER_DIR)/parse_grid_utils.c \
            $(PARSER_DIR)/parse_map.c \
            $(PARSER_DIR)/parser.c \
            $(CHECKER_DIR)/checker_map.c

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Executable
TARGET = Cub3D

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
	@$(MAKE) -C mlx

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
