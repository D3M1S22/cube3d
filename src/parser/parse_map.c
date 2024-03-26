#include "../cud3d.h"

void	find_longest(t_map *map, int *longest)
{
	int	x;
	int	i;

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

	i = 0;
	j = 0;
	line = (char *)malloc(sizeof(char) * length + 2);
	line[length + 2] = '\0';
	line[0] = 'c';
	while (++i < length+2)
	{
		if (src[j])
		{
			line[i] = src[j];
			j++;
		}
		else
			line[i] = ' ';
	}
	line[i-1] = 'c';
	return (line);
}


void	pop_mtx(char **mtx, t_map *map, int length)
{
	int	i;
	int j;

	i = -1;
	mtx[0] = (char *)malloc(sizeof(char)* length +2);
	mtx[0][length + 2] = '\0';
	while (++i < length +2)
		mtx[0][i] = 'c';
	i = -1;
	while (map->grid[++i])
		mtx[i+1] = copy_line(map->grid[i], length);
	j = -1;
	i++;
	mtx[i] = (char *)malloc(sizeof(char)* length +2);
	mtx[i][length+2] = '\0';
	while (++j < length + 2)
		mtx[map->rows+2][j] = 'c';
	free_matrix(map->grid);
	map->grid = mtx;
}

void	parse_map(t_map *map)
{
	int rows;
	int x;
	char **mtx;

	rows = 0;
	while (map->grid[rows])
		rows++;
	map->rows = rows -1;
	find_longest(map, &x);
	mtx = (char **)malloc(sizeof(char *) * (map->rows + 4));
	mtx[map->rows+3] = 0;
	map->len = x -1;
	if (map->len <  2 || map->rows < 2)
		error("wrong map dimension");
	if (map->len > map->rows)
		map->longest = map->len + 1;
	else
		map->longest = map->rows + 1;
	pop_mtx(mtx, map, x);
}
