NAME 			=	ft_ping

LIBFT_PATH		=	./libft/
LIBFT			=	${LIBFT_PATH}libft.a

HEADER_PATH		=	./includes/

SRC_PATH 		=	./src/
OBJ_PATH 		=	./obj/

FILES 			=	main.c	\
					utils.c	\
					prints.c

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
			$(CC) -c $(CFLAGS) -I$(HEADER_PATH) $< -o $@

$(LIBFT):
			make -C $(LIBFT_PATH)

clean:
			make -C $(LIBFT_PATH) clean
			rm -rf $(OBJ_PATH)

fclean:			clean
			make -C $(LIBFT_PATH) fclean
			rm -f $(NAME)

re:				fclean all

run:			all
			sudo ./$(NAME) 1.1.1.1

valgrind:			all
			sudo valgrind ./$(NAME) 1.1.1.1

valgrind_detailed:		all
			sudo valgrind --leak-check=full --leak-check=full --show-leak-kinds=all ./$(NAME) 1.1.1.1

.PHONY: all clean fclean re run valgrind valgrind_detailed
