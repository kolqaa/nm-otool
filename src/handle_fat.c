/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/11 18:12:49 by nsimonov          #+#    #+#             */
/*   Updated: 2018/08/11 18:16:00 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

void	handle_fat(void *ptr, t_macho *macho)
{
	uint64_t	nfat_arch;
	uint64_t	i;
	void		*fat_arch;
	uint32_t	offset;

	nfat_arch = ((struct fat_header*)ptr)->nfat_arch;
	nfat_arch = SWAP_ENDIANESS(nfat_arch);
	i = 0;
	while (i < nfat_arch)
	{
		fat_arch = ((void*)ptr + sizeof(struct fat_header))
                                  + (i * sizeof(struct fat_arch));
		offset = SWAP_ENDIANESS(((struct fat_arch *)fat_arch)->offset);
         	macho->handle_arch[set_arch(ptr + offset, macho)](ptr + offset, macho);
		i++;
	}
}
