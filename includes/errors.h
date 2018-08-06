
#ifndef ERRORS_H
#define ERRORS_H

#define IS_DIR_ERR 	"Is a directory.\n"
#define	OBJ_ERR		"The file was not recognized as a valid object file.\n"
#define	FSTAT_ERR	"Fstat error.\n"
#define	OPEN_ERR	"Can't open file.\n"
#define	EMPTY_ERR	"File is empty.\n"
#define MMAP_ERR	"Failed to allocate memory\n"
#define SYMTAB_ERR	"Fails during parsing LC_SYMTAB section\n"
#define SEGMENT_ERR	"Fails during parsing LC_SEGMENT section\n"

enum NM_ERROR_CODES
{
    EISDIR			=	-101,
    ENOEXEC			=	-102,
    EINVAL_FSTAT	=	-103,
    EINVAL_OPEN		=	-104,
    EISEMPTY 		=	-105,
    ENOMEM 			=	-106,
	EINVAL_SYM		=	-107,
	EINVAL_SEG		=	-108
};

struct NM_ERR
{
    int code;
    const char *err_msg;
};

extern struct NM_ERR nm_err_info[];

void print_error(const char *err_msg, char *file_name);
int nm_error(char *file, int nm_err_code);
void print_error(const char *err_msg, char *file_name);

#endif
