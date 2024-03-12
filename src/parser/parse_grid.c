#include "../cud3d.h"
#include <strings.h>

int	empty_line(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (line[i] != ' ' && line[i] != '\t')
			return (0);
	}
	return (1);
}

int	good_map_line(char *line)
{
	int	i;

	i = -1;
	if (empty_line(line))
		error("empty line in map");
	while (line[++i])
	{
		if (line[i] != 'N' && line[i] != 'S' && line[i] != 'W' && line[i] != 'E'
			&& line[i] != '1' && line[i] != '0' && line[i] != ' '
			&& line[i] != '\t')
		{
			printf("wrong line = %s\n", line);
			return (0);
		}
	}
	return (1);
}

void	pop_grid(t_map *map, int *skip)
{
	int		i;
	char	*line;
	int		fd;

	fd = open("test.cub" , O_RDONLY, 0);
	i = 0;
	while (++i < *skip)
	{
		line = get_next_line(fd);
		free(line);
	}
	line = get_next_line(fd);
	map->grid[0] = line;
	i = 1;
	while (line != 0)
	{
		if (!good_map_line(line))
			error("wrong map input");
		line = get_next_line(fd);
		if(line)
			map->grid[i] = line;
		i++;
	}
	
}

void	parse_grid(int fd, t_map *map, int *skip)
{
	int		x;

	skip_till_map(fd, skip);
	x = count_rows(fd);
	map->grid = (char **)malloc(sizeof(char *) * (x + 1));
	pop_grid(map, skip);
}
