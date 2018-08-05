#include "../includes/ft_nm.h"

int		add_section2(t_obj *obj)
{
	t_sect				*sec;
	t_sect				*sec2;
	struct section_64	*section;

	sec2 = obj->x86_64o.list;
	section = obj->x86_64o.sect;
	if ((sec = (t_sect *)malloc(sizeof(t_sect))) == NULL)
		return (printf("malloc error"));
	sec->name = strdup(section->sectname);
	sec->next = NULL;
	if (obj->x86_64o.list)
	{
		while (sec2->next)
			sec2 = sec2->next;
		sec->sect_nbr = sec2->sect_nbr + 1;
		sec2->next = sec;
	}
	else
	{
		sec->sect_nbr = 1;
		obj->x86_64o.list = sec;
	}
	return (0);
}

int		add_section(void *lc, t_obj *obj)
{
	int							i;
	int							nsects;
	struct segment_command_64	*seg;
	struct section_64			*sec;

	i = 0;
	seg = lc;
	nsects = seg->nsects;
	sec = (void *)seg + sizeof(*seg);

	obj->x86_64o.seg = lc;
	obj->x86_64o.nsects = nsects;
	obj->x86_64o.sect = sec;
	while (i < nsects)
	{
		if (add_section2(obj) == -1)
			return (-1);
		sec = (void *)sec + sizeof(*sec);
		obj->x86_64o.sect = sec;
		i++;
	}
	return (0);
}

char	get_type_by_sect(unsigned char s, t_obj *obj)
{
	t_sect	*sect;

	sect = obj->x86_64o.list;
	while (sect)
	{
		if (s == sect->sect_nbr)
		{
			if (!strcmp(sect->name, SECT_DATA))
				return ('d');
			else if (!strcmp(sect->name, SECT_BSS))
				return ('b');
			else if (!strcmp(sect->name, SECT_TEXT))
				return ('t');
			else
				return ('s');
		}
		sect = sect->next;
	}
	return ('s');
}

char	get_type(unsigned char c, unsigned char s, t_obj *obj)
{
	unsigned char	a;

	a = c & N_TYPE;
	if (a == N_UNDF)
		a = 'u';
	else if (a == N_PBUD)
		a = 'u';
	else if (a == N_ABS)
		a = 'a';
	else if (a == N_SECT)
		a = get_type_by_sect(s, obj);
	else if (a == N_INDR)
		a = 'i';
	else
		a = '?';
	if (c & N_EXT && a != '?')
		c = toupper(a);
	else
		return (a);
	return (c);
}

void handle_x86_64_arch(void *ptr, t_obj *obj)
{
	int							i;
	struct load_command			*lc_tmp;

	i = 0;
	obj->x86_64o.header = (struct mach_header_64 *)ptr;
	obj->x86_64o.ncmds = obj->x86_64o.header->ncmds;
	lc_tmp = ptr + sizeof(*(obj->x86_64o.header));
	obj->x86_64o.lc = ptr + sizeof(*(obj->x86_64o.header));
	while (i < obj->x86_64o.ncmds)
	{
		if (lc_tmp->cmd == LC_SEGMENT_64)
			if (add_section(lc_tmp, obj) == -1)
				return ;
		if (obj->x86_64o.lc->cmd == LC_SYMTAB)
		{
			obj->x86_64o.sym = (struct symtab_command *)obj->x86_64o.lc;
			if (add_elem(ptr, obj) == -1)
				return ;
			break ;
		}

		obj->x86_64o.lc = (void *)obj->x86_64o.lc + obj->x86_64o.lc->cmdsize;
		lc_tmp = (void *) lc_tmp + lc_tmp->cmdsize;
		i++;
	}
	print_file(obj);

	/*i = 0;
	while (i < obj->x86_64o.ncmds)
	{
		if (obj->x86_64o.lc->cmd == LC_SYMTAB)
		{
			obj->x86_64o.sym = (struct symtab_command *)obj->x86_64o.lc;
			if (add_elem(ptr, obj) == -1)
				return ;
			break ;
		}
		obj->x86_64o.lc = (void *)obj->x86_64o.lc + obj->x86_64o.lc->cmdsize;
		i++;*/
}