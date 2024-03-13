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
    char    **texture;

}t_textures;

typedef struct s_player
{
    t_textures  *player_textures;
} t_player;

typedef struct s_map
{
    int         len;
    int         rows;
    char        **grid;
    t_textures  *map_textures;
    t_color     *ceiling;
    t_color     *floor;

} t_map;

typedef struct s_game
{
    t_map       *map;
    t_player    *player;
} t_game;

// Errors
void    error(char *msg);
void	free_matrix(char **mtx);
void    print_mtx(char **mtx);


// Parser
void    check_name(char *file);
void    parse_file(char *file, t_game *game);
void	parse_arguments(int fd, t_map *map, int *j);
void	parse_grid(int fd, t_map *map, int *skip);
char 	*skip_till_map(int fd, int *j);
int		count_rows(int fd);
void    parse_map(t_map *map);

// Checker
void	checker_map(t_map *map);

//Strings
char	**ft_split(char const *s, char c);
char	*ft_strdup(const char *s1);



#endif