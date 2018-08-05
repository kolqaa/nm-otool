#include "../includes/ft_nm.h"

int		add_section2(t_macho *macho)
{
	t_sect				*sec;
	t_sect				*sec2;
	struct section_64	*section;

	sec2 = macho->x86_64o.list;
	section = macho->x86_64o.sect;
	if ((sec = (t_sect *)malloc(sizeof(t_sect))) == NULL)
		return (printf("malloc error"));
	sec->name = strdup(section->sectname);
	sec->next = NULL;
	if (macho->x86_64o.list)
	{
		while (sec2->next)
			sec2 = sec2->next;
		sec->sect_nbr = sec2->sect_nbr + 1;
		sec2->next = sec;
	}
	else
	{
		sec->sect_nbr = 1;
		macho->x86_64o.list = sec;
	}
	return (0);
}

int		add_section(void *lc, t_macho *macho)
{
	int							i;
	int							nsects;
	struct segment_command_64	*seg;
	struct section_64			*sec;

	i = 0;
	seg = lc;
	nsects = seg->nsects;
	sec = (void *)seg + sizeof(*seg);

	macho->x86_64o.seg = lc;
	macho->x86_64o.nsects = nsects;
	macho->x86_64o.sect = sec;
	while (i < nsects)
	{
		if (add_section2(macho) == -1)
			return (-1);
		sec = (void *)sec + sizeof(*sec);
		macho->x86_64o.sect = sec;
		i++;
	}
	return (0);
}

char	get_type_by_sect(unsigned char s, t_macho *macho)
{
	t_sect	*sect;

	sect = macho->x86_64o.list;
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

char	get_type(unsigned char c, unsigned char s, t_macho *obj)
{
	unsigned char	a;

	a = APPLAY_MASK(c);
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

	if (NOT_EXTERNAL_SYM(c,a))
		c = toupper(a);
	else
		return (a);
	return (c);
}

void handle_x86_64_arch(void *ptr, t_macho *macho)
{
	int							i;

	i = 0;
	macho->x86_64o.header = (struct mach_header_64 *)ptr;
	macho->x86_64o.ncmds = macho->x86_64o.header->ncmds;
	macho->x86_64o.lc = ptr + sizeof(*(macho->x86_64o.header));
	while (i < macho->x86_64o.ncmds)
	{
		if (macho->x86_64o.lc->cmd == LC_SEGMENT_64)
			if (add_section(macho->x86_64o.lc, macho) == -1)
				return ;
		if (macho->x86_64o.lc->cmd == LC_SYMTAB)
		{
			macho->x86_64o.sym = (struct symtab_command *)macho->x86_64o.lc;
			if (add_elem(ptr, macho) == -1)
				return ;
			break ;
		}
		macho->x86_64o.lc = (void *)macho->x86_64o.lc +
				macho->x86_64o.lc->cmdsize;
		i++;
	}
	print_file(macho, x86_64);
}