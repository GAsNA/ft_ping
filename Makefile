NAME 			=	ft_ping

LIBFT_PATH		=	./libft/
LIBFT			=	${LIBFT_PATH}libft.a

SRC_PATH 		=	./src/
OBJ_PATH 		=	./obj/

FILES 			=	main.c

SRC				=	$(addprefix $(SRC_PATH), $(FILES))
OBJ 			=	$(addprefix $(OBJ_PATH), $(FILES:.c=.o))

CLEAR			=	clear
CC				=	clang
RM				=	rm -rf
CFLAGS			=	-Wall -Wextra -Werror

# COMMANDS

all:			$(NAME)

$(NAME):		$(OBJ) $(LIBFT)
			$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

$(OBJ_PATH)%.o:	$(SRC_PATH)%.c
			mkdir -p $(dir $@)
			$(CC) -c $(CFLAGS) $< -o $@

$(LIBFT):
			make -C $(LIBFT_PATH)

clean:
			make -C $(LIBFT_PATH) clean
			rm -rf $(OBJ_PATH)

fclean:			clean
			make -C $(LIBFT_PATH) fclean
			rm -f $(NAME)

re:				fclean all

.PHONY: all clean fclean re
