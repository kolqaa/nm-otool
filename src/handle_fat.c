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

	cpu_type_t		cpu_type;

	nfat_arch = ((struct fat_header*)ptr)->nfat_arch;
	nfat_arch = SWAP_ENDIANESS(nfat_arch);
	i = 0;
	while (i < nfat_arch)
	{
		fat_arch = ((void*)ptr + sizeof(struct fat_header))
                                  + (i * sizeof(struct fat_arch));
		if (check_malformed(fat_arch, macho))
			return ;
		cpu_type = SWAP_ENDIANESS(((struct fat_arch*)fat_arch)->cputype);
		if (cpu_type == CPU_TYPE_X86_64 ||
			cpu_type == CPU_TYPE_ARM ||
			cpu_type == CPU_TYPE_ARM64)
		{
			offset = SWAP_ENDIANESS(((struct fat_arch *)fat_arch)->offset);
			if (check_malformed(ptr + offset, macho))
				return ;
         	macho->handle_arch[set_arch(ptr + offset, macho)](ptr + offset, macho);
		}
		cpu_type = 0;
		i++;
	}
}
