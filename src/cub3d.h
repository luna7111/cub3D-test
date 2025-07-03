#include "../libft/libft.h"
#include "../gctrl/src/garbage_control.h"

#define PROG_BLOCK 1

typedef struct s_player
{
	int	x;
	int	y;
	int	dir;
}	t_player;

typedef struct s_data
{
	char		**map;
	int			map_height;
	int			map_width;
	t_player	*player;
}	t_data;
