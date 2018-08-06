#include "../includes/errors.h"
#include "../includes/ft_nm.h"

struct NM_ERR nm_err_info[] = {
    { .code = EISDIR,       .err_msg = IS_DIR_ERR },
    { .code = ENOEXEC,      .err_msg = OBJ_ERR },
    { .code = EINVAL_FSTAT, .err_msg = FSTAT_ERR },
    { .code = EINVAL_OPEN,  .err_msg = OPEN_ERR },
    { .code = EISEMPTY,     .err_msg = EMPTY_ERR },
    { .code = ENOMEM,       .err_msg = MMAP_ERR },
    { .code = EINVAL_SYM,   .err_msg = SYMTAB_ERR },
    { .code = EINVAL_SEG,   .err_msg = SEGMENT_ERR}
};

void print_error(const char *err_msg, char *file_name)
{
    ft_putstr("nm: ");
    ft_putstr(file_name);
    ft_putstr(": ");
    ft_putstr(err_msg);
}

int nm_error(char *file, int nm_err_code)
{
    int i = 0;
    int ret = 0;

    while (i < ARRAY_SIZE(nm_err_info))
    {
        if (nm_err_info[i].code == nm_err_code)
        {
            print_error(nm_err_info[i].err_msg, file);
            return nm_err_info[i].code;
        }
        i++;
    }

    return ret;
}