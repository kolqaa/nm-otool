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

enum FILE_ARCH
{
	x86,
	x86_64,
	FAT,
	UNKNOWN
};

typedef	struct		s_sect
{
	char			*name;
	int				sect_nbr;
	struct s_sect	*next;
}					t_sect;

typedef struct		s_file
{
	char			*name;
	char			type;
	unsigned long	value;
	char			show_addr;
	int				arch;
	struct s_file	*next;
}					t_file;

typedef struct s_x86
{
	int arch;
	int ncmds;
	int nscets;
	struct mach_header *header;
	struct load_command *lc;
	struct symtab_command *sym;
	struct nlist *el;
	struct section *sect;
	t_sect			*list;
	t_file			*file;
}				t_x86;



typedef struct s_x86_64
{
	int arch;
	int ncmds;
	int nsects;
	uint32_t sizeofcmds;
	struct mach_header_64 *header;
	struct load_command *lc;
	struct symtab_command *sym;
	struct segment_command_64 *seg;
	struct nlist_64 *el;
	struct section_64 *sect;
	t_sect			*list;
	t_file			*file;
}				t_x86_64;

typedef struct s_obj
{
	int					args_num;
	int					ptr_size;
	int					fd;
	int					arch;
	t_x86				x86o;
	t_x86_64			x86_64o;
	char				*name;
	void				*obj_ptr;
	void (*handle_arch[4])(void *ptr, struct s_obj *obj);
	struct stat			buf;
}				t_obj;

/* I/O function */

void ft_putchar(char c);
void ft_putstr(const char *str);
unsigned long	ft_ullen(unsigned long n);
void			ft_putaddr(unsigned long n);
void			print_zeros(int i, unsigned long j);
void			print_file2(t_file *file);
void			print_file(t_obj *obj);


/* read and mmap file function */

int mmap_obj(t_obj *obj);
int get_file(char *file, t_obj *obj);

/* sorting function */

void	sort_ascii(t_file **file, t_file *tmp);
void	sort_list(t_file **file, t_file *tmp);

/* simple linked list function */

int		add_elem2(void *ptr, char *stringtable, t_obj *obj);
int		add_elem(void *ptr, t_obj *obj);

/* process files with x86_64 arch */
void handle_x86_64_arch(void *ptr, t_obj *obj);
int		add_section2(t_obj *obj);
int		add_section(void *lc, t_obj *obj);
char	get_type_by_sect(unsigned char s, t_obj *obj);
char	get_type(unsigned char c, unsigned char s, t_obj *obj);

int set_arch(void *ptr);

/* FAT arch */
void		handle_fat(void *ptr, t_obj *obj);
void		handle_x86_arch(void *ptr, t_obj *obj);



#endif