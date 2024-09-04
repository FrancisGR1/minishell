NAME = minishell
DEBUG = -g3
CFLAGS = -Wall -Werror -Wextra $(DEBUG)
OPTIMIZE = -03

SRC_DIR = src
SRC_FILES = main.c
SRC = $(addprefix $(SRC_DIR)/,$(SRC_FILES))

OBJ_DIR = obj
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

HEADER_FILES = minishell.h
HEADERS = $(addprefix $(SRC_DIR)/,$(HEADER_FILES))

MK = Makefile

LIB = libft.a
LIB_DIR = libft

all: $(NAME)

$(NAME): $(OBJ) $(LIB_DIR)/$(LIB)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -L$(LIB_DIR) -lft

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)%.c | $(OBJ_DIR)

$(OBJ): $(HEADER) $(MK)

$(LIB_DIR)/$(LIB):
	$(MAKE) -C $(LIB_DIR)
run:

leaks: $(NAME)
	valgrind --track-origins=yes --show-leak-kinds=all --leak-check=full --log-file=valgrind.out ./$(NAME)

norm:

format: 

clean:

fclean: clean

re: fclean all

watch:
