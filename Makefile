
NAME_NM = ft_nm

SRC = ./src/

INC = ./includes/


CC = gcc

CFLAGS = -Wall -Wextra -Werror $(INCLUDES)


SRC_NM =	$(SRC)main.c \
		$(SRC)errors.c \
		$(SRC)print_functions.c 

all: $(NAME_NM)

$(NAME_NM):
	$(CC) $(SRC_NM) -o $(NAME_NM)

clean:
	rm -f $(NAME_NM)


fclean: clean
	rm -f $(NAME_NM)

re: fclean all
