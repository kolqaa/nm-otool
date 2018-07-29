#include "../includes/ft_nm.h"

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