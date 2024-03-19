#include "../cud3d.h"

void	pop_rgb(t_color *color, char *_rgb)
{
	char	**rgb;

	rgb = ft_split(_rgb, ',');
	color->r = atoi(rgb[0]);
	color->g = atoi(rgb[1]);
	color->b = atoi(rgb[2]);
	free_matrix(rgb);
}

void	init_map_textures(t_map *map)
{
	map->map_textures = (t_textures *)malloc(sizeof(t_textures));
	map->map_textures->texture = (void **)malloc(sizeof(void *) * 4);
	map->map_textures->texture[NO] = 0;
	map->map_textures->texture[SO] = 0;
	map->map_textures->texture[WE] = 0;
	map->map_textures->texture[EA] = 0;
}

void	pop_structs(char *line, t_map *map, int *x)
{
	char	**mapped_args;

	mapped_args = ft_split(line, ' ');
	if (mapped_args[0][0] == 'N' && ++*x <= 6)
		map->map_textures->texture[NO] = ft_strdup(mapped_args[1]);
	else if (mapped_args[0][0] == 'S' && ++*x <= 6)
		map->map_textures->texture[SO] = ft_strdup(mapped_args[1]);
	else if (mapped_args[0][0] == 'W' && ++*x <= 6)
		map->map_textures->texture[WE] = ft_strdup(mapped_args[1]);
	else if (mapped_args[0][0] == 'E' && ++*x <= 6)
		map->map_textures->texture[EA] = ft_strdup(mapped_args[1]);
	else if ((mapped_args[0][0] == 'F') && ++*x <= 6)
		pop_rgb(map->floor, mapped_args[1]);
	else if ((mapped_args[0][0] == 'C') && ++*x <= 6)
		pop_rgb(map->ceiling, mapped_args[1]);
	free_matrix(mapped_args);
}

void	parse_arguments(int fd, t_map *map, int *j)
{
	char	*line;
	int		i;
	int		x;

	x = 0;
	line = get_next_line(fd);
	*j = 1;
	init_map_textures(map);
	while (line && x < 6 && (*j)++)
	{
		i = 0;
		if (line[i] != 0)
		{
			while (line[i] && (line[i] == ' ' || line[i] == '\t'))
				i++;
			if (line[i] && (line[i] != 'N' && line[i] != 'S' && line[i] != 'W'
					&& line[i] != 'E' && line[i] != 'F' && line[i] != 'C'))
				error("wrong input");
			if (line[i] != 0)
				pop_structs(line + i, map, &x);
		}
		free(line);
		line = get_next_line(fd);
	}
}
