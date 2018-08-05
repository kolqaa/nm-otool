#include "../includes/ft_nm.h"

unsigned long	ft_ullen(unsigned long n)
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

void			ft_putaddr(unsigned long n)
{
	int			i;
	char		hex[13];
	static char	tab[] = "0123456789abcdef";

	bzero(hex, 13);
	i = ft_ullen(n);
	while (n > 0)
	{
		hex[i] = tab[n % 16];
		n /= 16;
		i--;
	}
	hex[12] = '\0';
	ft_putstr(hex);
}

void			print_zeros(int i, unsigned long j)
{
	if (j > 0)
		i -= ft_ullen(j) + 1;
	while (--i > 0)
		ft_putchar('0');
}

void			print_file2(t_file *file)
{
	//if (!strncmp(file->name, "radr:", 5) || file->type == 'u')
	//	return ;
	if (file->show_addr)
	{
		if (file->arch == x86_64)
			print_zeros(17, file->value);
		else if (file->arch == 32)
			print_zeros(9, file->value);
		if (file->value > 0)
			ft_putaddr(file->value);
	}
	else if (file->arch == 32)
		ft_putstr("        ");
	else
		ft_putstr("                ");
	ft_putchar(' ');
	ft_putchar(file->type);
	ft_putchar(' ');
	printf("%s\n", file->name);
}

void			print_file(t_obj *obj)
{
	t_file	*tmp;

	tmp = obj->x86_64o.file;
	if (obj->args_num)
		printf("\n%s\n", obj->name);
	while (tmp)
	{
		print_file2(tmp);
		tmp = tmp->next;
	}
}

void ft_putchar(char c)
{
    write(1, &c, 1);
}

void ft_putstr(const char *str)
{
    int i = 0;
    while (str[i])
    {
        ft_putchar(str[i]);
        str++;
    }
}