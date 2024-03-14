#include "../cud3d.h"

void	check_borders(t_map *map, int y)
{
	int	i;

	(void)y;
	i = 0;
	while (map->grid[map->rows + 1][++i])
		if (map->grid[map->rows + 1][i] != '1' && map->grid[map->rows
			+ 1][i] != ' ' && map->grid[map->rows + 1][i] != 'c')
			error("wrong bot border");
}

void	check_spaces(t_map *map, int y)
{
	int	x;

	x = 0;
	while (map->grid[y][++x] != 'c')
	{
		if (map->grid[y][x] == ' ')
		{
			if (!((map->grid[y][x + 1] == ' ' || map->grid[y][x + 1] == '1'
					|| map->grid[y][x+1] == 'c') && (map->grid[y][x- 1] == ' ' 
					|| map->grid[y][x-1] == '1' || map->grid[y][x- 1] == 'c') 
					&& (map->grid[y+1][x] == ' ' || map->grid[y+ 1][x] == '1' 
					|| map->grid[y+1][x] == 'c') && (map->grid[y - 1][x] == ' ' 
					|| map->grid[y-1][x] == '1'|| map->grid[y - 1][x] == 'c') 
					&& (map->grid[y-1][x+1] == ' ' || map->grid[y - 1][x + 1] == '1'
					|| map->grid[y-1][x+1] == 'c') && (map->grid[y + 1][x - 1] == ' '
					|| map->grid[y + 1][x - 1] == '1' || map->grid[y + 1][x
					- 1] == 'c') && (map->grid[y + 1][x + 1] == ' '
					|| map->grid[y + 1][x + 1] == '1' || map->grid[y + 1][x
					+ 1] == 'c') && (map->grid[y - 1][x - 1] == ' '
					|| map->grid[y - 1][x - 1] == '1' || map->grid[y - 1][x
					- 1] == 'c')))
				error("wrong map");
		}
	}
}

void	checker_map(t_map *map)
{
	int y;

	y = 0;
	while (map->grid[++y])
	{
		check_borders(map, y);
		check_spaces(map, y);
	}
}