CC=cc
CFLAG=-fsanitize=thread -g

SRC = main.c \
	utils1.c \
	init.c \
	debug.c \
	reset.c

OBJ = $(SRC:%.c=%.o)
NAME = chess

LIBFT = libft.a

all: $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	cd libft && make;
	cp ./libft/libft.a $(LIBFT)

clean:
	rm -f $(OBJ)
	cd libft && make clean

fclean: clean
	rm -f $(LIBFT) $(NAME)
	cd libft && make fclean

re: fclean all
