/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/11 19:00:53 by nsimonov          #+#    #+#             */
/*   Updated: 2018/08/11 19:07:22 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

unsigned long	get_len(unsigned long n)
{
	int		i;

	i = 0;
	while (n > 15)
	{
		n = n / 16;
		i++;
	}
	return (i);
}

void			print_addr(unsigned long n)
{
	int			i;
	char		hex[13];
	static char	charset[] = "0123456789abcdef";

	ft_bzero(hex, 13);
	i = get_len(n);
	while (n > 0)
	{
		hex[i] = charset[n % 16];
		n /= 16;
		i--;
	}
	hex[12] = '\0';
	ft_putstr(hex);
}

void			put_zeros(int i, unsigned long j)
{
	if (j > 0)
		i -= get_len(j) + 1;
	while (--i > 0)
		ft_putchar('0');
}

void			display_help(t_macho_info *obj)
{
	if (obj->displayable)
	{
		if (obj->arch == x86_64)
			put_zeros(17, obj->value);
		else if (obj->arch == x86)
			put_zeros(9, obj->value);
		if (obj->value > 0)
			print_addr(obj->value);
	}
	else if (obj->arch == x86)
		ft_putstr("        ");
	else
		ft_putstr("                ");
	ft_putchar(' ');
	ft_putchar(obj->type);
	ft_putchar(' ');
	ft_putstr(obj->name);
	ft_putchar('\n');
}

void			display_nm(t_macho *macho, int ar)
{
	t_macho_info	*tmp;

	ar == x86_64 ? (tmp = macho->x86_64o.obj) :
					(tmp = macho->x86o.obj);
	if (macho->args_num && !macho->fat)
	{
		ft_putchar('\n');
		ft_putstr(macho->name);
		ft_putchar('\n');
	}
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, "radr:", 5) || tmp->type == 'u')
		{
			tmp = tmp->next;
			continue ;
		}
		display_help(tmp);
		tmp = tmp->next;
	}
}
