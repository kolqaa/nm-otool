
#include "../includes/ft_nm.h"
#include "../includes/errors.h"
#include <string.h>
#include <ctype.h>


int get_file(char *file, t_macho *macho)
{
	macho->name = file;

	if ((macho->fd = open(file, O_RDONLY)) < 0)
		return (nm_error(macho->name, EINVAL_OPEN));

	if (fstat(macho->fd, &(macho->buf)) < 0)
		return (nm_error(macho->name, EINVAL_FSTAT));

	if (macho->buf.st_size <= 0)
		return (nm_error(macho->name, EISEMPTY));
	else
		macho->ptr_size = macho->buf.st_size;

	if (S_ISREG(macho->buf.st_mode))
		return (EXIT_SUCCESS);
	else
		return (nm_error(macho->name, EISDIR));
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



int mmap_obj(t_macho *macho)
{
	void *ptr;

	if ((ptr = mmap(0, macho->buf.st_size, PROT_READ, MAP_PRIVATE, macho->fd, 0))
		== MAP_FAILED)
		return (nm_error(macho->name, ENOMEM));

	macho->obj_ptr = ptr;
	macho->handle_arch[set_arch(ptr)](ptr, macho);

	return 0;
}

void init(t_macho *obj)
{
	obj->handle_arch[x86] = handle_x86_arch;
	obj->handle_arch[x86_64] = handle_x86_64_arch;
	obj->handle_arch[FAT] = handle_fat;
	//macho->handle_arch[UNKNOWN] = arch_miss;
	obj->x86_64o.list = NULL;
	obj->x86_64o.file = NULL;
	obj->x86o.list = NULL;
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

void reinit_obj(t_macho *macho)
{
	close(macho->fd);
	munmap(macho->obj_ptr, macho->ptr_size);
	if(macho->x86_64o.list)
		free_sect(macho->x86_64o.list);
	if (macho->x86_64o.file)
		free_file(macho->x86_64o.file);

	if (macho->x86o.list)
		free_sect(macho->x86o.list);
	if (macho->x86o.file)
		free_file(macho->x86o.file);

	macho->x86_64o.list = NULL;
	macho->x86_64o.file = NULL;
	macho->x86o.file = NULL;
	macho->x86o.list = NULL;
}

int with_args(int argc, char **argv, t_macho *macho)
{
	int iter = 0;
	int ret;

	ret = 0;
	if (argc > 2)
		macho->args_num = 1;
	while (++iter < argc)
	{
		if (get_file(argv[iter], macho) < 0)
		{
			ret = -1;
			continue;
		}

		if (mmap_obj(macho) < 0)
		{
			ret = -1;
			continue;
		}

		reinit_obj(macho);
	}

	return (ret < 0 ? -1 : 1);
}

int no_args(t_macho *macho)
{
	if (get_file("a.out", macho) < 0)
		return -1;

	if (mmap_obj(macho) < 0)
		return -1;

	reinit_obj(macho);

	return 0;
}

int main(int argc, char **argv)
{
	int ret;

	ret = 0;
	t_macho macho;

	init(&macho);
	if (argc == 1)
		ret = no_args(&macho);
	else
		ret = with_args(argc, argv, &macho);

	/* TODO */
	/* munmap ptr after usage free mem */
	return 0;
}
