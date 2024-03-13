#include "../cud3d.h"
#include <strings.h>

void	check_grid(t_map *map)
{
	int	i;
	int	j;

	j = -1;
	while (map->grid[++j])
	{
		i = 0;
		while (map->grid[j][i] == ' ')
			i++;
		if (map->grid[j][i] != '1')
			error("wrong start border");
	}
}

void	parse_file(char *file, t_game *game)
{
	int	fd;
	int	skip;

	fd = open(file, O_RDONLY, 0);
	if (!fd)
		error("error opening file");
	parse_arguments(fd, game->map, &skip);
	parse_grid(fd, game->map, &skip);
	check_grid(game->map);
}

void	free_matrix(char **mtx)
{
	int	i;

	i = -1;
	while (mtx[++i])
		free(mtx[i]);
	free(mtx);
}
