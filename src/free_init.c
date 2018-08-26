/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/11 18:22:37 by nsimonov          #+#    #+#             */
/*   Updated: 2018/08/26 13:18:58 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

void			init(t_macho *obj, uint32_t prog)
{
	const unsigned char *ch_types = (unsigned char *)"abditus?";

	obj->handle_arch[x86] = handle_x86_arch;
	obj->handle_arch[x86_64] = handle_x86_64_arch;
	obj->handle_arch[FAT] = handle_fat;
	obj->handle_arch[x86_OTOOL] = ot_x86_handle;
	obj->handle_arch[x86_64_OTOOL] = ot_x86_64_handle;
	obj->handle_arch[UNKNOWN] = unknown_nm;
	obj->handle_arch[UNKNOWN_OTOOL] = unknown_otool;
	obj->program = prog;
	obj->x86_64o.seg_info = NULL;
	obj->x86_64o.obj = NULL;
	obj->x86o.seg_info = NULL;
	obj->x86_64o.obj = NULL;
	obj->args_num = 0;
	obj->fat = 0;
	obj->type_charests = ch_types;
}

static void		free_file(t_macho_info *obj)
{
	while (obj)
	{
		free(obj->name);
		free(obj);
		obj = obj->next;
	}
}

static void		free_sect(t_segment_info *obj_seg)
{
	while (obj_seg)
	{
		free(obj_seg->name);
		free(obj_seg);
		obj_seg = obj_seg->next;
	}
}

void			reinit_obj(t_macho *macho, int part)
{
	if (macho->x86_64o.seg_info)
		free_sect(macho->x86_64o.seg_info);
	if (macho->x86_64o.obj)
		free_file(macho->x86_64o.obj);
	if (macho->x86o.seg_info)
		free_sect(macho->x86o.seg_info);
	if (macho->x86o.obj)
		free_file(macho->x86o.obj);
	macho->x86_64o.obj = NULL;
	macho->x86o.obj = NULL;
	macho->x86_64o.seg_info = NULL;
	macho->x86o.seg_info = NULL;
	if (!part)
	{
		close(macho->fd);
		munmap(macho->obj_ptr, macho->ptr_size);
		macho->fat = 0;
		macho->x86_64o.obj = NULL;
		macho->x86o.obj = NULL;
	}
}

void			make_order_align(t_macho_info **macho, t_macho_info *tmp)
{
	t_macho_info	*tmp2;

	if (*macho)
	{
		tmp2 = *macho;
		if (ft_strcmp(tmp->name, tmp2->name) < 0)
		{
			tmp->next = tmp2;
			*macho = tmp;
			return ;
		}
		while (tmp2->next)
		{
			if (ft_strcmp(tmp->name, tmp2->next->name) < 0)
			{
				tmp->next = tmp2->next;
				tmp2->next = tmp;
				return ;
			}
			tmp2 = tmp2->next;
		}
		tmp2->next = tmp;
	}
}
