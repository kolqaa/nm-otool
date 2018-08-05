#include "../includes/ft_nm.h"


void handle_x86_arch(void *ptr, t_obj *obj)
{
	printf("HERE\n");
	int offset = (int)ptr + obj->args_num;
	printf("86 ARCH %d\n", offset);
	exit (0);
}