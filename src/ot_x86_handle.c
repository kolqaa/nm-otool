#include "../includes/ft_nm.h"
#include "../includes/errors.h"

static int		display_otool32(struct section *sec, char *ptr, char *str)
{
	int		i;
	long unsigned int	addr;

	i = -1;
	addr = (long unsigned int)sec->addr;
	while (++i < sec->size)
	{
		if (i == 0 || i % 16 == 0)
		{
			if (i != 0)
				addr += 16;
			put_zeros(17, addr);
			print_addr(addr);
			ft_putstr("        ");
		}
		if ((str = ft_itoa_base(ptr[i], 16, 2)) == NULL)
			return -1;
		ft_putstr(str);
		ft_putchar(' ');
		free(str);
		if ((i + 1) % 16 == 0 && (i + 1) < sec->size)
			ft_putchar('\n');
	}
	ft_putchar('\n');
	return (0);
}

int		get_text_section(void *ptr, t_macho *macho)
{
	int							i;
	char						*str;

	i = -1;
	macho->x86o.seg = (struct segment_command *)macho->x86o.lc;
	macho->x86o.nsects = macho->x86o.seg->nsects;
	macho->x86o.sect = (void *)macho->x86o.seg +
						  sizeof(*(macho->x86o.seg));
	while (++i < macho->x86o.nsects)
	{
		if (!ft_strcmp(macho->x86o.sect->segname, "__TEXT")
			&& !ft_strcmp(macho->x86o.sect->sectname, "__text"))
		{
			ft_putstr("Contents of (__TEXT,__text) section\n");
			if (display_otool32(macho->x86o.sect, ptr +
					macho->x86o.sect->offset, str) == -1)
				return (-1);
		}
		macho->x86o.sect = (void *)macho->x86o.sect +
							  sizeof(*(macho->x86o.sect));
	}
	return (0);
}

void ot_x86_handle(void *ptr, struct s_macho *macho)
{
	int							i;

	i = -1;
	macho->x86o.header = (struct mach_header *)ptr;
	macho->x86o.ncmds = macho->x86o.header->ncmds;
	macho->x86o.lc = ptr + sizeof(*(macho->x86o.header));
	while (++i < macho->x86o.ncmds)
	{
		if (macho->x86o.lc->cmd == LC_SEGMENT)
			if (get_text_section(ptr, macho) < 0)
				return ;
		macho->x86o.lc = (void *)macho->x86o.lc +
							macho->x86o.lc->cmdsize;
	}
}


