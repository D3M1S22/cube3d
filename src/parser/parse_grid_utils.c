#include "../cud3d.h"

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

char 	*skip_till_map(int fd, int *j)
{
	char	*line;

	line = get_next_line(fd);
	(*j)++;
	while (!find_one(line))
	{
		free(line);
		line = get_next_line(fd);
		(*j)++;
	}
	return (line);
}

int		count_rows(int fd)
{
	char	*line;
	int		rows;

	rows = 0;
	line = get_next_line(fd);
	if (!line)
		return (rows);
	rows++;
	while (line)
	{
		free(line);
		line = get_next_line(fd);
		rows++;
	}
	return (rows);
}
