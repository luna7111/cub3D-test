#include "../libft/libft.h"
#include "../mlx/mlx.h"
#include "../gctrl/src/garbage_control.h"

#include <math.h>

#define PROG_BLOCK 1

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define WINDOW_NAME "CUB3D"
#define FOV 60

typedef struct s_player
{
	float	x;
	float	y;
	float	dir;
}	t_player;

typedef struct s_data
{
	char		**map;
	int			map_height;
	int			map_width;
	t_player	*player;
	t_gctrl		gctrl;
	void		*mlx;
	void		*window;
}	t_data;
