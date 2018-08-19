/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/11 18:30:12 by nsimonov          #+#    #+#             */
/*   Updated: 2018/08/12 16:24:39 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach-o/ranlib.h>
# include <mach/machine.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <unistd.h>

# define ARRAY_SIZE(array) (int)(sizeof((array))/sizeof((array)[0]))

# define APPLAY_MASK(x) (x & N_TYPE)
# define NOT_EXTERNAL_SYM(a,b) (a & N_EXT && b != '?')
# define FIND_SEGMENT(seg) ({ while(seg->next) seg = seg->next;})
# define SYM_DISPLAYABLE(el) ((el.n_type & N_TYPE) != N_UNDF)
 
# define HS24(val) ((val & 0xFF000000) >> 24)
# define HS8(val)  ((val & 0x00FF0000) >> 8)
# define SH8(val)  ((val & 0x0000FF00) << 8)
# define SH24(val) ((val & 0x000000FF) << 24)
# define SWAP_ENDIANESS(val) ((SH24(val)  | SH8(val) | HS8(val) | HS24(val)))

typedef enum		e_prog
{
	NM,
	OTOOL
}					t_e_prog;

typedef enum		e_file_arch
{
	x86,
	x86_64,
	FAT,
	x86_OTOOL,
	x86_64_OTOOL,
	UNKNOWN,
	UNKNOWN_OTOOL
}					t_e_file_arch;

typedef enum		e_types
{
	ABS,
	S_BSS,
	S_DATA,
	INDIR,
	S_TEXT,
	U_TYPE,
	S_TYPE,
	UNKNOWN_T
}					t_e_types;

typedef enum		e_print_info
{
	HEX_BASE = 16,
	x86_BASE_ZERO = 9,
	x86_64_BASE_ZERO = 17
}					t_e_print_info;

typedef	struct		s_segment_info
{
	int						s_num;
	char					*name;
	struct s_segment_info	*next;
}					t_segment_info;

typedef struct		s_macho_info
{
	char				type;
	char				displayable;
	int					arch;
	unsigned long		value;
	char				*name;
	struct s_macho_info	*next;
}					t_macho_info;

typedef struct		s_x86
{
	int							arch;
	uint32_t						ncmds;
	uint32_t						nsects;

	struct nlist				*el;
	struct section				*sect;
	struct load_command			*lc;
	struct mach_header			*header;
	struct symtab_command		*sym;
	struct segment_command		*seg;

	t_segment_info				*seg_info;
	t_macho_info				*obj;
}					t_x86;

typedef struct		s_x86_64
{
	int						arch;
	uint32_t					ncmds;
	uint32_t					nsects;

	struct nlist_64				*el;
	struct section_64			*sect;
	struct load_command			*lc;
	struct symtab_command		*sym;
	struct mach_header_64		*header;
	struct segment_command_64	*seg;

	t_segment_info				*seg_info;
	t_macho_info				*obj;
}					t_x86_64;

typedef struct		s_macho
{
	int					fd;
	int					args_num;
	int					ptr_size;
	int					fat;
	int					arch;
	uint32_t			i;

	t_x86				x86o;
	t_x86_64			x86_64o;

	void				*obj_ptr;
	void				*max_len;
	char				*name;
	struct stat			buf;
	const unsigned char	*type_charests;

	uint32_t			program;
	void				(*handle_arch[7])(void *ptr,
						struct s_macho *macho);
}					t_macho;

void				ft_bzero(void *str, size_t n);
size_t				ft_strlen(const char *str);
char				*ft_strdup(const char *str);
void				ft_putchar(char c);
void				ft_putstr(const char *str);
unsigned long		get_len(unsigned long n);
void				print_addr(unsigned long n);
void				put_zeros(int i, unsigned long j);
void				display_help(t_macho_info *file);
void				display_nm(t_macho *macho, int ar);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_toupper(int c);
char				*ft_itoa_base(int val, int base, int output_size);
char				read_tab(int i);

int					mmap_obj(t_macho *macho, int prog);
int					get_file(char *file, t_macho *macho, int prog);
void				reinit_obj(t_macho *macho);
void				init(t_macho *obj, uint32_t prog);
int					with_args(int argc, char **argv, t_macho *macho, int prog);
int					no_args(t_macho *macho, int prog);

void				make_order_align(t_macho_info **file, t_macho_info *tmp);

void				handle_x86_64_arch(void *ptr, t_macho *macho);
char				get_type_by_sect(unsigned char s, t_macho *macho);
char				get_type(unsigned char c, unsigned char s, t_macho *macho,
							unsigned long n_value);
int					set_arch(void *ptr, t_macho *macho);
void				handle_fat(void *ptr, t_macho *macho);
void				handle_x86_arch(void *ptr, t_macho *macho);
void				ot_x86_64_handle(void *ptr, struct s_macho *macho);
void				ot_x86_handle(void *ptr, struct s_macho *macho);

void				unknown_nm(void *ptr, t_macho *macho);
void				unknown_otool(void *ptr, t_macho *macho);
int				check_malformed(void *ptr, t_macho *macho);

#endif
