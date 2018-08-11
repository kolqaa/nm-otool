NAME_NM = ft_nm
NAME_OTOOL = ft_otool

SRC = ./src/

INC = ./includes/

CC = gcc

CFLAGS = -Wall -Wextra -Werror

SRC_NM =	$(SRC)ft_nm.c \
		$(SRC)errors.c \
		$(SRC)print_functions.c \
		$(SRC)list_functions.c \
		$(SRC)section_types.c \
		$(SRC)handle_x86_64.c \
		$(SRC)handle_fat.c \
		$(SRC)handle_x86.c \
		$(SRC)common.c \
		$(SRC)process_file.c \
		$(SRC)ot_x86_handle.c \
		$(SRC)ot_x86_64_handle.c \

SRC_OTOOL =	$(SRC)ft_otool.c \
		$(SRC)errors.c \
		$(SRC)print_functions.c \
		$(SRC)list_functions.c \
		$(SRC)section_types.c \
		$(SRC)handle_x86_64.c \
		$(SRC)handle_fat.c \
		$(SRC)handle_x86.c \
		$(SRC)common.c \
		$(SRC)process_file.c \
		$(SRC)ot_x86_handle.c \
		$(SRC)ot_x86_64_handle.c \

OBJ_NM		= $(SRC_NM:.c=.o)
OBJ_OTOOL	= $(SRC_OTOOL:.c=.o)

all:		$(NAME_NM) $(NAME_OTOOL)

$(NAME_NM):	$(OBJ_NM)
	$(CC) $(CFLAGS) -o $(NAME_NM) $(OBJ_NM)

$(NAME_OTOOL):	$(OBJ_OTOOL)
	$(CC) $(CFLAGS) -o $(NAME_OTOOL) $(OBJ_OTOOL)

%.o:		%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(NAME_NM)
	rm -f $(NAME_OTOOL)

fclean: clean
	rm -f $(NAME_NM)
	rm -f $(NAME_OTOOL)

re: fclean all
