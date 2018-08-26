/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ot_x86_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/12 13:16:53 by nsimonov          #+#    #+#             */
/*   Updated: 2018/08/12 14:07:35 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"
#include "../includes/errors.h"

static int		display_otool32(struct section *sec, char *ptr, char *str)
{
	uint64_t			i;
	long unsigned int	addr;

	i = 0;
	addr = (long unsigned int)sec->addr;
	while (i < sec->size)
	{
		if (i == 0 || i % HEX_BASE == 0)
		{
			if (i != 0)
				addr += HEX_BASE;
			put_zeros(x86_BASE_ZERO, addr);
			print_addr(addr);
			ft_putstr("\t");
		}
		if ((str = ft_itoa_base(ptr[i], HEX_BASE, 2)) == NULL)
			return (-1);
		ft_putstr(str);
		ft_putchar(' ');
		free(str);
		if ((i + 1) % HEX_BASE == 0 && (i + 1) < sec->size)
			ft_putchar('\n');
		i++;
	}
	return (0);
}

static int		get_text_section32(void *ptr, t_macho *macho)
{
	uint32_t		i;

	i = 0;
	macho->x86o.seg = (struct segment_command *)macho->x86o.lc;
	macho->x86o.nsects = macho->x86o.seg->nsects;
	macho->x86o.sect = (void *)macho->x86o.seg +
						sizeof(*(macho->x86o.seg));
	if (check_malformed(macho->x86o.sect, macho))
                  return -1;
	while (i < macho->x86o.nsects)
	{
		if (!ft_strcmp(macho->x86o.sect->segname, "__TEXT")
			&& !ft_strcmp(macho->x86o.sect->sectname, "__text"))
		{
			ft_putstr(macho->name);
			ft_putstr(":\n");
			ft_putstr("Contents of (__TEXT,__text) section\n");
			if ((display_otool32(macho->x86o.sect, ptr +
					macho->x86o.sect->offset, (char *) { NULL })) == -1)
				return (nm_error(macho->name, EINVAL_DUMP, OTOOL));
			ft_putchar('\n');
		}
		macho->x86o.sect = (void *)macho->x86o.sect +
					 sizeof(*(macho->x86o.sect));
		if (check_malformed((void *)macho->x86o.sect +
					(i * sizeof(*(macho->x86o.sect))), macho))
                  	return -1;
		i++;
	}
	return (0);
}

void			ot_x86_handle(void *ptr, struct s_macho *macho)
{
	uint32_t	i;

	i = 0;
	macho->x86o.header = (struct mach_header *)ptr;
	macho->x86o.ncmds = macho->x86o.header->ncmds;
	macho->x86o.lc = ptr + sizeof(*(macho->x86o.header));
	if (check_malformed(macho->x86o.lc, macho))
                 return ;
	while (i < macho->x86o.ncmds)
	{
		if (macho->x86o.lc->cmd == LC_SEGMENT)
			if (get_text_section32(ptr, macho) < 0)
				return ;
		macho->x86o.lc = (void *)macho->x86o.lc +
						macho->x86o.lc->cmdsize;
		if (check_malformed(macho->x86o.lc, macho))
			return ;
		i++;
	}
}
