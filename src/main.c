
#include "../includes/ft_nm.h"
#include "../includes/errors.h"
#include <string.h>
#include <ctype.h>


int get_file(char *file, t_obj *obj)
{
	obj->name = file;

	if ((obj->fd = open(file, O_RDONLY)) < 0)
		return (nm_error(obj->name, EINVAL_OPEN));

	if (fstat(obj->fd, &(obj->buf)) < 0)
		return (nm_error(obj->name, EINVAL_FSTAT));

	if (obj->buf.st_size <= 0)
		return (nm_error(obj->name, EISEMPTY));
	else
		obj->ptr_size = obj->buf.st_size;

	if (S_ISREG(obj->buf.st_mode))
		return (EXIT_SUCCESS);
	else
		return (nm_error(obj->name, EISDIR));
}

int set_arch(void *ptr)
{
	uint32_t  magic_number;
	magic_number = *(uint32_t *)ptr;

	if (magic_number == MH_MAGIC || magic_number == MH_CIGAM) {
		printf("X86 binary\n");
		return (x86);
	}
	else if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64) {
		printf("x664 binary\n");
		return (x86_64);
	}
	else if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM) {
		printf("FAT binary\n");
		return (FAT);
	}
	else
		return (UNKNOWN);
}



int mmap_obj(t_obj *obj)
{
	void *ptr;

	if ((ptr = mmap(0, obj->buf.st_size, PROT_READ, MAP_PRIVATE, obj->fd, 0))
		== MAP_FAILED)
		return (nm_error(obj->name, ENOMEM));

	obj->obj_ptr = ptr;
	obj->handle_arch[set_arch(ptr)](ptr, obj);

	return 0;
}

void init(t_obj *obj)
{
	obj->handle_arch[x86] = handle_x86_arch;
	obj->handle_arch[x86_64] = handle_x86_64_arch;
	obj->handle_arch[FAT] = handle_fat;
	//obj->handle_arch[UNKNOWN] = arch_miss;
	obj->x86_64o.list = NULL;
	obj->x86_64o.file = NULL;
	obj->args_num = 0;
}

void	free_file(t_file *file)
{
	while (file)
	{
		free(file->name);
		free(file);
		file = file->next;
	}
}

void	free_sect(t_sect *sect)
{
	while (sect)
	{
		free(sect->name);
		free(sect);
		sect = sect->next;
	}
}

void reinit_obj(t_obj *obj)
{
	close(obj->fd);
	munmap(obj->obj_ptr, obj->ptr_size);
	if(obj->x86_64o.list)
		free_sect(obj->x86_64o.list);
	if (obj->x86_64o.file)
		free_file(obj->x86_64o.file);

	obj->x86_64o.list = NULL;
	obj->x86_64o.file = NULL;
}

int with_args(int argc, char **argv, t_obj *obj)
{
	int iter = 0;
	int ret;

	ret = 0;
	if (argc > 2)
		obj->args_num = 1;
	while (++iter < argc)
	{
		if (get_file(argv[iter], obj) < 0)
		{
			ret = -1;
			continue;
		}

		if (mmap_obj(obj) < 0)
		{
			ret = -1;
			continue;
		}

		reinit_obj(obj);
	}

	return (ret < 0 ? -1 : 1);
}

int no_args(t_obj *obj)
{
	if (get_file("a.out", obj) < 0)
		return -1;

	if (mmap_obj(obj) < 0)
		return -1;

	reinit_obj(obj);

	return 0;
}

int main(int argc, char **argv)
{
	int ret;

	ret = 0;
	t_obj obj;

	init(&obj);
	if (argc == 1)
		ret = no_args(&obj);
	else
		ret = with_args(argc, argv, &obj);

	/* TODO */
	/* munmap ptr after usage free mem */
	return 0;
}
