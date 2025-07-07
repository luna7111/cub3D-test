##########################################

NAME		:=	cub3d

CC			:=	cc

CFLAGS		:=	-Wall -Wextra -Werror -Isrc -g3

LIBFLAGS	:=	-Lmlx -lmlx -lXext -lX11 -lm

SRC 		:=	src/cub3d.c

OBJ 		:=	$(SRC:.c=.o)

###########################################

GCTRL		:=	gctrl/garbage_control.a
GCTRLDIR	:=	gctrl

LIBFT		:=	libft/libft.a
LIBFTDIR	:=	libft

MLX			:=	mlx/libmlx.a
MLXDIR		:=	mlx

###########################################

all: $(NAME)

$(LIBFT):
	make -C $(LIBFTDIR)

$(GCTRL):
	make -C $(GCTRLDIR)

$(MLX):
	make -C $(MLXDIR)

$(NAME): $(LIBFT) $(GCTRL) $(MLX) $(OBJ)
	$(CC) $(CFLAGS) $(LIBFLAGS) -o $(NAME) $(OBJ) $(GCTRL) $(LIBFT) $(MLX)

clean:
	rm -f $(OBJ)
	make -C $(GCTRLDIR) fclean
	make -C $(LIBFTDIR) fclean

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
