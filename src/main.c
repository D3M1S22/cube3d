#include "./cud3d.h"
#define EPSILON 1e-1
#define DR 0.0174533

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

int draw_line(t_img *img, int beginX, int beginY, int endX, int endY, int color)
{
    double deltaX = endX - beginX;
    double deltaY = endY - beginY;
    double distance = sqrt(deltaX * deltaX + deltaY * deltaY);

    // Check if distance is very small, treat as a single point
    if (distance < 0.0001) {
        img_pix_put(img, beginX, beginY, color);
        return 0;
    }

    int pixels = (int)distance; // Convert distance to an integer

    // Calculate increments for each pixel
    deltaX /= distance;
    deltaY /= distance;

    double pixelX = beginX;
    double pixelY = beginY;
    for (int i = 0; i < pixels; ++i)
    {
        img_pix_put(img, (int)pixelX, (int)pixelY, color);
        pixelX += deltaX;
        pixelY += deltaY;
    }
    return 0;
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
            else
                render_rect(game->img, (t_rect){(x-1) * M_SIZE, (y-1) * M_SIZE, M_SIZE-1, M_SIZE-1, GRAY_PIXEL});
        }
    }
    game->started = 1;
}

int wall_collision(t_map *map, t_player *player)
{
    float x;
    float y;

    if (player->movement == W)
    {
        x = player->x + player->dx * MP;
        y = player->y + player->dy * MP;
    }
    else if (player->movement == S)
    {
        x = player->x - player->dx * MP;
        y = player->y - player->dy * MP;
    }
    x /= 16;
    y /= 16;
    // printf("x = %f    y = %f   map= %c\n", x, y, map->grid[10][29]);
    if (map->grid[(int)y+1][(int)x+1] == '1')
        return (1);
    return (0);
}

void calculate_player_mov(t_map *map, t_player *player)
{
    if(player->movement == ARR_LEFT)
    {
        player->da -= 0.1 * MP;
        if(player->da < 0)
        {
            player->da += 2 * M_PI;
        }
        player->dx = cos(player->da) * 5;
        player->dy = sin(player->da) * 5;
    }
    if(player->movement == ARR_RIGHT)
    {
        player->da += 0.1 * MP;
        if(player->da > 2 * M_PI)
        {
            player->da = 0;
        }
        player->dx = cos(player->da) * 5;
        player->dy = sin(player->da) * 5;
    }
    if(player->movement == W)
    {
        if (wall_collision(map, player))
            return ;
        player->x += player->dx * MP;
        player->y += player->dy * MP;
    }
    else if(player->movement == S)
    {
        if (wall_collision(map, player))
            return ;
        player->x -= player->dx * MP;
        player->y -= player->dy * MP;
    }
}

float distance(float ax, float ay, float bx, float by)
{
    return ( sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)) );
}


// float degToRad(int a) { return a*M_PI/180.0;}
// int FixAng(int a){ if(a>359){ a-=360;} if(a<0){ a+=360;} return a;}

