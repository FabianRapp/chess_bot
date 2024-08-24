CC=cc
CFLAG=-fsanitize=address -g
SRC = main.c
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
	rm -f $(OBJ_PARENT) $(OBJ_CHILD)
	cd libft && make clean

fclean: clean
	rm -f $(LIBFT) $(CHILD) $(PARENT)
	cd libft && make fclean
