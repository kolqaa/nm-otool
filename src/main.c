#include <mach-o/nlist.h>
#include "../includes/ft_nm.h"
#include "../includes/errors.h"

void print_output(int syms, int symoff, int stroff, char *ptr)
{
	int 	i = 0;
	char 	*str;
	struct	nlist_64 *el;

	el = (void*)ptr + symoff;
	str = ptr + stroff;
	for (i = 0; i < syms; i++) 
	{
		printf("%s\n", str + el[i].n_un.n_strx);
	}
}

void handle_symtab(struct obj_info *obj, void *ptr)
{
	char *str;

	obj->x86_64o.el = ptr + obj->x86_64o.sym->symoff;
	str = ptr + obj->x86_64o.sym->stroff;

	printf("%d\n", obj->x86_64o.sym->nsyms);
	printf("%s\n", str + obj->x86_64o.el[0].n_un.n_strx);

}

void handle_64(char *ptr, struct obj_info *obj)
{
	int i = -1;

	while (++i < obj->x86_64o.ncmds)
	{
		obj->x86_64o.lc = (void *)obj->x86_64o.lc +
				obj->x86_64o.lc->cmdsize;

		if (obj->x86_64o.lc->cmd == LC_SYMTAB)
		{
			obj->x86_64o.sym = (struct symtab_command *)obj->x86_64o.lc;
			handle_symtab(obj, ptr);
			printf("LC_SYMTAB\n");
		}
		else if (obj->x86_64o.lc->cmd == LC_SEGMENT_64)
		{
		//	obj->x86_64o.seg = (struct segment_command_64 *)obj->x86_64o.lc;
		//	handle_segment_command(format, seg, is_64);
			printf("LC_SEGMENT\n");
		}
	}
}


int get_file(char *file, struct obj_info *obj)
{
	obj->name = file;

	if ((obj->fd = open(file, O_RDONLY)) < 0)
		return (nm_error(obj->name, EINVAL_OPEN));

	if (fstat(obj->fd, &(obj->buf)) < 0)
		return (nm_error(obj->name, EINVAL_FSTAT));

	if (obj->buf.st_size <= 0)
		return (nm_error(obj->name, EISEMPTY));

	if (S_ISREG(obj->buf.st_mode))
		return (EXIT_SUCCESS);
	else
		return (nm_error(obj->name, EISDIR));
}



int set_arch(void *ptr)
{
	int magic_number;
	magic_number = *(int *)ptr;

	if (magic_number == MH_MAGIC_64)
		return x86_64;
	else if (magic_number == MH_MAGIC)
		return x86;
	else
		return UNKNOWN;
}

void handle_x86_arch(void *ptr, struct obj_info *obj)
{
	printf("x86 obj proceed");
}


void handle_x86_64_arch(void *ptr, struct obj_info *obj)
{
	obj->x86_64o.header = (struct mach_header_64 *)ptr;
	obj->x86_64o.ncmds = obj->x86_64o.header->ncmds;
	obj->x86_64o.sizeofcmds = obj->x86_64o.header->sizeofcmds;

	obj->x86_64o.lc = ptr + sizeof(*(obj->x86_64o.header));

	handle_64(ptr, obj);
}

int get_info(struct obj_info *obj)
{
	void *ptr;

	if ((ptr = mmap(0, obj->buf.st_size, PROT_READ, MAP_PRIVATE, obj->fd, 0))
			== MAP_FAILED)
		return (nm_error(obj->name, ENOMEM));

	obj->obj_ptr = ptr;
	obj->handle_arch_clbk[set_arch(ptr)](ptr, obj);

	return 0;
}

void arch_miss(void)
{
	printf("Unsuported arch\n");
}

int main(int argc, char **argv)
{
	int iter = 0;
	struct obj_info obj;

	obj.handle_arch_clbk[x86] = handle_x86_arch;
	obj.handle_arch_clbk[x86_64] = handle_x86_64_arch;
	obj.handle_arch_clbk[UNKNOWN] = arch_miss;

	if (argc == 1)
	{
		if (get_file("a.out", &obj) < 0)
			return 0;

		if (get_info(&obj) < 0)
			return 0;

		//if (print_info(obj) < 0)
			//return 0;
		return 0;
	}

	while (++iter < argc)
	{
		if (get_file(argv[iter], &obj) < 0)
			continue ;
			//return 0;

		if (get_info(&obj) < 0)
			continue ;
        //if (print_info(obj) < 0)
        // continue ;
	}

	return 0;
	//nm(ptr);
	//if (munmap(ptr, buf.st_size) < 0)
		return 0;

}
