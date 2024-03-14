/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbriglia <fbriglia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 17:48:17 by fbriglia          #+#    #+#             */
/*   Updated: 2024/03/14 18:08:54 by fbriglia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cud3d.h"

void	check_borders(t_map *map)
{
	int	i;

	i = 0;
	while (map->grid[map->rows + 1][++i])
		if (map->grid[map->rows + 1][i] != '1' && map->grid[map->rows
			+ 1][i] != ' ' && map->grid[map->rows + 1][i] != 'c')
			error("wrong bot border");
}

void	check_8(t_map *map, int y, int x)
{
	if (!((map->grid[y][x + 1] == ' ' || map->grid[y][x + 1] == '1'
				|| map->grid[y][x + 1] == 'c') && (map->grid[y][x - 1] == ' '
				|| map->grid[y][x - 1] == '1' || map->grid[y][x - 1] == 'c')
			&& (map->grid[y + 1][x] == ' ' || map->grid[y + 1][x] == '1'
				|| map->grid[y + 1][x] == 'c') && (map->grid[y - 1][x] == ' '
				|| map->grid[y - 1][x] == '1' || map->grid[y - 1][x] == 'c')
			&& (map->grid[y - 1][x + 1] == ' ' || map->grid[y - 1][x + 1] == '1'
				|| map->grid[y - 1][x + 1] == 'c') && (map->grid[y + 1][x
				- 1] == ' ' || map->grid[y + 1][x - 1] == '1' || map->grid[y
				+ 1][x - 1] == 'c') && (map->grid[y + 1][x + 1] == ' '
				|| map->grid[y + 1][x + 1] == '1' || map->grid[y + 1][x
				+ 1] == 'c') && (map->grid[y - 1][x - 1] == ' ' || map->grid[y
				- 1][x - 1] == '1' || map->grid[y - 1][x - 1] == 'c')))
		error("wrong map");
}
void	check_player(t_map *map, int y, int x, int *p)
{
	if (map->grid[y][x] == 'N' || map->grid[y][x] == 'W'
		|| map->grid[y][x] == 'E' || map->grid[y][x] == 'S')
		(*p)++;
}

void	check_spaces(t_map *map, int y, int *p)
{
	int	x;

	x = 0;
	while (map->grid[y][++x] != 'c')
	{
		check_player(map, y, x, p);
		if (map->grid[y][x] == ' ')
			check_8(map, y, x);
	}
}

void	checker_map(t_map *map)
{
	int y;
	int player;

	player = 0;
	y = 0;
	while (map->grid[++y])
	{
		check_borders(map);
		check_spaces(map, y, &player);
	}
	if (player != 1)
		error("too many or no players");
}