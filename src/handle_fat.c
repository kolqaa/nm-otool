#include "../includes/ft_nm.h"


uint32_t	endianness(uint32_t value)
{
	uint32_t result;

	result = 0;
	result |= (value & 0x000000FF) << 24;
	result |= (value & 0x0000FF00) << 8;
	result |= (value & 0x00FF0000) >> 8;
	result |= (value & 0xFF000000) >> 24;
	return (result);
}

void		handle_fat(void *ptr, t_macho *macho)
{
	int					narch;
	int					i;
	uint32_t			offset;
	struct fat_header	*fat;
	struct fat_arch		*arch;

	i = 0;
	fat = (struct fat_header *)macho->obj_ptr;
	arch = (void *)fat + sizeof(fat);
	narch = endianness(fat->nfat_arch);
	macho->fat = 1;
	while (i < narch)
	{
		if (endianness(arch->cputype) == CPU_TYPE_X86_64)
			offset = endianness(arch->offset);
		arch = (void *)arch + sizeof(*arch);
		i++;
	}

	macho->handle_arch[set_arch(ptr + offset, macho->name,macho)](ptr +
																  offset, macho);
}