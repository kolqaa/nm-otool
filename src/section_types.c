/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   section_types.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/11 19:20:25 by nsimonov          #+#    #+#             */
/*   Updated: 2018/08/12 15:54:46 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

unsigned char	get_by_section(unsigned char s, t_macho *macho)
{
	t_segment_info	*seg;

	(macho->arch == x86) ? (seg = macho->x86o.seg_info) :
		(seg = macho->x86_64o.seg_info);
	while (seg)
	{
		if (s == seg->s_num)
		{
			if (!ft_strcmp(seg->name, SECT_BSS))
				return (macho->type_charests[S_BSS]);
			else if (!ft_strcmp(seg->name, SECT_TEXT))
				return (macho->type_charests[S_TEXT]);
			else if (!ft_strcmp(seg->name, SECT_DATA))
				return (macho->type_charests[S_DATA]);
			else
				return (macho->type_charests[S_TYPE]);
		}
		seg = seg->next;
	}
	return (macho->type_charests[S_TYPE]);
}

char			get_type(unsigned char c, unsigned char s, t_macho *obj,
						unsigned long n_value)
{
	unsigned char	a;

	a = APPLAY_MASK(c);
	if (a == N_UNDF)
	{
		a = obj->type_charests[U_TYPE];
		if (n_value != 0)
			a = 'c';
	}
	else if (a == N_SECT)
		a = get_by_section(s, obj);
	else if (a == N_ABS)
		a = obj->type_charests[ABS];
	else if (a == N_INDR)
		a = obj->type_charests[INDIR];
	else if (a == N_PBUD)
		a = obj->type_charests[U_TYPE];
	else
		a = obj->type_charests[UNKNOWN_T];
	if (NOT_EXTERNAL_SYM(c, a))
		c = ft_toupper(a);
	else
		return (a);
	return (c);
}
