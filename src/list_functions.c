#include "../includes/ft_nm.h"

static void	free_file(t_macho_info *obj)
{
	while (obj)
	{
		free(obj->name);
		free(obj);
		obj= obj->next;
	}
}

static void	free_sect(t_segment_info *obj_seg)
{
	while (obj_seg)
	{
		free(obj_seg->name);
		free(obj_seg);
		obj_seg = obj_seg->next;
	}
}

void reinit_obj(t_macho *macho)
{
	close(macho->fd);
	munmap(macho->obj_ptr, macho->ptr_size);
	if(macho->x86_64o.seg_info)
		free_sect(macho->x86_64o.seg_info);
	if (macho->x86_64o.obj)
		free_file(macho->x86_64o.obj);

	if (macho->x86o.seg_info)
		free_sect(macho->x86o.seg_info);
	if (macho->x86o.obj)
		free_file(macho->x86o.obj);

	macho->fat = 0;
	macho->x86_64o.seg_info = NULL;
	macho->x86_64o.obj = NULL;
	macho->x86o.obj = NULL;
	macho->x86o.seg_info = NULL;
}

void	make_order_align(t_macho_info **macho, t_macho_info *tmp)
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