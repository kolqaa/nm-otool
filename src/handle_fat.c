#include "../includes/ft_nm.h"


static uint32_t	big_little_converter(uint32_t value)
{
	uint32_t res;

	res = 0;
	res |= (value & 0x000000FF) << 24;
	res |= (value & 0x0000FF00) << 8;
	res |= (value & 0x00FF0000) >> 8;
	res |= (value & 0xFF000000) >> 24;
	return (res);
}

void		handle_fat(void *ptr, t_macho *macho)
{
	int					i;
	int					narch;
	uint32_t			offset;
	struct fat_header	*fat;
	struct fat_arch		*arch;

	i = 0;
	fat = (struct fat_header *)macho->obj_ptr;
	arch = (void *)fat + sizeof(fat);
	narch = big_little_converter(fat->nfat_arch);
	macho->fat = 1;
	while (i < narch)
	{
		if (big_little_converter(arch->cputype) == CPU_TYPE_X86_64)
			offset = big_little_converter(arch->offset);
		arch = (void *)arch + sizeof(*arch);
		i++;
	}

	macho->handle_arch[set_arch(ptr +
								offset, macho->name,macho)](ptr +
															offset, macho);
}