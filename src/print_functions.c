#include "../includes/ft_nm.h"

int		ft_strcmp(const char *s1, const char *s2)
{
	int					i;
	unsigned char		*tmp1;
	unsigned char		*tmp2;

	i = 0;
	tmp1 = (unsigned char *)s1;
	tmp2 = (unsigned char *)s2;
	if (tmp1 == NULL || tmp2 == NULL)
		return (0);
	while (tmp1[i] && tmp2[i])
	{
		if (tmp1[i] != tmp2[i])
			return (tmp1[i] - tmp2[i]);
		i++;
	}
	return (tmp1[i] - tmp2[i]);
}

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t				i;
	unsigned char		*tmp1;
	unsigned char		*tmp2;

	i = 0;
	tmp1 = (unsigned char *)s1;
	tmp2 = (unsigned char *)s2;
	if (n == 0 || tmp1 == NULL || tmp2 == NULL)
		return (0);
	while (tmp1[i] && tmp2[i] && i < n)
	{
		if (tmp1[i] != tmp2[i])
			return (tmp1[i] - tmp2[i]);
		i++;
	}
	if (i != n)
		return (tmp1[i] - tmp2[i]);
	return (tmp1[i - 1] - tmp2[i - 1]);
}

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

void		put_zeros(int i, unsigned long j)
{
	if (j > 0)
		i -= get_len(j) + 1;
	while (--i > 0)
		ft_putchar('0');
}

void		display_help(t_macho_info *obj)
{
	/* TODO split this shit */
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
	printf("%s\n", obj->name);
}

void			display_nm(t_macho *macho, int ar)
{
	t_macho_info	*tmp;

	ar == x86_64 ? (tmp = macho->x86_64o.obj) :
					(tmp = macho->x86o.obj);
	if (macho->args_num && !macho->fat)
		printf("\n%s\n", macho->name);
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, "radr:", 5) || tmp->type == 'u') {
			tmp = tmp->next;
			continue ;
		}

		display_help(tmp);
		tmp = tmp->next;
	}
}