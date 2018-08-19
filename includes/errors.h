/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/11 18:01:26 by nsimonov          #+#    #+#             */
/*   Updated: 2018/08/12 16:18:03 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# define IS_DIR_ERR "Is a directory.\n"
# define OBJ_ERR "The file was not recognized as a valid object file.\n"
# define FSTAT_ERR "Fstat error.\n"
# define OPEN_ERR "No such file or directory.\n"
# define EMPTY_ERR "File is empty.\n"
# define MMAP_ERR "Failed to allocate memory\n"
# define SYMTAB_ERR "Fails during parsing LC_SYMTAB section\n"
# define SEGMENT_ERR "Fails during parsing LC_SEGMENT section\n"
# define OBJDUMP_ERR "internal objdump command failed\n"
# define NO_ARGS "At least one file must be specified\n"

enum					e_nm_err_codes
{
	EISDIR = -101,
	ENOEXEC = -102,
	EINVAL_FSTAT = -103,
	EINVAL_OPEN = -104,
	EISEMPTY = -105,
	ENOMEM = -106,
	EINVAL_SYM = -107,
	EINVAL_SEG = -108,
	EINVAL_DUMP = -109,
	EINVAL_ARGS = -110
}						t_e_nm_err_codes;

struct					s_nm_err
{
	int			code;
	const char	*err_msg;
}						t_nm_err;

extern struct s_nm_err nm_err_info[];

int						nm_error(char *file, int nm_err_code, int prog);
#endif
