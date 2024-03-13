#include "../cud3d.h"

void	check_name(char *file)
{
	int	i;

	i = 0;
	while (file[i] != '.')
		i++;
	if (file[i + 1] != 'c')
		error("wrong file name");
	if (file[i + 2] != 'u')
		error("wrong file name");
	if (file[i + 3] != 'b')
		error("wrong file name");
	if (file[i + 4])
		error("wrong file name");
}
