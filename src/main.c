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

void handle_64(char *ptr)
{
	int			ncmds;
	int			i;
	struct mach_header_64	*header;
	struct load_command	*lc;
	struct symtab_command	*sym;
	
	header = (struct mach_header_64 *)ptr;
	ncmds = header->ncmds;

	lc = (void *)ptr + sizeof(*header);
	for (i = 0; i < ncmds; ++i)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command*)lc;
			printf("nb symboles %d\n", sym->nsyms);
			print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);
			break ;
		}
		lc = (void *)lc + lc->cmdsize;
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
	{
        return x86_64;
	}
}


void handle_x86_64_arch(void *ptr)
{
    handle_64(ptr);
}

int get_info(struct obj_info *obj)
{
	void *ptr;

	if ((ptr = mmap(0, obj->buf.st_size, PROT_READ, MAP_PRIVATE, obj->fd, 0))
			== MAP_FAILED)
		return (nm_error(obj->name, ENOMEM));

	obj->obj_ptr = ptr;
    obj->arch = set_arch(ptr);
	return 0;
}

int main(int argc, char **argv)
{
	struct obj_info obj;
	int iter = 0;

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
        //if (print_info) < 0)
        // continue ;
		printf("%s\n", argv[iter]);
	}

	return 0;
	//nm(ptr);
	//if (munmap(ptr, buf.st_size) < 0)
		return 0;

}
