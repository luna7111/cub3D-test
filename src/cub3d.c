#include <cub3d.h>

char **get_temp_map(t_gctrl *gctrl)
{
	char **map;
	int	i = 0;
	int fd = open("map", O_RDONLY);

	map = gctrl_malloc(gctrl, PROG_BLOCK, sizeof(char *) * 16);
	while (i < 16)
	{
		map[i] = get_next_line(fd);
		i++;
	}
	return (map);
}

t_data	*init_data(t_gctrl *gctrl)
{
	t_data 		*data;

	data = gctrl_malloc(gctrl, PROG_BLOCK, sizeof(t_data));
	data->player = gctrl_malloc(gctrl, PROG_BLOCK, sizeof(t_player));
	data->map_height = 16;	
	data->map_width = 16;	
	data->player->x = 8;
	data->player->y = 8;
	data->player->dir = 0;
	data->map = get_temp_map(gctrl);
	data->mlx = mlx_init();
	data->window = mlx_new_window(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
	data->image = mlx_new_image(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	data->image_address = mlx_get_data_addr(data->image, &data->img_bpp, &data->img_l_len, &data->img_endian);
	data->input = 0;
//////////////////////
	data->window_test = mlx_new_window(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
	data->image_test = mlx_new_image(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	data->image_address_test = mlx_get_data_addr(data->image_test, &data->img_bpp_test, &data->img_l_len_test, &data->img_endian_test);
	return (data);
}

float	deg_to_rad(float degrees)
{
	return (degrees * M_PI / 180);
}

void	put_pixel(t_data *data, int x, int y, int color)
{
	char	*dst;
	if (x < 0 || y < 0 || x > WINDOW_WIDTH || y > WINDOW_HEIGHT)
		return ;
	dst = (char *)data->image_address;
	dst += y * data->img_l_len + x * (data->img_bpp / 8);
	*(unsigned int *)dst = color;
}

void	draw_wall_section(t_data *data, int x, float distance, int color)
{
	int	size;
	int	start_point;
	int i;

	if (distance > 0)
		size = WINDOW_HEIGHT * 2 / distance;
	else
		size = WINDOW_HEIGHT;
	start_point = (WINDOW_HEIGHT - size) / 2;
	i = 0;
	while (i < WINDOW_HEIGHT)
	{
		if (i < start_point)
			put_pixel(data, x, i, 0x11001111);
		else if(i > size + start_point)
			put_pixel(data, x, i, 0x11110011);
		else
			put_pixel(data, x, i, color);
		i++;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void	put_pixel_test(t_data *data, int x, int y, int color)
{
	char	*dst;
	if (x < 0 || y < 0 || x > WINDOW_WIDTH || y > WINDOW_HEIGHT)
		return ;
	dst = (char *)data->image_address_test;
	dst += y * data->img_l_len_test + x * (data->img_bpp_test / 8);
	*(unsigned int *)dst = color;
}

void	draw_wall_section_test(t_data *data, int x, float distance, int color)
{
	int	size;
	int	start_point;
	int i;

	if (distance > 0)
		size = WINDOW_HEIGHT * 2 / distance;
	else
		size = WINDOW_HEIGHT;
	start_point = (WINDOW_HEIGHT - size) / 2;
	i = 0;
	while (i < WINDOW_HEIGHT)
	{
		if (i < start_point)
			put_pixel_test(data, x, i, 0x11001111);
		else if(i > size + start_point)
			put_pixel_test(data, x, i, 0x11110011);
		else
			put_pixel_test(data, x, i, color);
		i++;
	}
}

void	render_line_test(t_data *data, int x, float ray_angle)
{
	float	dx;	
	float	dy;	
	float	i;

	dx = cos(deg_to_rad(ray_angle));
	dy = sin(deg_to_rad(ray_angle));
	i = 0;
	while (data->map[(int)(data->player->y + (dy * i))][(int)(data->player->x + (dx * i))] == '0')
		i+= 0.02;
	if ((int)(data->player->y + (dy * (i - 0.02))) != (int)(data->player->y + (dy * i)))
		draw_wall_section_test(data, x, i, 0xAAAAAAAA);
	else
		draw_wall_section_test(data, x, i, 0xBBBBBBBB);
}
///////////////////////////////////////////////////////////////////////////////////////////////////

float	sqr(float nb)
{
	return (nb * nb);
}

float	cast_row_ray(t_data *data, float dx, float dy)
{
	float	x;
	float	y;

	dx = dx / fabs(dy);
	if (dy > 0)	
		dy = 1;
	else
		dy = -1;

	if (dy > 0)
		y = (int)data->player->y;
	else
		y = (int)data->player->y - 1;
	if (dx > 0)
		x = (int)data->player->x;
	else
		x = (int)data->player->x - 1;

	if (dy == 1)
	{
		while (x > 0 && x < data->map_width && y > 0 && y < data->map_height && data->map[(int)y][(int)x] == '0')
		{
			x += dx;
			y += dy;
		}
	}
	else
	{
		while (x > 0 && x < data->map_width && y > 0 && y < data->map_height && data->map[(int)y - 1][(int)x] == '0')
		{
			x += dx;
			y += dy;
		}
	}
	return (sqrt(sqr(x - data->player->x) + sqr(y - data->player->y)));
}

float	cast_column_ray(t_data *data, float dx, float dy)
{
	float	x;
	float	y;

	dy = dy / fabs(dx);
	if (dx > 0)	
		dx = 1;
	else
		dx = -1;

	if (dx > 0)
		x = (int)data->player->x;
	else
		x = (int)data->player->x - 1;
	if (dy > 0)
		y = (int)data->player->y;
	else
		y = (int)data->player->y - 1;

	if (dx == 1)
	{
		while (x > 0 && x < data->map_width && y > 0 && y < data->map_height && data->map[(int)y][(int)x] == '0')
		{
			x += dx;
			y += dy;
		}
	}
	else 
	{
		while (x > 0 && x < data->map_width && y > 0 && y < data->map_height && data->map[(int)y][(int)x - 1] == '0')
		{
			x += dx;
			y += dy;
		}
	}
	return (sqrt(sqr(x - data->player->x) + sqr(y - data->player->y)));
}

void	render_line(t_data *data, int x, float ray_angle)
{
	float	dx;
	float	dy;
	float	row_distance;
	float	column_distance;

	dx = cos(deg_to_rad(ray_angle));
	dy = sin(deg_to_rad(ray_angle));
	row_distance = cast_row_ray(data, dx, dy);
	column_distance = cast_column_ray(data, dx, dy);
	if (row_distance < column_distance)
		draw_wall_section(data, x, row_distance, 0xAAAAAAAA);
	else
		draw_wall_section(data, x, column_distance, 0xBBBBBBBB);
}

void	render_frame(t_data *data)
{
	int	i;
	
	i = 1;
	while (i  < WINDOW_WIDTH)
	{
		render_line_test(data, i, data->player->dir + (((float)FOV / WINDOW_WIDTH) * i) - (FOV / 2));
		render_line(data, i, data->player->dir + (((float)FOV / WINDOW_WIDTH) * i) - (FOV / 2));
		i++;
	}
	mlx_put_image_to_window(data->mlx, data->window, data->image, 0, 0);
	mlx_put_image_to_window(data->mlx, data->window_test, data->image_test, 0, 0);
}

#define K_ESC 65307
#define K_LEFT 65361
#define K_UP 65362
#define K_RIGHT 65363
#define K_DOWN 65364
#define K_W 119
#define K_S 115
#define K_D 100
#define K_A 97

int	key_hook(int keycode, t_data *data)
{
	if (keycode == K_LEFT)
		data->input = data->input | I_LEFT;
	else if (keycode == K_RIGHT)
		data->input = data->input | I_RIGHT;
	else if (keycode == K_W)
		data->input = data->input | I_W;
	else if (keycode == K_S)
		data->input = data->input | I_S;
	else if (keycode == K_D) //maybe not needed, might just need to turn
		data->input = data->input | I_D;
	else if (keycode == K_A)
		data->input = data->input | I_A;
	else if (keycode == K_ESC)
		exit(0); //bad
	render_frame(data);
	return (0);
}

int	key_release_hook(int keycode, t_data *data)
{	
	if (keycode == K_LEFT)
		data->input = data->input & ~I_LEFT;
	else if (keycode == K_RIGHT)
		data->input = data->input & ~I_RIGHT;
	else if (keycode == K_W)
		data->input = data->input & ~I_W;
	else if (keycode == K_S)
		data->input = data->input & ~I_S;
	else if (keycode == K_D) //maybe not needed, might just need to turn
		data->input = data->input & ~I_D;
	else if (keycode == K_A)
		data->input = data->input & ~I_A;
	else if (keycode == K_ESC)
		exit(0); //bad
	return (0);
}

int	move_player(t_data *data)
{
	//printf("position %f %f direction %f\n", data->player->x, data->player->y, data->player->dir);
	printf("input %X\n", data->input);
	if ((data->input & I_W) != 0)
	{
		data->player->x += cos(deg_to_rad(data->player->dir)) * 0.1;
		data->player->y += sin(deg_to_rad(data->player->dir)) * 0.1;
	}
	else if ((data->input & I_S) != 0)
	{
		data->player->x -= cos(deg_to_rad(data->player->dir)) * 0.1;
		data->player->y -= sin(deg_to_rad(data->player->dir)) * 0.1;
	}
	if ((data->input & I_LEFT) != 0)
		data->player->dir -= 0.5;
	else if ((data->input & I_RIGHT) != 0)
		data->player->dir += 0.5;
	render_frame(data);
	return (0);
}

int	main(void)
{
	t_data	*data;
	t_gctrl	*gctrl;

	gctrl = gctrl_init();
	data = init_data(gctrl);
	render_frame(data);
	mlx_hook(data->window, 2, 1L, key_hook, data);
	mlx_hook(data->window, 3, 1L << 1, key_release_hook, data);
	mlx_loop_hook(data->mlx, move_player, data);
	mlx_loop(data->mlx);
}
