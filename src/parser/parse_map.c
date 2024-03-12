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

int	find_one(char *line)
{
	int	i;
	int	c;

	i = -1;
	c = ft_strlen(line);
	while (line[++i])
	{
		if (line[i] == '1')
		{
			while (line[i] == '1' || line[i] == ' ' || line[i] == '\t')
				i++;
			if (i == c)
				return (1);
		}
		if (line[i] != '1' && line[i] != ' ' && line[i] != '\t')
			error("wrong arguments");
	}
	return (0);
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
			return (0);
	}
	return (1);
}
// int		count_rows(int fd)
// {

// }

void	parse_map(int fd, t_map *map)
{
	char	*line;
	int		i;
	int		x;

	i = -1;
	line = get_next_line(fd);
	while (!find_one(line))
	{
		free(line);
		line = get_next_line(fd);
	}
	// x = count_rows(fd);
	x = 14;
	map->grid = (char **)malloc(sizeof(char *) * x);
	while (++i <= x - 1)
	{
		if (!good_map_line(line))
			error("wrong map input");
		map->grid[i] = ft_strdup(line);
		free(line);
		line = get_next_line(fd);
	}
}