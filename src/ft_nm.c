#include "../includes/ft_nm.h"
#include "../includes/errors.h"

int main(int argc, char **argv)
{
	int ret;

	ret = 0;
	t_macho macho;

	init(&macho, NM);
	if (argc == 1)
		ret = no_args(&macho, NM);
	else
		ret = with_args(argc, argv, &macho, NM);

	/* TODO */
	/* munmap ptr after usage free mem */
	return 0;
}
