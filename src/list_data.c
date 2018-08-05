#include "../includes/ft_nm.h"

int		add_elem2(void *ptr, char *stringtable, t_obj *obj)
{
	t_file			*tmp;
	struct nlist_64	array;

	array = *(struct nlist_64 *)ptr;
	if (get_type(array.n_type, array.n_sect, obj) == '?'
		|| !strlen(stringtable + array.n_un.n_strx))
		return (0);
	if ((tmp = (t_file *)malloc(sizeof(t_file))) == NULL)
		return (printf("malloc error"));
	tmp->name = strdup(stringtable + array.n_un.n_strx);
	tmp->type = get_type(array.n_type, array.n_sect, obj);
	tmp->value = 0;
	if (array.n_value)
		tmp->value = (unsigned long)array.n_value;
	tmp->show_addr = 0;
	if ((array.n_type & N_TYPE) != N_UNDF)
		tmp->show_addr = 1;
	tmp->arch = x86_64;
	tmp->next = NULL;
	if (obj->x86_64o.file)
		sort_list(&obj->x86_64o.file, tmp);
	else
		obj->x86_64o.file = tmp;
	return (0);
}

int		add_elem(void *ptr, t_obj *obj)
{
	int						i;
	char					*stringtable;
	struct symtab_command	*sym;

	sym = obj->x86_64o.sym;
	i = 0;
	obj->x86_64o.el = ptr + sym->symoff;
	stringtable = ptr + obj->x86_64o.sym->stroff;
	while (i < (int)sym->nsyms)
	{
		if (add_elem2(&obj->x86_64o.el[i], stringtable, obj) == -1)
			return (-1);
		i++;
	}
	return (0);
}
