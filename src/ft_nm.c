/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/11 18:11:30 by nsimonov          #+#    #+#             */
/*   Updated: 2018/08/11 18:11:57 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"
#include "../includes/errors.h"

int	main(int argc, char **argv)
{
	t_macho macho;

	init(&macho, NM);
	if (argc == 1)
		no_args(&macho, NM);
	else
		with_args(argc, argv, &macho, NM);
	return (0);
}
