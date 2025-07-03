##########################################

NAME		:=	cub3d

CC			:=	cc

CFLAGS		:=	-Wall -Wextra -Werror -Isrc

SRC 		:=	src/cub3d.c

OBJ 		:=	$(SRC:.c=.o)

###########################################

GCTRL		:=	gctrl/garbage_control.a
GCTRLDIR	:=	gctrl

LIBFT		:=	libft/libft.a
LIBFTDIR	:=	libft

###########################################

all: $(NAME)

$(LIBFT):
	make -C $(LIBFTDIR)

$(GCTRL):
	make -C $(GCTRLDIR)

$(NAME): $(LIBFT) $(GCTRL) $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(GCTRL) $(LIBFT) -lreadline 

clean:
	rm -f $(OBJ)
	make -C $(GCTRLDIR) fclean
	make -C $(LIBFTDIR) fclean

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
