/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/11 19:08:00 by nsimonov          #+#    #+#             */
/*   Updated: 2018/08/12 16:24:46 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"
#include "../includes/errors.h"

int		get_file(char *file, t_macho *macho, int prog)
{
	macho->name = file;
	if ((macho->fd = open(file, O_RDONLY)) < 0)
		return (nm_error(macho->name, EINVAL_OPEN, prog));
	if (fstat(macho->fd, &(macho->buf)) < 0)
		return (nm_error(macho->name, EINVAL_FSTAT, prog));
	if (macho->buf.st_size <= 0)
		return (nm_error(macho->name, EISEMPTY, prog));
	else
		macho->ptr_size = macho->buf.st_size;
	if (S_ISREG(macho->buf.st_mode))
		return (EXIT_SUCCESS);
	else
		return (nm_error(macho->name, EISDIR, prog));
}

int		set_arch(void *ptr, t_macho *macho)
{
	uint32_t	magic_number;

	magic_number = *(uint32_t *)ptr;
	if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
	{
		macho->arch = x86;
		return (macho->program == NM ? (x86) : (x86_OTOOL));
	}
	else if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
	{
		macho->arch = x86_64;
		return (macho->program == NM ? (x86_64) : (x86_64_OTOOL));
	}
	else if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
		return (FAT);
	else
		return (macho->program == NM ? (UNKNOWN) : (UNKNOWN_OTOOL));
}

int		mmap_obj(t_macho *macho, int prog)
{
	void *ptr;

	if ((ptr = mmap(0, macho->buf.st_size, PROT_READ, MAP_PRIVATE,
				macho->fd, 0)) == MAP_FAILED)
		return (nm_error(macho->name, ENOMEM, prog));
	macho->obj_ptr = ptr;
	macho->handle_arch[set_arch(ptr, macho)](ptr, macho);
	return (0);
}

int		with_args(int argc, char **argv, t_macho *macho, int prog)
{
	int iter;
	int ret;

	ret = 0;
	iter = 0;
	if (argc > 2)
		macho->args_num = 1;
	while (++iter < argc)
	{
		if (get_file(argv[iter], macho, prog) < 0)
		{
			ret = -1;
			continue;
		}
		if (mmap_obj(macho, prog) < 0)
		{
			ret = -1;
			continue;
		}
		reinit_obj(macho);
	}
	return (ret);
}

int		no_args(t_macho *macho, int prog)
{
	if (prog == OTOOL)
		return (nm_error(NULL, EINVAL_ARGS, prog));
	if (get_file("a.out", macho, prog) < 0)
		return (-1);
	if (mmap_obj(macho, prog) < 0)
		return (-1);
	reinit_obj(macho);
	return (0);
}
