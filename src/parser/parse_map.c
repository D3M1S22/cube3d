#include "../cud3d.h"

void	find_longest(t_map *map, int *longest)
{
	int	x;
	int i;

	i = -1;
	*longest = 0;
	while (map->grid[++i])
	{
		x = ft_strlen(map->grid[i]);
		if (x > *longest)
			*longest = x;
	}
}

char	*copy_line(char *src, int length)
{
	char	*line;
	int		i;
	int		j;

	i = -1;
	j = 0;
	line = (char *)malloc(sizeof(char)* length + 1);
	line[length] = 0;
	while (++i < length)
	{
		if (src[j])
		{
			line[i] = src[j];
			j++;
		}
		else
			line[i] = ' ';
	}
	return (line);
}

void pop_mtx(char **mtx, t_map *map, int length)
{
	int i;

	i = -1;
	while (map->grid[++i])
		mtx[i] = copy_line(map->grid[i], length);
	mtx[i] = 0;
	free_matrix(map->grid);
	map->grid = mtx;
}

void parse_map(t_map *map)
{
	int		rows;
	int		x;
	char	**mtx;

	rows = 0;
	while (map->grid[rows])
		rows++;
	find_longest(map, &x);
	mtx = (char **)malloc(sizeof(char *) * rows + 1);
	pop_mtx(mtx, map, x);
}