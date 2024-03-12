#include "../cud3d.h"
#include <strings.h>

void	init_map_textures(t_map *map)
{
	map->map_textures = (t_textures *)malloc(sizeof(t_textures));
	map->map_textures->texture = (char **)malloc(sizeof(char *) * 4);
	map->map_textures->texture[0] = 0;
	map->map_textures->texture[1] = 0;
	map->map_textures->texture[2] = 0;
	map->map_textures->texture[3] = 0;

}

void	pop_structs(char *line, t_map *map, int *x)
{
	char	**mapped_args;
	char	**rgb;
	mapped_args = ft_split(line, ' ');
	if (mapped_args[0][0] == 'N' && ++*x <= 6)
		map->map_textures->texture[0] = ft_strdup(mapped_args[1]);
	else if (mapped_args[0][0] == 'S' && ++*x <= 6)
		map->map_textures->texture[1] = ft_strdup(mapped_args[1]);
	else if (mapped_args[0][0] == 'W' && ++*x <= 6)
		map->map_textures->texture[2] = ft_strdup(mapped_args[1]);
	else if (mapped_args[0][0] == 'E' && ++*x <= 6)
		map->map_textures->texture[3] = ft_strdup(mapped_args[1]);
	else if ((mapped_args[0][0] == 'F' || mapped_args[0][0] == 'C') && ++*x <= 6)
	{
		rgb = ft_split(mapped_args[1], ',');
		map->color->r = atoi(rgb[0]);
		map->color->g = atoi(rgb[1]);
		map->color->b = atoi(rgb[2]);
	}
}

void	parse_arguments(int fd, t_map *map)
{
	char	*line;
	int 	i;
	int		x;

	x = 0;
	line = get_next_line(fd);
	init_map_textures(map);
	while (line && x < 6)
	{
		i = 0;
		if(line[i]!=0)
		{
			while (line[i] && (line[i] == ' ' || line[i] == '\t'))
				i++;
			if(line[i] && (line[i] != 'N' && line[i] != 'S' && line[i] != 'W'
					&& line[i] != 'E' && line[i] != 'F' && line[i] != 'C'))
				error("wrong input");	
			if (line[i] != 0)
				pop_structs(line + i, map, &x);
		}
		free(line);
		line = get_next_line(fd);
	}
}