void draw_rays_3d(t_game *game, t_map *map, t_player *player)
{
    // trying to find first point horizontally
    float rx, ry, ra, xo, yo, aTan, nTan;
    int dof, mx, my;
    float distH, distV, hx, hy, vx, vy;
    int r = -1;
    ra = player->da - (DR * 30);
    if(ra < 0)
        ra += 2 * M_PI;
    else if(ra > 2 * M_PI)
        ra -= 2 * M_PI;
    while(++r < 60)
    {
        // HORIZONTAL CHECK
        aTan = -1/tan(ra);
        dof = 0;
        distH = 100000000;
        hx = player->x;
        hy = player->y;
        if(ra > M_PI)
        {
            ry = (((int)player->y>>4)<<4)-0.0001;
            rx = (player->y - ry) * aTan + player->x;
            yo = -16;
            xo = -yo* aTan;
        }
        else if(ra < M_PI)
        {
            ry = (((int)player->y>>4)<<4)+16;
            rx = (player->y - ry) * aTan + player->x;
            yo = 16;
            xo = -yo* aTan;
        }else{
            rx += player->x;
            ry += player->y;
            dof = (map->longest);
        }
        while(dof < (map->longest))
        {
            my = ((int)ry>>4) + 1;
            mx = ((int)rx>>4) + 1;
            if(my > 0 && mx > 0 && my < (map->rows + 2) && mx < (map->len + 2) && map->grid[my][mx] == '1')
            {
                hx = rx;
                hy = ry;
                distH = distance(player->x, player->y, hx, hy);
                dof = (map->longest);
            }
            else
            {
                dof +=1;
                rx += xo;
                ry += yo;
            }
        }
        // // VERTICAL CHECK
        nTan = -tan(ra);
        dof = 0;
        rx = 0;
        ry = 0;
        distV = 100000000;
        vx = player->x;
        vy = player->y;
        if(ra > M_PI/2 && ra < (3*M_PI)/2)
        {
            rx = (((int)player->x>>4)<<4)-0.0001;
            ry = (player->x - rx) * nTan + player->y;
            xo = -16;
            yo = -xo * nTan;
        }
        else if(ra < M_PI/2 || ra > (3*M_PI)/2)
        {
            rx = (((int)player->x>>4)<<4)+16;
            ry = (player->x - rx) * nTan + player->y;
            xo = 16;
            yo = -xo * nTan;
        }
        else
        {
            dof = map->longest;
            rx += xo;
            ry += yo;
        }
        while(dof < (map->longest))
        {
            my = ((int)ry>>4) + 1;
            mx = ((int)rx>>4) + 1;
            if(my > 0 && mx > 0 && my < (map->rows + 2) && mx < (map->len + 2) && map->grid[my][mx] == '1')
            {
                vx = rx;
                vy = ry;
                distV = distance(player->x, player->y, vx, vy);
                dof = (map->longest);
            }
            else
            {
                dof +=1;
                rx += xo;
                ry += yo;
            }
        }
        if(distV < distH)
        {
            rx = vx;
            ry = vy;
        }else if(distV>distH)
        {
            rx = hx;
            ry = hy;
        }
        if(r == 30)
            printf("rx = %f  ry = %f\n", rx, ry);
        draw_line(game->img, player->x + 2.5, player->y + 2.5, rx, ry, BLUE_PIXEL);
        ra += DR;
        if(ra < 0)
            ra += 2 * M_PI;
        else if(ra > 2 * M_PI)
            ra -= 2 * M_PI;
    }
    (void)game;
}


void draw_player(t_game *game, t_player *player)
{
    calculate_player_mov(game->map, player);
    render_rect(game->img, (t_rect){player->x, player->y, P_SIZE, P_SIZE, RED_PIXEL});
}

int	render(t_game *game)
{
    if (game->mlx->win == NULL)
        return (1);
    render_background(game->img, 0x0000, ((game->map->rows+1) * M_SIZE), ((game->map->len+1) * M_SIZE));
    draw_map(game, game->map);
    draw_player(game, game->player);
    draw_rays_3d(game, game->map, game->player);
    mlx_put_image_to_window(game->mlx->mlx, game->mlx->win, game->img->mlx_img, 0, 0);

    return (0);
}

int handle_keypress(int keycode, t_game *game)
{
    if(keycode == ARR_LEFT || keycode == ARR_RIGHT || keycode == W || keycode == A || keycode == S || keycode == D)
        game->player->movement = keycode;
    return 0;
}

int handle_keyrelease(int keycode, t_game *game)
{

    (void)keycode;
    game->player->movement = -1;
    return 0;
}

void init_player(t_player *player, char **map)
{
    int y;
    int x;

    y = 0;
    while(map[++y][1] != 'c')
    {
        x = 0;
        while(map[y][++x] != 'c')
        {
            if(map[y][x] != ' ' && map[y][x] != '0' && map[y][x] != '1')
            {
                if(map[y][x] == 'W')
                    player->da = M_PI;
                if(map[y][x] == 'N')
                    player->da = 3*M_PI/2;
                if(map[y][x] == 'E')
                    player->da = 0;
                if(map[y][x] == 'S')
                    player->da = M_PI/2;
                player->x = ((x-1) * M_SIZE) + (M_SIZE / 2 - P_SIZE /2);
                player->y = ((y-1) * M_SIZE) + (M_SIZE / 2 - P_SIZE /2);
                player->dx = cos(player->da) * 5;
                player->dy = sin(player->da) * 5;
                return;
            }
        }
    }
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
    init_player(game->player, game->map->grid);
    // redef_map(game->map);
    /// TEST IMAGE CREATION WITH PIXEL AND COLORS

	game->mlx->mlx = mlx_init();
	if (game->mlx->mlx == NULL)
		return (MLX_ERROR);
	game->mlx->win = mlx_new_window(game->mlx->mlx, (game->map->len + 1)*M_SIZE * 3, (game->map->rows + 1)*M_SIZE * 3, "CUB3D");
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
