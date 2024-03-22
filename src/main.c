#include "./cud3d.h"
#define EPSILON 1e-1

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
    game->mlx = (t_mlx *)malloc(sizeof(t_mlx));
    game->img = (t_img *)malloc(sizeof(t_img));
    game->map = (t_map *)malloc(sizeof(t_map));
    game->player = (t_player *)malloc(sizeof(t_player));
    game->map->ceiling = (t_color *)malloc(sizeof(t_color));
    game->map->floor = (t_color *)malloc(sizeof(t_color));
    game->map->map_textures = (t_textures *)malloc(sizeof(t_textures));
    game->player->player_textures = (t_textures *)malloc(sizeof(t_textures));
}

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 500
#define MLX_ERROR 1
#define RED_PIXEL 0xFF0000
#define YELLOW_PIXEL 0xFFFF00
#define GREEN_PIXEL 0xFF00
#define WHITE_PIXEL 0xFFFFFF
#define GRAY_PIXEL 0xAEAEAE
#define BLUE_PIXEL 0x0000FF

typedef struct s_data
{
	void	*mlx_ptr;
	void	*win_ptr;
	t_img	img;
	int		cur_img;
}	t_data;

typedef struct s_rect
{
	int	x;
	int	y;
	int width;
	int height;
	int color;
}	t_rect;

void	img_pix_put(t_img *img, int x, int y, int color)
{
    char    *pixel;
    int		i;

    i = img->bpp - 8;
    pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
    while (i >= 0)
    {
        /* big endian, MSB is the leftmost bit */
        if (img->endian != 0)
            *pixel++ = (color >> i) & 0xFF;
        /* little endian, LSB is the leftmost bit */
        else
            *pixel++ = (color >> (img->bpp - 8 - i)) & 0xFF;
        i -= 8;
    }
}

