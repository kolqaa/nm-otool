# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/08/12 16:54:08 by nsimonov          #+#    #+#              #
#    Updated: 2018/08/12 16:54:11 by nsimonov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_NM = ft_nm
NAME_OTOOL = ft_otool

SRC = ./src/

INC = ./includes/

CC = gcc

CFLAGS = -g -Wall -Wextra -Werror

SRC_NM =	$(SRC)ft_nm.c \
		$(SRC)errors.c \
		$(SRC)print_functions.c \
		$(SRC)free_init.c \
		$(SRC)section_types.c \
		$(SRC)handle_x86_64.c \
		$(SRC)handle_fat.c \
		$(SRC)handle_x86.c \
		$(SRC)common.c \
		$(SRC)common2.c \
		$(SRC)process_file.c \
		$(SRC)ot_x86_handle.c \
		$(SRC)ot_x86_64_handle.c \

SRC_OTOOL =	$(SRC)ft_otool.c \
		$(SRC)errors.c \
		$(SRC)print_functions.c \
		$(SRC)free_init.c \
		$(SRC)section_types.c \
		$(SRC)handle_x86_64.c \
		$(SRC)handle_fat.c \
		$(SRC)handle_x86.c \
		$(SRC)common.c \
		$(SRC)common2.c \
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
	rm -rf $(OBJ_NM)
	rm -rf $(OBJ_OTOOL)
#	rm -f $(NAME_NM)
#	rm -f $(NAME_OTOOL)

fclean: clean
	rm -f $(NAME_NM)
	rm -f $(NAME_OTOOL)

re: fclean all
