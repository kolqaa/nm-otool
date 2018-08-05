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
			/* TODO */
			/* ft_strcmp */
			if (!ft_strcmp(seg->name, SECT_BSS))
				return (macho->type_charests[S_BSS]);
				//return ('b');
			else if (!ft_strcmp(seg->name, SECT_TEXT))
				return (macho->type_charests[S_TEXT]);
				//return ('t');
			else if (!ft_strcmp(seg->name, SECT_DATA))
				return (macho->type_charests[S_DATA]);
				//return ('d');
			else
				return (macho->type_charests[S_TYPE]);
				//return ('s');
		}
		seg = seg->next;
	}
	//return ('s');
	return (macho->type_charests[S_TYPE]);
}

char	get_type(unsigned char c, unsigned char s, t_macho *obj)
{
	unsigned char	a;

	a = APPLAY_MASK(c);
	if (a == N_UNDF)
		//a = 'u';
		a = obj->type_charests[U_TYPE];
	else if (a == N_SECT)
		a = get_by_section(s, obj);
	else if (a == N_ABS)
		//a = 'a';
		a = obj->type_charests[ABS];
	else if (a == N_INDR)
		//a = 'i';
		a = obj->type_charests[INDIR];
	else if (a == N_PBUD)
		//a = 'u';
		a = obj->type_charests[U_TYPE];
	else
		a = obj->type_charests[UNKNOWN_T];
		//a = '?';

	if (NOT_EXTERNAL_SYM(c,a))
		c = toupper(a);
	else
		return (a);

	return (c);
}