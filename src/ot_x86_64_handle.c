/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ot_x86_64_handle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/11 18:25:00 by nsimonov          #+#    #+#             */
/*   Updated: 2018/08/12 14:06:50 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"
#include "../includes/errors.h"

char		read_tab(int i)
{
	char	*tab;

	tab = "0123456789abcdefghijklmnopqrstuvwxyz";
	return (tab[i]);
}

char		*ft_itoa_base(int val, int base, int output_size)
{
	char			buffer[output_size + 1];
	char			*p;
	unsigned int	unsigned_val;

	unsigned_val = (unsigned int)val;
	if (base > 36 || base < 2)
		return (NULL);
	buffer[output_size] = 0;
	p = buffer + output_size - 1;
	*p = '0';
	if (unsigned_val > 0)
	{
		while (unsigned_val > 0)
		{
			*p-- = read_tab(unsigned_val % base);
			unsigned_val = unsigned_val / base;
			if (p < buffer)
				break ;
		}
		p++;
	}
	while (p > buffer)
		*--p = '0';
	return (ft_strdup(p));
}

static int	display_otool64(struct section_64 *sec, char *ptr, char *str)
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
			put_zeros(x86_64_BASE_ZERO, addr);
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

static int	get_text_section64(void *ptr, t_macho *macho)
{
	uint32_t		i;

	i = 0;
	macho->x86_64o.seg = (struct segment_command_64 *)macho->x86_64o.lc;
	macho->x86_64o.nsects = macho->x86_64o.seg->nsects;
	macho->x86_64o.sect = (void *)macho->x86_64o.seg +
			sizeof(*(macho->x86_64o.seg));
	if (check_malformed(macho->x86_64o.sect, macho))
		return -1;
	while (i < macho->x86_64o.nsects)
	{
		if (!ft_strcmp(macho->x86_64o.sect->segname, "__TEXT")
			&& !ft_strcmp(macho->x86_64o.sect->sectname, "__text"))
		{
			ft_putstr(macho->name);
			ft_putstr(":\n");
			ft_putstr("Contents of (__TEXT,__text) section\n");
			if ((display_otool64(macho->x86_64o.sect, ptr +
					macho->x86_64o.sect->offset, (char *) { NULL })) == -1)
				return (nm_error(macho->name, EINVAL_DUMP, OTOOL));
			ft_putchar('\n');
		}
		macho->x86_64o.sect = (void *)macho->x86_64o.sect +
				 sizeof(*(macho->x86_64o.sect));
		if (check_malformed((void *)macho->x86_64o.sect +
					(i * sizeof(*(macho->x86_64o.sect))), macho))
			return -1;
		i++;
	}
	return (0);
}

void		ot_x86_64_handle(void *ptr, struct s_macho *macho)
{
	uint32_t	i;

	i = 0;
	macho->x86_64o.header = (struct mach_header_64 *)ptr;
	macho->x86_64o.ncmds = macho->x86_64o.header->ncmds;
	macho->x86_64o.lc = ptr + sizeof(*(macho->x86_64o.header));
	if (check_malformed(macho->x86_64o.lc, macho))
		return ;
	while (i < macho->x86_64o.ncmds)
	{
		if (macho->x86_64o.lc->cmd == LC_SEGMENT_64)
			if (get_text_section64(ptr, macho) < 0)
				return ;
		macho->x86_64o.lc = (void *)macho->x86_64o.lc +
				macho->x86_64o.lc->cmdsize;
		if (check_malformed(macho->x86_64o.lc, macho))
			return ;
		i++;
	}
}
