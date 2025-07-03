#include <cub3d.h>

char **get_temp_map(t_gctrl *gctrl)
{
	char **map;
	int	i = 0;
	int fd = open("map", O_RDONLY);

	map = gctrl_malloc(gctrl, PROG_BLOCK, sizeof(char *) * 16);
	while (i < 16)
	{
		map = get_next_line(fd);
		i++;
	}
	return (map);
}

t_data	init_data(t_gctrl *gctrl)
{
	t_data 		*data;

	data = gctrl_malloc(gctrl, PROG_BLOCK, sizeof(t_data));
	data->player = gctrl_malloc(gctrl, PROG_BLOCK, sizeof(t_player));
	data->map_height = 16;	
	data->map_width = 16;	
	data->player->x = 800;
	data->player->y = 800;
	data->player->dir = 0;
	data->map = get_temp_map(gctrl);
	return (data);
}

int	main(void)
{
	t_data	*data;
	t_gctrl	*gctrl;

	gctrl = gctrl_init();
	data = init_data(gctrl);
}
