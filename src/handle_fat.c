#include "../includes/ft_nm.h"

void		handle_fat(void *ptr, t_macho *macho)
{
	int					i;
	int					narch;
	uint32_t			offset;
	uint32_t			swaped;
	struct fat_header	*fat;
	struct fat_arch		*arch;

	i = 0;
	narch = 0;
	offset = 0;
	fat = (struct fat_header *)macho->obj_ptr;
	arch = (void *)fat + sizeof(fat);
	BIG_LITTLE(narch, fat->nfat_arch);
	macho->fat = 1;
	while (i < narch)
	{
		swaped = 0;
		if (BIG_LITTLE(swaped, arch->cputype) == CPU_TYPE_X86_64)
			BIG_LITTLE(offset, arch->offset);
		arch = (void *)arch + sizeof(*arch);
		i++;
	}
	macho->handle_arch[set_arch(ptr + offset, macho)](ptr + offset, macho);
}