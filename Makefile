CC=cc
CFLAGS= -fsanitize=address -g -lm -I. -mavx2 -mfma
CFLAGS_FAST = -O3 -march=native -mavx2 -DNO_DEBUG=1 -lm -I. -mfma
SRC = main.c \
	utils1.c \
	init.c \
	debug.c \
	reset.c \
	engine.c \
	neural_network.c

OBJ = $(SRC:%.c=%.o)
NAME = chess

LIBFT = libft.a

all: $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

fast:
	$(CC) -I./libft $(SRC) libft/*.c -o $(NAME) $(CFLAGS_FAST) 
#cd libft && make fclean && make CFLAGS='$(CFLAGS_FAST)';
#make CFLAGS="$(CFLAGS_FAST)"

prof:
	$(CC) $(CFLAGS_FAST) -g -pg -I./libft $(SRC) libft/*.c -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	cd libft && make CFLAGS='$(CFLAGS)';
	cp ./libft/libft.a $(LIBFT)

clean:
	rm -f $(OBJ)
	cd libft && make clean

fclean: clean
	rm -f $(LIBFT) $(NAME)
	cd libft && make fclean

re: fclean all

refast: fclean fast
