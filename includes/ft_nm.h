#ifndef FT_NM_H
#define FT_NM_H

#include <stdio.h>
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/fat.h>
#include <mach-o/ranlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define ARRAY_SIZE(array) (int)(sizeof((array))/sizeof((array)[0]))
#define APPLAY_MASK(x) (x & N_TYPE)
#define NOT_EXTERNAL_SYM(a,b) (a & N_EXT && b != '?')
#define FIND_SEGMENT(seg) ({ while(seg->next) seg = seg->next;})
#define SYM_DISPLAYABLE(el) ((el.n_type & N_TYPE) != N_UNDF)

enum FILE_ARCH
{
	x86,
	x86_64,
	FAT,
	UNKNOWN
};

enum types
{
	ABS,
	S_BSS,
	S_DATA,
	INDIR,
	S_TEXT,
	U_TYPE,
	S_TYPE,
	UNKNOWN_T
};

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
	int							ncmds;
	int							nsects;

	struct	nlist				*el;
	struct	section				*sect;
	struct	load_command		*lc;
	struct	mach_header			*header;
	struct	symtab_command		*sym;
	struct	segment_command		*seg;

	t_segment_info				*seg_info;
	t_macho_info				*obj;
}					t_x86;



typedef struct		s_x86_64
{
	int							arch;
	int							ncmds;
	int							nsects;

	struct	nlist_64			*el;
	struct	section_64			*sect;
	struct	load_command		*lc;
	struct	symtab_command		*sym;
	struct	mach_header_64		*header;
	struct	segment_command_64	*seg;

	t_segment_info				*seg_info;
	t_macho_info				*obj;
}					t_x86_64;

typedef struct		s_macho
{
	int				fd;
	int				args_num;
	int				ptr_size;
	int				fat;
	int 			arch;


	t_x86			x86o;
	t_x86_64		x86_64o;

	void			*obj_ptr;
	char			*name;
	struct stat		buf;
	const unsigned char	*type_charests;

	void (*handle_arch[4])(void *ptr,
						   struct s_macho
						   *macho);

}					t_macho;

/* I/O function */
void			ft_bzero(void *str, size_t n);
size_t			ft_strlen(const char *str);
char			*ft_strdup(const char *str);
void			ft_putchar(char c);
void			ft_putstr(const char *str);
unsigned long	get_len(unsigned long n);
void			print_addr(unsigned long n);
void			put_zeros(int i, unsigned long j);
void			display_help(t_macho_info *file);
void			display_nm(t_macho *macho, int ar);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp(const char *s1, const char *s2);


/* read and mmap file function */

int				mmap_obj(t_macho *macho);
int				get_file(char *file, t_macho *macho);

/* sorting function */

void			sort_list(t_macho_info **file, t_macho_info *tmp);
void			sort_ascii(t_macho_info **obj, t_macho_info *tmp);

/* simple linked list function */

int				add_elem2(void *ptr, char *str, t_macho *macho);
int				add_elem(void *ptr, t_macho *macho);
void			reinit_obj(t_macho *macho);

/* process files with x86_64 arch */
void			handle_x86_64_arch(void *ptr, t_macho *macho);
int				add_section2(t_macho *macho);
int				add_section(void *lc, t_macho *macho);
char			get_type_by_sect(unsigned char s, t_macho *macho);
char			get_type(unsigned char c, unsigned char s, t_macho *macho);

int				set_arch(void *ptr, char *name, t_macho *macho);

/* FAT arch */
void			handle_fat(void *ptr, t_macho *macho);
void			handle_x86_arch(void *ptr, t_macho *macho);



#endif