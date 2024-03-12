# ifndef CUB3D_H
# define CUB3D_H

# include "../get_next_line/get_next_line.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

#define NO 0 //Paths 
#define SO 1
#define WE 2
#define EA 3

typedef struct s_color
{
	int r;
    int g;
    int b;
} t_color;

typedef struct s_textures
{
    char **texture;

}t_textures;

typedef struct s_player
{
    t_textures *player_textures;
} t_player;

typedef struct s_map
{
    char **grid;
    t_textures *map_textures;
    t_color *color;

} t_map;

typedef struct s_game
{
    t_map *map;
    t_player *player;
} t_game;

// Errors
void    error(char *msg);\

// Parser
void    check_name(char *file);
void    parse_file(char *file, t_game *game);
void	parse_arguments(int fd, t_map *map);
void	parse_map(int fd, t_map *map);


//Strings
char	**ft_split(char const *s, char c);
char	*ft_strdup(const char *s1);


#endif