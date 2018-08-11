
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

int set_arch(void *ptr, char *name, t_macho *macho)
{
	uint32_t  magic_number;
	magic_number = *(uint32_t *)ptr;

	if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
		return ((macho->program == NM) ? (macho->arch = x86, x86) :
				(macho->arch = x86, x86_OTOOL));
	else if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
		return ((macho->program == NM) ? (macho->arch = x86_64, x86_64) :
				(macho->arch = x86_64, x86_64_OTOOL));
	else if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
		return (FAT);
	else
		exit(nm_error(name, ENOEXEC));
}

int mmap_obj(t_macho *macho)
{
	void *ptr;

	if ((ptr = mmap(0, macho->buf.st_size, PROT_READ, MAP_PRIVATE, macho->fd, 0))
		== MAP_FAILED)
		return (nm_error(macho->name, ENOMEM));

	macho->obj_ptr = ptr;
	macho->handle_arch[set_arch(ptr, macho->name, macho)](ptr, macho);

	return 0;
}

void init(t_macho *obj, uint32_t prog)
{
	const unsigned char *ch_types =  (unsigned char *)"abditus?";

	obj->handle_arch[x86] = handle_x86_arch;
	obj->handle_arch[x86_64] = handle_x86_64_arch;
	obj->handle_arch[FAT] = handle_fat;
	obj->handle_arch[x86_OTOOL] = ot_x86_handle;
	obj->handle_arch[x86_64_OTOOL] = ot_x86_64_handle;

	obj->program = prog;
	obj->x86_64o.seg_info = NULL;
	obj->x86_64o.obj = NULL;
	obj->x86o.seg_info = NULL;
	obj->x86_64o.obj = NULL;
	obj->args_num = 0;
	obj->fat = 0;
	obj->type_charests = ch_types;
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
