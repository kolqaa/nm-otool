#include "../includes/ft_nm.h"


int		add_section2_32(t_macho *macho)
{
	t_sect				*sec;
	t_sect				*sec2;
	struct section		*section;

	sec2 = macho->x86o.list;
	section = macho->x86o.sect;
	if ((sec = (t_sect *)malloc(sizeof(t_sect))) == NULL)
		return -1;
	sec->name = strdup(section->sectname);
	sec->next = NULL;
	if (macho->x86o.list)
	{
		while (sec2->next)
			sec2 = sec2->next;
		sec->sect_nbr = sec2->sect_nbr + 1;
		sec2->next = sec;
	}
	else
	{
		sec->sect_nbr = 1;
		macho->x86o.list = sec;
	}
	return (0);
}

int		add_section_32(void *lc, t_macho *macho)
{
	int							i;
	int							nsects;
	struct segment_command		*seg;
	struct section				*sec;

	i = 0;
	seg = lc;
	nsects = seg->nsects;
	sec = (void *)seg + sizeof(*seg);

	macho->x86o.seg = lc;
	macho->x86o.nsects = nsects;
	macho->x86o.sect = sec;
	while (i < nsects)
	{
		if (add_section2_32(macho) == -1)
			return (-1);
		sec = (void *)sec + sizeof(*sec);
		i++;
	}
	return (0);
}

int		add_elem2_32(void *ptr, char *str, t_macho *macho)
{
	t_file			*tmp;
	struct nlist	array;

	array = *(struct nlist *)ptr;
	if (get_type(array.n_type, array.n_sect, macho) == '?'
		|| !strlen(str + array.n_un.n_strx))
		return (0);
	if ((tmp = (t_file *)malloc(sizeof(t_file))) == NULL)
		return -1;
	tmp->name = strdup(str + array.n_un.n_strx);
	tmp->type = get_type(array.n_type, array.n_sect, macho);
	tmp->value = 0;
	if (array.n_value)
		tmp->value = (unsigned long)array.n_value;
	tmp->show_addr = 0;
	if ((array.n_type & N_TYPE) != N_UNDF)
		tmp->show_addr = 1;
	tmp->arch = 32;
	tmp->next = NULL;
	if (macho->x86o.file)
		sort_list(&macho->x86o.file, tmp);
	else
		macho->x86o.file = tmp;
	return (0);
}

int		add_elem_32(void *ptr, t_macho *macho)
{
	int						i;
	char					*str;

	i = 0;
	macho->x86o.el = ptr + macho->x86o.sym->symoff;
	str = ptr + macho->x86o.sym->stroff;
	while (i < (int)macho->x86o.sym->nsyms)
	{
		if (add_elem2_32(&macho->x86o.el[i], str, macho) == -1)
			return (-1);
		i++;
	}
	return (0);
}

void handle_x86_arch(void *ptr, t_macho *macho)
{
	int						i;

	i = 0;
	macho->x86o.header = (struct mach_header *)ptr;
	macho->x86o.ncmds = macho->x86o.header->ncmds;
	macho->x86o.lc = ptr + sizeof(*(macho->x86o.header));
	while (i < macho->x86o.ncmds)
	{
		if (macho->x86o.lc->cmd == LC_SEGMENT)
		{
			if (add_section_32(macho->x86o.lc, macho) == -1)
				return ;
		}

		if (macho->x86o.lc->cmd == LC_SYMTAB)
		{
			macho->x86o.sym = (struct symtab_command *)macho->x86o.lc;
			if (add_elem_32(ptr, macho) == -1)
				return ;
			break ;
		}

		macho->x86o.lc = (void *)macho->x86o.lc + macho->x86o.lc->cmdsize;
		i++;
	}


	print_file(macho, x86);
}