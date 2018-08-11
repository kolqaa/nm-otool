#include "../includes/ft_nm.h"
#include "../includes/errors.h"

int main(int argc, char **argv)
{
	int ret;

	ret = 0;
	t_macho macho;

	init(&macho, OTOOL);
	if (argc == 1)
		ret = no_args(&macho, OTOOL);
	else
		ret = with_args(argc, argv, &macho, OTOOL);

	/* TODO */
	/* munmap ptr after usage free mem */
	return 0;
}
