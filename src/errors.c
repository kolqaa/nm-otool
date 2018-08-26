/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/11 18:00:34 by nsimonov          #+#    #+#             */
/*   Updated: 2018/08/26 13:17:19 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/errors.h"
#include "../includes/ft_nm.h"

struct s_nm_err		nm_err_info[] = {
	{.code = EISDIR, .err_msg = IS_DIR_ERR},
	{.code = ENOEXEC, .err_msg = OBJ_ERR},
	{.code = EINVAL_FSTAT, .err_msg = FSTAT_ERR},
	{.code = EINVAL_OPEN, .err_msg = OPEN_ERR},
	{.code = EISEMPTY, .err_msg = EMPTY_ERR},
	{.code = ENOMEM, .err_msg = MMAP_ERR},
	{.code = EINVAL_SYM, .err_msg = SYMTAB_ERR},
	{.code = EINVAL_SEG, .err_msg = SEGMENT_ERR},
	{.code = EINVAL_DUMP, .err_msg = OBJ_ERR},
	{.code = EINVAL_ARGS, .err_msg = NO_ARGS}
};

static void		print_error(const char *err_msg, char *file_name, int prog)
{
	prog == NM ? (ft_putstr("ft_nm: ")) : (ft_putstr("ft_otool: "));
	if (file_name)
	{
		ft_putstr(file_name);
		ft_putstr(": ");
	}
	ft_putstr(err_msg);
}

int				check_malformed(void *ptr, t_macho *macho)
{
	if (ptr > macho->max_len)
	{
		ft_putstr("truncated or malformed object\n");
		return (1);
	}
	return (0);
}

int				nm_error(char *file, int nm_err_code, int prog)
{
	int i;
	int ret;

	i = 0;
	ret = 0;
	while (i < ARRAY_SIZE(nm_err_info))
	{
		if (nm_err_info[i].code == nm_err_code)
		{
			print_error(nm_err_info[i].err_msg, file, prog);
			return (nm_err_info[i].code);
		}
		i++;
	}
	return (ret);
}
