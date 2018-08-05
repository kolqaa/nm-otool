#include "../includes/ft_nm.h"

void	sort_ascii(t_file **file, t_file *tmp)
{
	t_file	*tmp2;

	tmp2 = *file;
	if (strcmp(tmp->name, tmp2->name) < 0)
	{
		tmp->next = tmp2;
		*file = tmp;
		return ;
	}
	while (tmp2->next)
	{
		if (strcmp(tmp->name, tmp2->next->name) < 0)
		{
			tmp->next = tmp2->next;
			tmp2->next = tmp;
			return ;
		}
		tmp2 = tmp2->next;
	}
	tmp2->next = tmp;
}

void	sort_list(t_file **file, t_file *tmp)
{
	if (*file)
	{
		sort_ascii(file, tmp);
	}
}
