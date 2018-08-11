/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/11 18:12:14 by nsimonov          #+#    #+#             */
/*   Updated: 2018/08/11 18:12:42 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"
#include "../includes/errors.h"

int	main(int argc, char **argv)
{
	t_macho macho;

	init(&macho, OTOOL);
	if (argc == 1)
		no_args(&macho, OTOOL);
	else
		with_args(argc, argv, &macho, OTOOL);
	return (0);
}
