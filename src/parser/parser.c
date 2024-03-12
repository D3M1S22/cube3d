#include "../cud3d.h"
#include <strings.h>

void	parse_file(char *file, t_game *game)
{
	int	fd;

	fd = open(file, O_RDONLY, 0);
	if (!fd)
		error("error opening file");
	parse_arguments(fd, game->map);
	parse_map(fd, game->map);
}
