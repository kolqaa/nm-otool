#ifndef FT_NM_H
#define FT_NM_H

#include <stdio.h>
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array)[0]))

enum FILE_ARCH
{
	x86,
	x86_64,
	UNKNOWN
};

struct obj_x86
{
	int arch;
	int ncmds;
	struct mach_header *header;
	struct load_command *lc;
	struct symtab_command *sym;
	struct nlist *el;
	struct section *sect;
};

struct obj_x86_64
{
	int arch;
	int ncmds;
	uint32_t sizeofcmds;
	struct mach_header_64 *header;
	struct load_command *lc;
	struct symtab_command *sym;
	struct segment_command_64 *seg;
	struct nlist_64 *el;
	struct section_64 *sect;
};

struct obj_info
{
	char				*name;
	int					fd;
	void				*obj_ptr;
	int					arch;
	struct stat			buf;
	struct obj_x86		x86o;
	struct obj_x86_64	x86_64o;
	void (*handle_arch_clbk[3])(void *ptr, struct obj_info *obj);
};

void ft_putchar(char c);
void ft_putstr(const char *str);

int get_info(struct obj_info *obj);
int get_file(char *file, struct obj_info *obj);

#endif