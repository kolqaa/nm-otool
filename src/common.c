/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/11 18:07:01 by nsimonov          #+#    #+#             */
/*   Updated: 2018/08/11 19:04:54 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"
#include "../includes/errors.h"

int		ft_strcmp(const char *s1, const char *s2)
{
	int				i;
	unsigned char	*tmp1;
	unsigned char	*tmp2;

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

void	unknown_nm(void *ptr, t_macho *macho)
{
	nm_error(macho->name, ENOEXEC, NM);
	ptr = NULL;
	macho = NULL;
}

void	unknown_otool(void *ptr, t_macho *macho)
{
	nm_error(macho->name, ENOEXEC, OTOOL);
	ptr = NULL;
	macho = NULL;
}

int		ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	return (c);
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
