#include "./cud3d.h"

void    error(char *msg)
{
    printf("%s\n", msg);
    exit(0);
}

void    print_mtx(char **mtx)
{
    int i;

    i = -1;
    while(mtx[++i])
        printf("%s\n", mtx[i]);
}

void    init_game_struct(t_game *game)
{
    game->map = (t_map *)malloc(sizeof(t_map));
    game->player = (t_player *)malloc(sizeof(t_player));
    game->map->ceiling = (t_color *)malloc(sizeof(t_color));
    game->map->floor = (t_color *)malloc(sizeof(t_color));
    game->map->map_textures = (t_textures *)malloc(sizeof(t_textures));
    game->player->player_textures = (t_textures *)malloc(sizeof(t_textures));
}

int	main(int argc, char **argv)
{
    t_game *game;
    if (argc != 2)
        error("wrong arguments");    
    game = (t_game *)malloc(sizeof(t_game));
    init_game_struct(game);
    check_name(argv[1]);
    parse_file(argv[1], game);
    parse_map(game->map);
    checker_map(game->map);

    
    // TEST PARSER
    // printf("maxlen =  %d\n", game->map->len);
//     printf("texture N = %s\n", game->map->map_textures->texture[0]);
// 	printf("texture S = %s\n", game->map->map_textures->texture[1]);
// 	printf("texture W = %s\n", game->map->map_textures->texture[2]);
// 	printf("texture E = %s\n", game->map->map_textures->texture[3]);
//     printf("ceiling R = %d\nceiling G = %d\nceiling B = %d\n", game->map->ceiling->r, game->map->ceiling->g, game->map->ceiling->b);
//     printf("floor R = %d\nfloor G = %d\nfloor B = %d\n", game->map->floor->r, game->map->floor->g, game->map->floor->b);
    print_mtx(game->map->grid);
}
