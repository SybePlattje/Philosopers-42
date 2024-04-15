NAME = philo
HEADER = ./include/philo.h

SRC_DIR = ./src
SRC = $(SRC_DIR)/main.c \
	$(SRC_DIR)/time.c \
	$(SRC_DIR)/error_and_free.c \
	$(SRC_DIR)/initializer.c \
	$(SRC_DIR)/utils.c \
	$(SRC_DIR)/routine.c \
	$(SRC_DIR)/philo_routine.c \
	$(SRC_DIR)/eating.c \
	$(SRC_DIR)/sleeping.c \
	$(SRC_DIR)/eating_utils.c \
	$(SRC_DIR)/routine_utils.c \

OBJ_DIR = ./obj
$(shell mkdir -p $(OBJ_DIR))

OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

CC = cc
CFLAG = -Wall -Werror -Wextra -g3 #-fsanitize=thread

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
	@cp $(HEADER) $(OBJ_DIR)
	@$(CC) $(CFLAG) $(OBJ) -lpthread -o $(NAME)
	@echo Done compiling!

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAG) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo Deleted objects!

fclean: clean
	@rm -rf $(NAME)
	@echo Deleted program!

re: fclean all

.PHONY: all clean fclean re