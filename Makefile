NAME = minishell
DEBUG = -g3 #-DDEBUG_REPLACE=1
CFLAGS = -Wall -Werror -Wextra $(DEBUG)
LDFLAGS = -lreadline
OPTIMIZE = -03

SRC_DIR = src
SRC_FILES = main.c parse.c parse_redirs.c exec.c redirections.c heredoc.c signals.c terminal.c errors.c expand.c env.c path.c initializers.c debug.c
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
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -L$(LIB_DIR) -lft $(LDFLAGS) 

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJ_DIR)/%.o: $(SRC_DIR)%.c | $(OBJ_DIR)

$(OBJ): $(HEADERS) $(MK)

$(LIB_DIR)/$(LIB):
	$(MAKE) -C $(LIB_DIR)

run: $(NAME)
	./$(NAME)

leaks: $(NAME) $(VAL_SUPP)
	valgrind --track-origins=yes --show-leak-kinds=all --track-fds=yes --suppressions=readline.supp --leak-check=full ./$(NAME) 

leaks-file: $(NAME) $(VAL_SUPP)
	valgrind --track-origins=yes --show-leak-kinds=all --track-fds=yes --suppressions=readline.supp --leak-check=full --log-file=valgrind.out ./$(NAME) 

leaks-gdb: $(NAME) $(VAL_SUPP)
	valgrind --vgdb=yes --vgdb-error=0 --track-origins=yes --show-leak-kinds=all --track-fds=yes --suppressions=readline.supp --leak-check=full ./$(NAME)

format: 
	./format.sh $(SRC)

clean:
	rm -rf $(OBJ_DIR)
	cd $(LIB_DIR) && make clean

fclean: clean
	cd $(LIB_DIR) && rm -f $(LIB)
	rm -f $(NAME)

re: fclean all

watch:
	@while inotifywait -r -e modify,delete,move src; do \
		$(MAKE); \
		done
