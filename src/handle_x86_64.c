/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_x86_64.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/11 18:18:47 by nsimonov          #+#    #+#             */
/*   Updated: 2018/08/12 15:53:25 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"
#include "../includes/errors.h"

static int		add_segment64_help(t_macho *macho)
{
	t_segment_info		*seg;
	t_segment_info		*tmp_seg;
	struct section_64	*section;

	tmp_seg = macho->x86_64o.seg_info;
	section = macho->x86_64o.sect;
	if ((seg = (t_segment_info *)malloc(sizeof(t_segment_info))) == NULL)
		return (nm_error(macho->name, ENOMEM, NM));
	seg->name = ft_strdup(section->sectname);
	seg->next = NULL;
	if (macho->x86_64o.seg_info)
	{
		FIND_SEGMENT(tmp_seg);
		seg->s_num = tmp_seg->s_num + 1;
		tmp_seg->next = seg;
	}
	else
	{
		seg->s_num = 1;
		macho->x86_64o.seg_info = seg;
	}
	return (0);
}

static int		add_segment64_node(void *lc, t_macho *macho)
{
	uint32_t	i;

	i = 0;
	macho->x86_64o.seg = lc;
	macho->x86_64o.nsects = macho->x86_64o.seg->nsects;
	macho->x86_64o.sect = (void*)macho->x86_64o.seg +
						sizeof(*(macho->x86_64o.seg));
	if (check_malformed(macho->x86_64o.sect, macho))
		return -1;
	while (i < macho->x86_64o.nsects)
	{
		if (add_segment64_help(macho) < 0)
			return (nm_error(macho->name, EINVAL_SEG, NM));
		macho->x86_64o.sect = (void *)macho->x86_64o.sect +
			 sizeof(*(macho->x86_64o.sect));
		if (check_malformed((void*)macho->x86_64o.sect +
			(i * sizeof(*(macho->x86_64o.sect))), macho))
			return -1;
		i++;
	}
	return (0);
}

static int		add_symtab64_help(int i, char *str, t_macho *macho)
{
	t_macho_info	*tmp;

	if (get_type(macho->x86_64o.el[i].n_type,
	macho->x86_64o.el[i].n_sect, macho, macho->x86_64o.el[i].n_value) == '?' ||
				!ft_strlen(str + macho->x86_64o.el[i].n_un.n_strx))
		return (0);
	if ((tmp = (t_macho_info *)malloc(sizeof(t_macho_info))) == NULL)
		return (nm_error(macho->name, ENOMEM, NM));
	tmp->name = ft_strdup(str + macho->x86_64o.el[i].n_un.n_strx);
	tmp->type = get_type(macho->x86_64o.el[i].n_type,
		macho->x86_64o.el[i].n_sect, macho, macho->x86_64o.el[i].n_value);
	tmp->arch = x86_64;
	tmp->next = NULL;
	!macho->x86_64o.el[i].n_value ? (tmp->value = 0) :
		(tmp->value = (unsigned long)macho->x86_64o.el[i].n_value);
	if (((macho->x86_64o.el[i].n_type & N_TYPE) == N_UNDF) && tmp->value != 0)
		tmp->displayable = 1;
	else if (SYM_DISPLAYABLE(macho->x86_64o.el[i]))
		tmp->displayable = 1;
	else
		tmp->displayable = 0;
	macho->x86_64o.obj ? make_order_align(&macho->x86_64o.obj, tmp) :
						(macho->x86_64o.obj = tmp);
	return (0);
}

static int		add_symtab64_node(void *ptr, t_macho *obj)
{
	int						i;
	char					*str;

	i = 0;
	obj->x86_64o.el = ptr + obj->x86_64o.sym->symoff;
	if (check_malformed(obj->x86_64o.el, obj))
		return -1;
	str = ptr + obj->x86_64o.sym->stroff;
	if (check_malformed(str, obj))
		return -1;
	while (i < (int)obj->x86_64o.sym->nsyms)
	{
		if (add_symtab64_help(i, str, obj) < 0)
			return (nm_error(obj->name, EINVAL_SYM, NM));
		i++;
	}
	return (0);
}

#include <stdio.h>

void			handle_x86_64_arch(void *ptr, t_macho *macho)
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
			if (add_segment64_node(macho->x86_64o.lc, macho) < 0)
				return ;
		if (macho->x86_64o.lc->cmd == LC_SYMTAB)
		{
			macho->x86_64o.sym = (struct symtab_command *)macho->x86_64o.lc;
			if (add_symtab64_node(ptr, macho) < 0)
				return ;
			break ;
		}
		macho->x86_64o.lc = (void *)macho->x86_64o.lc +
							macho->x86_64o.lc->cmdsize;
		if (check_malformed(macho->x86_64o.lc, macho))
	                return ;
		i++;
	}
	display_nm(macho, x86_64);
}