void draw_line(t_img *img, int x0, int y0, int x1, int y1, int color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx, sy;
    if (x0 < x1) sx = 1; else sx = -1;
    if (y0 < y1) sy = 1; else sy = -1;
    int err = dx - dy;
    while (x0 != x1 || y0 != y1) {
        img_pix_put(img, x0, y0, color);
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

int render_rect(t_img *img, t_rect rect)
{
    int	i;
    int j;

    i = rect.y;
    while (i < rect.y + rect.height)
    {
        j = rect.x;
        while (j < rect.x + rect.width)
            img_pix_put(img, j++, i, rect.color);
        ++i;
    }
    return (0);
}

void	render_background(t_img *img, int color, int height, int width)
{
    int	i;
    int	j;

    i = 0;
    while (i < height)
    {
        j = 0;
        while (j < width)
        {
            img_pix_put(img, j++, i, color);
        }
        ++i;
    }
}

int player_or(char is_player, t_game *game, int x, int y)
{
    if((is_player == 'N' || is_player == 'W' ||
        is_player == 'E' || is_player == 'S') && game->started != 1)
    {
        game->player->movement = -1;
        if(is_player == 'N')
            game->player->da =  90;
        else if(is_player == 'W')
            game->player->da = 180;
        else if(is_player == 'E')
            game->player->da = 0;
        else if(is_player == 'S')
            game->player->da = 270;
        write(1,"a\n",2);
        game->player->x = ((x-1) * M_SIZE) + (M_SIZE / 2 - P_SIZE /2);
        game->player->y = ((y-1) * M_SIZE) + (M_SIZE / 2 - P_SIZE /2);
        game->player->dx = cos(game->player->da);
        game->player->dy = sin(game->player->da);
    }
    return 1;
}

void draw_map(t_game *game, t_map *map)
{
    int x;
    int y;

    y = 0;
    while(map->grid[++y][1] != 'c')
    {
        x = 0;
        while(map->grid[y][++x] != 'c')
        {
            if(map->grid[y][x] == ' ')
                render_rect(game->img, (t_rect){(x-1) * M_SIZE, (y-1) * M_SIZE, M_SIZE-1, M_SIZE-1, YELLOW_PIXEL});
            else if(map->grid[y][x] == '1')
                render_rect(game->img, (t_rect){(x-1) * M_SIZE, (y-1) * M_SIZE, M_SIZE-1, M_SIZE-1, GREEN_PIXEL});
            else if(map->grid[y][x] == '0')
                render_rect(game->img, (t_rect){(x-1) * M_SIZE, (y-1) * M_SIZE, M_SIZE-1, M_SIZE-1, GRAY_PIXEL});
            else if(player_or(map->grid[y][x], game, x, y))
            {
                render_rect(game->img, (t_rect){(x-1) * M_SIZE, (y-1) * M_SIZE, M_SIZE-1, M_SIZE-1, GRAY_PIXEL});
            }
        }
    }
    game->started = 1;
    // render_rect(game->img, (t_rect){0, 0, 50, 50, RED_PIXEL});
}

void calculate_player_mov(t_player *player)
{
    if(player->movement == ARR_LEFT)
    {
        player->da -= 0.1;
        if(player->da < 0)
        {
            player->da += 2 * M_PI;
        }
        player->dx = cos(player->da) * 5;
        player->dy = sin(player->da) * 5;
    }
    if(player->movement == ARR_RIGHT)
    {
        player->da += 0.1;
        if(player->da > 2 * M_PI)
        {
            player->da -= 2 * M_PI;
        }
        player->dx = cos(player->da) * 5;
        player->dy = sin(player->da) * 5;
    }
    if(player->movement == W)
    {
        player->x += player->dx/4;
        player->y += player->dy/4;
    }
    else if(player->movement == S)
    {
        player->x -= player->dx/4;
        player->y -= player->dy/4;
    }
}

float degToRad(int a) { return a*M_PI/180.0;}
int FixAng(int a){ if(a>359){ a-=360;} if(a<0){ a+=360;} return a;}

void draw_rays_3d(t_game *game, t_player *player, t_map *map)
{
    int r, mx, my, dof;
    float rx, ry, ra, xo, yo, aTan;

    r = -1;
    ra = player->da;
    
    while(++r < 1)
    {
        aTan = -1/tan(ra);
        dof = 0;
        if(ra > M_PI)
        {
            // printf("> M_PI\n");
            ry = (((int)player->y>>6)<<6)-0.0001;
            rx = (player->y - ry) * aTan + player->x;
            yo = -64;
            xo = -yo * aTan;
        }
        else if(ra < M_PI)
        {
            // printf("< M_PI\n");
            ry = (((int)player->y>>6)<<6)+64;
            rx = (player->y - ry) * aTan + player->x;
            yo = 64;
            xo = -yo * aTan;
        }
        else if(fabs(ra - 0) < EPSILON || fabs(ra - M_PI) < EPSILON)
        {
            // printf("== PI or 0\n");
            rx = player->x;
            ry = player->y;
            dof = 8;
        }
        while(dof < 8)
        {
            mx = ((int)(rx)>>6) + 1;
            my = ((int)(ry)>>6) + 1;
            // mp = my * (map->len + 1) + mx;
            if( mx > 0 && my > 0 && mx < (map->len + 1) && my < (map->rows + 1) && map->grid[my][mx] == '1')
            {
                // printf("mx : %d my: %d\n", mx, my);
                dof = 8;
            }
            else
            {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }
    draw_line(game->img, player->x + 10, player->y + 10, rx, ry, BLUE_PIXEL);
    }
}

void draw_player(t_game *game, t_player *player)
{
    calculate_player_mov(player);
    render_rect(game->img, (t_rect){player->x, player->y, P_SIZE, P_SIZE, RED_PIXEL});
    draw_line(game->img, player->x+10, player->y+10, (player->x+10)+player->dx * 5, (player->y+10)+player->dy * 5, RED_PIXEL);
}

int	render(t_game *game)
{
    // static int ciao = 0;
    if (game->mlx->win == NULL)
        return (1);
    // if(ciao == 0){
        // render_background(game->img, WHITE_PIXEL, ((game->map->rows+1) * 50), ((game->map->len+1) * 50));
    draw_map(game, game->map);
    draw_player(game, game->player);
    draw_rays_3d(game, game->player, game->map);
    // ciao++;
    // }
    mlx_put_image_to_window(game->mlx->mlx, game->mlx->win, game->img->mlx_img, 0, 0);

    return (0);
}

int handle_keypress(int keycode, t_game *game)
{
    printf("key code pressed %d\n", keycode);
    if(keycode == ARR_LEFT || keycode == ARR_RIGHT || keycode == W || keycode == A || keycode == S || keycode == D)
        game->player->movement = keycode;
    printf("keycode = %d, player_pos = x:%f y:%f\n", keycode, game->player->x, game->player->y);
    return 0;
}

int handle_keyrelease(int keycode, t_game *game)
{

    (void)keycode;
    game->player->movement = -1;
    return 0;
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


    /// TEST IMAGE CREATION WITH PIXEL AND COLORS

	game->mlx->mlx = mlx_init();
	if (game->mlx->mlx == NULL)
		return (MLX_ERROR);
	game->mlx->win = mlx_new_window(game->mlx->mlx, (game->map->len + 1)*M_SIZE, (game->map->rows + 1)*M_SIZE, "CUB3D");
	if (game->mlx->win == NULL)
	{
		free(game->mlx->win);
		return (MLX_ERROR);
	}

	/* Setup hooks */ 
	game->img->mlx_img = mlx_new_image(game->mlx->mlx, (game->map->len + 1)*M_SIZE, (game->map->rows + 1)*M_SIZE);
	
	game->img->addr = mlx_get_data_addr(game->img->mlx_img, &game->img->bpp,
			&game->img->line_len, &game->img->endian);

	mlx_loop_hook(game->mlx->mlx, &render, game);
	// mlx_hook(data.win_ptr, KeyPress, KeyPressMask, &handle_keypress, &data);
    // mlx_key_hook(game->mlx->win, handle_keypress, &game);
    mlx_hook(game->mlx->win, KeyPress, KeyPressMask, handle_keypress, game);
    mlx_hook(game->mlx->win, KeyRelease, KeyRelease, handle_keyrelease, game);
    // mlx_loop_hook(game->mlx->win, handle_keypress, &game);

	mlx_loop(game->mlx->mlx);

	/* we will exit the loop if there's no window left, and execute this code */
	mlx_destroy_image(game->mlx->mlx, game->img->mlx_img);
	mlx_destroy_window(game->mlx->mlx, game->mlx->win);
	free(game->mlx->mlx);
    
    // TEST PARSER
    // printf("maxlen =  %d\n", game->map->len);
//     printf("texture N = %s\n", game->map->map_textures->texture[0]);
// 	printf("texture S = %s\n", game->map->map_textures->texture[1]);
// 	printf("texture W = %s\n", game->map->map_textures->texture[2]);
// 	printf("texture E = %s\n", game->map->map_textures->texture[3]);
//     printf("ceiling R = %d\nceiling G = %d\nceiling B = %d\n", game->map->ceiling->r, game->map->ceiling->g, game->map->ceiling->b);
//     printf("floor R = %d\nfloor G = %d\nfloor B = %d\n", game->map->floor->r, game->map->floor->g, game->map->floor->b);
    // print_mtx(game->map->grid);
}
