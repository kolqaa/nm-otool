#include "../includes/ft_nm.h"

int		ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	return (c);
}

void	ft_bzero(void *str, size_t n)
{
	size_t			i;
	unsigned char	*tmp;

	i = 0;
	if (str == NULL)
		return ;
	tmp = str;
	while (i < n)
	{
		tmp[i] = 0;
		i++;
	}
}

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *str)
{
	int		i;
	char	*str2;

	i = 0;
	if (str == NULL)
		return (NULL);
	if ((str2 = (char *)malloc(sizeof(char) * ft_strlen(str) + 1)) == NULL)
		return (NULL);
	while (str[i] != '\0')
	{
		str2[i] = str[i];
		i++;
	}
	str2[i] = '\0';
	return (str2);
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