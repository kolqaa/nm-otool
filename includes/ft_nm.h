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
    struct nlist *sym;
    struct section *sect;
};

struct obj_x86_64
{
    int arch;
    struct nlist64 *sym;
    struct section_64 *sect;
};

struct obj_info
{
    char				*name;
    int					fd;
    void				*obj_ptr;
    int                 arch;
    struct stat			buf;
    struct obj_x86		x86obj;
    struct obj_x86_64	x8664_obj;
    void (*select_arch[3])(void *ptr);
};

void ft_putchar(char c);
void ft_putstr(const char *str);

int get_info(struct obj_info *obj);
int get_file(char *file, struct obj_info *obj);

#endif