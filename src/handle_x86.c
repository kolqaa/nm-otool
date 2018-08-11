#include "../includes/ft_nm.h"
#include "../includes/errors.h"


int		add_segment32_help(t_macho *macho)
{
	t_segment_info		*seg;
	t_segment_info		*tmp_seg;
	struct section		*section;

	tmp_seg = macho->x86o.seg_info;
	section = macho->x86o.sect;
	if ((seg = (t_segment_info *)malloc(sizeof(t_segment_info))) == NULL)
		return (nm_error(macho->name, ENOMEM));
	seg->name = ft_strdup(section->sectname);
	seg->next = NULL;
	if (macho->x86o.seg_info)
	{
		FIND_SEGMENT(tmp_seg);
		seg->s_num = tmp_seg->s_num + 1;
		tmp_seg->next = seg;
	}
	else
	{
		seg->s_num = 1;
		macho->x86o.seg_info = seg;
	}
	return (0);
}

int		add_segment32_node(void *lc, t_macho *macho)
{
	int							i;

	i = 0;
	macho->x86o.seg = lc;
	macho->x86o.nsects = macho->x86o.seg->nsects;
	macho->x86o.sect = (void*)macho->x86o.seg + sizeof(*(macho->x86o.seg));
	while (i < macho->x86o.nsects)
	{
		if (add_segment32_help(macho) < 0)
			return (nm_error(macho->name, EINVAL_SEG));
		macho->x86o.sect = (void *)macho->x86o.sect + sizeof(*(macho->x86o.sect));
		i++;
	}
	return (0);
}

int		add_symtab32_help(int i, char *str, t_macho *macho)
{
	t_macho_info	*tmp;

	if (get_type(macho->x86o.el[i].n_type,
				 macho->x86o.el[i].n_sect, macho) == '?' ||
			!ft_strlen(str + macho->x86o.el[i].n_un.n_strx))
		return (0);
	if ((tmp = (t_macho_info *)malloc(sizeof(t_macho_info))) == NULL)
		return (nm_error(macho->name, ENOMEM));
	tmp->name = ft_strdup(str + macho->x86o.el[i].n_un.n_strx);
	tmp->type = get_type(macho->x86o.el[i].n_type,
						 macho->x86o.el[i].n_sect, macho);
	tmp->value = 0;
	tmp->arch = x86;
	tmp->next = NULL;
	if (macho->x86o.el[i].n_value)
		tmp->value = (unsigned long)macho->x86o.el[i].n_value;
	tmp->displayable = 0;
	if (SYM_DISPLAYABLE(macho->x86o.el[i]))
		tmp->displayable = 1;
	if (macho->x86o.obj)
		make_order_align(&macho->x86o.obj, tmp);
	else
		macho->x86o.obj = tmp;
	return (0);
}

int		add_symtab32_node(void *ptr, t_macho *macho)
{
	int						i;
	char					*str;

	i = 0;
	macho->x86o.el = ptr + macho->x86o.sym->symoff;
	str = ptr + macho->x86o.sym->stroff;
	while (i < (int)macho->x86o.sym->nsyms)
	{
		if (add_symtab32_help(i, str, macho) < 0)
			return (nm_error(macho->name, EINVAL_SYM));
		i++;
	}
	return (0);
}

void handle_x86_arch(void *ptr, t_macho *macho)
{
	int						i;

	i = -1;
	macho->x86o.header = (struct mach_header *)ptr;
	macho->x86o.ncmds = macho->x86o.header->ncmds;
	macho->x86o.lc = ptr + sizeof(*(macho->x86o.header));
	while (++i < macho->x86o.ncmds)
	{
		if (macho->x86o.lc->cmd == LC_SEGMENT)
			if (add_segment32_node(macho->x86o.lc, macho) < 0)
				return ;
		if (macho->x86o.lc->cmd == LC_SYMTAB)
		{
			macho->x86o.sym = (struct symtab_command *)macho->x86o.lc;
			if (add_symtab32_node(ptr, macho) < 0)
				return ;
			break ;
		}
		macho->x86o.lc = (void *)macho->x86o.lc + macho->x86o.lc->cmdsize;
	}
	display_nm(macho, x86);
}