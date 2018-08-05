
NAME_NM = ft_nm

SRC = ./src/

INC = ./includes/


CC = gcc

CFLAGS = -Wall -Wextra -Werror $(INCLUDES)


SRC_NM =	$(SRC)main.c \
		$(SRC)errors.c \
		$(SRC)print_functions.c \
		$(SRC)sort_list.c \
		$(SRC)list_data.c \
		$(SRC)handle_x86_64.c \
		$(SRC)handle_fat.c \
		$(SRC)handle_x86.c 

all: $(NAME_NM)

$(NAME_NM):
	$(CC) $(SRC_NM) $(CFLAGS) -o $(NAME_NM)

clean:
	rm -f $(NAME_NM)


fclean: clean
	rm -f $(NAME_NM)

re: fclean all
