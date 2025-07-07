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
	return (data);
}

/*
void	cast_rows_ray()
{
	
}

void	cast_columns_ray()
{

}
*/

float	deg_to_rad(float degrees)
{
	return (degrees * M_PI / 180);
}

void	put_pixel(t_data *data, int x, int y)
{
	mlx_pixel_put(data->mlx, data->window, x, y, 0xFFFFFFFF);
}

void	draw_wall_section(t_data *data, int x, float distance)
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
	while (i < size)
	{
		put_pixel(data, x, i + start_point);
		i++;
	}
}

void	render_line(t_data *data, int x, float ray_angle)
{
	float	dx;	
	float	dy;	
	float	i;

	dx = cos(deg_to_rad(ray_angle));
	dy = sin(deg_to_rad(ray_angle));
	i = 0;
	while (data->map[(int)(data->player->y + (dy * i))][(int)(data->player->x + (dx * i))] == '0')
		i+= 0.01;
	draw_wall_section(data, x, i);
}

void	render_frame(t_data *data)
{
	int	i;
	
	i = 1;
	while (i  < WINDOW_WIDTH)
	{
		render_line(data, i, (((float)FOV / WINDOW_WIDTH) * i) - (FOV / 2));
		i++;
	}
}

#define K_LEFT 65361
#define K_UP 65362
#define K_RIGHT 65363
#define K_DOWN 65364

int	key_hook(int keycode, t_data *data)
{
	if (keycode == K_LEFT)
		data->player->x --;
	if (keycode == K_RIGHT)
		data->player->x ++;
	if (keycode == K_UP)
		data->player->y --;
	if (keycode == K_DOWN)
		data->player->y ++;
	render_frame(data);
	return (0);
}

int	main(void)
{
	t_data	*data;
	t_gctrl	*gctrl;

	gctrl = gctrl_init();
	data = init_data(gctrl);
	for (int i = 15; i >= 0; i--)
		printf("%s\n", data->map[i]);
	render_frame(data);
	mlx_key_hook(data->window, key_hook, data);
	mlx_loop(data->mlx);
}
