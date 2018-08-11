#include "../includes/ft_nm.h"
#include "../includes/errors.h"

void ot_x86_handle(void *ptr, struct s_macho *macho)
{
	printf("0x%X, 0x%X\n", (unsigned int)ptr, (unsigned int)macho);
	printf("OTOOL x86 hANDLE\n");
}


